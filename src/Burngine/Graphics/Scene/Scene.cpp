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

namespace burn {

const Matrix4f MVP_TO_SHADOWCOORD(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

Scene::Scene(const Window& parentWindow) :
_window(parentWindow) {

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

	if(!_shadowMap.create(ShadowMap::HIGH)){
		Reporter::report("Unable to create shadowmap!", Reporter::ERROR);
		exit(14);
	}

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
	_fullscreenVbo.uploadDataToGpu(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

}

Scene::~Scene() {

	detachAll();

}

void Scene::drawGBuffers(const Camera& camera) {

	OpenGlControl::Settings ogl;
	ogl.enableCulling(true);
	ogl.setCulledSide(OpenGlControl::INSIDE);
	ogl.setVertexOrder(OpenGlControl::COUNTER_CLOCKWISE);
	OpenGlControl::useSettings(ogl);

	_gBuffer.clear();
	_gBuffer.bindAsTarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::G_BUFFER);

	//Calculate matrices
	glm::mat4 projectionMatrix = glm::perspective(camera.getFov(), camera.getAspectRatio(), 0.1f, 10000.0f);
	glm::mat4 viewMatrix = glm::lookAt(camera.getPosition(), camera.getLookAt(), glm::vec3(0, 1, 0));

	shader.setUniform("viewMatrix", viewMatrix);
	shader.setUniform("projectionMatrix", projectionMatrix);

	for(size_t i = 0; i < _nodes.size(); ++i){

		//_nodes[i] is a StaticMeshNode
		if(typeid(*(_nodes[i])) == typeid(StaticMeshNode)){

			StaticMeshNode* node = static_cast<StaticMeshNode*>(_nodes[i]);

			node->update();
			const Model& model = node->getModel();
			if(!model.isUpdated())
				continue; //Next node

			Matrix4f normalMatrix = glm::transpose(glm::inverse(/*viewMatrix * */node->getModelMatrix()));
			shader.setUniform("modelMatrix", node->getModelMatrix());
			shader.setUniform("normalMatrix", normalMatrix);

			for(size_t j = 0; j < model.getMeshCount(); ++j){

				const Mesh& mesh = model.getMesh(j);

				if(mesh.getMaterial().getType() == Material::SOLID_COLOR){
					shader.setUniform("diffuseType", 1);
					shader.setUniform("meshColor", mesh.getMaterial().getDiffuseColor());
				}else{
					shader.setUniform("diffuseType", 0); //Type = TEXTURED
					mesh.getTexture().bindAsSource();
				}

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);

				mesh.getPositionVbo().bind();
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				mesh.getNormalVbo().bind();
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				mesh.getUvVbo().bind();
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);

			}

		}

	}

}

void Scene::draw(const Camera& camera, const RenderModus& modus) {

	if(!Window::isContextCreated())
		return;

	drawGBuffers(camera);

	_window.bind(); // As Target
	_gBuffer.bindAsSource();
	switch (modus) {
		case COMPOSITION:

			//Copy diffuse gBuffer to windowframebuffer:
			_gBuffer.setSourceBuffer(GBuffer::DIFFUSE);
			glBlitFramebuffer(0, 0, _gBuffer.getDimensions().x, _gBuffer.getDimensions().y, 0, 0,
								_window.getSettings().getWidth(), _window.getSettings().getHeight(),
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR);

			lightPass(camera);

			break;
		case DIFFUSE:
			_gBuffer.setSourceBuffer(GBuffer::DIFFUSE);
			glBlitFramebuffer(0, 0, _gBuffer.getDimensions().x, _gBuffer.getDimensions().y, 0, 0,
								_window.getSettings().getWidth(), _window.getSettings().getHeight(),
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR);
			break;
		case NORMAL_WS:
			_gBuffer.setSourceBuffer(GBuffer::NORMAL_WS);
			glBlitFramebuffer(0, 0, _gBuffer.getDimensions().x, _gBuffer.getDimensions().y, 0, 0,
								_window.getSettings().getWidth(), _window.getSettings().getHeight(),
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR);
			break;
		case POSITION_WS:
			_gBuffer.setSourceBuffer(GBuffer::POSITION_WS);
			glBlitFramebuffer(0, 0, _gBuffer.getDimensions().x, _gBuffer.getDimensions().y, 0, 0,
								_window.getSettings().getWidth(), _window.getSettings().getHeight(),
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR);
			break;
			/*case SSAO:
			 _gBuffer.setSourceBuffer(GBuffer::SSAO);
			 glBlitFramebuffer(0, 0, _gBuffer.getDimensions().x, _gBuffer.getDimensions().y, 0, 0,
			 _window.getSettings().getWidth(), _window.getSettings().getHeight(),
			 GL_COLOR_BUFFER_BIT,
			 GL_LINEAR);
			 break;*/
		case DEPTH:
			dumpOutDepthGBuffer(); //Special, because no GL_COLOR_BUFFER
			break;
	}

}

void Scene::lightPass(const Camera& camera) {

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

			//Render light
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

	OpenGlControl::useSettings(OpenGlControl::Settings());
}

Matrix4f Scene::drawShadowmap(const DirectionalLight& dirLight) {

	OpenGlControl::Settings ogl;
	ogl.enableCulling(true);
	ogl.setCulledSide(OpenGlControl::INSIDE);
	ogl.setVertexOrder(OpenGlControl::COUNTER_CLOCKWISE);
	OpenGlControl::useSettings(ogl);

	_shadowMap.clear();
	_shadowMap.bindAsRendertarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);

	Matrix4f projection = glm::ortho(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f);
	Matrix4f view = glm::lookAt(Vector3f(0.f), Vector3f(dirLight.getDirection()), Vector3f(1.f));
	shader.setUniform("projectionMatrix", projection);
	shader.setUniform("viewMatrix", view);

	for(size_t i = 0; i < _nodes.size(); ++i){

		if(typeid(*(_nodes[i])) == typeid(StaticMeshNode)){

			StaticMeshNode* node = static_cast<StaticMeshNode*>(_nodes[i]);

			const Model& model = node->getModel();

			shader.setUniform("modelMatrix", node->getModelMatrix());

			for(size_t j = 0; j < model.getMeshCount(); ++j){

				const Mesh& mesh = model.getMesh(j);

				glEnableVertexAttribArray(0);
				mesh.getPositionVbo().bind();
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);
				glDisableVertexAttribArray(0);

			}

		}

	}

	return MVP_TO_SHADOWCOORD * projection * view;
}

Matrix4f Scene::drawShadowmap(const SpotLight& spotlight) {

	OpenGlControl::Settings ogl;
	ogl.enableCulling(true);
	ogl.setCulledSide(OpenGlControl::INSIDE);
	ogl.setVertexOrder(OpenGlControl::COUNTER_CLOCKWISE);
	OpenGlControl::useSettings(ogl);

	_shadowMap.clear();
	_shadowMap.bindAsRendertarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);

	Matrix4f projection = glm::perspective<float>(spotlight.getConeAngle() * 2.f, 1.f, 0.01f, 2000.f);
	Matrix4f view = glm::lookAt(spotlight.getPosition(), spotlight.getPosition() + Vector3f(spotlight.getDirection()),
								Vector3f(0.f, 1.f, 0.f));
	shader.setUniform("projectionMatrix", projection);
	shader.setUniform("viewMatrix", view);

	for(size_t i = 0; i < _nodes.size(); ++i){

		if(typeid(*(_nodes[i])) == typeid(StaticMeshNode)){

			StaticMeshNode* node = static_cast<StaticMeshNode*>(_nodes[i]);

			const Model& model = node->getModel();

			shader.setUniform("modelMatrix", node->getModelMatrix());

			for(size_t j = 0; j < model.getMeshCount(); ++j){

				const Mesh& mesh = model.getMesh(j);

				glEnableVertexAttribArray(0);
				mesh.getPositionVbo().bind();
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);
				glDisableVertexAttribArray(0);

			}

		}

	}

	return MVP_TO_SHADOWCOORD * projection * view;

}

void Scene::ambientPart() {

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::SINGLECOLOR);
	shader.setUniform("gColor", _ambientColor);

	glEnableVertexAttribArray(0);
	_fullscreenVbo.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);

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

void Scene::drawFullscreenQuad(const Shader& shader, const OpenGlControl::Settings& rendersettings) const {

	OpenGlControl::useSettings(rendersettings);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	_fullscreenVbo.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

/*void Scene::drawNodes(const Camera& camera) {
 //Render objects without lighting:
 _window.bind();
 OpenGlControl::useSettings(OpenGlControl::Settings());
 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->draw(camera);
 }
 }

 bool Scene::drawDiffusepart(const Camera& camera) {

 if(!_diffuseLightTexture.isCreated())
 return false;

 //Render objects' lightings:

 OpenGlControl::Settings oglSettings;
 oglSettings.setClearColor(Vector4f(1.f));
 OpenGlControl::useSettings(oglSettings);

 _diffuseLightTexture.clear();
 _diffuseLightTexture.bindAsRendertarget();

 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawSingleColor(camera, Vector4f(0.f, 0.f, 0.f, 0.f));
 }

 oglSettings.setDepthtestTechnique(OpenGlControl::EQUAL);
 oglSettings.setBlendMode(OpenGlControl::ADD);
 OpenGlControl::useSettings(oglSettings);

 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawLighting(SceneNode::DIFFUSE, camera, _lights, _ambientColor);
 }

 return true;
 }

 bool Scene::drawSpecularpart(const Camera& camera) {
 if(!_specularLightTexture.isCreated())
 return false;

 OpenGlControl::Settings oglSettings;
 oglSettings.setClearColor(Vector4f(0.f, 0.f, 0.f, 1.f));
 OpenGlControl::useSettings(oglSettings);

 _specularLightTexture.clear();
 _specularLightTexture.bindAsRendertarget();

 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawSingleColor(camera, Vector4f(0.f, 0.f, 0.f, 0.f));
 }

 oglSettings.setDepthtestTechnique(OpenGlControl::EQUAL);
 oglSettings.setBlendMode(OpenGlControl::ADD);
 OpenGlControl::useSettings(oglSettings);
 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawLighting(SceneNode::SPECULAR, camera, _lights, _ambientColor);
 }

 return true;
 }*/

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

//Remove from attachement-list
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

} /* namespace burn */
