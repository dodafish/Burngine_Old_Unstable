/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Scene.h"

#include "StaticMeshNode.h"

namespace burn {

Scene::Scene() :
				_activeCamera(nullptr) {
}

Scene::~Scene() {
	removeAllNodes();
	removeAllCameras();
}

void Scene::drawAll() {
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->draw(_activeCamera);
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

Camera* Scene::createCamera(bool active) {
	Camera* cam = new Camera();
	_cameras.push_back(cam);
	if(active){
		_activeCamera = cam;
	}
	return cam;
}

void Scene::removeCamera(Camera* cam) {
	if(cam != nullptr){
		for(size_t i = 0; i < _cameras.size(); ++i){
			if(_cameras[i] == cam){
				delete _cameras[i];
				_cameras.erase(_cameras.begin() + i);
				_activeCamera = nullptr;
				return;
			}
		}
	}
}

void Scene::removeAllCameras() {
	for(size_t i = 0; i < _cameras.size(); ++i){
		delete _cameras[i];
	}
	_cameras.clear();
	_activeCamera = nullptr;
}

} /* namespace burn */
