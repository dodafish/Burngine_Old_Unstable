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

#include <Burngine/Graphics/Texture/VarianceShadowMap.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/System/Reporter.h>

namespace burn {

VarianceShadowMap::VarianceShadowMap() :
_framebuffer(0),
_depthbuffer(0),
_texture(0),
_isCreated(false) {

}

VarianceShadowMap::~VarianceShadowMap() {
	cleanup();
}

void VarianceShadowMap::cleanup() {
	if(!_isCreated)
		return;

	ensureContext();

	glDeleteFramebuffers(1, &_framebuffer);
	glDeleteRenderbuffers(1, &_depthbuffer);
	glDeleteTextures(1, &_texture);

	_framebuffer = 0;
	_texture = 0;
	_depthbuffer = 0;

	_isCreated = false;
}

bool VarianceShadowMap::create(const Vector2ui& dimensions) {

	cleanup();
	_dimensions = dimensions;

	GLuint previousDrawBufferBinding = OpenGlControl::getDrawBufferBinding();
	GLuint previousTextureBinding = getTextureBinding(0);
	GLuint previousRenderBufferBinding = OpenGlControl::getRenderBufferBinding();

	ensureContext();

	//Framebuffer:
	glGenFramebuffers(1, &_framebuffer);
	OpenGlControl::bindDrawBuffer(_framebuffer);

	//Texture:
	glGenTextures(1, &_texture);
	//Clear texture
	bindTexture(_texture, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, _dimensions.x, _dimensions.y, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Depthbuffer:
	glGenRenderbuffers(1, &_depthbuffer);
	OpenGlControl::bindRenderBuffer (_depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _dimensions.x, _dimensions.y);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

	//Configure:
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);
	GLenum drawBuffers[1] = {
	GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	//Check:
	if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("RenderTexture: Failed to create framebuffer!\n", Reporter::ERROR);
		return false;
	}

	//Restore previous bindings
	OpenGlControl::bindDrawBuffer(previousDrawBufferBinding);
	bindTexture(previousTextureBinding, 0);
	OpenGlControl::bindRenderBuffer(previousRenderBufferBinding);

	_isCreated = true;
	return true;
}

void VarianceShadowMap::bindAsTarget() const {

	if(!_isCreated)
		return;

	OpenGlControl::bindDrawBuffer(_framebuffer, true);
	glViewport(0, 0, _dimensions.x, _dimensions.y);
}

void VarianceShadowMap::clear() const {

	GLuint previousDrawBufferBinding = OpenGlControl::getDrawBufferBinding();

	OpenGlControl::bindDrawBuffer(_framebuffer, true);

	ensureContext();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OpenGlControl::bindDrawBuffer(previousDrawBufferBinding);
}

bool VarianceShadowMap::isCreated() const {
	return _isCreated;
}

void VarianceShadowMap::onBind(const unsigned int& unit) const {

	if(!isCreated()){
		Reporter::report("Unable to bind VarianceShadowMap. VarianceShadowMap not created!", Reporter::ERROR);
		return;
	}

	bindTexture(_texture, unit);
}

const Vector2ui& VarianceShadowMap::getDimensions() const {

	return _dimensions;

}

} /* namespace burn */
