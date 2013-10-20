/*
 * Light.cpp
 *
 *  Created on: 20.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Light.h>

namespace burn {

void Light::removeAllParents() {
	Scene* parents[_parents.size()];
	for(size_t i = 0; i != _parents.size(); ++i)
		parents[i] = _parents[i];
	size_t size = _parents.size();
	for(size_t i = 0; i != size; ++i)
		parents[i]->detachLight(*this);
}

Light::Light(const Type& type, const Vector3f& color, const float& intensity) :
_color(color),
_intensity((intensity > 0) ? intensity : 0),
_type(type),
_cutoffAngle(20.f) {

}

Light::Light(const Light& other) :
_color(other._color),
_intensity(other._intensity),
_type(other._type),
_cutoffAngle(other._cutoffAngle) {
	for(size_t i = 0; i < other._parents.size(); ++i){
		other._parents[i]->attachLight(*this);
	}
}

Light& Light::operator=(const Light& other) {
	_color = other._color;
	_intensity = other._intensity;
	_type = other._type;
	_cutoffAngle = other._cutoffAngle;

	removeAllParents();

	for(size_t i = 0; i < other._parents.size(); ++i){
		other._parents[i]->attachLight(*this);
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
	}
	else if(angle > 360.f){
		_cutoffAngle = 360.f;
	}
	else{
		_cutoffAngle = angle;
	}
}

const float& Light::getCutoffAngle() const {
	return _cutoffAngle;
}

} /* namespace burn */
