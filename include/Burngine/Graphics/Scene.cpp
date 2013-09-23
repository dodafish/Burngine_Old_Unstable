/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Scene.h"

#include "StaticMeshNode.h"
#include "Window.h"
#include "RenderTexture.h"

namespace burn {

Scene::Scene(Window& window) :
				_window(window) {
}

Scene::~Scene() {
	removeAllNodes();
	removeAllCameras();
	removeAllLights();
}

void Scene::drawAll() {
	if(Window::isContextCreated()){

		//Render objects without lighting:
		_window.bind();
		Window::setBlendMode(Window::OVERWRITE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
		for(size_t i = 0; i < _nodes.size(); ++i){
			_nodes[i]->draw(_activeCamera);
		}
		glEnable(GL_BLEND);

		RenderTexture rt;
		if(rt.create(_window.getSettings().getWidth(), _window.getSettings().getHeight())){
			//Render objects' lightings:
			rt.bind();
			rt.clear();

			glDepthFunc(GL_LESS);
			Window::setBlendMode(Window::OVERWRITE);
			for(size_t i = 0; i < _nodes.size(); ++i){
				_nodes[i]->drawDepthColorless(_activeCamera);
			}

			glDepthFunc(GL_EQUAL);
			Window::setBlendMode(Window::ADD);
			for(size_t i = 0; i < _nodes.size(); ++i){
				_nodes[i]->drawLighting(_activeCamera, _lights, _ambientColor);
			}

			//Add lighting to scene:
			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);
			_window.bind();
			Window::setBlendMode(Window::MULTIPLY);
			rt.drawFullscreen(RenderTexture::TEXTURE0); //Diffuse lightings
			_window.bind();
			Window::setBlendMode(Window::ADD);
			rt.drawFullscreen(RenderTexture::TEXTURE1); //Specular lightings

			//Debug-view:
			Window::setBlendMode(Window::OVERWRITE);
			rt.draw(RenderTexture::TEXTURE0, Vector2f(-1.f, 1.f), Vector2f(0.4f, 0.4f));
			rt.draw(RenderTexture::TEXTURE1, Vector2f(-0.6f, 1.f), Vector2f(0.4f, 0.4f));

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);

		}

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

void Scene::setAmbientColor(const Vector3f& color) {
	_ambientColor = color;
}

const Vector3f& Scene::getAmbientColor() const {
	return _ambientColor;
}

} /* namespace burn */
