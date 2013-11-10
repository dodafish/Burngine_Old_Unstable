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

#ifndef BASETEXTURE_H_
#define BASETEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>
#include <Burngine/Graphics/Texture/Sampler.h>

namespace burn {

/**
 * @brief Baseclass providing major features for textures
 */
class BURNGINE_API BaseTexture {
public:
	/**
	 * @brief Converts any dimensions into powers of 2 dimensions.
	 * E.g. 130x100 will become 256x128
	 *
	 * @param dimensions The dimensions to convert from
	 *
	 * @return The dimensions as powers of 2
	 */
	static Vector2ui calculatePow2Dimensions(const Vector2ui& dimensions);

	/**
	 * @brief Default constructor creating the Sampler
	 */
	BaseTexture();

	/**
	 * @brief Default Destructor for derived classes
	 */
	virtual ~BaseTexture();

	/**
	 * @brief Binds a texture as source to sample from the given
	 * texture unit
	 *
	 * @param unit The unit where the texture binds to
	 *
	 * @see unbindAsSource()
	 */
	void bindAsSource(const unsigned int& unit = 0) const;

	/**
	 * @brief Unbinds everything (texture and sampler) from the given
	 * texture unit
	 *
	 * @param unit The unit to free
	 *
	 * @see bindAsSource()
	 *
	 * @note This is freeing only the source, not the target. Only sampling
	 * from the chosen unit is impossible when not bound later
	 */
	void unbindAsSource(const unsigned int& unit = 0) const;

	/**
	 * @brief Sets the filtering method of the texture.
	 *
	 * @param mag The magnification filter
	 * @param min The minification filter
	 *
	 * @return Returns true if settings filtering was successful.
	 * False otherwise
	 *
	 * @see Sampler::setFiltering()
	 */
	bool setFiltering(const Sampler::MagnificationFiltering& mag, const Sampler::MinificationFiltering& min);

	/**
	 * @brief Sets a sampler parameter to the given value
	 *
	 * @param parameter The sampler parameter
	 * @param value The value of the parameter
	 */
	bool setSamplerParameter(GLenum parameter, GLenum value);

	/**
	 * @brief Sets the anisotropical level. The lowest one and
	 * meaning "no anisotropic filtering" is 1.0f
	 *
	 * @param level The anisotropical level
	 */
	bool setAnisotropicLevel(const GLfloat& level);

protected:

	virtual void onBind(const unsigned int& unit) const = 0;
	virtual void onUnbind(const unsigned int& unit) const = 0;

private:
	Sampler _sampler;
};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
