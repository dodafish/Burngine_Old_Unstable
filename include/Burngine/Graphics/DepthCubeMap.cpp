/*
 * DepthCubeMap.cpp
 *
 *  Created on: 24.09.2013
 *      Author: Dominik
 */

#include "DepthCubeMap.h"
#include "Window.h"

namespace burn {

DepthCubeMap::DepthCubeMap() :
_isCreated(false),
_framebuffer(0),
_cubemap(0) {

}

DepthCubeMap::~DepthCubeMap() {
	destroy();
}

bool DepthCubeMap::create() {

	if(!Window::isContextCreated()){
		return false;
	}

	destroy(); //if needed

	GLint lastFB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);

	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glGenTextures(1, &_cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap);

	for(int i = 0; i != 6; ++i){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT,
		GL_FLOAT, 0);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Use the following to render to all faces of the cubemap.
	//This actually renders only to one of them!
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, _cubemap, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
		_isCreated = true;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);

	return _isCreated;
}

void DepthCubeMap::destroy() {
	if(Window::isContextCreated() && _isCreated){
		glDeleteTextures(1, &_cubemap);
		_isCreated = false;
	}
}

void DepthCubeMap::bind(GLenum face) {
	if(_isCreated && Window::isContextCreated()){
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glViewport(0, 0, 1024, 1024);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, _cubemap, 0);
	}
}

void DepthCubeMap::clear() {
	if(_isCreated && Window::isContextCreated()){
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap);

		for(int i = 0; i != 6; ++i){
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT,
			GL_FLOAT, 0);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		GLint lastFB = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	}
}

} /* namespace burn */
