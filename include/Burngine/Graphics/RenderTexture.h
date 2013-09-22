/*
 * RenderTexture.h
 *
 *  Created on: 22.09.2013
 *      Author: Dominik
 */

#ifndef RENDERTEXTURE_H_
#define RENDERTEXTURE_H_

#include "../Export.h"
#include "OpenGL.h"

namespace burn {

class BURNGINE_API RenderTexture {
public:
	RenderTexture();
	~RenderTexture();

	bool create(const unsigned int& width = 1024, const unsigned int& height = 768);

	void bind(const RenderTexture* renderTexture) const;

private:
	void destroy();

	GLuint _framebuffer, _texture, _depthbuffer;
	bool _isCreated;
	unsigned int _width, _height;
};

} /* namespace burn */
#endif /* RENDERTEXTURE_H_ */
