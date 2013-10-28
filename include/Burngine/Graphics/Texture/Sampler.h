/*
 * Sampler.h
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

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
