//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/Graphics/Scene/SceneRenderSystem.h>

//General Includes
#include <Burngine/Graphics/General/BurngineShaders.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>
#include <Burngine/Graphics/Window/Window.h>

//SceneNodes
#include <Burngine/Graphics/Scene/SceneNode.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/Graphics/Scene/DirectionalLight.h>
#include <Burngine/Graphics/Scene/SpotLight.h>

//Additional Includes
#include <Burngine/Graphics/General/PostEffects.h>

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

namespace burn {

//RenderFlag to ArrayIndex Mapping:
#define POSITION_ARRAY_INDEX 0
#define NORMAL_ARRAY_INDEX 1
#define UV_ARRAY_INDEX 2

//Variables in shader
#define DIFFUSE_TYPE_COLORED 1
#define DIFFUSE_TYPE_TEXTURED 0

const Matrix4f MVP_TO_SHADOWCOORD(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

void SceneRenderSystem::setVboIndex(const RenderFlag& flag,
									const GLuint& index) {
	switch (flag) {
		case POSITION:
			_vboIndices[POSITION_ARRAY_INDEX] = index;
			break;
		case NORMAL:
			_vboIndices[NORMAL_ARRAY_INDEX] = index;
			break;
		case UV:
			_vboIndices[UV_ARRAY_INDEX] = index;
			break;
	}
}

SceneRenderSystem::SceneRenderSystem(const Window& parentWindow) :
_window(parentWindow) {
	SceneRenderSystem::setVboIndex(POSITION, 0);
	SceneRenderSystem::setVboIndex(NORMAL, 1);
	SceneRenderSystem::setVboIndex(UV, 2);

	if(!_gBuffer.create(Vector2ui(parentWindow.getSettings().getWidth(), parentWindow.getSettings().getHeight()))){
		Reporter::report("Unable to create gBuffer!", Reporter::ERROR);
		exit(11);
	}

	if(!_renderTexture.create(Vector2ui(parentWindow.getSettings().getWidth(),
										parentWindow.getSettings().getHeight()))){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(12);
	}

	if(!_renderTexture.addColorAttachment(1)){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(13);
	}

	_renderTexture.clear();

	if(!_vsm.create(Vector2ui(512, 512))){
		Reporter::report("Unable to create VarianceShadowMap!", Reporter::ERROR);
		exit(16);
	}
	_vsm.setFiltering(Sampler::MAG_BILINEAR, Sampler::MIN_BILINEAR);
	_vsm.setAnisotropicLevel(8.f);
	_vsm.clear();

	Reporter::report("HERE", Reporter::ERROR);

	if(!_vscm.create(VarianceShadowCubeMap::MEDIUM)){
		Reporter::report("Unable to create VarianceShadowCubeMap!", Reporter::ERROR);
		exit(17);
	}
	_vscm.setFiltering(Sampler::MAG_BILINEAR, Sampler::MIN_BILINEAR);
	_vscm.setAnisotropicLevel(8.f);
	_vscm.clear();

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

}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void SceneRenderSystem::renderNode(	SceneNode* node,
									const int& constflags,
									const Camera& camera,
									const Shader& shader,
									bool shadowMapRendering) {

	ensureContext();

	//Set camera matrices
	shader.setUniform("viewMatrix", camera.getViewMatrix());
	shader.setUniform("projectionMatrix", camera.getProjectionMatrix());

	if(typeid(*(node)) == typeid(StaticMeshNode)){

		//Cast this ugly pointer to a real object
		StaticMeshNode n = *(static_cast<StaticMeshNode*>(node));

		//Calculate and set model's matrices
		Matrix4f normalMatrix = glm::transpose(glm::inverse(node->getModelMatrix()));
		shader.setUniform("modelMatrix", node->getModelMatrix());
		shader.setUniform("normalMatrix", normalMatrix);

		//StaticMeshNode consists of several meshes
		for(size_t i = 0; i < n.getMeshCount(); ++i){

			//Retreive the mesh
			const Mesh& mesh = n.getMesh(i);

			//Set OpenGL according to mesh's flags
			mesh.getMaterial().setOpenGlByFlags();

			//Set uniforms depending on mesh's material
			if(mesh.getMaterial().getType() == Material::SOLID_COLOR){
				shader.setUniform("diffuseType", DIFFUSE_TYPE_COLORED);
				shader.setUniform("meshColor", mesh.getMaterial().getDiffuseColor());
			}else{
				shader.setUniform("diffuseType", DIFFUSE_TYPE_TEXTURED); //Type = TEXTURED
				mesh.getTexture().bindAsSource();
			}

			//Bind bufferobjects according to renderflags
			glEnableVertexAttribArray(_vboIndices[POSITION_ARRAY_INDEX]);
			glEnableVertexAttribArray(_vboIndices[NORMAL_ARRAY_INDEX]);
			glEnableVertexAttribArray(_vboIndices[UV_ARRAY_INDEX]);

			//Reset flags to original parameter
			int flags = constflags;

			if(flags >= UV){
				mesh.getUvVbo().bind();
				glVertexAttribPointer(_vboIndices[UV_ARRAY_INDEX], 2,
				GL_FLOAT,
										GL_FALSE, 0, (void*)0);
				flags -= UV;
			}
			if(flags >= NORMAL){
				mesh.getNormalVbo().bind();
				glVertexAttribPointer(_vboIndices[NORMAL_ARRAY_INDEX], 3,
				GL_FLOAT,
										GL_FALSE, 0, (void*)0);
				flags -= NORMAL;
			}
			if(flags >= POSITION){
				mesh.getPositionVbo().bind();
				glVertexAttribPointer(_vboIndices[POSITION_ARRAY_INDEX], 3,
				GL_FLOAT,
										GL_FALSE, 0, (void*)0);
				flags -= POSITION;
			}

			OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);

			glDisableVertexAttribArray(_vboIndices[POSITION_ARRAY_INDEX]);
			glDisableVertexAttribArray(_vboIndices[NORMAL_ARRAY_INDEX]);
			glDisableVertexAttribArray(_vboIndices[UV_ARRAY_INDEX]);

		}

	}

}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void SceneRenderSystem::render(	const GLuint& targetFramebuffer, ///< Window is default
								const Vector2ui& targetFramebufferDimensions,
								const Camera& camera,
								const RenderMode& mode,
								const std::vector<SceneNode*>& nodes,
								const std::vector<Light*>& lights,
								const Vector3f& ambient,
								bool isLightingEnabled) {

	ensureContext();

	if(nodes.size() == 0)
		return;

	drawGBuffers(camera, nodes);

	_window.bind();
	_gBuffer.bindAsSource();
	if(mode == COMPOSITION || mode == LIGHTING){

		if(mode != LIGHTING){
			//Copy diffuse gBuffer to windowframebuffer:

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
			shader.setUniform("modelMatrix", Matrix4f(1.f));
			shader.setUniform("viewMatrix", Matrix4f(1.f));
			shader.setUniform("projectionMatrix", Matrix4f(1.f));
			shader.setUniform("mixColor", Vector3f(1.f));
			shader.setUniform("gSampler", GBuffer::DIFFUSE);

			drawFullscreenQuad(shader, OpenGlControl::Settings());

		}

		if(isLightingEnabled)
			lightPass(camera, nodes, lights, ambient, mode == LIGHTING);

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

void SceneRenderSystem::renderTextureToFramebuffer(const BaseTexture& source) {

}

void SceneRenderSystem::drawGBuffers(	const Camera& camera,
										const std::vector<SceneNode*>& nodes) {

	_gBuffer.clear();
	_gBuffer.bindAsTarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::G_BUFFER);

	for(size_t i = 0; i < nodes.size(); ++i){
		renderNode(nodes[i], POSITION | NORMAL | UV, camera, shader);
	}

}

void SceneRenderSystem::lightPass(	const Camera& camera,
									const std::vector<SceneNode*>& nodes,
									const std::vector<Light*>& lights,
									const Vector3f& ambient,
									bool dumpLighting) {

	OpenGlControl::Settings toLightPassOgl;
	toLightPassOgl.enableDepthtest(false);
	toLightPassOgl.enableDepthbufferWriting(false);
	toLightPassOgl.enableCulling(false);
	toLightPassOgl.enableBlending(true);
	toLightPassOgl.setBlendMode(OpenGlControl::ADD);
	toLightPassOgl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(toLightPassOgl);

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
	_renderTexture.bindAsTarget();		// <- Diffuse light (attachment 0); specular (1)

	ambientPart(ambient);

	for(size_t i = 0; i < lights.size(); ++i){

		if(typeid(*(lights[i])) == typeid(DirectionalLight)){

			DirectionalLight* light = static_cast<DirectionalLight*>(lights[i]);

			//Render shadowmap:
			VpMatrix vpm = drawShadowmap(*light, nodes);

			//Render light
			_renderTexture.bindAsTarget();
			_vsm.bindAsSource(8);
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::DIRECTIONAL_LIGHT);
			shader.setUniform("gLightDirection", Vector3f(light->getDirection()));
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());
			shader.setUniform("shadowPMatrix", vpm.p);
			shader.setUniform("shadowVMatrix", vpm.v);

			drawFullscreenQuad(shader, toLightPassOgl);

		}else if(typeid(*(lights[i])) == typeid(Light)){

			Light* light = static_cast<Light*>(lights[i]);

			//Render shadowmap:
			drawShadowmap(*light, nodes);

			//Render light
			_vscm.bindAsSource(8);
			_renderTexture.bindAsTarget();
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);
			shader.setUniform("gLightPosition", light->getPosition());
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());

			drawFullscreenQuad(shader, toLightPassOgl);
		}else{ //Spotlight

			SpotLight* light = static_cast<SpotLight*>(lights[i]);

			//Render shadowmap:
			Matrix4f shadowMatrix = drawShadowmap(*light, nodes);

			//Render light
			_renderTexture.bindAsTarget();
			_vsm.bindAsSource(8);
			float lightConeCosine = std::cos(light->getConeAngle() / (180.f / 3.1415f));

			//glGenerateMipmap(GL_TEXTURE_2D);

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);
			shader.setUniform("gLightDirection", Vector3f(light->getDirection()));
			shader.setUniform("gLightPosition", light->getPosition());
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());
			shader.setUniform("gLightConeCosine", lightConeCosine);
			shader.setUniform("shadowMatrix", shadowMatrix);

			drawFullscreenQuad(shader, toLightPassOgl);

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
		toLightPassOgl.setBlendMode(OpenGlControl::MULTIPLY);
		drawFullscreenQuad(shader, toLightPassOgl);

		//Compose with specular part:
		_renderTexture.bindAsSource();
		shader.setUniform("gSampler", 1); //sample from diffuse
		toLightPassOgl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, toLightPassOgl);

	}else{

		_renderTexture.bindAsTarget();
		_renderTexture.bindAsSource();
		shader.setUniform("gSampler", 1); //sample from diffuse
		toLightPassOgl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, toLightPassOgl);

		_window.bind();
		shader.setUniform("gSampler", 0); //sample from diffuse
		toLightPassOgl.setBlendMode(OpenGlControl::OVERWRITE);
		drawFullscreenQuad(shader, toLightPassOgl);

	}

	OpenGlControl::useSettings(OpenGlControl::Settings());
}

SceneRenderSystem::VpMatrix SceneRenderSystem::drawShadowmap(	const DirectionalLight& dirLight,
																const std::vector<SceneNode*>& nodes) {

	OpenGlControl::Settings ogl;
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	_vsm.clear();
	_vsm.bindAsTarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

	Camera virtualCamera;
	virtualCamera.lookAt(Vector3f(dirLight.getDirection()));
	virtualCamera.setType(Camera::ORTHOGONAL);
	virtualCamera.setFov(100.f); //Dimensions of the "box"
	virtualCamera.setNear(-500.f);
	virtualCamera.setFar(500.f);
	virtualCamera.setAspectRatio(1.f);

	for(size_t i = 0; i < nodes.size(); ++i){
		renderNode(nodes[i], POSITION, virtualCamera, shader, true);
	}

	const float& softness = dirLight.getSoftness();
	if(softness != 0.f)
		PostEffects::gaussianBlur(_vsm, softness / _vsm.getDimensions().x);

	VpMatrix vpm;
	vpm.p = virtualCamera.getProjectionMatrix();
	vpm.v = virtualCamera.getViewMatrix();

	return vpm;
}

void SceneRenderSystem::drawShadowmap(	const Light& pointlight,
										const std::vector<SceneNode*>& nodes) {

	OpenGlControl::Settings ogl;
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	_vscm.clear();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

	for(int face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face != GL_TEXTURE_CUBE_MAP_POSITIVE_X + 6; ++face){
		_vscm.bindAsRendertarget(face);
		glClear(GL_DEPTH_BUFFER_BIT);

		Camera virtualCamera = findCamera(face, pointlight);

		for(size_t i = 0; i < nodes.size(); ++i){
			renderNode(nodes[i], POSITION, virtualCamera, shader, true);
		}

	}
}

Camera SceneRenderSystem::findCamera(	const int& face,
										const Light& pointlight) {

	Camera virtualCamera;
	virtualCamera.setFov(90.f);

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

	virtualCamera.setPosition(pointlight.getPosition());
	virtualCamera.lookAt(pointlight.getPosition() + direction);
	virtualCamera.setHeadUp(headUp);
	virtualCamera.setNear(0.01f);
	virtualCamera.setFar(500.f);

	return virtualCamera;
}

Matrix4f SceneRenderSystem::drawShadowmap(	const SpotLight& spotlight,
											const std::vector<SceneNode*>& nodes) {

	OpenGlControl::Settings ogl;
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	_vsm.clear();
	_vsm.bindAsTarget(true);

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

	//Camera from light's view
	Camera virtualCamera;
	virtualCamera.setFov(spotlight.getConeAngle() * 2.f);
	virtualCamera.setAspectRatio(1.f);
	virtualCamera.setPosition(spotlight.getPosition());
	virtualCamera.lookAt(spotlight.getPosition() + Vector3f(spotlight.getDirection()));
	virtualCamera.setFar(std::sqrt(spotlight.getIntensity() / 0.02f));

	for(size_t i = 0; i < nodes.size(); ++i){
		renderNode(nodes[i], POSITION, virtualCamera, shader, true);
	}

	_vsm.finishMultisampling();

	const float& softness = spotlight.getSoftness();
	if(softness != 0.f)
		PostEffects::gaussianBlur(_vsm, softness / _vsm.getDimensions().x);

	return (virtualCamera.getProjectionMatrix() * virtualCamera.getViewMatrix());
}

void SceneRenderSystem::ambientPart(const Vector3f& ambient) {

	OpenGlControl::Settings ogl;
	ogl.enableDepthtest(false);
	ogl.enableDepthbufferWriting(false);
	ogl.enableCulling(false);
	ogl.enableBlending(true);
	ogl.setBlendMode(OpenGlControl::OVERWRITE);
	OpenGlControl::useSettings(ogl);

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::SINGLECOLOR);
	shader.setUniform("gColor", Vector4f(ambient, 1.f));
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

void SceneRenderSystem::dumpOutDepthGBuffer() {

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

void SceneRenderSystem::drawFullscreenQuad(	const Shader& shader,
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

} /* namespace burn */
