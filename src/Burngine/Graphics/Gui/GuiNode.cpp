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

#include <Burngine/Graphics/Gui/GuiNode.h>
#include <Burngine/Graphics/Gui/Gui.h>

namespace burn {

GuiNode::GuiNode() :
_rotation(0.f),
_zIndex(0) {
}

GuiNode::~GuiNode() {
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->detachNode(*this);
	}
}

void GuiNode::addParentGui(Gui* parent) {
	//Gui should be stored once only
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == parent)
			return;
	}

	_parents.push_back(parent);
}

void GuiNode::removeParentGui(Gui* parent) {
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == parent){
			_parents.erase(_parents.begin() + i);
			return;
		}
	}
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

void GuiNode::setZIndex(const Int32& zIndex) {

	bool needSort = (_zIndex != zIndex) ? true : false;

	_zIndex = zIndex;

	if(needSort){
		for(size_t i = 0; i < _parents.size(); ++i){
			_parents[i]->sortNodes();
		}
	}

}

const Int32& GuiNode::getZIndex() const {
	return _zIndex;
}

} /* namespace burn */
