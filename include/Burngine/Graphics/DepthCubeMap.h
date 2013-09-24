/*
 * DepthCubeMap.h
 *
 *  Created on: 24.09.2013
 *      Author: Dominik
 */

#ifndef DEPTHCUBEMAP_H_
#define DEPTHCUBEMAP_H_

#include "../Export.h"
#include "OpenGL.h"

namespace burn {

class BURNGINE_API DepthCubeMap {
public:
	DepthCubeMap();
	~DepthCubeMap();

	bool create();
	void destroy();

	void bind(GLenum face);
	void clear();

private:
	bool _isCreated;
	GLuint _framebuffer, _cubemap;

};

} /* namespace burn */
#endif /* DEPTHCUBEMAP_H_ */
