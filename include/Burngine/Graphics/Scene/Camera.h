//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
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
	 * - AspectRatio: 1
	 * - LookAt: 0/0/0 (Origin)
	 * - FOV: 45
	 *
	 * @note In order to see results by changing values, ensure
	 * that your camera is active.
	 *
	 */
	Camera();

	/**
	 * @brief Copies another camera
	 */
	Camera(const Camera& other);

	Camera& operator=(const Camera& other);

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

	/**
	 * @brief This returns a 4x4 matrix which represents the
	 * projection matrix according to the set FOV, aspect ratio and
	 * the far clipping distance
	 *
	 * @return Projectionmatrix
	 *
	 * @see setFov()
	 * @see setAspectRatio()
	 * @see setFar()
	 * @see setType()
	 *
	 * @note If you have an orthogonal camera, the orthogonal "box" is
	 * set with the FOV. Where FOV describes the height of the box and
	 * the width is set according to the aspect ratio. So
	 * if you want a box size of 1280x800 you set FOV to 800 and
	 * the aspect ratio to 16/10
	 */
	Matrix4f getProjectionMatrix() const;

	/**
	 * @brief This returns a 4x4 matrix which represents the
	 * view matrix according to the looking direction and the position
	 * of the camera
	 *
	 * @return Viewmatrix
	 *
	 * @see setPosition()
	 * @see lookAt()
	 */
	Matrix4f getViewMatrix() const;

	/**
	 * @brief This sets the far clipping distance. Everything that is
	 * further away than far won't be drawn anymore
	 *
	 * @param far Limit of distance
	 */
	void setFar(const float& far);
	void setNear(const float& near);

	enum Type {
		PERSPECTIVE,    ///< The camera has perspective projection
		ORTHOGONAL    ///< The camera has orthogonal projection
	};

	void setType(const Type& type);

	const Type& getType() const;

	void setHeadUp(const Vector3f& headup);
	const Vector3f& getHeadUp() const;

	void setViewMatrix(const Matrix4f& matrix);
	void setProjectionMatrix(const Matrix4f& matrix);

	void setProjectionMatrixOffset(const Matrix4f& offset);

private:
	float _aspectRatio;
	float _fov;
	float _far;
	float _near;
	Type _type;
	Vector3f _headUp;

	mutable Matrix4f _viewMatrix, _projectionMatrix;
	Matrix4f _projectionMatrixOffset;
};

} /* namespace burn */
#endif /* CAMERA_H_ */
