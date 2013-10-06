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

	bool create(const Vector2ui& dimensions, const Uint8& bpp = 24);
	void destroy();

	void bind();
	static void unbind();

private:
	Uint32 nextPowerOf2(const Uint32& n) const;

	void generate();
	void cleanup();

	bool validBpp(const Uint8& bpp) const;

	GLuint _texture, _sampler; ///< Texture's and Sampler's ID
	Vector2ui _dimensions; ///< Width and height
	Uint8 _bpp; ///< Bits per pixel
	bool _mipmapsGenerated; ///< Whether or not mipmaps have been generated
	MagnificationFiltering _magnificationFiltering; ///< Used magnification filtering method
	MinificationFiltering _minificationFiltering; ///< Used minification filtering method
};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
