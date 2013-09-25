/*
 * Model.cpp
 *
 *  Created on: 17.09.2013
 *      Author: Dominik
 */

#include "Model.h"

#include <zlib.h>

#include "../extern/assimp/Importer.hpp"      // C++ importer interface
#include "../extern/assimp/scene.h"           // Output data structure
#include "../extern/assimp/postprocess.h"     // Post processing flags
#include <iostream>

namespace burn {

Model::Model() {
}

Model::~Model() {
}

size_t Model::getMeshCount() const {
	return _meshes.size();
}

const Mesh& Model::getMesh(const size_t& index) const {
	return *(_meshes[index]);
}

bool Model::loadFromFile(const std::string& file) {

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

	_meshes.clear();

	std::cout << "--- Total count of meshes: " << scene->mNumMeshes << "\n";
	std::vector<Vertex> vertices;
	for(unsigned int i = 0; i < scene->mNumMeshes; ++i){

		std::cout << "----- Loading mesh #" << i << "\n";

		aiMesh* mesh = scene->mMeshes[i];

		vertices.clear();

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

				Vector3f color = Vector3f(0, 1, 0);
				if(mesh->mColors[0] != 0){
					color = Vector3f(mesh->mColors[0]->r, mesh->mColors[0]->g, mesh->mColors[0]->b);
				}

				vertices.push_back(
						Vertex(Vector3f(pos.x, pos.y, pos.z), color, Vector2f(uv.x, uv.y),
								Vector3f(normal.x, normal.y, normal.z)));
			}

		}

		_meshes.push_back(std::shared_ptr<Mesh>(new Mesh()));
		_meshes.back()->setVertices(vertices);

		Material mat = _meshes.back()->getMaterial();
		mat.setIndex(mesh->mMaterialIndex);
		_meshes.back()->setMaterial(mat);

		std::cout << "----- Created mesh #" << i << " - " << &_meshes.back() << "\n";

	}

	//Material Settings:
	std::cout << "----- Total count of materials: " << scene->mNumMaterials << "\n";
	for(unsigned int i = 0; i < scene->mNumMaterials; ++i){

		std::cout << "-------- Loading material #" << i << "\n";

		aiMaterial* material = scene->mMaterials[i];

		aiColor3D diffuseColor(1.f, 0.7f, 0.f);
		aiColor3D specularColor(1.f, 1.f, 1.f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		std::cout << "-------- Setting diffuse/specular colors\n";
		for(size_t j = 0; j < _meshes.size(); ++j){
			if(_meshes[j]->getMaterial().getIndex() == i){
				Material mat = _meshes[j]->getMaterial();

				mat.setDiffuseColor(Vector3f(diffuseColor.r, diffuseColor.g, diffuseColor.b));
				mat.setSpecularColor(Vector3f(specularColor.r, specularColor.g, specularColor.b));
				_meshes[j]->setMaterial(mat);
				_meshes[j]->data();
			}
		}

		//Textures:

		unsigned int textureIndex = 0;
		aiString assimpFile;
		if(material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &assimpFile) == AI_SUCCESS){
			std::string file = assimpFile.data; //convert string-type

			std::cout << "-------- Searching according mesh for texture...\n";
			for(size_t j = 0; j < _meshes.size(); ++j){
				if(_meshes[j]->getMaterial().getIndex() == i){
					std::cout << "Attempting to load texture: " << file << "\n";
					if(_meshes[j]->_texture.loadFromFile(file)){
						_meshes[j]->_material.setType(Material::Type::TEXTURED);
						std::cout << "Texture '" << file << "' successfully loaded.\n";
						std::cout << "Linked texture to mesh (" << &_meshes[j] << "). Material index = " << i << "\n";
						break;
					}else{
						std::cout << "!Failed to load texture: " << file << "\n";
						return false;
					}
				}
			}

		}else{
			std::cout << "!Material texture is invalid.\n";
		}

	}

	return true;
}

void Model::update() {
	for(size_t i = 0; i < _meshes.size(); ++i){
		_meshes[i]->data();
	}
}

} /* namespace burn */
