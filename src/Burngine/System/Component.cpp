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

#include <Burngine/System/Component.h>

namespace burn {

Component::Component() {
	//std::cout << "Component created (Ctor) with ID: " << _id.asString() << "\n";
}

Component::Component(const Component& other) {
	//std::cout << "Component created (Copy-Ctor) with ID: " << _id.asString() << "\n";
}

Component& Component::operator=(const Component& other) {

	if(this == &other)
		return *this;

	return *this;
}

Component::~Component() {
	//std::cout << "Component destructed with ID: " << _id.asString() << "\n";
}

const Id& Component::getId() const {
	return _id;
}

} /* namespace burn */
