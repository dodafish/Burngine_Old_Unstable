/*
 * SpotLight.cpp
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/SpotLight.h>

namespace burn {

SpotLight::SpotLight() :
_coneAngle(30.f) {
}

void SpotLight::setConeAngle(const float& angle) {
	_coneAngle = angle;
}

const float& SpotLight::getConeAngle() const {
	return _coneAngle;
}

void SpotLight::updateShadowMap(const std::vector<SceneNode*>& nodes){

}

} /* namespace burn */
