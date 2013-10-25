/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Scene.h>

#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <Burngine/Graphics/Scene/SceneNode.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Light.h>

namespace burn {

Scene::Scene(const Window& parentWindow) :
_window(parentWindow),
_camera(_defaultCamera) {

	_diffuseLightTexture.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()));
	_specularLightTexture.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()));

}

Scene::~Scene() {

	if(&_camera != &_defaultCamera){
		_camera._parent = nullptr;
	}

	detachAll();

}

void Scene::draw(const RenderModus& modus) {

	if(!Window::isContextCreated())
		return;

	//Update all lights' shadowMaps
	for(size_t i = 0; i < _lights.size(); ++i){
		_lights[i]->updateShadowMap(_nodes);
	}

	switch (modus) {
		case ALL:
			drawNodes();

			if(drawDiffusepart()){
				//Add diffuse lighting to scene:
				OpenGlControl::Settings oglSettings; //default
				oglSettings.setBlendMode(OpenGlControl::MULTIPLY);
				oglSettings.enableDepthtest(false);
				oglSettings.enableDepthbufferWriting(false);
				OpenGlControl::useSettings(oglSettings);
				_window.bind();
				_diffuseLightTexture.drawFullscreen(); //Diffuse lightings

				if(drawSpecularpart()){
					//Add specular lighting to scene:
					oglSettings = OpenGlControl::Settings(); //default
					oglSettings.setBlendMode(OpenGlControl::ADD);
					oglSettings.enableDepthtest(false);
					oglSettings.enableDepthbufferWriting(false);
					OpenGlControl::useSettings(oglSettings);
					_window.bind();
					_specularLightTexture.drawFullscreen(); //Specular lightings
				}
			}

			break;
		case COLOR:
			drawNodes();
			break;
		case DIFFUSE:
			if(drawDiffusepart()){
				//Add diffuse lighting to scene:
				OpenGlControl::Settings oglSettings; //default
				OpenGlControl::useSettings(oglSettings);
				_window.bind();
				_diffuseLightTexture.drawFullscreen(); //Diffuse lightings
			}
			break;
		case SPECULAR:
			if(drawSpecularpart()){
				//Add diffuse lighting to scene:
				OpenGlControl::Settings oglSettings; //default
				OpenGlControl::useSettings(oglSettings);
				_window.bind();
				_specularLightTexture.drawFullscreen(); //Diffuse lightings
			}
			break;
		case LIGHTING:
			if(drawDiffusepart()){
				//Add diffuse lighting to scene:
				OpenGlControl::Settings oglSettings; //default
				oglSettings.setBlendMode(OpenGlControl::OVERWRITE);
				oglSettings.enableDepthtest(false);
				oglSettings.enableDepthbufferWriting(false);
				OpenGlControl::useSettings(oglSettings);
				_window.bind();
				_diffuseLightTexture.drawFullscreen(); //Diffuse lightings

				if(drawSpecularpart()){
					//Add specular lighting to scene:
					oglSettings = OpenGlControl::Settings(); //default
					oglSettings.setBlendMode(OpenGlControl::ADD);
					oglSettings.enableDepthtest(false);
					oglSettings.enableDepthbufferWriting(false);
					OpenGlControl::useSettings(oglSettings);
					_window.bind();
					_specularLightTexture.drawFullscreen(); //Specular lightings
				}
			}
			break;
	}

	//Restore default OpenGL settings
	OpenGlControl::useSettings(OpenGlControl::Settings());

}

void Scene::drawNodes() {
	//Render objects without lighting:
	_window.bind();
	OpenGlControl::useSettings(OpenGlControl::Settings());
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->draw(_camera);
	}
}

bool Scene::drawDiffusepart() {

	if(!_diffuseLightTexture.isCreated())
		return false;

	//Render objects' lightings:

	OpenGlControl::Settings oglSettings;
	oglSettings.setClearColor(Vector4f(1.f));
	OpenGlControl::useSettings(oglSettings);

	_diffuseLightTexture.clear();
	_diffuseLightTexture.bindAsRendertarget();

	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->drawSingleColor(_camera, Vector4f(0.f, 0.f, 0.f, 0.f));
	}

	oglSettings.setDepthtestTechnique(OpenGlControl::EQUAL);
	oglSettings.setBlendMode(OpenGlControl::ADD);
	OpenGlControl::useSettings(oglSettings);

	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->drawLighting(SceneNode::DIFFUSE, _camera, _lights, _ambientColor);
	}

	return true;
}

bool Scene::drawSpecularpart() {
	if(!_specularLightTexture.isCreated())
		return false;

	OpenGlControl::Settings oglSettings;
	oglSettings.setClearColor(Vector4f(0.f, 0.f, 0.f, 1.f));
	OpenGlControl::useSettings(oglSettings);

	_specularLightTexture.clear();
	_specularLightTexture.bindAsRendertarget();

	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->drawSingleColor(_camera, Vector4f(0.f, 0.f, 0.f, 0.f));
	}

	oglSettings.setDepthtestTechnique(OpenGlControl::EQUAL);
	oglSettings.setBlendMode(OpenGlControl::ADD);
	OpenGlControl::useSettings(oglSettings);
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->drawLighting(SceneNode::SPECULAR, _camera, _lights, _ambientColor);
	}

	return true;
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
