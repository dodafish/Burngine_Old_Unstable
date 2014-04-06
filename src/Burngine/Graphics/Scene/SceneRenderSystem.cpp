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
#include <Burngine/System/Rotation.h>
#include <Burngine/System/RotationUtil.h>

//SceneNodes
#include <Burngine/Graphics/Scene/SkyBox.h>

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

SceneRenderSystem::SceneRenderSystem() {
	SceneRenderSystem::setVboIndex(POSITION, 0);
	SceneRenderSystem::setVboIndex(NORMAL, 1);
	SceneRenderSystem::setVboIndex(UV, 2);

	for(int i = 0; i != 3; ++i){
		if(!_vsm[i].create(Vector2ui(1024, 1024), Texture::RG32F)){
			Reporter::report("Unable to create VarianceShadowMap!", Reporter::ERROR);
			exit(16);
		}
		_vsm[i].setFiltering(Texture::MAG_BILINEAR, Texture::MIN_BILINEAR);
		if(!_vsmTarget[i].create(_vsm[i].getDimensions(), RenderTarget::DEPTHBUFFER_32, _vsm[i])){
			Reporter::report("Unable to create VarianceShadowMap RenderTarget!", Reporter::ERROR);
			exit(18);
		}
	}

	if(!_vscm.create(Vector2ui(512, 512), Texture::RG32F)){
		Reporter::report("Unable to create VarianceShadowCubeMap!", Reporter::ERROR);
		exit(17);
	}

	_vscm.setFiltering(Texture::MAG_BILINEAR, Texture::MIN_BILINEAR);
	if(!_vscmTarget.create(_vscm.getDimensions(), CubeRenderTarget::DEPTHBUFFER_32, _vscm)){
		Reporter::report("Unable to create VarianceShadowCubeMap RenderTarget!", Reporter::ERROR);
		exit(19);
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

	//Get uniform locations that are used. Avoid unnecessary lookups
	const GLint viewMatrixLoc = shader.getUniformLocation("viewMatrix");
	const GLint projectionMatrixLoc = shader.getUniformLocation("projectionMatrix");
	const GLint modelMatrixLoc = shader.getUniformLocation("modelMatrix");
	const GLint normalMatrixLoc = shader.getUniformLocation("normalMatrix");
	const GLint diffuseTypeLoc = shader.getUniformLocation("diffuseType");
	const GLint meshColorLoc = shader.getUniformLocation("meshColor");

	//Set camera matrices
	shader.setUniform(viewMatrixLoc, camera.getViewMatrix());
	shader.setUniform(projectionMatrixLoc, camera.getProjectionMatrix());

	//if(typeid(*(node)) == typeid(StaticMeshNode)){

	//Cast this ugly pointer to a real object
	StaticMeshNode* n = (static_cast<StaticMeshNode*>(node));

	if(!n->getModel().isLoaded())
		return;

	//Calculate and set model's matrices
	Matrix4f normalMatrix = glm::transpose(glm::inverse(node->getModelMatrix()));
	shader.setUniform(modelMatrixLoc, node->getModelMatrix());
	shader.setUniform(normalMatrixLoc, normalMatrix);

	//StaticMeshNode consists of several meshes
	const std::vector<std::shared_ptr<Mesh>>& meshes = n->getModel().getMeshes();
	for(size_t i = 0; i < meshes.size(); ++i){

		//Retreive the mesh
		const Mesh& mesh = *(meshes[i].get());

		//Set OpenGL according to mesh's flags
		mesh.getMaterial().setOpenGlByFlags();

		//Set uniforms depending on mesh's material
		if(mesh.getMaterial().getType() == Material::SOLID_COLOR){
			shader.setUniform(diffuseTypeLoc, DIFFUSE_TYPE_COLORED);
			shader.setUniform(meshColorLoc, mesh.getMaterial().getDiffuseColor());
		}else{
			shader.setUniform(diffuseTypeLoc, DIFFUSE_TYPE_TEXTURED);    //Type = TEXTURED
			mesh.getTexture().bind();
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

	//}

}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void SceneRenderSystem::adjustRenderTextures(const Vector2ui& resolution) {

	//Don't do anything when the resolution fits
	if(resolution == _currentRenderTexturesResolution)
		return;

	//Recreate GBuffer
	if(!_gBuffer.create(resolution)){
		Reporter::report("Unable to create gBuffer!", Reporter::ERROR);
		exit(11);
	}

	/*
	 * Recreate rendertextures:
	 */

	_sceneTexture.create(resolution, Texture::RGB);
	_sceneTextureTarget.create(resolution, RenderTarget::NO_DEPTHBUFFER, _sceneTexture);

	_diffusePartTexture.create(resolution, Texture::RGB);
	_specularPartTexture.create(resolution, Texture::RGB);

	if(!_renderTarget.create(resolution, RenderTarget::DEPTHBUFFER_16, _diffusePartTexture)){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(12);
	}

	if(!_renderTarget.addColorAttachment(_specularPartTexture, 1)){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(13);
	}

	_renderTarget.clear();

	//Remember the current resolution
	_currentRenderTexturesResolution = resolution;
}

void SceneRenderSystem::render(	const GLuint& targetFramebuffer,    ///< Window is default
								const Vector2ui& targetFramebufferDimensions,
								const Camera& camera,
								const RenderMode& mode,
								const std::vector<SceneNode*>& nodes,
								const std::vector<Light*>& lights,
								const Vector3f& ambient,
								const SkyBox& skyBox,
								bool isLightingEnabled) {

	ensureContext();

	if(nodes.size() == 0)
		return;

	//Make the rendertextures fit the resolution (This is slow, when it has to adjust!)
	//Don't use one Scene for different resolutions, unless you don't care about performance!
	adjustRenderTextures(targetFramebufferDimensions);

	_sceneTextureTarget.clear();

	drawGBuffers(camera, nodes, skyBox);

	_sceneTextureTarget.bind();
	_gBuffer.bindAsSource();
	if(mode == COMPOSITION || mode == LIGHTING){

		OpenGlControl::PolygonMode polygonMode = OpenGlControl::getPolygonMode();
		OpenGlControl::setPolygonMode(OpenGlControl::FILLED);

		if(mode != LIGHTING){
			//Copy diffuse gBuffer to windowframebuffer:

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
			shader.setUniform("modelMatrix", Matrix4f(1.f));
			shader.setUniform("viewMatrix", Matrix4f(1.f));
			shader.setUniform("projectionMatrix", Matrix4f(1.f));
			shader.setUniform("mixColor", Vector4f(1.f));
			shader.setUniform("gSampler", GBuffer::DIFFUSE);

			_sceneTextureTarget.bind();
			glViewport(0, 0, targetFramebufferDimensions.x, targetFramebufferDimensions.y);

			drawFullscreenQuad(shader, OpenGlControl::Settings());

		}

		if(isLightingEnabled)
			lightPass(123, targetFramebufferDimensions, camera, nodes, lights, ambient, mode == LIGHTING);

		OpenGlControl::setPolygonMode(polygonMode);

		OpenGlControl::useSettings(OpenGlControl::Settings());

		OpenGlControl::bindReadBuffer(_sceneTextureTarget.getFramebufferId());
		_sceneTexture.bind(0);
		OpenGlControl::bindDrawBuffer(targetFramebuffer);
		glBlitFramebuffer(	0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);

	}else if(mode == DIFFUSE){
		_gBuffer.setSourceBuffer(GBuffer::DIFFUSE);
		OpenGlControl::bindDrawBuffer(targetFramebuffer);
		glBlitFramebuffer(	0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
	}else if(mode == NORMAL_WS){
		_gBuffer.setSourceBuffer(GBuffer::NORMAL_WS);
		OpenGlControl::bindDrawBuffer(targetFramebuffer);
		glBlitFramebuffer(	0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
	}else if(mode == POSITION_WS){
		_gBuffer.setSourceBuffer(GBuffer::POSITION_WS);
		OpenGlControl::bindDrawBuffer(targetFramebuffer);
		glBlitFramebuffer(	0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							0,
							0,
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
	}else if(mode == DEPTH){
		dumpOutDepthGBuffer(targetFramebuffer, targetFramebufferDimensions);    //Special, because no GL_COLOR_BUFFER

	}

}

void SceneRenderSystem::drawGBuffers(	const Camera& camera,
										const std::vector<SceneNode*>& nodes,
										const SkyBox& skyBox) {

	_gBuffer.clear();
	_gBuffer.bindAsTarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::G_BUFFER);

	for(size_t i = 0; i < nodes.size(); ++i){
		renderNode(nodes[i], POSITION | NORMAL | UV, camera, shader);
	}

	skyBox.draw(camera);

}

void SceneRenderSystem::lightPass(	const GLuint& targetFramebuffer,    ///< Window as default
									const Vector2ui& targetFramebufferDimensions,
									const Camera& camera,
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

		shader.setUniform("gEyePosition", camera.getPosition());
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);
		shader.setUniform("gEyePosition", camera.getPosition());
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);
		shader.setUniform("gEyePosition", camera.getPosition());
	}
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////

	//Render all dirlights together into rendertexture
	_renderTarget.clear();
	_renderTarget.bind();		// <- Diffuse light (attachment 0); specular (1)

	ambientPart(ambient);

	for(size_t i = 0; i < lights.size(); ++i){

		if(typeid(*(lights[i])) == typeid(DirectionalLight)){

			DirectionalLight* light = static_cast<DirectionalLight*>(lights[i]);

			//Render shadowmap:
			HiMidLowResMatrices shadowMatrices = drawShadowmap(*light, nodes, camera.getPosition());

			//Render light
			_renderTarget.bind();
			_vsm[0].bind(8);
			_vsm[1].bind(9);
			_vsm[2].bind(10);

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::DIRECTIONAL_LIGHT);

			shader.setUniform("modelMatrix", Matrix4f(1.f));
			shader.setUniform("viewMatrix", Matrix4f(1.f));
			shader.setUniform("projectionMatrix", Matrix4f(1.f));
			shader.setUniform("gSamplerNormals", GBuffer::NORMAL_WS);
			shader.setUniform("gSamplerPositions", GBuffer::POSITION_WS);
			shader.setUniform("gSamplerShadowmapHI", 8);
			shader.setUniform("gSamplerShadowmapMID", 9);
			shader.setUniform("gSamplerShadowmapLOW", 10);

			shader.setUniform("gLightDirection", Vector3f(light->getDirection()));
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());
			shader.setUniform("shadowMatrixHI", shadowMatrices.matrices[0]);
			shader.setUniform("shadowMatrixMID", shadowMatrices.matrices[1]);
			shader.setUniform("shadowMatrixLOW", shadowMatrices.matrices[2]);
			shader.setUniform("shadowViewMatrix", shadowMatrices.view);

			drawFullscreenQuad(shader, toLightPassOgl);

		}else if(typeid(*(lights[i])) == typeid(Light)){

			Light* light = static_cast<Light*>(lights[i]);

			//Render shadowmap:
			drawShadowmap(*light, nodes);

			//Render light
			_vscm.bind(8);
			_renderTarget.bind();
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);

			shader.setUniform("modelMatrix", Matrix4f(1.f));
			shader.setUniform("viewMatrix", Matrix4f(1.f));
			shader.setUniform("projectionMatrix", Matrix4f(1.f));
			shader.setUniform("gSamplerNormals", GBuffer::NORMAL_WS);
			shader.setUniform("gSamplerPositions", GBuffer::POSITION_WS);
			shader.setUniform("gSamplerShadowcubemap", 8);

			shader.setUniform("gLightPosition", light->getPosition());
			shader.setUniform("gLightColor", light->getColor());
			shader.setUniform("gLightIntensity", light->getIntensity());

			drawFullscreenQuad(shader, toLightPassOgl);

		}else{    //Spotlight

			SpotLight* light = static_cast<SpotLight*>(lights[i]);

			//Render shadowmap:
			Matrix4f shadowMatrix = drawShadowmap(*light, nodes);

			//Render light
			_renderTarget.bind();
			_vsm[0].bind(8);
			float lightConeCosine = std::cos(light->getConeAngle() / (180.f / 3.1415f));

			//glGenerateMipmap(GL_TEXTURE_2D);

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);

			shader.setUniform("modelMatrix", Matrix4f(1.f));
			shader.setUniform("viewMatrix", Matrix4f(1.f));
			shader.setUniform("projectionMatrix", Matrix4f(1.f));
			shader.setUniform("gSamplerNormals", GBuffer::NORMAL_WS);
			shader.setUniform("gSamplerPositions", GBuffer::POSITION_WS);
			shader.setUniform("gSamplerShadowmap", 8);

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
	shader.setUniform("mixColor", Vector4f(1.f));

	_sceneTextureTarget.bind();
	glViewport(0, 0, targetFramebufferDimensions.x, targetFramebufferDimensions.y);

	if(!dumpLighting){
		//Compose with diffuse part:
		_diffusePartTexture.bind(1);
		shader.setUniform("gSampler", 1);    //sample from diffuse
		toLightPassOgl.setBlendMode(OpenGlControl::MULTIPLY);
		drawFullscreenQuad(shader, toLightPassOgl);

		//Compose with specular part:
		_specularPartTexture.bind(1);
		toLightPassOgl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, toLightPassOgl);

	}else{
		_diffusePartTexture.bind(1);
		shader.setUniform("gSampler", 1);    //sample from diffuse
		toLightPassOgl.setBlendMode(OpenGlControl::OVERWRITE);
		drawFullscreenQuad(shader, toLightPassOgl);

		_specularPartTexture.bind(1);
		toLightPassOgl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, toLightPassOgl);
	}

	OpenGlControl::useSettings(OpenGlControl::Settings());
}

SceneRenderSystem::HiMidLowResMatrices SceneRenderSystem::drawShadowmap(const DirectionalLight& dirLight,
																		const std::vector<SceneNode*>& nodes,
																		const Vector3f& cameraPosition) {

	OpenGlControl::Settings ogl;
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	HiMidLowResMatrices mats;
	for(int q = 0; q != 3; ++q){
		OpenGlControl::useSettings(ogl);
		_vsmTarget[q].clear();
		_vsmTarget[q].bind();

		const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

		shader.setUniform("onlyZ", true);

		Camera virtualCamera;
		virtualCamera.setRotation(RotationUtil::RotationBetweenVectors(	Vector3f(0.f, 0.f, -1.f),
																		Vector3f(dirLight.getDirection())));
		virtualCamera.setType(Camera::ORTHOGONAL);
		//Dimensions of the "box"
		if(q == 0)
			virtualCamera.setFov(50.f);
		else if(q == 1){
			virtualCamera.setFov(100.f);
		}else{
			virtualCamera.setFov(250.f);
		}

		virtualCamera.setNear(-750.f);
		virtualCamera.setFar(750.f);
		virtualCamera.setAspectRatio(1.f);
		virtualCamera.setPosition(cameraPosition);

		for(size_t i = 0; i < nodes.size(); ++i){
			renderNode(nodes[i], POSITION, virtualCamera, shader, true);
		}

		mats.matrices[q] = virtualCamera.getProjectionMatrix() * virtualCamera.getViewMatrix();
		mats.view = virtualCamera.getViewMatrix();
	}

	if(dirLight.isSofteningShadow()){
		PostEffects::gaussianBlur(_vsm[0]);
		OpenGlControl::useSettings(ogl);
		_vsmTarget[1].bind();
		PostEffects::gaussianBlur(_vsm[1]);
		OpenGlControl::useSettings(ogl);
		_vsmTarget[2].bind();
		PostEffects::gaussianBlur(_vsm[2]);
	}

	return mats;
}

void SceneRenderSystem::drawShadowmap(	const Light& pointlight,
										const std::vector<SceneNode*>& nodes) {

	OpenGlControl::Settings ogl;
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	_vscmTarget.clear();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

	shader.setUniform("onlyZ", false);

	for(int face = 0; face != 6; ++face){
		_vscmTarget.bind(face);
		glClear(GL_DEPTH_BUFFER_BIT);

		Camera virtualCamera = findCamera(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, pointlight);

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
	virtualCamera.setRotation(RotationUtil::RotationBetweenVectors(Vector3f(0.f, 0.f, -1.f), direction));
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

	_vsmTarget[0].clear();
	_vsmTarget[0].bind();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

	shader.setUniform("onlyZ", false);

	//Camera from light's view
	Camera virtualCamera;
	virtualCamera.setFov(spotlight.getConeAngle() * 2.f);
	virtualCamera.setAspectRatio(1.f);
	virtualCamera.setPosition(spotlight.getPosition());
	virtualCamera.setRotation(RotationUtil::RotationBetweenVectors(	Vector3f(0.f, 0.f, -1.f),
																	Vector3f(spotlight.getDirection())));
	virtualCamera.setFar(std::sqrt(spotlight.getIntensity() / 0.02f));

	for(size_t i = 0; i < nodes.size(); ++i){
		renderNode(nodes[i], POSITION, virtualCamera, shader, true);
	}

	//_vsm.finishMultisampling();

	if(spotlight.isSofteningShadow())
		PostEffects::gaussianBlur(_vsm[0]);

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

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::AMBIENT);
	shader.setUniform("gColor", Vector4f(ambient, 1.f));
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));
	_gBuffer.bindDepthBufferAsSourceTexture();

	drawFullscreenQuad(shader, ogl);

}

void SceneRenderSystem::dumpOutDepthGBuffer(const GLuint& targetFramebuffer,    ///< Window as default
											const Vector2ui& targetFramebufferDimensions) {

	OpenGlControl::Settings ogl;
	ogl.enableDepthtest(false);
	ogl.enableDepthbufferWriting(false);
	ogl.enableCulling(false);

	OpenGlControl::bindDrawBuffer(targetFramebuffer);
	glViewport(0, 0, targetFramebufferDimensions.x, targetFramebufferDimensions.y);
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
