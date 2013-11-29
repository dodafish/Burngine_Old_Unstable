//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/Graphics/Scene/Scene.h>

#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <Burngine/Graphics/Scene/SceneNode.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/Graphics/Scene/DirectionalLight.h>
#include <Burngine/Graphics/Scene/SpotLight.h>
#include <Burngine/Graphics/Scene/Mesh.h>

#include <Burngine/System/Reporter.h>

#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

//Shortcut for easy access
typedef SceneRenderSystem::RenderFlag RF;

Scene::Scene(const Window& parentWindow) :
_window(parentWindow),
_renderSystem(parentWindow),
_isLightingEnabled(false) {

}

Scene::~Scene() {
	detachAll();
}

void Scene::draw(	const Camera& camera,
					const SceneRenderSystem::RenderMode& mode) {

	_renderSystem.render(	0,
							Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()),
							camera,
							mode,
							_nodes,
							_lights,
							_isLightingEnabled);

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

	//Remove from attachment-list
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

void Scene::setAmbientColor(const Vector3f& color) {
	_ambientColor = color;
}

const Vector3f& Scene::getAmbientColor() const {
	return _ambientColor;
}

void Scene::setSkyBox(const SkyBox& skyBox) {
	_skyBox = skyBox;
}

void Scene::setLightingEnabled(bool enabled) {
	_isLightingEnabled = enabled;
}

bool Scene::isLightingEnabled() const {
	return _isLightingEnabled;
}

} /* namespace burn */
