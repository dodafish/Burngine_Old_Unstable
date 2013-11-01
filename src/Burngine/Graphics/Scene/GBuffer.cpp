/*
 * GBuffer.cpp
 *
 *  Created on: 01.11.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/GBuffer.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/Texture/Sampler.h>
#include <vector>

namespace burn {

GBuffer::GBuffer() :
_isCreated(false),
_depthTexture(0),
_framebuffer(0) {

}

GBuffer::~GBuffer() {

	if(_isCreated){
		glDeleteTextures(COUNT, _textures);
		glDeleteTextures(1, &_depthTexture);
		glDeleteFramebuffers(1, &_framebuffer);
	}

}

bool GBuffer::create(const Vector2ui& dimensions) {

	if(_isCreated){
		Reporter::report("Attempt of creating gBuffer multiple times!", Reporter::WARNING);
		return false;
	}

	if(!Window::isContextCreated()){
		Reporter::report("Unable to create gBuffer! No valid context!", Reporter::ERROR);
		return false;
	}

	////////////////////////////////////////

	_dimensions = dimensions;

	//Framebuffer:
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);

	//Textures
	std::vector<GLenum> drawBuffers;

	glGenTextures(COUNT, _textures);
	for(unsigned int i = 0; i != COUNT; ++i){
		glBindTexture(GL_TEXTURE_2D, _textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	//DepthTexture:
	glGenTextures(1, &_depthTexture);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _dimensions.x, _dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
					0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Set Filtering for dumping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

	//Configure:
	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

	//Check:
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("gBuffer: Failed to create framebuffer!\n", Reporter::ERROR);
		return false;
	}

	//Unbind all:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/////////////////////////////////////////
	_isCreated = true;
	return true;
}

void GBuffer::clear() {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to clear gBuffer. No valid context.", Reporter::ERROR);
		return;
	}

	if(!_isCreated){
		Reporter::report("Unable to clear gBuffer. gBuffer is not created.", Reporter::WARNING);
		return;
	}

	//Get previous binding
	GLint lastFB = 0;
	GLint previousTex = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTex);

	//Clear buffers
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _dimensions.x, _dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
					0);

	//Restore previous bindings
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_2D, previousTex);
}

void GBuffer::bindAsTarget() const {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to bind gBuffer. No valid context.", Reporter::ERROR);
		return;
	}

	if(!_isCreated){
		Reporter::report("Unable to bind gBuffer. gBuffer is not created.", Reporter::ERROR);
		return;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
}

void GBuffer::bindAsSource() const {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to bind gBuffer. No valid context.", Reporter::ERROR);
		return;
	}

	if(!_isCreated){
		Reporter::report("Unable to bind gBuffer. gBuffer is not created.", Reporter::ERROR);
		return;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);

}

void GBuffer::setSourceBuffer(const GBufferType& buffer) {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to set source gBuffer. No valid context.", Reporter::ERROR);
		return;
	}

	if(buffer == COUNT){
		Reporter::report("Refused attempt to bind gBuffer enum: COUNT.", Reporter::WARNING);
		return;
	}

	if(!_isCreated){
		Reporter::report("Unable to bind gBuffer. gBuffer is not created.", Reporter::ERROR);
		return;
	}

	glReadBuffer(GL_COLOR_ATTACHMENT0 + buffer);

}

const Vector2ui& GBuffer::getDimensions() const {
	return _dimensions;
}

void GBuffer::bindDepthBufferAsSourceTexture() const {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to bind depth gBuffer as source texture. No valid context.", Reporter::ERROR);
		return;
	}

	if(!_isCreated){
		Reporter::report("Unable to bind gBuffer. gBuffer is not created.", Reporter::ERROR);
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	Sampler::unbind(0);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
}

} /* namespace burn */
