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

#include <Burngine/System/Name.h>

namespace burn {

std::hash<std::string> Name::_stringToHash;

Name::Name(const std::string& s) :
_string(s),
_hash(_stringToHash(s)) {

}

void Name::setString(const std::string& s) {
	_string = s;
	_hash = _stringToHash(s);
}

const std::string& Name::getString() const {
	return _string;
}

const size_t& Name::getHash() const{
	return _hash;
}

bool Name::operator==(const Name& other) const {
	return _hash == other.getHash();
}

bool Name::operator==(const std::string& s) const {
	return _hash == _stringToHash(s);
}

} /* namespace burn */
