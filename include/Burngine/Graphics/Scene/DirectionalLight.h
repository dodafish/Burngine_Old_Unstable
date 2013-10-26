/*
 * Light.h
 *
 *  Created on: 20.09.2013
 *      Author: Dominik
 */

#ifndef DIRECTIONAL_LIGHT_H_
#define DIRECTIONAL_LIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/Graphics/Texture/ShadowMap.h>

//template class BURNGINE_API std::vector<burn::Scene*>;

namespace burn {

class BURNGINE_API DirectionalLight : public Light {
public:

	DirectionalLight();

	Vector4f getDirection() const;

	void bindShadowMap() const;

	virtual void updateShadowMap(const std::vector<SceneNode*>& nodes);

protected:
	ShadowMap _shadowMap;
};

} /* namespace burn */
#endif /* DIRECTIONAL_LIGHT_H_ */
