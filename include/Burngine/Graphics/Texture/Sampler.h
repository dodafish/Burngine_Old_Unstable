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

class BURNGINE_API Sampler {
public:

	static void unbind(const unsigned int& unit = 0);

	//////////////////////////////////////////////////

	Sampler();
	Sampler(const Sampler& other);
	Sampler& operator=(const Sampler& other);
	~Sampler();

	bool create();
	void destroy();

	bool isCreated() const;

	void bind(const unsigned int& unit = 0) const ;

	enum MagnificationFiltering {
		MAG_NEAREST, MAG_BILINEAR
	};
	enum MinificationFiltering {
		MIN_NEAREST, MIN_BILINEAR, MIN_TRILINEAR, MIN_NEAREST_MIPMAP, MIN_BILINEAR_MIPMAP
	};

	bool setFiltering(const MagnificationFiltering& mag, const MinificationFiltering& min);
	bool setSamplerParameter(GLenum parameter, GLenum value);
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
