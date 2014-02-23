//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#include <Burngine/Graphics/Scene/GBuffer.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
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

	ensureContext();

	_dimensions = dimensions;

	//Framebuffer:
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);

	//Textures
	std::vector<GLenum> drawBuffers;

	glGenTextures(COUNT, _textures);

	//Diffuse part texture:
	glBindTexture(GL_TEXTURE_2D, _textures[DIFFUSE]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + DIFFUSE, GL_TEXTURE_2D, _textures[DIFFUSE], 0);
	drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + DIFFUSE);

	//Normal part texture:
	glBindTexture(GL_TEXTURE_2D, _textures[NORMAL_WS]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER,
	GL_COLOR_ATTACHMENT0 + NORMAL_WS,
							GL_TEXTURE_2D, _textures[NORMAL_WS], 0);
	drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + NORMAL_WS);

	//Position part texture:
	glBindTexture(GL_TEXTURE_2D, _textures[POSITION_WS]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER,
	GL_COLOR_ATTACHMENT0 + POSITION_WS,
							GL_TEXTURE_2D, _textures[POSITION_WS], 0);
	drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + POSITION_WS);

	//DepthTexture:
	glGenTextures(1, &_depthTexture);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glTexImage2D( GL_TEXTURE_2D, 0,
	GL_DEPTH_COMPONENT32,
					_dimensions.x, _dimensions.y, 0,
					GL_DEPTH_COMPONENT,
					GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

	ensureContext();

	if(!_isCreated){
		Reporter::report("Unable to clear gBuffer. gBuffer is not created.", Reporter::WARNING);
		return;
	}

	//Get previous binding
	const GLuint& previousDrawBuffer = OpenGlControl::getDrawBufferBinding();

	//Clear buffers
	OpenGlControl::bindDrawBuffer(_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Restore previous bindings
	OpenGlControl::bindDrawBuffer(previousDrawBuffer);
}

void GBuffer::bindAsTarget() const {

	ensureContext();

	if(!_isCreated){
		Reporter::report("Unable to bind gBuffer. gBuffer is not created.", Reporter::ERROR);
		return;
	}

	OpenGlControl::bindDrawBuffer(_framebuffer);
	glViewport(0, 0, _dimensions.x, _dimensions.y);
}

void GBuffer::bindAsSource(const unsigned int& offset) const {

	ensureContext();

	if(!_isCreated){
		Reporter::report("Unable to bind gBuffer. gBuffer is not created.", Reporter::ERROR);
		return;
	}

	OpenGlControl::bindReadBuffer(_framebuffer);

	//Bind all buffers to texture units
	for(unsigned int i = 0; i != COUNT; ++i){
		//BaseTexture::bindTexture(_textures[i], i + offset);
		//Sampler::unbind(i + offset);
		glActiveTexture(GL_TEXTURE0 + i + offset);
		glBindTexture(GL_TEXTURE_2D, _textures[i]);
		glBindSampler(i + offset, 0);

	}
	//And the depth one
	//BaseTexture::bindTexture(_depthTexture, COUNT + offset);
	//Sampler::unbind(COUNT + offset);
	glActiveTexture(GL_TEXTURE0 + COUNT + offset);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glBindSampler(COUNT + offset, 0);

}

void GBuffer::setSourceBuffer(const GBufferType& buffer) {

	ensureContext();

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

	ensureContext();

	if(!_isCreated){
		Reporter::report("Unable to bind gBuffer. gBuffer is not created.", Reporter::ERROR);
		return;
	}

	//BaseTexture::bindTexture(_depthTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glBindSampler(0, 0);
}

} /* namespace burn */
