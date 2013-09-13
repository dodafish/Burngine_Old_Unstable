/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Scene.h"

#include "StaticMeshNode.h"

namespace burn {

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::drawAll() {
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->draw();
	}
}

StaticMeshNode* Scene::createStaticMeshNode() {
	StaticMeshNode* n = new StaticMeshNode;
	_nodes.push_back(n);
	return n;
}

void Scene::removeAllNodes() {
	//StaticMeshNodes:
	for(size_t i = 0; i < _nodes.size(); ++i){
		delete _nodes[i];
	}
	_nodes.clear();

}

void Scene::removeNode(SceneNode* node) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == node){
			delete node;
			_nodes.erase(_nodes.begin() + i);
			return;
		}
	}
}

} /* namespace burn */
