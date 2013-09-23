/*
 * DepthTexture.h
 *
 *  Created on: 23.09.2013
 *      Author: Dominik
 */

#ifndef DEPTHTEXTURE_H_
#define DEPTHTEXTURE_H_

#include "../Export.h"
#include "OpenGL.h"

namespace burn {

class BURNGINE_API DepthTexture {
public:
	DepthTexture();
	~DepthTexture();

	bool create();
	void destroy();

	void bind();
	void clear();

private:
	GLuint _framebuffer, _texture;
	bool _isCreated;
};

} /* namespace burn */
#endif /* DEPTHTEXTURE_H_ */
