/*
 * Light.cpp
 *
 *  Created on: 20.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Window/Window.h>
#include <iostream>

namespace burn {

void Light::removeAllParents() {
	if(_parents.size() == 0)
		return;

	Scene* parents[_parents.size()];
	for(size_t i = 0; i != _parents.size(); ++i)
		parents[i] = _parents[i];
	size_t size = _parents.size();
	for(size_t i = 0; i != size; ++i)
		parents[i]->detachLight(*this);

	_parents.clear();
}

Light::Light(const Type& type, const Vector3f& color, const float& intensity) :
_color(color),
_intensity((intensity > 0) ? intensity : 0),
_type(type),
_cutoffAngle(20.f) {

	_shadowMap.create();

}

Light::Light(const Light& other) :
_color(other._color),
_intensity(other._intensity),
_type(other._type),
_cutoffAngle(other._cutoffAngle),
_shadowMap(other._shadowMap) {
	_parents = other._parents;
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->attachLight(*this);
	}
}

Light& Light::operator=(const Light& other) {

	_shadowMap.~ShadowMap();

	_color = other._color;
	_intensity = other._intensity;
	_type = other._type;
	_cutoffAngle = other._cutoffAngle;
	_shadowMap = other._shadowMap;

	removeAllParents();

	_parents = other._parents;
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->attachLight(*this);
	}

	return *this;
}

Light::~Light() {
	removeAllParents();
}

void Light::addParentScene(Scene* scene) {
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == scene)
			return; //Already added as parent
	}
	_parents.push_back(scene); //Add to parents
}

void Light::removeParentScene(Scene* scene) {
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == scene){
			_parents.erase(_parents.begin() + i);
			return; //addParentScene() ensures, that a scene is added only once. So return
		}
	}
}

void Light::setColor(const Vector3f& color) {
	_color = color;
}

const Vector3f& Light::getColor() const {
	return _color;
}

void Light::setIntensity(const float& intensity) {
	_intensity = (intensity > 0) ? intensity : 0;
}

const float& Light::getIntensity() const {
	return _intensity;
}

void Light::setType(const Light::Type& type) {
	_type = type;
}

const Light::Type& Light::getType() const {
	return _type;
}

void Light::setCutoffAngle(const float& angle) {
	if(angle < 0.f){
		_cutoffAngle = 0.f;
	}else if(angle > 360.f){
		_cutoffAngle = 360.f;
	}else{
		_cutoffAngle = angle;
	}
}

const float& Light::getCutoffAngle() const {
	return _cutoffAngle;
}

Vector4f Light::getDirection() const {
	Matrix4f rotMat = glm::rotate(Matrix4f(1.f), _rotation.x, Vector3f(1.f, 0.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.y, Vector3f(0.f, 1.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.z, Vector3f(0.f, 0.f, 1.f));
	return (rotMat * Vector4f(1.f, 0.f, 0.f, 1.0f));
}

void Light::updateShadowMap(const std::vector<SceneNode*> nodes) {

	if(!Window::isContextCreated())
		return;

	//Testing for dirlights only
	if(_type == POINTLIGHT || _type == SPOTLIGHT)
		return;

	//Clear old shadowMap and bind it for rendering
	_shadowMap.clear();
	_shadowMap.bindAsRendertarget();

	//Select shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);

	//Calculate matrices
	Matrix4f rotMat = glm::rotate(Matrix4f(1.f), _rotation.x, Vector3f(1.f, 0.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.y, Vector3f(0.f, 1.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.z, Vector3f(0.f, 0.f, 1.f));
	Vector4f lightDirTemp = rotMat * Vector4f(1.f, 0.f, 0.f, 1.0f);
	Vector3f lightDir(lightDirTemp.x, lightDirTemp.y, lightDirTemp.z);
	Matrix4f projectionMatrix = glm::ortho<float>(-50, 50, -50, 50, -50, 50);
	Matrix4f viewMatrix = glm::lookAt(-lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	Matrix4f modelMatrix = glm::mat4(1.0);

	//Set uniforms
	shader.setUniform("modelMatrix", modelMatrix);
	shader.setUniform("viewMatrix", viewMatrix);
	shader.setUniform("projectionMatrix", projectionMatrix);

	//Scan through all nodes
	for(size_t i = 0; i < nodes.size(); ++i){

		if(typeid(*nodes[i]) == typeid(StaticMeshNode)){
			StaticMeshNode* node = static_cast<StaticMeshNode*>(nodes[i]);

			node->update();

			//Skip if updating failed or is incomplete
			if(!node->getModel().isUpdated())
				continue;

			//0 = Positions
			glEnableVertexAttribArray(0);
			node->getModel().getMesh(i).getPositionVbo().bind();
			glVertexAttribPointer(0, // attribute 0
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

			//Draw
			OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, node->getModel().getMesh(i).getVertexCount(), shader);

			glDisableVertexAttribArray(0);

		}else{
			std::cout << "SceneNode unknown: " << typeid(*nodes[i]).name() << "\n";
		}

	}

}

} /* namespace burn */
