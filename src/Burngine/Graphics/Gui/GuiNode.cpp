/*
 * GuiNode.cpp
 *
 *  Created on: 13.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/GuiNode.h>

namespace burn {

GuiNode::GuiNode() :
_rotation(0.f) {
}

void GuiNode::setPosition(const Vector2f& position) {
	_position = position;
}

const Vector2f& GuiNode::getPosition() const {
	return _position;
}

void GuiNode::setRotation(const float& rotation) {
	_rotation = rotation;
}

const float& GuiNode::getRotation() const {
	return _rotation;
}

void GuiNode::rotate(const float& angle) {
	_rotation += angle;
}

void GuiNode::move(const Vector2f& offset) {
	_position += offset;
}

void GuiNode::move(const float& offsetX, const float& offsetY) {
	_position.x += offsetX;
	_position.y += offsetY;
}

} /* namespace burn */
