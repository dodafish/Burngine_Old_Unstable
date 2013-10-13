/*
 * GuiNode.h
 *
 *  Created on: 13.10.2013
 *      Author: Dominik
 */

#ifndef GUINODE_H_
#define GUINODE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

namespace burn {

class BURNGINE_API GuiNode {
public:
	GuiNode();

	void setPosition(const Vector2f& position);
	const Vector2f& getPosition() const;

	void setRotation(const float& rotation);
	const float& getRotation() const;

	void rotate(const float& angle);
	void move(const Vector2f& offset);
	void move(const float& offsetX, const float& offsetY);

protected:
	Vector2f _position;
	float _rotation;
};

} /* namespace burn */
#endif /* GUINODE_H_ */
