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
_texture(0),
_resolution(HIGH),
_isCreated(false){
}



ShadowMap::~ShadowMap() {
}

void ShadowMap::cleanup(){

	if(!isCreated())
		return;

	if(!Window::isContextCreated()){
		Reporter::report("Unable to cleanup shadowmap. No valid context!", Reporter::ERROR);
		return;
	}

	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_framebuffer);

	_isCreated = false;

}

bool ShadowMap::create(const Resolution& resolution) {

	//Cleanup first
	cleanup();

	if(!Window::isContextCreated()){
		Reporter::report("Unable to create ShadowMap. No valid context!", Reporter::ERROR);
		return false;
	}

	//Save old bindings
	GLint lastFB = 0;
	GLint lastTex = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

	//Save resolution
	_resolution = resolution;

	//Generate texture
	glGenTextures(1, &_texture);

	//Generate framebuffer
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	//Bind and modify texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	setSamplerParameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

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

	_isCreated = true;

	return true;
}

void ShadowMap::bindAsRendertarget() const {
	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated()){
		Reporter::report("Unable to bind ShadowMap. No valid context!", Reporter::ERROR);
		return;
	}

	if(!isCreated()){
		Reporter::report("Binding of uncreated ShadowMap disallowed!", Reporter::WARNING);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glViewport(0, 0, _resolution, _resolution);
}

bool ShadowMap::isCreated() const {
	return _isCreated;
}

void ShadowMap::clear() const {

	if(!Window::isContextCreated()){
			Reporter::report("Unable to clear ShadowMap. No valid context!", Reporter::ERROR);
			return;
		}

		if(!isCreated()){
			Reporter::report("Unable to clear ShadowMap. ShadowMap not created!", Reporter::WARNING);
			return;
		}

		//Get previous bindings
		GLint previousTexture = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
		GLint lastFB = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);

		//Clear texture
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _resolution, _resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		//Clear buffers
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		//Restore previous bindings
		glBindTexture(GL_TEXTURE_2D, previousTexture);
		glBindFramebuffer(GL_FRAMEBUFFER, lastFB);

}

void ShadowMap::onBind(const unsigned int& unit) const {

	if(!isCreated()){
		Reporter::report("Unable to bind ShadowMap. ShadowMap not created!", Reporter::ERROR);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, _texture);
}

void ShadowMap::onUnbind(const unsigned int& unit) const {

	glBindTexture(GL_TEXTURE_2D, 0);

}

} /* namespace burn */
