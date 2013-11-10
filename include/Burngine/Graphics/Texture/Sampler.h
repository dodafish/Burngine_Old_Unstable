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

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

/**
 * @brief Sets values when sampling from a texture
 */
class BURNGINE_API Sampler {
public:

	/**
	 * @brief Unbinds all samplers from the given unit
	 *
	 * @param unit The unit to free
	 *
	 * @see bind()
	 */
	static void unbind(const unsigned int& unit = 0);

	//////////////////////////////////////////////////

	/**
	 * @brief Constructor initializes some values
	 */
	Sampler();

	/**
	 * @brief Copies all attributes of another Sampler
	 */
	Sampler(const Sampler& other);

	/**
	 * @brief Copies all attributes of another Sampler
	 */
	Sampler& operator=(const Sampler& other);

	/**
	 * @brief Destructor cleaning up Sampler
	 */
	~Sampler();

	/**
	 * @brief Creates the sampler
	 *
	 * @return Returns true on success. False otherwise
	 *
	 * @see destroy()
	 */
	bool create();

	/**
	 * @brief Destroys the sampler, just like the destructor does
	 */
	void destroy();

	/**
	 * @Checks if sampler has already been created
	 *
	 * @return Returns true if Sampler has already been created
	 */
	bool isCreated() const;

	/**
	 * @brief Binds the sampler to the given unit
	 *
	 * @param unit The unit which the sampler should be bound
	 * to
	 *
	 * @see unbind()
	 */
	void bind(const unsigned int& unit = 0) const;

	enum MagnificationFiltering {
		MAG_NEAREST, MAG_BILINEAR
	};
	enum MinificationFiltering {
		MIN_NEAREST, MIN_BILINEAR, MIN_TRILINEAR, MIN_NEAREST_MIPMAP, MIN_BILINEAR_MIPMAP
	};

	/**
	 * @brief Sets the filtering method of the sampler.
	 *
	 * @param mag The magnification filter
	 * @param min The minification filter
	 *
	 * @return Returns true if settings filtering was successful.
	 * False otherwise
	 */
	bool setFiltering(const MagnificationFiltering& mag, const MinificationFiltering& min);

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

	const GLfloat& getAnisotropicLevel() const;

private:
	bool updateFiltering() const;

	GLuint _id;
	unsigned int* _referenceCounter;

	MagnificationFiltering _magnificationFiltering; ///< Used magnification filtering method
	MinificationFiltering _minificationFiltering; ///< Used minification filtering method
	GLfloat _anisotropicLevel;

	mutable bool _needsFilteringUpdate;
};

} /* namespace burn */
#endif /* SAMPLER_H_ */
