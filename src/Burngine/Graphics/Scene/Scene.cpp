//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#include <Burngine/Graphics/Scene/Scene.h>

#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <Burngine/Graphics/Scene/SceneNode.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/Graphics/Scene/DirectionalLight.h>
#include <Burngine/Graphics/Scene/SpotLight.h>
#include <Burngine/Graphics/Scene/Mesh.h>

#include <Burngine/System/Reporter.h>

#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/Scene/RenderHelper.h>

namespace burn {

const Matrix4f MVP_TO_SHADOWCOORD(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

Scene::Scene(	const Window& parentWindow,
				const ShadowMap::Resolution& shadowmapRes,
				const ShadowCubeMap::Resolution& shadowcubemapRes) :
_window(parentWindow),
_isLightingEnabled(false) {

	if(!_gBuffer.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()))){
		Reporter::report("Unable to create gBuffer!", Reporter::ERROR);
		exit(11);
	}

	if(!_renderTexture.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()))){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(12);
	}

	if(!_renderTexture.addColorAttachment(1)){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(13);
	}

	_renderTexture.clear();

	if(!_shadowMap.create(shadowmapRes)){
		Reporter::report("Unable to create shadowmap!", Reporter::ERROR);
		exit(14);
	}

	if(!_shadowCubeMap.create(shadowcubemapRes)){
		Reporter::report("Unable to create shadowcubemap!", Reporter::ERROR);
		exit(15);
	}

	_shadowMap.clear();
	_shadowCubeMap.clear();
	_shadowMap.bindAsSource(8);
	_shadowCubeMap.bindAsSource(8);

	Vector3f posData[] = {
	Vector3f(-1.f, -1.f, 0.f),
	Vector3f(1.f, -1.f, 0.f),
	Vector3f(-1.f, 1.f, 0.f),
	Vector3f(1.f, 1.f, 0.f) };

	Vector2f uvData[] = {
	Vector2f(0.f, 0.f),
	Vector2f(1.f, 0.f),
	Vector2f(0.f, 1.f),
	Vector2f(1.f, 1.f), };

	_fullscreenVbo.create();
	for(int i = 0; i != 4; ++i){
		_fullscreenVbo.addData(&posData[i], sizeof(Vector3f));
		_fullscreenVbo.addData(&uvData[i], sizeof(Vector2f));
	}
	_fullscreenVbo.uploadDataToGpu( GL_ARRAY_BUFFER,
	GL_STATIC_DRAW);

	//Setup RenderHelper
	RenderHelper::setVboIndex(RF::POSITION, 0);
	RenderHelper::setVboIndex(RF::NORMAL, 1);
	RenderHelper::setVboIndex(RF::UV, 2);

}

Scene::~Scene() {

	detachAll();

}

void Scene::drawGBuffers(const Camera& camera) {

	_gBuffer.clear();
	_gBuffer.bindAsTarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::G_BUFFER);

	for(size_t i = 0; i < _nodes.size(); ++i){

		RenderHelper::render(_nodes[i], RF::POSITION | RF::NORMAL | RF::UV, camera, shader);

	}

}

void Scene::draw(	const Camera& camera,
					const RenderMode& mode) {

	ensureContext();

	if(_nodes.size() == 0)
		return;

	drawGBuffers(camera);

	_window.bind(); // As Target
	_gBuffer.bindAsSource();
	if(mode == COMPOSITION || mode == LIGHTING){

		if(mode != LIGHTING){
			//Copy diffuse gBuffer to windowframebuffer:
			_gBuffer.setSourceBuffer(GBuffer::DIFFUSE);

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
			shader.setUniform("modelMatrix", Matrix4f(1.f));
			shader.setUniform("viewMatrix", Matrix4f(1.f));
			shader.setUniform("projectionMatrix", Matrix4f(1.f));
			shader.setUniform("mixColor", Vector3f(1.f));
			shader.setUniform("gSampler", GBuffer::DIFFUSE);

			drawFullscreenQuad(shader, OpenGlControl::Settings());

		}

		if(_isLightingEnabled)
			lightPass(camera, mode == LIGHTING);

		OpenGlControl::useSettings(OpenGlControl::Settings());

		/*if(mode != LIGHTING){
		 _window.bind();
		 _skyBox.draw();
		 }*/

	}else if(mode == DIFFUSE){
		_gBuffer.setSourceBuffer(GBuffer::DIFFUSE);
		glBlitFramebuffer(	0,
							0,
							_gBuffer.getDimensions().x,
							_gBuffer.getDimensions().y,
							0,
							0,
							_window.getSettings().getWidth(),
							_window.getSettings().getHeight(),
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
	}else if(mode == NORMAL_WS){
		_gBuffer.setSourceBuffer(GBuffer::NORMAL_WS);
		glBlitFramebuffer(	0,
							0,
							_gBuffer.getDimensions().x,
							_gBuffer.getDimensions().y,
							0,
							0,
							_window.getSettings().getWidth(),
							_window.getSettings().getHeight(),
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
	}else if(mode == POSITION_WS){
		_gBuffer.setSourceBuffer(GBuffer::POSITION_WS);
		glBlitFramebuffer(	0,
							0,
							_gBuffer.getDimensions().x,
							_gBuffer.getDimensions().y,
							0,
							0,
							_window.getSettings().getWidth(),
							_window.getSettings().getHeight(),
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
	}else if(mode == DEPTH){
		dumpOutDepthGBuffer(); //Special, because no GL_COLOR_BUFFER

	}

}

void Scene::lightPass(	const Camera& camera,
						bool dumpLighting) {

	OpenGlControl::Settings ogl;
	ogl.enableDepthtest(false);
	ogl.enableDepthbufferWriting(false);
	ogl.enableCulling(false);
	ogl.enableBlending(true);
	ogl.setBlendMode(OpenGlControl::ADD);
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	//Pre-Adjust Shaders:
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::DIRECTIONAL_LIGHT);
		shader.setUniform("modelMatrix", Matrix4f(1.f));
		shader.setUniform("viewMatrix", Matrix4f(1.f));
		shader.setUniform("projectionMatrix", Matrix4f(1.f));
		shader.setUniform("gSamplerNormals", GBuffer::NORMAL_WS);
		shader.setUniform("gSamplerPositions", GBuffer::POSITION_WS);
		shader.setUniform("gSamplerColor", GBuffer::DIFFUSE);
		shader.setUniform("gSamplerShadowmap", 8);
		shader.setUniform("gEyePosition", camera.getPosition());
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);
		shader.setUniform("modelMatrix", Matrix4f(1.f));
		shader.setUniform("viewMatrix", Matrix4f(1.f));
		shader.setUniform("projectionMatrix", Matrix4f(1.f));
		shader.setUniform("gSamplerNormals", GBuffer::NORMAL_WS);
		shader.setUniform("gSamplerPositions", GBuffer::POSITION_WS);
		shader.setUniform("gSamplerColor", GBuffer::DIFFUSE);
		shader.setUniform("gSamplerShadowcubemap", 8);
		shader.setUniform("gEyePosition", camera.getPosition());
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);
		shader.setUniform("modelMatrix", Matrix4f(1.f));
		shader.setUniform("viewMatrix", Matrix4f(1.f));
		shader.setUniform("projectionMatrix", Matrix4f(1.f));
		shader.setUniform("gSamplerNormals", GBuffer::NORMAL_WS);
		shader.setUniform("gSamplerPositions", GBuffer::POSITION_WS);
		shader.setUniform("gSamplerColor", GBuffer::DIFFUSE);
		shader.setUniform("gSamplerShadowmap", 8);
		shader.setUniform("gEyePosition", camera.getPosition());
	}
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////

	//Render all dirlights together into rendertexture
	_renderTexture.clear();
	_renderTexture.bindAsTarget(); // <- Diffuse light (attachment 0); specular (1)

	ambientPart();

	for(size_t i = 0; i < _lights.size(); ++i){

		//Clear shadowmap
		_shadowMap.clear();

		if(typeid(*(_lights[i])) == typeid(DirectionalLight)){

			DirectionalLight* light = static_cast<DirectionalLight*>(_lights[i]);

			//Render shadowmap:
			Matrix4f shadowMatrix = drawShadowmap(*light);

			//Render light
			_renderTexture.bindAsTarget();
			_shadowMap.bindAsSource(8);
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::DIRECTIONAL_LIGHT);
			shader.setUniform("gLightDirection", Vector3f(light->getDirection()));
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());
			shader.setUniform("shadowMatrix", shadowMatrix);

			drawFullscreenQuad(shader, ogl);

		}else if(typeid(*(_lights[i])) == typeid(Light)){

			Light* light = static_cast<Light*>(_lights[i]);

			//Render shadowmap:
			drawShadowmap(*light);

			//Render light
			_shadowCubeMap.bindAsSource(8);
			_renderTexture.bindAsTarget();
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);
			shader.setUniform("gLightPosition", light->getPosition());
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());

			drawFullscreenQuad(shader, ogl);
		}else{ //Spotlight

			SpotLight* light = static_cast<SpotLight*>(_lights[i]);

			//Render shadowmap:
			Matrix4f shadowMatrix = drawShadowmap(*light);

			//Render light
			_renderTexture.bindAsTarget();
			_shadowMap.bindAsSource(8);
			float lightConeCosine = std::cos(light->getConeAngle() / (180.f / 3.1415f));

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);
			shader.setUniform("gLightDirection", Vector3f(light->getDirection()));
			shader.setUniform("gLightPosition", light->getPosition());
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());
			shader.setUniform("gLightConeCosine", lightConeCosine);
			shader.setUniform("shadowMatrix", shadowMatrix);

			drawFullscreenQuad(shader, ogl);

		}

	}

	///////////////////////////////////////////////////////////////

	//Multiply result with the scene
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));
	shader.setUniform("mixColor", Vector3f(1.f));
	_window.bind();

	if(!dumpLighting){
		//Compose with diffuse part:
		_renderTexture.bindAsSource();
		shader.setUniform("gSampler", 0); //sample from diffuse
		ogl.setBlendMode(OpenGlControl::MULTIPLY);
		drawFullscreenQuad(shader, ogl);

		//Compose with specular part:
		_renderTexture.bindAsSource();
		shader.setUniform("gSampler", 1); //sample from diffuse
		ogl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, ogl);

	}else{

		_renderTexture.bindAsTarget();
		_renderTexture.bindAsSource();
		shader.setUniform("gSampler", 1); //sample from diffuse
		ogl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, ogl);

		_window.bind();
		shader.setUniform("gSampler", 0); //sample from diffuse
		ogl.setBlendMode(OpenGlControl::OVERWRITE);
		drawFullscreenQuad(shader, ogl);

	}

	OpenGlControl::useSettings(OpenGlControl::Settings());
}

Matrix4f Scene::drawShadowmap(const DirectionalLight& dirLight) {

	OpenGlControl::useSettings(OpenGlControl::Settings());

	_shadowMap.clear();
	_shadowMap.bindAsRendertarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);

	//Cover whole scene with sceneBb
	Matrix4f view = glm::lookAt(Vector3f(0.f), Vector3f(dirLight.getDirection()), Vector3f(1.f));

	Matrix4f projection = glm::ortho(-30.f, 30.f, -30.f, 30.f, -30.f, 30.f);

	shader.setUniform("projectionMatrix", projection);
	shader.setUniform("viewMatrix", view);

	for(size_t i = 0; i < _nodes.size(); ++i){

		if(typeid(*(_nodes[i])) == typeid(StaticMeshNode)){

			StaticMeshNode* node = static_cast<StaticMeshNode*>(_nodes[i]);

			const Model& model = node->getModel();

			shader.setUniform("modelMatrix", node->getModelMatrix());

			for(size_t j = 0; j < model.getMeshCount(); ++j){

				const Mesh& mesh = model.getMesh(j);

				//Only draw when mesh can cast shadows
				if(mesh.getMaterial().isFlagSet(Material::CAST_SHADOWS) == false)
					continue;

				//Set OpenGL according to mesh's flags
				mesh.getMaterial().setOpenGlByFlags();

				glEnableVertexAttribArray(0);
				mesh.getPositionVbo().bind();
				glVertexAttribPointer(0, 3,
				GL_FLOAT,
										GL_FALSE, 0, (void*)0);
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);
				glDisableVertexAttribArray(0);

			}

		}

	}

	return MVP_TO_SHADOWCOORD * projection * view;
}

void Scene::drawShadowmap(const Light& pointlight) {

	OpenGlControl::useSettings(OpenGlControl::Settings());

	_shadowCubeMap.clear();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);
	Matrix4f projection = glm::perspective(90.f, 1.f, 0.01f, 2000.f);
	shader.setUniform("projectionMatrix", projection);

	for(int face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face != GL_TEXTURE_CUBE_MAP_POSITIVE_X + 6; ++face){
		_shadowCubeMap.bindAsRendertarget(face);
		glClear(GL_DEPTH_BUFFER_BIT);

		Matrix4f view = findViewMatrix(face, pointlight);
		shader.setUniform("viewMatrix", view);

		for(size_t i = 0; i < _nodes.size(); ++i){

			if(typeid(*(_nodes[i])) == typeid(StaticMeshNode)){

				StaticMeshNode* node = static_cast<StaticMeshNode*>(_nodes[i]);

				const Model& model = node->getModel();

				shader.setUniform("modelMatrix", node->getModelMatrix());

				for(size_t j = 0; j < model.getMeshCount(); ++j){

					const Mesh& mesh = model.getMesh(j);

					//Only draw when mesh can cast shadows
					if(mesh.getMaterial().isFlagSet(Material::CAST_SHADOWS) == false)
						continue;

					//Set OpenGL according to mesh's flags
					mesh.getMaterial().setOpenGlByFlags();

					glEnableVertexAttribArray(0);
					mesh.getPositionVbo().bind();
					glVertexAttribPointer(0, 3,
					GL_FLOAT,
											GL_FALSE, 0, (void*)0);
					OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);
					glDisableVertexAttribArray(0);

				}

			}

		}

	}
}

Matrix4f Scene::findViewMatrix(	const int& face,
								const Light& pointlight) {

	Vector3f direction(1.f, 0.f, 0.f);
	Vector3f headUp(0.f, 1.f, 0.f);

	if(face == GL_TEXTURE_CUBE_MAP_POSITIVE_X){
		direction = Vector3f(1.f, 0.f, 0.f);
	}else if(face == GL_TEXTURE_CUBE_MAP_NEGATIVE_X){
		direction = Vector3f(-1.f, 0.f, 0.f);
	}else if(face == GL_TEXTURE_CUBE_MAP_POSITIVE_Y){
		direction = Vector3f(0.f, -1.f, 0.f);
		headUp = Vector3f(0.f, 0.f, -1.f);
	}else if(face == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y){
		direction = Vector3f(0.f, 1.f, 0.f);
		headUp = Vector3f(0.f, 0.f, 1.f);
	}else if(face == GL_TEXTURE_CUBE_MAP_POSITIVE_Z){
		direction = Vector3f(0.f, 0.f, -1.f);
	}else{
		direction = Vector3f(0.f, 0.f, 1.f);
	}

	return glm::lookAt(pointlight.getPosition(), pointlight.getPosition() + direction, headUp);
}

Matrix4f Scene::drawShadowmap(const SpotLight& spotlight) {

	OpenGlControl::useSettings(OpenGlControl::Settings());

	_shadowMap.clear();
	_shadowMap.bindAsRendertarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);

	//Camera from light's view
	Camera virtualCamera;
	virtualCamera.setFov(spotlight.getConeAngle() * 2.f);
	virtualCamera.setAspectRatio(1.f);
	virtualCamera.setPosition(spotlight.getPosition());
	virtualCamera.lookAt(spotlight.getPosition() + Vector3f(spotlight.getDirection()));

	for(size_t i = 0; i < _nodes.size(); ++i){
		RenderHelper::render(_nodes[i], RF::POSITION, virtualCamera, shader, true);
	}

	return MVP_TO_SHADOWCOORD * virtualCamera.getProjectionMatrix() * virtualCamera.getViewMatrix();
}

void Scene::ambientPart() {

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::SINGLECOLOR);
	shader.setUniform("gColor", _ambientColor);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	glEnableVertexAttribArray(0);
	_fullscreenVbo.bind();
	glVertexAttribPointer(0, 3,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);
	glDisableVertexAttribArray(0);

}

void Scene::dumpOutDepthGBuffer() {

	OpenGlControl::Settings ogl;
	ogl.enableDepthtest(false);
	ogl.enableDepthbufferWriting(false);
	ogl.enableCulling(false);

	_window.bind();
	_gBuffer.bindDepthBufferAsSourceTexture();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE_ONE_COMPONENT);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	drawFullscreenQuad(shader, ogl);

	OpenGlControl::useSettings(OpenGlControl::Settings());

}

void Scene::drawFullscreenQuad(	const Shader& shader,
								const OpenGlControl::Settings& rendersettings) const {

	OpenGlControl::useSettings(rendersettings);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	_fullscreenVbo.bind();
	glVertexAttribPointer(0, 3,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	glVertexAttribPointer(1, 2,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void Scene::detachAll() {
	//All SceneNodes:
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->removeParentScene(this);
	}
	_nodes.clear();

	//All Lights:
	for(size_t i = 0; i < _lights.size(); ++i){
		_lights[i]->removeParentScene(this);
	}
	_lights.clear();

}

void Scene::attachSceneNode(SceneNode& node) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node)
			return; //Already attached
	}
	_nodes.push_back(&node);
	node.addParentScene(this);
}

void Scene::detachSceneNode(SceneNode& node) {

	node.removeParentScene(this);

	//Remove from attachment-list
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node){
			_nodes.erase(_nodes.begin() + i);
			return;
		}
	}

}

void Scene::attachLight(Light& light) {
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == &light)
			return; //Already attached
	}
	_lights.push_back(&light);
	light.addParentScene(this);
}

void Scene::detachLight(Light& light) {

	light.removeParentScene(this);

	//Remove from attachement-list
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == &light){
			_lights.erase(_lights.begin() + i);
			return;
		}
	}

}

void Scene::setAmbientColor(const Vector3f& color) {
	_ambientColor = color;
}

const Vector3f& Scene::getAmbientColor() const {
	return _ambientColor;
}

void Scene::setSkyBox(const SkyBox& skyBox) {
	_skyBox = skyBox;
}

void Scene::setLightingEnabled(bool enabled) {
	_isLightingEnabled = enabled;
}

bool Scene::isLightingEnabled() const {
	return _isLightingEnabled;
}

} /* namespace burn */
