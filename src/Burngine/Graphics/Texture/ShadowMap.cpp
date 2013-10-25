/*
 * ShadowMap.cpp
 *
 *  Created on: 25.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Texture/ShadowMap.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>

namespace burn {

ShadowMap::ShadowMap() :
_framebuffer(0),
_resolution(HIGH) {
}

ShadowMap::ShadowMap(const ShadowMap& other) :
_framebuffer(other._framebuffer),
_resolution(other._resolution) {

}

ShadowMap& ShadowMap::operator=(const ShadowMap& other) {

	if(isLastReference()){
		glDeleteFramebuffers(1, &_framebuffer);
	}
	cleanup();

	_framebuffer = other._framebuffer;
	_resolution = other._resolution;

	return *this;

}

ShadowMap::~ShadowMap() {
	if(isLastReference()){
		glDeleteFramebuffers(1, &_framebuffer);
	}

	cleanup();
}

bool ShadowMap::create(const Resolution& resolution) {

	//Cleanup first
	if(isLastReference()){
		glDeleteFramebuffers(1, &_framebuffer);
	}
	cleanup();

	//Save old bindings
	GLint lastFB = 0;
	GLint lastTex = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

	//Save resolution
	_resolution = resolution;

	//Generate texture and sampler
	generate(Vector2ui(resolution, resolution));

	//Generate framebuffer
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	//Bind and modify texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Bind texture to framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("ShadowMap: Failed to create framebuffer!\n", Reporter::ERROR);
		return false;
	}

	//Restore old bindings
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_2D, lastTex);

	return true;
}

void ShadowMap::bindAsRendertarget() const {
	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	if(!isCreated())
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glViewport(0, 0, getOriginalDimensions().x, getOriginalDimensions().y);
}

void ShadowMap::clear() const {

	if(!Window::isContextCreated())
		return;

	GLint previousTexture = getCurrentBoundTexture();
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, getDimensions().x, getDimensions().y, 0, GL_DEPTH_COMPONENT,
					GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, previousTexture);

	GLint lastFB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);

}

} /* namespace burn */
