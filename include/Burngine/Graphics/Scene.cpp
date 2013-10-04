/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Scene.h"

#include "Window.h"
#include "RenderTexture.h"

#include "SceneNode.h"
#include "StaticMeshNode.h"
#include "Camera.h"
#include "Light.h"

namespace burn {

Scene::Scene(const Window& parentWindow) :
_window(parentWindow),
_camera(_defaultCamera) {
}

Scene::~Scene() {

	if(&_camera != &_defaultCamera){
		_camera._parent = nullptr;
	}

}

void Scene::drawAll() {

	if(Window::isContextCreated()){

		//Render objects without lighting:
		_window.bind();
		Window::setBlendMode(Window::OVERWRITE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
		for(size_t i = 0; i < _nodes.size(); ++i){
			_nodes[i]->draw(_camera);
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
				_nodes[i]->drawDepthColorless(_camera);
			}

			glDepthFunc(GL_EQUAL);
			Window::setBlendMode(Window::ADD);
			for(size_t i = 0; i < _nodes.size(); ++i){
				_nodes[i]->drawLighting(_camera, _lights, _ambientColor);
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

void Scene::attachSceneNode(SceneNode& node) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node)
			return;
	}
	_nodes.push_back(&node);
	node._parents.push_back(this);
}

void Scene::detachSceneNode(SceneNode& node) {
	for(size_t i = 0; i < node._parents.size(); ++i){
		if(node._parents[i] == this){
			node._parents.erase(node._parents.begin() + i);
			break;
		}
	}
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node){
			_nodes.erase(_nodes.begin() + i);
			return;
		}
	}
}

void Scene::attachLight(Light& light) {
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == &light)
			return;
	}
	_lights.push_back(&light);
	light._parents.push_back(this);
}

void Scene::detachLight(Light& light) {
	for(size_t i = 0; i < light._parents.size(); ++i){
		if(light._parents[i] == this){
			light._parents.erase(light._parents.begin() + i);
			break;
		}
	}
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == &light){
			_lights.erase(_lights.begin() + i);
			return;
		}
	}
}

void Scene::setCamera(Camera& camera) {
	_camera = camera;
}

Camera& Scene::getDefaultCamera() {
	return _defaultCamera;
}

void Scene::setAmbientColor(const Vector3f& color) {
	_ambientColor = color;
}

const Vector3f& Scene::getAmbientColor() const {
	return _ambientColor;
}

} /* namespace burn */
