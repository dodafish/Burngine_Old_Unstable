/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Scene.h"

namespace burn {

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

void Scene::removeAllNodes(){
	//StaticMeshNodes:
	for(size_t i = 0; i < _staticMeshes.size(); ++i){
		delete _staticMeshes[i];
	}
	_staticMeshes.clear();

}

StaticMeshNode* Scene::createStaticMeshNode(){
	StaticMeshNode* n = new StaticMeshNode;
	_staticMeshes.push_back(n);
	return n;
}

void Scene::removeStaticMeshNode(StaticMeshNode* node){
	for(size_t i = 0; i < _staticMeshes.size(); ++i){
		if(_staticMeshes[i] == node){
			delete node;
			_staticMeshes.erase(_staticMeshes.begin() + i);
			return;
		}
	}
}

} /* namespace burn */
