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

#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

#include <vector>

namespace burn {

class BURNGINE_API CubeMap : public BaseTexture {
public:

	CubeMap();
	CubeMap(const Vector2ui& dimensions,
			const InternalFormat& internalFormat);
	~CubeMap();

	///////////////////////////////////////////////////////////////////////////

	bool create(const Vector2ui& dimensions,
				const InternalFormat& internalFormat);

	bool bind(const Uint32& unit = 0) const;

	bool loadFromFile(	const std::string& filePositiveX,
						const std::string& fileNegativeX,
						const std::string& filePositiveY,
						const std::string& fileNegativeY,
						const std::string& filePositiveZ,
						const std::string& fileNegativeZ);

	const GLuint& getId() const;

private:
	GLuint _cubemapId;

	//Cleans up OpenGL
	void cleanup();

	//Once called at first instanciation
	void ensureConstants();

	//Used for keeping track of bound textures
	static GLuint _currentCubeMapBinding[MAX_TEXTURE_BINDINGS];
};

} /* namespace burn */
#endif /* CUBEMAP_H_ */
