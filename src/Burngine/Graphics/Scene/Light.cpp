/*
 * Light.cpp
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Light.h>

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

Light::Light() :
_color(Vector3f(1.f)),
_intensity(10.f) {

}

Light::Light(const Light& other) :
_color(other._color),
_intensity(other._intensity) {
	_parents = other._parents;
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->attachLight(*this);
	}
}

Light& Light::operator=(const Light& other) {

	_color = other._color;
	_intensity = other._intensity;

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

const Matrix4f& Light::getBiasViewMatrix() const {
	return _biasViewMatrix;
}

const Matrix4f& Light::getBiasProjectionMatrix() const {
	return _biasProjectionMatrix;
}

void Light::updateShadowMap(const std::vector<SceneNode*>& nodes){

}

} /* namespace burn */
