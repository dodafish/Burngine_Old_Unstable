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

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

/**
 * @brief Holds OpenGL comfort images as textures.
 */
class BURNGINE_API Texture : public BaseTexture {
public:
	/**
	 * @brief Initializes some values
	 */
	Texture();

	/**
	 * @brief Copies the attributes and data from another
	 * Texture
	 */
	Texture(const Texture& other);

	/**
	 * @brief Copies the attributes and data from another
	 * Texture
	 */
	Texture& operator=(const Texture& other);

	/**
	 * Destructor cleaning up Texture
	 */
	~Texture();

	/**
	 * @brief Loads an image from file and stores it
	 * as texture ready for use.
	 *
	 * @param file The image to load
	 *
	 * @return Returns true on success
	 */
	bool loadFromFile(const std::string& file);

	/**
	 * @brief Loads an image from data and stores it
	 * as texture ready for use.
	 *
	 * @param data The data to load from
	 * @param dimensions The image's dimensions
	 * @param bpp The bits-per-pixel
	 * @param format The internal format (RGB, LUMINANCE etc.)
	 *
	 * @return Returns true on success. False otherwise
	 */
	bool loadFromData(GLubyte* data, const Vector2ui& dimensions, const Int32& bpp, const GLenum& format);

protected:
	GLuint _textureId;

	virtual void onBind(const unsigned int& unit) const;
	virtual void onUnbind(const unsigned int& unit) const;

private:
	void generate();
	void cleanup();

	unsigned int* _referenceCount;
};

} /* namespace burn */
#endif /* TEXTURE_H_ */
