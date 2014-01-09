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

#include <Burngine/Graphics/Texture/ShadowMap.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

namespace burn {

ShadowMap::ShadowMap() :
_framebuffer(0),
_texture(0),
_resolution(HIGH),
_isCreated(false) {
}

ShadowMap::~ShadowMap() {
	cleanup();
}

void ShadowMap::cleanup() {

	if(!isCreated())
		return;

	ensureContext();

	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_framebuffer);

	_isCreated = false;

}

bool ShadowMap::create(const Resolution& resolution) {

	//Cleanup first
	cleanup();

	ensureContext();

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
	ensureContext();

	if(!isCreated()){
		Reporter::report("Binding of uncreated ShadowMap disallowed!", Reporter::WARNING);
		return;
	}

	OpenGlControl::bindDrawBuffer(_framebuffer);
	glViewport(0, 0, _resolution, _resolution);
}

bool ShadowMap::isCreated() const {
	return _isCreated;
}

void ShadowMap::clear() {

	ensureContext();

	if(!isCreated()){
		Reporter::report("Unable to clear ShadowMap. ShadowMap not created!", Reporter::WARNING);
		return;
	}

	//Get previous binding
	const GLuint& previousDrawBuffer = OpenGlControl::getDrawBufferBinding();

	//Clear buffers
	OpenGlControl::bindDrawBuffer(_framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Restore previous binding
	OpenGlControl::bindDrawBuffer(previousDrawBuffer);

}

void ShadowMap::onBind(const unsigned int& unit) const {

	if(!isCreated()){
		Reporter::report("Unable to bind ShadowMap. ShadowMap not created!", Reporter::ERROR);
		return;
	}

	bindTexture(_texture, unit);
}

} /* namespace burn */
