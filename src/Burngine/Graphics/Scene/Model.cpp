/*
 * Model.cpp
 *
 *  Created on: 17.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Model.h>
#include <Burngine/Graphics/Scene/Mesh.h>

#include <zlib.h>

#include <Burngine/extern/assimp/Importer.hpp>    // C++ importer interface
#include <Burngine/extern/assimp/scene.h>           // Output data structure
#include <Burngine/extern/assimp/postprocess.h>     // Post processing flags
#include <iostream>

#include <Burngine/System/Reporter.h>
#include <sstream>

namespace burn {

size_t Model::getMeshCount() const {
	return _meshes.size();
}

const Mesh& Model::getMesh(const size_t& index) const {
	return *(_meshes[index]);
}

bool Model::loadFromFile(const std::string& file) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
	file.c_str(),
	aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if(!scene){
		Reporter::report("Cannot load asset! (" + std::stringstream(importer.GetErrorString()).str() + ")",
							Reporter::ERROR);
		return false;
	}else{
		Reporter::report("Successfully loaded asset: " + file);
	}

	//Asset successfully loaded.

	_meshes.clear();

	{
		std::stringstream ss;
		ss << scene->mNumMeshes;
		Reporter::report("--- Total count of meshes: " + ss.str());
	}

	std::vector<Vertex> vertices;
	for(unsigned int i = 0; i < scene->mNumMeshes; ++i){

		{
			std::stringstream ss;
			ss << i;
			//Reporter::report("----- Loading mesh #" + ss.str());
		}

		aiMesh* mesh = scene->mMeshes[i];

		vertices.clear();

		{
			std::stringstream ss;
			ss << i << ": " << mesh->mNumFaces;
			Reporter::report("----- Total count of faces for mesh #" + ss.str());
		}

		for(unsigned int j = 0; j < mesh->mNumFaces; ++j){

			const aiFace& face = mesh->mFaces[j];
			for(unsigned int k = 0; k != 3; ++k){
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];

				aiVector3D uv =
				mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][face.mIndices[k]] : aiVector3D(0.0f, 0.0f, 0.0f);

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

	}

	//Material Settings:
	{
		std::stringstream ss;
		ss << scene->mNumMaterials;
		Reporter::report("----- Total count of materials: " + ss.str());
	}

	for(unsigned int i = 0; i < scene->mNumMaterials; ++i){

		{
			std::stringstream ss;
			ss << i;
			//Reporter::report("-------- Loading material #" + ss.str());
		}

		aiMaterial* material = scene->mMaterials[i];

		aiColor3D diffuseColor(1.f, 0.7f, 0.f);
		aiColor3D specularColor(1.f, 1.f, 1.f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		//Reporter::report("-------- Setting diffuse/specular colors");
		for(size_t j = 0; j < _meshes.size(); ++j){
			if(_meshes[j]->getMaterial().getIndex() == i){
				Material mat = _meshes[j]->getMaterial();

				mat.setDiffuseColor(Vector3f(diffuseColor.r, diffuseColor.g, diffuseColor.b));
				mat.setSpecularColor(Vector3f(specularColor.r, specularColor.g, specularColor.b));
				_meshes[j]->setMaterial(mat);
				_meshes[j]->forceUpdate(); //set color buffer asap
			}
		}

		//Textures:

		unsigned int textureIndex = 0;
		aiString assimpFile;
		if(material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &assimpFile) == AI_SUCCESS){
			std::string file = assimpFile.data; //convert string-type

			//Reporter::report("-------- Searching according mesh for texture...");
			for(size_t j = 0; j < _meshes.size(); ++j){
				if(_meshes[j]->getMaterial().getIndex() == i){
					//Reporter::report("-------- Attempting to load texture: " + file);
					if(_meshes[j]->_texture.loadFromFile(file)){
						_meshes[j]->_material.setType(Material::Type::TEXTURED);
						Reporter::report("-------- Texture '" + file + "' successfully loaded.");

						{
							std::stringstream ss;
							ss << &_meshes[j] << "). Material index = " << i;
							//Reporter::report("-------- Linked texture to mesh (" + ss.str());
						}

						break;
					}else{
						Reporter::report("Failed to load texture: " + file, Reporter::ERROR);
						return false;
					}
				}
			}

		}else{
			Reporter::report("Material texture is invalid.", Reporter::WARNING);
		}

	}

	return true;
}

void Model::setFlag(const Material::Flag& flag, const bool& enabled) {
	for(size_t i = 0; i < _meshes.size(); ++i){
		Material temp = _meshes[i]->getMaterial();
		temp.setFlag(flag, enabled);
		_meshes[i]->setMaterial(temp);
	}
}

void Model::update() {
	for(size_t i = 0; i < _meshes.size(); ++i){
		_meshes[i]->update();
	}
}

bool Model::isUpdated() const {
	for(size_t i = 0; i < _meshes.size(); ++i){
		if(!_meshes[i]->isUpdated())
			return false;
	}
	return true;
}

} /* namespace burn */
