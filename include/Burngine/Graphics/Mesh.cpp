/*
 * Mesh.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Mesh.h"

#include "OpenGL.h"
#include "Window.h"

#include <zlib.h>

#include "../extern/assimp/Importer.hpp"      // C++ importer interface
#include "../extern/assimp/scene.h"           // Output data structure
#include "../extern/assimp/postprocess.h"     // Post processing flags
#include <iostream>

namespace burn {

Mesh::Mesh() :
				_needUpdate(false),
				_vertexPositionBuffer(0),
				_vertexColorBuffer(0),
				_vertexUvBuffer(0) {

	if(Window::isContextCreated()){
		glGenBuffers(1, &_vertexPositionBuffer);
		glGenBuffers(1, &_vertexColorBuffer);
		glGenBuffers(1, &_vertexUvBuffer);
	}

}

Mesh::~Mesh() {

	if(Window::isContextCreated()){
		glDeleteBuffers(1, &_vertexPositionBuffer);
		glDeleteBuffers(1, &_vertexColorBuffer);
		glDeleteBuffers(1, &_vertexUvBuffer);
	}

}

bool Mesh::loadFromFile(const std::string& file) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file.c_str(),
			aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
					| aiProcess_SortByPType);

	if(!scene){
		std::cout << "Error while loading asset: " << importer.GetErrorString() << "\n";
		return false;
	}else{
		std::cout << "Successfully loaded asset: " << file << "\n";
	}

	//Asset successfully loaded.

	_vertices.clear();
	clearTextures();

	std::cout << "----- Total count of meshes: " << scene->mNumMeshes << "\n";
	for(unsigned int i = 0; i < scene->mNumMeshes; ++i){

		aiMesh* mesh = scene->mMeshes[i];

		std::cout << "----- Total count of faces for mesh #" << i << ": " << mesh->mNumFaces << "\n";
		for(unsigned int j = 0; j < mesh->mNumFaces; ++j){

			const aiFace& face = mesh->mFaces[j];
			for(unsigned int k = 0; k != 3; ++k){
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];

				aiVector3D uv =
						mesh->HasTextureCoords(0) ?
								mesh->mTextureCoords[0][face.mIndices[k]] : aiVector3D(0.0f, 0.0f, 0.0f);

				aiVector3D normal =
						mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);

				_vertices.push_back(
						Vertex(Vector3f(pos.x, pos.y, pos.z), Vector3f(0, 1, 0), Vector2f(uv.x, uv.y),
								Vector3f(normal.x, normal.y, normal.z)));
			}

		}

	}

	std::cout << "----- Total count of materials: " << scene->mNumMaterials << "\n";
	for(unsigned int i = 0; i < scene->mNumMaterials; ++i){

		aiMaterial* material = scene->mMaterials[i];

		unsigned int textureIndex = 0;
		aiString assimpFile;
		if(material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &assimpFile) == AI_SUCCESS){
			std::string file = assimpFile.data; //convert string-type
			std::cout << "Attempting to load texture: " << file << "\n";

			_textures.push_back(Texture());
			if(!_textures.back().loadFromFile(file)){
				std::cout << "Failed to load texture: " << file << "\n";
				_textures.pop_back();
				return false;
			}else{
				std::cout << "Texture '" << file << "' successfully loaded.\n";
			}

		}else{
			std::cout << "Material texture is invalid.\n";
		}

	}

	_needUpdate = true;
	data();

	return true;
}

size_t Mesh::getVertexCount() const {
	return _vertices.size();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
	_vertices = vertices;
	_needUpdate = true;

	data();
}

const GLuint& Mesh::getPositionBuffer() {
	if(_needUpdate)
		data();
	return _vertexPositionBuffer;
}

const GLuint& Mesh::getColorBuffer() {
	if(_needUpdate)
		data();
	return _vertexColorBuffer;
}

const GLuint& Mesh::getUvBuffer() {
	if(_needUpdate)
		data();
	return _vertexUvBuffer;
}

void Mesh::setTexture(const Texture& tex) {
	_textures.push_back(tex);
}

const Texture& Mesh::getTexture(const size_t& index) const {
	return (_textures.size() > index) ? _textures[index] : _textures[0];
}

void Mesh::clearTextures() {
	_textures.clear();
}

size_t Mesh::getTextureCount() const {
	return _textures.size();
}

void Mesh::data() {
	if(Window::isContextCreated() && _vertices.size() != 0){

		if(_vertexPositionBuffer == 0){
			glGenBuffers(1, &_vertexPositionBuffer);
			glGenBuffers(1, &_vertexColorBuffer);
			glGenBuffers(1, &_vertexUvBuffer);
		}

		std::vector<GLfloat> pos, col, uv;
		for(size_t i = 0; i < _vertices.size(); ++i){
			pos.push_back(_vertices[i].getPosition().x);
			pos.push_back(_vertices[i].getPosition().y);
			pos.push_back(_vertices[i].getPosition().z);

			col.push_back(_vertices[i].getColor().r);
			col.push_back(_vertices[i].getColor().g);
			col.push_back(_vertices[i].getColor().b);

			uv.push_back(_vertices[i].getUv().x);
			uv.push_back(_vertices[i].getUv().y);
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vertexPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pos.size(), &pos[0],
		GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * col.size(), &col[0],
		GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexUvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uv.size(), &uv[0],
		GL_STATIC_DRAW);

		_needUpdate = false;
	}
}

} /* namespace burn */
