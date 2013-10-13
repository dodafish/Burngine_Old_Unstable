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

	detachAll();

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

		RenderTexture rtDiffuse, rtSpecular;
		if(rtDiffuse.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()))){
			//Render objects' lightings:

			rtDiffuse.clear();
			rtDiffuse.bind();

			glDepthFunc(GL_LESS);
			Window::setBlendMode(Window::OVERWRITE);
			for(size_t i = 0; i < _nodes.size(); ++i){
				_nodes[i]->drawDepthColorless(_camera);
			}

			glDepthFunc(GL_EQUAL);
			Window::setBlendMode(Window::ADD);
			for(size_t i = 0; i < _nodes.size(); ++i){
				_nodes[i]->drawLighting(SceneNode::DIFFUSE, _camera, _lights, _ambientColor);
			}

			//Add lighting to scene:
			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);

			_window.bind();
			Window::setBlendMode(Window::MULTIPLY);
			rtDiffuse.drawFullscreen(); //Diffuse lightings

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);

		}
		if(rtSpecular.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()))){

			rtSpecular.clear();
			rtSpecular.bind();

			glDepthFunc(GL_LESS);
			Window::setBlendMode(Window::OVERWRITE);
			for(size_t i = 0; i < _nodes.size(); ++i){
				_nodes[i]->drawDepthColorless(_camera);
			}

			glDepthFunc(GL_EQUAL);
			Window::setBlendMode(Window::ADD);
			for(size_t i = 0; i < _nodes.size(); ++i){
				_nodes[i]->drawLighting(SceneNode::SPECULAR, _camera, _lights, _ambientColor);
			}

			//Add lighting to scene:
			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);

			_window.bind();
			Window::setBlendMode(Window::ADD);
			rtSpecular.drawFullscreen(); //Specular lightings

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);

		}

		//Debug-view:
		/*glDepthMask(GL_FALSE);
		 glDisable(GL_DEPTH_TEST);
		 Window::setBlendMode(Window::OVERWRITE);
		 rtDiffuse.draw(Vector2f(-1.f, 1.f), Vector2f(0.4f, 0.4f));
		 rtSpecular.draw(Vector2f(-0.6f, 1.f), Vector2f(0.4f, 0.4f));
		 glDepthMask(GL_TRUE);
		 glEnable(GL_DEPTH_TEST);*/

		glDepthFunc(GL_LESS);

	}
}

void Scene::detachAll() {
	//All SceneNodes:
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->removeParentScene(this);
	}
	_nodes.clear();

	//All Lights:
	for(size_t i = 0; i < _lights.size(); ++i){
		_lights[i]->removeParentScene(this);
	}
	_lights.clear();

}

void Scene::attachSceneNode(SceneNode& node) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node)
			return; //Already attached
	}
	_nodes.push_back(&node);
	node.addParentScene(this);
}

void Scene::detachSceneNode(SceneNode& node) {

	node.removeParentScene(this);

	//Remove from attachement-list
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
			return; //Already attached
	}
	_lights.push_back(&light);
	light.addParentScene(this);
}

void Scene::detachLight(Light& light) {

	light.removeParentScene(this);

	//Remove from attachement-list
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

void Scene::setDefaultCamera() {
	_camera = _defaultCamera;
}

const Camera& Scene::getDefaultCamera() {
	return _defaultCamera;
}

void Scene::setAmbientColor(const Vector3f& color) {
	_ambientColor = color;
}

const Vector3f& Scene::getAmbientColor() const {
	return _ambientColor;
}

} /* namespace burn */
