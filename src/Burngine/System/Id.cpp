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

#include <Burngine/System/Id.h>
#include <Burngine/System/Log.h>

#include <iostream>
#include <stdio.h>
#include <sstream>

namespace burn {

Uint64 Id::_nextId = 0;
std::vector<Uint64> Id::_releasedIds;

bool Id::getReleasedId() {
	if(_releasedIds.size() == 0)
		return false;

	_myId = _releasedIds[_releasedIds.size() - 1];
	_releasedIds.pop_back();

	return true;
}

Id::Id() {
	if(!getReleasedId())
		_myId = _nextId++;

	if(_nextId == 2000000000){
		Log::log("2 Mrd IDs had been created... That must not happen!", Log::WARNING);
	}
}

Id::~Id() {
	_releasedIds.push_back(_myId);
}

bool Id::operator==(const Id& other) const {
	return _myId == other.get();
}

const Uint64& Id::get() const {
	return _myId;
}

std::string Id::asString() const {
	std::stringstream ss;
	ss << _myId;
	return ss.str();
}


} /* namespace burn */
