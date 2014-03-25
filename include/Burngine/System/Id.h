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

#ifndef ID_H_
#define ID_H_

#include <Burngine/Export.h>

#include <vector>

namespace burn {

class BURNGINE_API Id {
public:
	Id();
	~Id();

	bool operator==(const Id& other) const;

	const Uint64& get() const;
	std::string asString() const;

private:
	//Non-Copyable:
	Id(const Id& other);
	Id& operator=(const Id& other);

	//Static
	static Uint64 _nextId;
	static std::vector<Uint64> _releasedIds;
	bool getReleasedId();

	//The actual id
	Uint64 _myId;
};

} /* namespace burn */
#endif /* ID_H_ */
