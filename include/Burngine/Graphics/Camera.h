/*
 * Camera.h
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "../Export.h"
#include "Transformable.h"

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
	 * @see Scene::setCameraActive()
	 */
	Camera();

	/**
	 * @brief Default Destructor.
	 *
	 * @note Make sure to delete the camera with
	 * Scene::removeCamera() when you
	 * have created it with Scene::createCamera() !
	 *
	 * @see Scene::removeCamera()
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
