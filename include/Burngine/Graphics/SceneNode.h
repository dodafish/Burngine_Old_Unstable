/*
 * SceneNode.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include "../Export.h"
#include "../System/Math.h"

namespace burn {

class BURNGINE_API SceneNode {
public:
	SceneNode();
	virtual ~SceneNode();

	virtual void draw() = 0;

	void setPosition(const Vector3f& position);
	const Vector3f& getPosition() const;

	void setRotation(const Vector3f& rotation);
	const Vector3f& getRotation() const;

	void setScale(const Vector3f& scale);
	const Vector3f& getScale() const;

private:
	Vector3f _position, _scale, _rotation;
};

} /* namespace burn */
#endif /* SCENENODE_H_ */
