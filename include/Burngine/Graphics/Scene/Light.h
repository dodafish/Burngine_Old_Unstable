//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

#ifndef LIGHT_H_
#define LIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/Graphics/Scene/Scene.h>
#include <Burngine/Graphics/Texture/ShadowCubeMap.h>

namespace burn {

class BURNGINE_API Light : public Transformable{
public:
	Light(const ShadowCubeMap::Resolution& shadowMapResolution = ShadowCubeMap::LOW);
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
