/*
 * BaseTexture.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#ifndef BASETEXTURE_H_
#define BASETEXTURE_H_

#include "../Export.h"
#include "OpenGL.h"
#include "../System/Math.h"

namespace burn {

class BURNGINE_API BaseTexture {
public:

	enum MagnificationFiltering {
		MAG_NEAREST, MAG_BILINEAR
	};
	enum MinificationFiltering {
		MIN_NEAREST, MIN_BILINEAR, MIN_TRILINEAR, MIN_NEAREST_MIPMAP, MIN_BILINEAR_MIPMAP
	};

	BaseTexture();
	~BaseTexture();

	bool create(const Vector2ui& dimensions);
	void destroy();

	void setFiltering(const MagnificationFiltering& mag, const MinificationFiltering& min);

	void bind();
	static void unbind();

	bool isCreated() const;

protected:
	GLuint _texture; ///< Texture's ID
private:
	Uint32 nextPowerOf2(const Uint32& n) const;
	GLint getCurrentBoundTexture() const;
	void generate();
	void cleanup();
	void updateFiltering();

	GLuint _sampler; ///< Sampler's ID

	Vector2ui _dimensions; ///< Width and height. Always a power of 2
	Vector2ui _originalDimensions; ///< Original dimension. May be no power of 2

	bool _mipmapsGenerated; ///< Whether or not mipmaps have been generated
	MagnificationFiltering _magnificationFiltering; ///< Used magnification filtering method
	MinificationFiltering _minificationFiltering; ///< Used minification filtering method
};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
