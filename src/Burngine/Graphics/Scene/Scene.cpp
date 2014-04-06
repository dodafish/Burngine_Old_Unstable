//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/System/Message.h>

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

#include <iostream>

namespace burn {

//Shortcut for easy access
typedef SceneRenderSystem::RenderFlag RF;

void Scene::onMessageReceive(const Message& msg) {
	if(msg.getName() == mn::SCENENODE_DESTRUCTED){
		Uint64 recId = 0;
		if(msg.getParameter<Uint64>(mp::COMPONENT_ID, &recId)){
			std::cout << "Scene: rmSN...\n";
			removeSceneNodeById(recId);
		}
	}else if(msg.getName() == mn::LIGHT_DESTRUCTED){
		Uint64 recId = 0;
		if(msg.getParameter<Uint64>(mp::COMPONENT_ID, &recId)){
			removeLightById(recId);
		}
	}
}

void Scene::removeSceneNodeById(const Uint64& id) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i]->getId().get() == id){
			_nodes.erase(_nodes.begin() + i);
			break;
		}
	}
}

void Scene::removeLightById(const Uint64& id) {
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i]->getId().get() == id){
			_lights.erase(_lights.begin() + i);
			return;
		}
	}
}

//////////////////////////////////////

Scene::Scene() :
_isLightingEnabled(false) {
	_messageReceiver.bindReceiveFunction(std::bind(&Scene::onMessageReceive, this, std::placeholders::_1));
}

Scene::~Scene() {
	detachAll();
}

void Scene::draw(	const RenderTarget& renderTarget,
					const Camera& camera,
					const SceneRenderSystem::RenderMode& mode) {

	if(!renderTarget.isCreated())
		return;

	_renderSystem.render(	renderTarget.getFramebufferId(),
							renderTarget.getDimensions(),
							camera,
							mode,
							_nodes,
							_lights,
							_ambientColor,
							_skyBox,
							_isLightingEnabled);

}

void Scene::draw(	const Window& renderTarget,
					const Camera& camera,
					const SceneRenderSystem::RenderMode& mode) {

	if(!renderTarget.isCreated())
		return;

	//Get window's dimensions
	Vector2ui targetDims(renderTarget.getSettings().getWidth(), renderTarget.getSettings().getHeight());

	//Bind window, so its framebuffer can be used (0)
	renderTarget.bind();
	_renderSystem.render(0, targetDims, camera, mode, _nodes, _lights, _ambientColor, _skyBox, _isLightingEnabled);

}

void Scene::detachAll() {
	//All SceneNodes:
	_nodes.clear();

	//All Lights:
	_lights.clear();
}

void Scene::attachSceneNode(StaticMeshNode& staticMeshNode) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &staticMeshNode)
			return;    //Already attached
	}
	_nodes.push_back(&staticMeshNode);
}

void Scene::detachSceneNode(const SceneNode& node) {

	//Remove from attachment-list
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node){
			_nodes.erase(_nodes.begin() + i);
			break;
		}
	}

}

void Scene::attachLight(Light& light) {
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == &light)
			return;    //Already attached
	}
	_lights.push_back(&light);
}

void Scene::detachLight(Light& light) {
	removeLightById(light.getId().get());
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

void Scene::setModelMatrixOffset(const Matrix4f& offset){
	_modelMatrixOffset = offset;
}

} /* namespace burn */
