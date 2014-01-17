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

struct MeshData {
	unsigned int index;
	std::vector<Vertex> vertices;
	Material material;
	Texture texture;
};

size_t Model::getMeshCount() const {
	return _meshes.size();
}

const Mesh& Model::getMesh(const size_t& index) const {
	return *(_meshes[index]);
}

bool Model::loadFromFile(const std::string& file) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(	file.c_str(),
												aiProcess_CalcTangentSpace | aiProcess_Triangulate
												| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if(!scene){
		Reporter::report(	"Cannot load asset! (" + std::stringstream(importer.GetErrorString()).str() + ")",
							Reporter::ERROR);
		return false;
	}else{
		Reporter::report("Successfully loaded asset: " + file);
	}

	//Asset successfully loaded.

	_meshes.clear();
	std::vector<MeshData> meshData;

	std::vector<Vertex> vertices;
	for(unsigned int i = 0; i < scene->mNumMeshes; ++i){

		{
			std::stringstream ss;
			ss << i;
			//Reporter::report("----- Loading mesh #" + ss.str());
		}

		aiMesh* mesh = scene->mMeshes[i];

		vertices.clear();

		//Load all mesh's vertices
		for(unsigned int j = 0; j < mesh->mNumFaces; ++j){

			const aiFace& face = mesh->mFaces[j];
			for(unsigned int k = 0; k != 3; ++k){

				//Get Position
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];

				//Get UV-Coords
				aiVector3D uv =
				mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][face.mIndices[k]] : aiVector3D(0.0f, 0.0f, 0.0f);

				//Get Normal
				aiVector3D normal =
				mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);

				//Store vertex
				vertices.push_back(Vertex(	Vector3f(pos.x, pos.y, pos.z),
											Vector2f(uv.x, uv.y),
											Vector3f(normal.x, normal.y, normal.z)));
			}

		}

		MeshData md;
		//Store loaded vertices
		md.vertices = vertices;
		//Assign material index to a material for setting later
		md.index = mesh->mMaterialIndex;

		meshData.push_back(md);
	}

	//Set all materials
	for(unsigned int i = 0; i < scene->mNumMaterials; ++i){

		//Get Assimp material
		aiMaterial* material = scene->mMaterials[i];

		//Get specular color
		aiColor3D specularColor(1.f);
		aiColor3D diffuseColor(1.f);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

		//Find and set according material
		for(size_t j = 0; j < meshData.size(); ++j){
			if(meshData[j].index == i){

				meshData[j].material.setSpecularColor(Vector3f(specularColor.r, specularColor.g, specularColor.b));
				meshData[j].material.setDiffuseColor(Vector3f(diffuseColor.r, diffuseColor.g, diffuseColor.b));

			}
		}

		//Textures:
		unsigned int textureIndex = 0;
		aiString assimpFile;

		//Do we have a diffuse texture?
		if(material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &assimpFile) == AI_SUCCESS){

			//Convert assimpstring to std::string
			std::string file = assimpFile.data;

			//Find material and set to mesh
			for(size_t j = 0; j < meshData.size(); ++j){
				if(meshData[j].index == i){

					//Load texture
					if(meshData[j].texture.loadFromFile(file)){
						meshData[j].material.setType(Material::Type::TEXTURED);
						break;
					}else{
						Reporter::report("Failed to load texture: " + file, Reporter::ERROR);
						return false;
					}

				}
			}

			//Texture type not supported
		}else{
			Reporter::report("Material texture is invalid.", Reporter::WARNING);
		}

	}

	//Create all meshes
	for(size_t i = 0; i != meshData.size(); ++i){
		std::shared_ptr<Mesh> mesh(new Mesh(*this));

		mesh->setVertices(meshData[i].vertices);
		mesh->setMaterial(meshData[i].material);
		mesh->setTexture(meshData[i].texture);

		_meshes.push_back(mesh);
	}

	Reporter::report("Successfully loaded model: " + file);

	return true;
}

void Model::setFlag(const Material::Flag& flag,
					const bool& enabled) {
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

void Model::recalculateBoundingBox() const {
	Vector3f p, pMax; //little pos, high pos

	for(size_t i = 0; i < _meshes.size(); ++i){
		p.x = std::min(p.x, _meshes[i]->getXMinMax().x);
		p.y = std::min(p.y, _meshes[i]->getYMinMax().x);
		p.z = std::min(p.z, _meshes[i]->getZMinMax().x);

		pMax.x = std::max(pMax.x, _meshes[i]->getXMinMax().y);
		pMax.y = std::max(pMax.y, _meshes[i]->getYMinMax().y);
		pMax.z = std::max(pMax.z, _meshes[i]->getZMinMax().y);
	}

	_bb.setPosition(p);
	_bb.setDimensions(Vector3f(pMax.x - p.x, pMax.y - p.y, pMax.z - p.z));
}

const BoundingBox& Model::getBoundingBox() const {
	return _bb;
}

} /* namespace burn */
