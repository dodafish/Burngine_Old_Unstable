/*
 * BaseLight.h
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/Graphics/Scene/Scene.h>
#include <Burngine/Graphics/Texture/ShadowCubeMap.h>

namespace burn {

class BURNGINE_API Light : public Transformable{
public:
	Light();
	Light(const Light& other);
	Light& operator=(const Light& other);

	virtual ~Light();

	void setColor(const Vector3f& color);
	const Vector3f& getColor() const;

	void setIntensity(const float& intensity);
	const float& getIntensity() const;

	void addParentScene(Scene* scene);
	void removeParentScene(Scene* scene);

	virtual void updateShadowMap(const std::vector<SceneNode*>& nodes);

	virtual const Matrix4f& getBiasViewMatrix() const;
	virtual const Matrix4f& getBiasProjectionMatrix() const;

	void bindShadowCubeMap() const;

	Matrix4f findViewMatrix(const int& face) const;

	const ShadowCubeMap& getShadowCubeMap() const;

protected:

	Vector3f _color;
	float _intensity;

	std::vector<Scene*> _parents;
	void removeAllParents();

	Matrix4f _biasViewMatrix, _biasProjectionMatrix;

	ShadowCubeMap _shadowCubeMap;

};

} /* namespace burn */
#endif /* LIGHT_H_ */
