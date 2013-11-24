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

#include <Burngine/Graphics/Scene/SceneRenderSystem.h>

#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

//RenderFlag to ArrayIndex Mapping:
#define POSITION_ARRAY_INDEX 0
#define NORMAL_ARRAY_INDEX 1
#define UV_ARRAY_INDEX 2

//Static members
GLuint SceneRenderSystem::_vboIndices[];
VertexBufferObject SceneRenderSystem::_fullscreenVbo;

//Variables in shader
#define DIFFUSE_TYPE_COLORED 1
#define DIFFUSE_TYPE_TEXTURED 0

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

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void SceneRenderSystem::render(	SceneNode* node,
								const int& constflags,
								const Camera& camera,
								const Shader& shader,
								bool onlyShadowCasters) {

	ensureContext();

	//Set camera matrices
	shader.setUniform("viewMatrix", camera.getViewMatrix());
	shader.setUniform("projectionMatrix", camera.getProjectionMatrix());

	if(typeid(*(node)) == typeid(StaticMeshNode)){

		//Cast this ugly pointer to a real object
		StaticMeshNode n = *(static_cast<StaticMeshNode*>(node));

		//Try updating its data
		n.update();
		const Model& model = n.getModel();
		if(!model.isUpdated())
			return; //Don't render not updated nodes

		//Calculate and set model's matrices
		Matrix4f normalMatrix = glm::transpose(glm::inverse(node->getModelMatrix()));
		shader.setUniform("modelMatrix", node->getModelMatrix());
		shader.setUniform("normalMatrix", normalMatrix);

		//StaticMeshNode consists of several meshes
		for(size_t i = 0; i < model.getMeshCount(); ++i){

			//Retreive the mesh
			const Mesh& mesh = model.getMesh(i);

			//If wanted, skip this mesh if it casts no shadows
			if(onlyShadowCasters && !mesh.getMaterial().isFlagSet(Material::CAST_SHADOWS))
				continue;

			//Set uniforms depending on mesh's material
			if(mesh.getMaterial().getType() == Material::SOLID_COLOR){
				shader.setUniform("diffuseType", DIFFUSE_TYPE_COLORED);
				shader.setUniform("meshColor", mesh.getMaterial().getDiffuseColor());
			}else{
				shader.setUniform("diffuseType", DIFFUSE_TYPE_TEXTURED); //Type = TEXTURED
				mesh.getTexture().bindAsSource();
			}

			//Set OpenGL according to mesh's flags
			mesh.getMaterial().setOpenGlByFlags();

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

void SceneRenderSystem::renderTextureToFramebuffer(	const BaseTexture& source,
													const GLuint& targetFramebuffer,
													const GLuint& textureAttachmentId,
													const OpenGlControl::BlendMode& blendmode) {

	if(!_fullscreenVbo.isCreated()){
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

	ensureContext();

	OpenGlControl::Settings ogl;
	ogl.setBlendMode(blendmode);
	ogl.enableCulling(false);
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	OpenGlControl::useSettings(ogl);

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));
	shader.setUniform("mixColor", Vector3f(1.f));
	shader.setUniform("gSampler", static_cast<int>(textureAttachmentId));

	source.bindAsSource(0); //Bind texture to unit 0
	OpenGlControl::bindDrawBuffer(targetFramebuffer);

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
