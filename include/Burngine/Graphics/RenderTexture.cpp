/*
 * RenderTexture.cpp
 *
 *  Created on: 22.09.2013
 *      Author: Dominik
 */

#include "RenderTexture.h"
#include "Window.h"

#include <iostream>

namespace burn {

RenderTexture::RenderTexture() :
				_framebuffer(0),
				_texture(0),
				_depthbuffer(0),
				_isCreated(false) {
}

RenderTexture::~RenderTexture() {
	destroy();
}

bool RenderTexture::create() {
	if(Window::isContextCreated() && !_isCreated){
		//Framebuffer:
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		//Texture:
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Depthbuffer:
		glGenRenderbuffers(1, &_depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

		//Configure:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		//Check:
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			std::cout << "RenderTexture: Failed to create framebuffer!\n";
			return false;
		}

		//Unbind all:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		_isCreated = true;
		return true;
	}
	return false;
}

void RenderTexture::destroy() {
	if(Window::isContextCreated() && _isCreated){
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteTextures(1, &_texture);
		glDeleteRenderbuffers(1, &_depthbuffer);

		_isCreated = false;
	}
}

} /* namespace burn */
