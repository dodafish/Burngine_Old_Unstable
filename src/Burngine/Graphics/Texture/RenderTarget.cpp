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

#include <Burngine/Graphics/Texture/RenderTarget.h>

#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/System/Reporter.h>

namespace burn {

RenderTarget::ColorAttachment::ColorAttachment(	const GLuint& textureIdArg,
												const GLuint& attachmentPositionArg) :
textureId(textureIdArg),
attachmentPosition(attachmentPositionArg) {
}

RenderTarget::RenderTarget() :
_framebuffer(0),
_depthbuffer(0) {

}

RenderTarget::~RenderTarget() {
	cleanup();
}

void RenderTarget::cleanup() {

	if(_framebuffer != 0){
		ensureContext();
		glDeleteFramebuffers(1, &_framebuffer);
		_framebuffer = 0;
		if(_depthbuffer != 0){
			glDeleteRenderbuffers(1, &_depthbuffer);
			_depthbuffer = 0;
		}
	}

}

bool RenderTarget::create(	const Vector2ui& dimensions,
							const DepthbufferType& depthbufferType,
							const Texture& texture) {

	if(dimensions.x == 0 || dimensions.y == 0){
		Reporter::report("Unable to create RenderTarget! Dimensions are too little.", Reporter::ERROR);
		return false;
	}

	if(texture.getId() == 0){
		Reporter::report("Unable to create RenderTarget! Initial color attachment not created.", Reporter::ERROR);
		return false;
	}

	//Store new attributes
	_dimensions = dimensions;

	ensureContext();

	//We want to remake whole rendertarget
	cleanup();

	//Fetch current bindings
	GLuint previousDrawBufferBinding = OpenGlControl::getDrawBufferBinding();
	GLuint previousRenderBufferBinding = OpenGlControl::getRenderBufferBinding();

	//Generate framebuffer
	glGenFramebuffers(1, &_framebuffer);
	OpenGlControl::bindDrawBuffer(_framebuffer);

	//Generate depthbuffer when needed
	if(depthbufferType != NO_DEPTHBUFFER){
		glGenRenderbuffers(1, &_depthbuffer);
		OpenGlControl::bindRenderBuffer(_depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, depthbufferType, _dimensions.x, _dimensions.y);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);
	}

	//addColorAttachment(texture, 0);

	//For later attachments:
	ColorAttachment ca(texture.getId(), 0);
	_colorAttachments.push_back(ca);

	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.getId(), 0);
	GLenum drawBuffers[1] = {
	GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("RenderTexture: Failed to create render target!\n", Reporter::ERROR);
		return false;
	}

	//Restore previous bindings
	OpenGlControl::bindDrawBuffer(previousDrawBufferBinding);
	OpenGlControl::bindRenderBuffer(previousRenderBufferBinding);

	return _framebuffer != 0;
}

bool RenderTarget::addColorAttachment(	const Texture& texture,
										const GLuint& attachmentPosition) {

	if(_framebuffer == 0){
		Reporter::report("Cannot add color attachment. No framebuffer created!", Reporter::ERROR);
		return false;
	}

	if(texture.getDimensions() != _dimensions){
		Reporter::report("Unable to add color attachment. Attachment has wrong dimensions!", Reporter::ERROR);
		return false;
	}

	if(attachmentPosition >= GL_MAX_COLOR_ATTACHMENTS){
		Reporter::report("Unable to add color attachment. Attachment position out of range!", Reporter::ERROR);
		return false;
	}

	for(size_t i = 0; i < _colorAttachments.size(); ++i){
		if(_colorAttachments[i].attachmentPosition == attachmentPosition){
			Reporter::report("Unable to add color attachment. Slot already taken!", Reporter::ERROR);
			return false;
		}else if(_colorAttachments[i].textureId == texture.getId()){
			Reporter::report("Unable to add color attachment. Texture already attached!", Reporter::ERROR);
			return false;
		}
	}

	ColorAttachment ca(texture.getId(), attachmentPosition);

	const GLuint& previousDrawBufferBinding = OpenGlControl::getDrawBufferBinding();

	//Reconfigure framebuffer:
	OpenGlControl::bindDrawBuffer(_framebuffer);
	//Connect texture to framebuffer
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER,
	GL_COLOR_ATTACHMENT0 + ca.attachmentPosition,
							GL_TEXTURE_2D, ca.textureId, 0);

	_colorAttachments.push_back(ca);

	//Tell the framebuffer the new drawbufferset
	std::vector<GLenum> drawBuffers;
	for(size_t i = 0; i < _colorAttachments.size(); ++i){
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + _colorAttachments[i].attachmentPosition);
	}
	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

	//Check:
	if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("RenderTexture: Failed to add color attachment!\n", Reporter::ERROR);
		return false;
	}

	//Restore:
	OpenGlControl::bindDrawBuffer(previousDrawBufferBinding);

	return true;
}

bool RenderTarget::removeColorAttachment(const Texture& texture) {

	if(_framebuffer == 0){
		//Framebuffer is not created. No need to remove something (nothing attached)
		return true;
	}

	//Lookup texture
	for(size_t i = 0; i < _colorAttachments.size(); ++i){
		if(_colorAttachments[i].textureId == texture.getId()){

			_colorAttachments.erase(_colorAttachments.begin() + i);

			const GLuint& previousDrawBufferBinding = OpenGlControl::getDrawBufferBinding();

			//Reconfigure framebuffer:
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);

			//Tell the framebuffer the new drawbufferset
			std::vector<GLenum> drawBuffers;
			for(size_t i = 0; i < _colorAttachments.size(); ++i){
				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + _colorAttachments[i].attachmentPosition);
			}
			glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

			//Check:
			if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
				Reporter::report("RenderTexture: Failed to remove color attachment!\n", Reporter::ERROR);
				return false;
			}

			//Restore:
			OpenGlControl::bindDrawBuffer(previousDrawBufferBinding);

			return true;
		}
	}

	//Texture was not attached
	return true;
}

bool RenderTarget::bind() const {

	if(_framebuffer == 0)
		return false;

	ensureContext();

	glViewport(0, 0, _dimensions.x, _dimensions.y);
	OpenGlControl::bindDrawBuffer(_framebuffer);

	return true;
}

void RenderTarget::clear() const {

	if(_framebuffer == 0)
		return;

	ensureContext();

	const GLuint& previousDrawBufferBinding = OpenGlControl::getDrawBufferBinding();

	OpenGlControl::bindDrawBuffer(_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Restore:
	OpenGlControl::bindDrawBuffer(previousDrawBufferBinding);

}

} /* namespace burn */
