/*
 * Light.h
 *
 *  Created on: 20.09.2013
 *      Author: Dominik
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "../Export.h"
#include "Transformable.h"

namespace burn {

/**
 * @brief Inherits from Transformable. Obviously the scale will have
 * no effect.
 */
class BURNGINE_API Light : public Transformable {
public:
	enum Type {
		POINT, SPOT, DIRECTIONAL
	};

	Light(const Type& type = POINT, const Vector3f& color = Vector3f(1.0f, 1.0f, 1.0f), const float& intensity = 1.0f);
	~Light();

	void setColor(const Vector3f& color);
	const Vector3f& getColor() const;

	void setIntensity(const float& intensity);
	const float& getIntensity() const;

	void setType(const Type& type);
	const Type& getType() const;

private:
	Vector3f _color;
	float _intensity;
	Type _type;
};

} /* namespace burn */
#endif /* LIGHT_H_ */
