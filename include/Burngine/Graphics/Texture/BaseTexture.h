/*
 * BaseTexture.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#ifndef BASETEXTURE_H_
#define BASETEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>

namespace burn {

class BURNGINE_API BaseTexture {
public:

	static GLfloat getMaxAnisotropicLevel();
	static Vector2ui calculatePow2Dimensions(const Vector2ui& dimensions);

	enum MagnificationFiltering {
		MAG_NEAREST, MAG_BILINEAR
	};
	enum MinificationFiltering {
		MIN_NEAREST, MIN_BILINEAR, MIN_TRILINEAR, MIN_NEAREST_MIPMAP, MIN_BILINEAR_MIPMAP
	};

	BaseTexture();
	BaseTexture(const BaseTexture& other);
	BaseTexture& operator=(const BaseTexture& other);
	virtual ~BaseTexture();

	void setFiltering(const MagnificationFiltering& mag, const MinificationFiltering& min);
	void setSamplerParameter(GLenum parameter, GLenum value);
	void setAnisotropicLevel(const GLfloat& level);
	const GLfloat& getAnisotropicLevel() const;

	virtual void bind(const unsigned int& unit) const = 0;
	bool isCreated() const = 0;

protected:
	bool createSampler();

	void updateFiltering() const;

	GLuint _sampler; ///< Sampler's ID

	unsigned int* _referenceCount;
private:
	void destroySampler();

	MagnificationFiltering _magnificationFiltering; ///< Used magnification filtering method
	MinificationFiltering _minificationFiltering; ///< Used minification filtering method
	GLfloat _anisotropicLevel;
};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
