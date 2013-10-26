/*
 * SpotLight.h
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/DirectionalLight.h>

namespace burn {

class BURNGINE_API SpotLight : public DirectionalLight {
public:
	SpotLight();

	void setConeAngle(const float& angle);
	const float& getConeAngle() const;

	virtual void updateShadowMap(const std::vector<SceneNode*>& nodes);

private:

	float _coneAngle;

};

} /* namespace burn */
#endif /* SPOTLIGHT_H_ */
