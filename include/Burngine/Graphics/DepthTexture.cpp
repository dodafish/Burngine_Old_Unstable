/*
 * DepthTexture.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dominik
 */

#include "DepthTexture.h"
#include "Window.h"

namespace burn {

DepthTexture::DepthTexture() :
				_framebuffer(0),
				_texture(0),
				_isCreated(false) {
}

DepthTexture::~DepthTexture() {
	destroy();
}

bool DepthTexture::create() {

	if(!Window::isContextCreated())
		return false;

	destroy(); //if needed

	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
		_isCreated = true;
	}
	return _isCreated;
}

void DepthTexture::destroy() {
	if(_isCreated){
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteTextures(1, &_texture);
		_isCreated = false;
	}
}

void DepthTexture::bind() {
	if(_isCreated && Window::isContextCreated()){
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glViewport(0, 0, 1024, 1024);
	}
}

void DepthTexture::clear() {
	if(_isCreated && Window::isContextCreated()){
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		GLint lastFB = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	}
}

} /* namespace burn */
