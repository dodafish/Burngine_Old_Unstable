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

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>
//#include <Burngine/Graphics/Texture/RenderTarget.h>

namespace burn {

/**
 * @brief A texture that holds 2D pixeldata
 */
class BURNGINE_API Texture : public BaseTexture {
public:
	Texture();
	Texture(const Vector2ui& dimensions,
			const InternalFormat& internalFormat);
	~Texture();

	///////////////////////////////////////////////////////////////////////////

	bool create(const Vector2ui& dimensions,
				const InternalFormat& internalFormat,
				GLubyte* data = 0);

	bool bind(const Uint32& unit = 0) const;

	bool loadFromFile(const std::string& file);

	const GLuint& getId() const;

private:
	GLuint _textureId;

	//Cleans up OpenGL
	void cleanup();

	//Once called at first instanciation
	void ensureConstants();

	//Used for keeping track of bound textures
	static GLuint _currentTexture2DBinding[MAX_TEXTURE_BINDINGS];
};

} /* namespace burn */
#endif /* TEXTURE_H_ */
