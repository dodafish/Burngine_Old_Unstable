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
	Camera();
	~Camera();

	void setAspectRatio(const float& aspectRatio);
	const float& getAspectRatio() const;

	void lookAt(const Vector3f& point);
	const Vector3f& getLookAt() const;

	void setFov(const float& fov);
	const float& getFov() const;

private:
	float _aspectRatio;
	Vector3f _lookAt;
	float _fov;

};

} /* namespace burn */
#endif /* CAMERA_H_ */
