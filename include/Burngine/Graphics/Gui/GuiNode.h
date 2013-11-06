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

#ifndef GUINODE_H_
#define GUINODE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

#include <vector>

namespace burn{
class Gui;
}

template class BURNGINE_API std::vector<burn::Gui*>;

namespace burn {

class BURNGINE_API GuiNode {
public:
	GuiNode();
	virtual ~GuiNode();

	virtual void draw() = 0;

	void setPosition(const Vector2f& position);
	const Vector2f& getPosition() const;

	void setRotation(const float& rotation);
	const float& getRotation() const;

	void rotate(const float& angle);
	void move(const Vector2f& offset);
	void move(const float& offsetX, const float& offsetY);

	void setZIndex(const Int32& zIndex);
	const Int32& getZIndex() const;

	void addParentGui(Gui* parent);
	void removeParentGui(Gui* parent);

protected:
	Vector2f _position;
	float _rotation;
	Int32 _zIndex;
	std::vector<Gui*> _parents;
};

} /* namespace burn */
#endif /* GUINODE_H_ */
