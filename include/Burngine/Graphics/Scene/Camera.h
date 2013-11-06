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

#ifndef CAMERA_H_
#define CAMERA_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <vector>

namespace burn {

class BURNGINE_API Camera : public Transformable {
public:
	/**
	 * @brief Default Contstructor of Camera.
	 * Default values:
	 * - AspectRatio: 16/9
	 * - LookAt: 0/0/0 (Origin)
	 * - FOV: 45
	 *
	 * @note In order to see results by changing values, ensure
	 * that your camera is active.
	 *
	 */
	Camera();

	/**
	 * @brief Default Destructor.
	 *
	 * @note Make sure to delete the camera with
	 * Scene::removeCamera() when you
	 * have created it with Scene::createCamera() !
	 *
	 */
	~Camera();

	/**
	 * @brief Changes the aspectratio that the scene will
	 * be drawn with.
	 *
	 * @param aspectRatio The AspectRatio. Usually the width/height
	 * of the Window
	 *
	 * @note Make sure to set your camera as active in order to
	 * see results.
	 *
	 * @see setAspectRatio()
	 */
	void setAspectRatio(const float& aspectRatio);

	/**
	 * @brief Returns the current aspectratio
	 *
	 * @return The current AspectRatio
	 *
	 * @seet setAspectRatio()
	 */
	const float& getAspectRatio() const;

	/**
	 * @brief Sets the point the camera will face to
	 *
	 * @param point A Vector3f defining the faced point
	 *
	 * @see getLookAt()
	 */
	void lookAt(const Vector3f& point);

	/**
	 * @brief Returns the point the camera is facing
	 *
	 * @return A Vector3f defining the faced point
	 *
	 * @see lookAt()
	 */
	const Vector3f& getLookAt() const;

	/**
	 * @brief Sets the Field-Of-View
	 *
	 * @param fov A value between 0 and 90
	 */
	void setFov(const float& fov);

	/**
	 * @brief Returns the current Field-Of-View
	 *
	 * @return A value between 0 and 90
	 */
	const float& getFov() const;

private:
	float _aspectRatio;
	Vector3f _lookAt;
	float _fov;
};

} /* namespace burn */
#endif /* CAMERA_H_ */
