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

SceneRenderSystem::UniformLocations SceneRenderSystem::uniformLocations;

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

	const Vector2ui& screenRes = Vector2ui(	parentWindow.getSettings().getWidth(),
											parentWindow.getSettings().getHeight());

	_diffusePartTexture.create(screenRes, Texture::RGB);
	_specularPartTexture.create(screenRes, Texture::RGB);

	if(!_renderTarget.create(screenRes, RenderTarget::DEPTHBUFFER_16, _diffusePartTexture)){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(12);
	}

	if(!_renderTarget.addColorAttachment(_specularPartTexture, 1)){
		Reporter::report("Unable to create rendertexture!", Reporter::ERROR);
		exit(13);
	}

	_renderTarget.clear();

	if(!_vsm.create(Vector2ui(1024, 1024), Texture::RG32F)){
		Reporter::report("Unable to create VarianceShadowMap!", Reporter::ERROR);
		exit(16);
	}
	_vsm.setFiltering(Texture::MAG_BILINEAR, Texture::MIN_BILINEAR);
	if(!_vsmTarget.create(_vsm.getDimensions(), RenderTarget::DEPTHBUFFER_32, _vsm)){
		Reporter::report("Unable to create VarianceShadowMap RenderTarget!", Reporter::ERROR);
		exit(18);
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

	/////////////////////////////////////////////////////////////////////////////////

	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
		uniformLocations.textureShader.modelMatrixLoc = shader.getUniformLocation("modelMatrix");
		uniformLocations.textureShader.viewMatrixLoc = shader.getUniformLocation("viewMatrix");
		uniformLocations.textureShader.projectionMatrixLoc = shader.getUniformLocation("projectionMatrix");
		uniformLocations.textureShader.mixColorLoc = shader.getUniformLocation("mixColor");
		uniformLocations.textureShader.gSamplerLoc = shader.getUniformLocation("gSampler");
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::DIRECTIONAL_LIGHT);
		uniformLocations.directionalLightShader.modelMatrixLoc = shader.getUniformLocation("modelMatrix");
		uniformLocations.directionalLightShader.viewMatrixLoc = shader.getUniformLocation("viewMatrix");
		uniformLocations.directionalLightShader.projectionMatrixLoc = shader.getUniformLocation("projectionMatrix");
		uniformLocations.directionalLightShader.gSamplerNormalsLoc = shader.getUniformLocation("gSamplerNormals");
		uniformLocations.directionalLightShader.gSamplerPositionsLoc = shader.getUniformLocation("gSamplerPositions");
		uniformLocations.directionalLightShader.gSamplerColorLoc = shader.getUniformLocation("gSamplerColor");
		uniformLocations.directionalLightShader.gSamplerShadowmapLoc = shader.getUniformLocation("gSamplerShadowmap");
		uniformLocations.directionalLightShader.gEyePositionLoc = shader.getUniformLocation("gEyePosition");
		uniformLocations.directionalLightShader.gLightDirectionLoc = shader.getUniformLocation("gLightDirection");
		uniformLocations.directionalLightShader.gLightColorLoc = shader.getUniformLocation("gLightColor");
		uniformLocations.directionalLightShader.gLightIntensityLoc = shader.getUniformLocation("gLightIntensity");
		uniformLocations.directionalLightShader.shadowMatrixLoc = shader.getUniformLocation("shadowMatrix");
		shader.setUniform(uniformLocations.directionalLightShader.modelMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.directionalLightShader.viewMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.directionalLightShader.projectionMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.directionalLightShader.gSamplerNormalsLoc, GBuffer::NORMAL_WS);
		shader.setUniform(uniformLocations.directionalLightShader.gSamplerPositionsLoc, GBuffer::POSITION_WS);
		shader.setUniform(uniformLocations.directionalLightShader.gSamplerColorLoc, GBuffer::DIFFUSE);
		shader.setUniform(uniformLocations.directionalLightShader.gSamplerShadowmapLoc, 8);
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);
		uniformLocations.spotLightShader.modelMatrixLoc = shader.getUniformLocation("modelMatrix");
		uniformLocations.spotLightShader.viewMatrixLoc = shader.getUniformLocation("viewMatrix");
		uniformLocations.spotLightShader.projectionMatrixLoc = shader.getUniformLocation("projectionMatrix");
		uniformLocations.spotLightShader.gSamplerNormalsLoc = shader.getUniformLocation("gSamplerNormals");
		uniformLocations.spotLightShader.gSamplerPositionsLoc = shader.getUniformLocation("gSamplerPositions");
		uniformLocations.spotLightShader.gSamplerColorLoc = shader.getUniformLocation("gSamplerColor");
		uniformLocations.spotLightShader.gSamplerShadowmapLoc = shader.getUniformLocation("gSamplerShadowmap");
		uniformLocations.spotLightShader.gEyePositionLoc = shader.getUniformLocation("gEyePosition");
		uniformLocations.spotLightShader.gLightDirectionLoc = shader.getUniformLocation("gLightDirection");
		uniformLocations.spotLightShader.gLightColorLoc = shader.getUniformLocation("gLightColor");
		uniformLocations.spotLightShader.gLightIntensityLoc = shader.getUniformLocation("gLightIntensity");
		uniformLocations.spotLightShader.shadowMatrixLoc = shader.getUniformLocation("shadowMatrix");
		uniformLocations.spotLightShader.gLightPositionLoc = shader.getUniformLocation("gLightPosition");
		uniformLocations.spotLightShader.gLightConeCosineLoc = shader.getUniformLocation("gLightConeCosine");
		shader.setUniform(uniformLocations.spotLightShader.modelMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.spotLightShader.viewMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.spotLightShader.projectionMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.spotLightShader.gSamplerNormalsLoc, GBuffer::NORMAL_WS);
		shader.setUniform(uniformLocations.spotLightShader.gSamplerPositionsLoc, GBuffer::POSITION_WS);
		shader.setUniform(uniformLocations.spotLightShader.gSamplerColorLoc, GBuffer::DIFFUSE);
		shader.setUniform(uniformLocations.spotLightShader.gSamplerShadowmapLoc, 8);
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);
		uniformLocations.pointLightShader.modelMatrixLoc = shader.getUniformLocation("modelMatrix");
		uniformLocations.pointLightShader.viewMatrixLoc = shader.getUniformLocation("viewMatrix");
		uniformLocations.pointLightShader.projectionMatrixLoc = shader.getUniformLocation("projectionMatrix");
		uniformLocations.pointLightShader.gSamplerNormalsLoc = shader.getUniformLocation("gSamplerNormals");
		uniformLocations.pointLightShader.gSamplerPositionsLoc = shader.getUniformLocation("gSamplerPositions");
		uniformLocations.pointLightShader.gSamplerColorLoc = shader.getUniformLocation("gSamplerColor");
		uniformLocations.pointLightShader.gSamplerShadowcubemapLoc = shader.getUniformLocation("gSamplerShadowcubemap");
		uniformLocations.pointLightShader.gEyePositionLoc = shader.getUniformLocation("gEyePosition");
		uniformLocations.pointLightShader.gLightPositionLoc = shader.getUniformLocation("gLightPosition");
		uniformLocations.pointLightShader.gLightColorLoc = shader.getUniformLocation("gLightColor");
		uniformLocations.pointLightShader.gLightIntensityLoc = shader.getUniformLocation("gLightIntensity");
		shader.setUniform(uniformLocations.pointLightShader.modelMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.pointLightShader.viewMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.pointLightShader.projectionMatrixLoc, Matrix4f(1.f));
		shader.setUniform(uniformLocations.pointLightShader.gSamplerNormalsLoc, GBuffer::NORMAL_WS);
		shader.setUniform(uniformLocations.pointLightShader.gSamplerPositionsLoc, GBuffer::POSITION_WS);
		shader.setUniform(uniformLocations.pointLightShader.gSamplerColorLoc, GBuffer::DIFFUSE);
		shader.setUniform(uniformLocations.pointLightShader.gSamplerShadowcubemapLoc, 8);
	}

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

	if(!n->isLoaded())
		return;

	//Calculate and set model's matrices
	Matrix4f normalMatrix = glm::transpose(glm::inverse(node->getModelMatrix()));
	shader.setUniform(modelMatrixLoc, node->getModelMatrix());
	shader.setUniform(normalMatrixLoc, normalMatrix);

	//StaticMeshNode consists of several meshes
	std::vector<Mesh>* meshes = n->getMeshesPointer();
	for(size_t i = 0; i < meshes->size(); ++i){

		//Retreive the mesh
		Mesh* mesh = &((*meshes)[i]);

		//Set OpenGL according to mesh's flags
		mesh->getMaterial().setOpenGlByFlags();

		//Set uniforms depending on mesh's material
		if(mesh->getMaterial().getType() == Material::SOLID_COLOR){
			shader.setUniform(diffuseTypeLoc, DIFFUSE_TYPE_COLORED);
			shader.setUniform(meshColorLoc, mesh->getMaterial().getDiffuseColor());
		}else{
			shader.setUniform(diffuseTypeLoc, DIFFUSE_TYPE_TEXTURED);    //Type = TEXTURED
			mesh->getTexture().bind();
		}

		//Bind bufferobjects according to renderflags
		glEnableVertexAttribArray(_vboIndices[POSITION_ARRAY_INDEX]);
		glEnableVertexAttribArray(_vboIndices[NORMAL_ARRAY_INDEX]);
		glEnableVertexAttribArray(_vboIndices[UV_ARRAY_INDEX]);

		//Reset flags to original parameter
		int flags = constflags;

		if(flags >= UV){
			mesh->getUvVbo().bind();
			glVertexAttribPointer(_vboIndices[UV_ARRAY_INDEX], 2,
			GL_FLOAT,
									GL_FALSE, 0, (void*)0);
			flags -= UV;
		}
		if(flags >= NORMAL){
			mesh->getNormalVbo().bind();
			glVertexAttribPointer(_vboIndices[NORMAL_ARRAY_INDEX], 3,
			GL_FLOAT,
									GL_FALSE, 0, (void*)0);
			flags -= NORMAL;
		}
		if(flags >= POSITION){
			mesh->getPositionVbo().bind();
			glVertexAttribPointer(_vboIndices[POSITION_ARRAY_INDEX], 3,
			GL_FLOAT,
									GL_FALSE, 0, (void*)0);
			flags -= POSITION;
		}

		OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh->getVertexCount(), shader);

		glDisableVertexAttribArray(_vboIndices[POSITION_ARRAY_INDEX]);
		glDisableVertexAttribArray(_vboIndices[NORMAL_ARRAY_INDEX]);
		glDisableVertexAttribArray(_vboIndices[UV_ARRAY_INDEX]);

	}

	//}

}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void SceneRenderSystem::render(	const GLuint& targetFramebuffer,    ///< Window is default
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

	OpenGlControl::bindDrawBuffer(targetFramebuffer);
	_gBuffer.bindAsSource();
	if(mode == COMPOSITION || mode == LIGHTING){

		Window::PolygonMode polygonMode = _window.getPolygonMode();
		_window.setPolygonMode(Window::FILLED);

		if(mode != LIGHTING){
			//Copy diffuse gBuffer to windowframebuffer:

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
			shader.setUniform(uniformLocations.textureShader.modelMatrixLoc, Matrix4f(1.f));
			shader.setUniform(uniformLocations.textureShader.viewMatrixLoc, Matrix4f(1.f));
			shader.setUniform(uniformLocations.textureShader.projectionMatrixLoc, Matrix4f(1.f));
			shader.setUniform(uniformLocations.textureShader.mixColorLoc, Vector3f(1.f));
			shader.setUniform(uniformLocations.textureShader.gSamplerLoc, GBuffer::DIFFUSE);

			OpenGlControl::bindDrawBuffer(targetFramebuffer);
			glViewport(0, 0, targetFramebufferDimensions.x, targetFramebufferDimensions.y);

			drawFullscreenQuad(shader, OpenGlControl::Settings());

		}

		if(isLightingEnabled)
			lightPass(targetFramebuffer, targetFramebufferDimensions, camera, nodes, lights, ambient, mode == LIGHTING);

		_window.setPolygonMode(polygonMode);

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
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
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
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
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
							targetFramebufferDimensions.x,
							targetFramebufferDimensions.y,
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
	}else if(mode == DEPTH){
		dumpOutDepthGBuffer(targetFramebuffer, targetFramebufferDimensions);    //Special, because no GL_COLOR_BUFFER

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

		shader.setUniform(uniformLocations.directionalLightShader.gEyePositionLoc, camera.getPosition());
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);
		shader.setUniform(uniformLocations.pointLightShader.gEyePositionLoc, camera.getPosition());
	}
	{
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);
		shader.setUniform(uniformLocations.spotLightShader.gEyePositionLoc, camera.getPosition());
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
			Matrix4f shadowMatrix = drawShadowmap(*light, nodes);

			//Render light
			_renderTarget.bind();
			_vsm.bind(8);
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::DIRECTIONAL_LIGHT);
			shader.setUniform(	uniformLocations.directionalLightShader.gLightDirectionLoc,
								Vector3f(light->getDirection()));
			shader.setUniform(uniformLocations.directionalLightShader.gLightColorLoc, light->getColor());
			shader.setUniform(uniformLocations.directionalLightShader.gLightIntensityLoc, light->getIntensity());
			shader.setUniform(uniformLocations.directionalLightShader.shadowMatrixLoc, shadowMatrix);

			drawFullscreenQuad(shader, toLightPassOgl);

		}else if(typeid(*(lights[i])) == typeid(Light)){

			Light* light = static_cast<Light*>(lights[i]);

			//Render shadowmap:
			drawShadowmap(*light, nodes);

			//Render light
			_vscm.bind(8);
			_renderTarget.bind();
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);
			shader.setUniform(uniformLocations.pointLightShader.gLightPositionLoc, light->getPosition());
			shader.setUniform(uniformLocations.pointLightShader.gLightColorLoc, light->getColor());
			shader.setUniform(uniformLocations.pointLightShader.gLightIntensityLoc, light->getIntensity());

			drawFullscreenQuad(shader, toLightPassOgl);

		}else{    //Spotlight

			SpotLight* light = static_cast<SpotLight*>(lights[i]);

			//Render shadowmap:
			Matrix4f shadowMatrix = drawShadowmap(*light, nodes);

			//Render light
			_renderTarget.bind();
			_vsm.bind(8);
			float lightConeCosine = std::cos(light->getConeAngle() / (180.f / 3.1415f));

			//glGenerateMipmap(GL_TEXTURE_2D);

			const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);
			shader.setUniform(uniformLocations.spotLightShader.gLightDirectionLoc, Vector3f(light->getDirection()));
			shader.setUniform(uniformLocations.spotLightShader.gLightPositionLoc, light->getPosition());
			shader.setUniform(uniformLocations.spotLightShader.gLightColorLoc, light->getColor());
			shader.setUniform(uniformLocations.spotLightShader.gLightIntensityLoc, light->getIntensity());
			shader.setUniform(uniformLocations.spotLightShader.gLightConeCosineLoc, lightConeCosine);
			shader.setUniform(uniformLocations.spotLightShader.shadowMatrixLoc, shadowMatrix);

			drawFullscreenQuad(shader, toLightPassOgl);

		}

	}

	///////////////////////////////////////////////////////////////

	//Multiply result with the scene
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
	shader.setUniform(uniformLocations.textureShader.modelMatrixLoc, Matrix4f(1.f));
	shader.setUniform(uniformLocations.textureShader.viewMatrixLoc, Matrix4f(1.f));
	shader.setUniform(uniformLocations.textureShader.projectionMatrixLoc, Matrix4f(1.f));
	shader.setUniform(uniformLocations.textureShader.mixColorLoc, Vector3f(1.f));

	OpenGlControl::bindDrawBuffer(targetFramebuffer);
	glViewport(0, 0, targetFramebufferDimensions.x, targetFramebufferDimensions.y);

	if(!dumpLighting){
		//Compose with diffuse part:
		_diffusePartTexture.bind(1);
		shader.setUniform(uniformLocations.textureShader.gSamplerLoc, 1);    //sample from diffuse
		toLightPassOgl.setBlendMode(OpenGlControl::MULTIPLY);
		drawFullscreenQuad(shader, toLightPassOgl);

		//Compose with specular part:
		_specularPartTexture.bind(1);
		toLightPassOgl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, toLightPassOgl);

	}else{
		_diffusePartTexture.bind(1);
		shader.setUniform(uniformLocations.textureShader.gSamplerLoc, 1);    //sample from diffuse
		toLightPassOgl.setBlendMode(OpenGlControl::OVERWRITE);
		drawFullscreenQuad(shader, toLightPassOgl);

		_specularPartTexture.bind(1);
		toLightPassOgl.setBlendMode(OpenGlControl::ADD);
		drawFullscreenQuad(shader, toLightPassOgl);
	}

	OpenGlControl::useSettings(OpenGlControl::Settings());
}

Matrix4f SceneRenderSystem::drawShadowmap(	const DirectionalLight& dirLight,
											const std::vector<SceneNode*>& nodes) {

	OpenGlControl::Settings ogl;
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	_vsmTarget.clear();
	_vsmTarget.bind();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

	Camera virtualCamera;
	virtualCamera.lookAt(Vector3f(dirLight.getDirection()));
	virtualCamera.setType(Camera::ORTHOGONAL);
	virtualCamera.setFov(100.f);    //Dimensions of the "box"
	virtualCamera.setNear(-500.f);
	virtualCamera.setFar(500.f);
	virtualCamera.setAspectRatio(1.f);

	for(size_t i = 0; i < nodes.size(); ++i){
		renderNode(nodes[i], POSITION, virtualCamera, shader, true);
	}

	if(dirLight.isSofteningShadow())
		PostEffects::gaussianBlur(_vsm);

	return virtualCamera.getProjectionMatrix() * virtualCamera.getViewMatrix();
}

void SceneRenderSystem::drawShadowmap(	const Light& pointlight,
										const std::vector<SceneNode*>& nodes) {

	OpenGlControl::Settings ogl;
	ogl.setClearColor(Vector4f(0.f));
	OpenGlControl::useSettings(ogl);

	_vscmTarget.clear();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::VSM_DRAW);

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

	_vsmTarget.clear();
	_vsmTarget.bind();

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

	//_vsm.finishMultisampling();

	if(spotlight.isSofteningShadow())
		PostEffects::gaussianBlur(_vsm);

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
