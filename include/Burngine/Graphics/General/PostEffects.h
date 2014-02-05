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

#ifndef POSTEFFECTS_H_
#define POSTEFFECTS_H_

#include <Burngine/Export.h>

namespace burn {

//class VarianceShadowMap;

class BURNGINE_API PostEffects {
public:

	//static void gaussianBlur(	const VarianceShadowMap& vsm,
	//							const float& blurSize = 0.001f);

private:

	//C-Tor as private (also older C++) as alternative to deleting it (only C++11)
	//Static class
	PostEffects() = delete;

};

} /* namespace burn */
#endif /* POSTEFFECTS_H_ */
