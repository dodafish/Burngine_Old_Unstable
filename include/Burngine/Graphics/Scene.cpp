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
	removeAllNodes();
	removeAllCameras();
}

void Scene::drawAll() {
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->draw(_activeCamera);
	}
}

std::shared_ptr<StaticMeshNode> Scene::createStaticMeshNode() {
	std::shared_ptr<StaticMeshNode> mesh(new StaticMeshNode());
	_nodes.push_back(mesh);
	return mesh;
}

void Scene::removeAllNodes() {
	//All nodes deleted by garbage collector of std::unique_ptr
	_nodes.clear();
}

void Scene::removeNode(std::shared_ptr<SceneNode> node) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == node){
			_nodes.erase(_nodes.begin() + i);
			return;
		}
	}
}

std::shared_ptr<Camera> Scene::createCamera(bool active) {
	std::shared_ptr<Camera> cam(new Camera());
	_cameras.push_back(cam);
	if(active){
		_activeCamera = cam;
	}
	return cam;
}

void Scene::removeCamera(std::shared_ptr<Camera> cam) {
	if(cam != nullptr){
		for(size_t i = 0; i < _cameras.size(); ++i){
			if(_cameras[i] == cam){
				if(cam == _activeCamera){
					_activeCamera = nullptr;
				}
				_cameras.erase(_cameras.begin() + i);
				return;
			}
		}
	}
}

void Scene::removeAllCameras() {
	_cameras.clear();
	_activeCamera = nullptr;
}

void Scene::setActiveCamera(std::shared_ptr<Camera> cam) {
	if(cam != nullptr){
		_activeCamera = cam;
	}
}

} /* namespace burn */
