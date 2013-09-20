/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Scene.h"

#include "StaticMeshNode.h"
#include "Window.h"

namespace burn {

Scene::Scene() {
}

Scene::~Scene() {
	removeAllNodes();
	removeAllCameras();
	removeAllLights();
}

void Scene::drawAll() {
	if(Window::isContextCreated()){

		GLuint lightBuffer, dataBuffer;

		std::vector<float> lightData;
		for(size_t i = 0; i < _lights.size(); ++i){
			lightData.push_back(_lights[i]->getPosition().x);
			lightData.push_back(_lights[i]->getPosition().y);
			lightData.push_back(_lights[i]->getPosition().z);
		}

		glGenBuffers(1, &dataBuffer);
		glBindBuffer(GL_TEXTURE_BUFFER, dataBuffer);
		glBufferData(GL_TEXTURE_BUFFER, sizeof(float) * lightData.size(), &lightData[0], GL_STATIC_DRAW);

		glGenTextures(1, &lightBuffer);
		glBindTexture(GL_TEXTURE_BUFFER, lightBuffer);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, dataBuffer);

		for(size_t i = 0; i < _nodes.size(); ++i){
			_nodes[i]->draw(_activeCamera, _lights);
		}

		glDeleteBuffers(1, &dataBuffer);
		glDeleteTextures(1, &lightBuffer);

	}
}

std::shared_ptr<StaticMeshNode> Scene::createStaticMeshNode() {
	std::shared_ptr<StaticMeshNode> mesh(new StaticMeshNode());
	_nodes.push_back(mesh);
	return mesh;
}

void Scene::removeAllNodes() {
	//All nodes deleted by garbage collector of std::shared_ptr
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

std::shared_ptr<Light> Scene::createLight() {
	std::shared_ptr<Light> light(new Light());
	_lights.push_back(light);
	return light;
}

void Scene::removeLight(std::shared_ptr<Light> light) {
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == light){
			_lights.erase(_lights.begin() + i);
			return;
		}
	}
}

void Scene::removeAllLights() {
	_lights.clear(); //Smart pointers ftw!
}

} /* namespace burn */
