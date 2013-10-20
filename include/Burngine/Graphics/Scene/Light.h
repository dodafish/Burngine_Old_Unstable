/*
 * Light.h
 *
 *  Created on: 20.09.2013
 *      Author: Dominik
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/Graphics/Scene/Scene.h>

//template class BURNGINE_API std::vector<burn::Scene*>;

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

	Light(const Light& other);

	Light& operator=(const Light& other);

	~Light();

	void setColor(const Vector3f& color);
	const Vector3f& getColor() const;

	void setIntensity(const float& intensity);
	const float& getIntensity() const;

	void setType(const Type& type);
	const Type& getType() const;

	void setCutoffAngle(const float& angle);
	const float& getCutoffAngle() const;

	void addParentScene(Scene* scene);
	void removeParentScene(Scene* scene);

private:
	Vector3f _color;
	float _intensity;
	Type _type;
	float _cutoffAngle;

	std::vector<Scene*> _parents;

	void removeAllParents();
};

} /* namespace burn */
#endif /* LIGHT_H_ */
