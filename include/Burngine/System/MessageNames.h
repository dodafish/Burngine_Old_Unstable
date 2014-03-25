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

#ifndef MESSAGENAMES_H_
#define MESSAGENAMES_H_

#include <string>

namespace burn{

	/**
	 * Names for Messages and for parameters
	 */
	namespace mn{
		const std::string SCENENODE_DESTRUCTED = "MBUS__SCENENODE_DESTRUCTED";
		const std::string PHYSICALSCENENODE_DESTRUCTED = "MBUS__PHYSICALSCENENODE_DESTRUCTED";
		const std::string LIGHT_DESTRUCTED = "MBUS__LIGHT_DESTRUCTED";
	}
	namespace mp{
		const std::string COMPONENT_ID = "MBUS__COMPONENT_ID";
	}

}

#endif /* MESSAGENAMES_H_ */
