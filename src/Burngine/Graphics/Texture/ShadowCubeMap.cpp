//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/Graphics/Texture/ShadowCubeMap.h>

#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/Window/Window.h>

#include <iostream>

namespace burn {

ShadowCubeMap::ShadowCubeMap() :
_framebuffer(0),
_cubeMap(0),
_resolution(HIGH),
_isCreated(false) {
}

ShadowCubeMap::~ShadowCubeMap() {
	cleanup();
}

const ShadowCubeMap::Resolution& ShadowCubeMap::getResolution() const{
	return _resolution;
}

bool ShadowCubeMap::create(const Resolution& resolution) {

	//Cleanup first
	cleanup();

	std::cout << "o";

	ensureContext();

	//Save old bindings
	GLint lastFB = 0;
	GLint lastTex = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &lastTex);
	std::cout << "o";
	//Save resolution
	_resolution = resolution;

	//Generate cubemap
	glGenTextures(1, &_cubeMap);
	std::cout << "o";
	//Generate framebuffer
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	std::cout << "o";
	//Bind and modify cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
	std::cout << "o";
	for(int i = 0; i != 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0,
		GL_DEPTH_COMPONENT,
						GL_FLOAT, 0);
	std::cout << "o";
	setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	setSamplerParameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	std::cout << "o";
	//Bind cubemap to framebuffer
	//for(int i = 0; i != 6; ++i)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, _cubeMap, 0);
	std::cout << "o";
	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	std::cout << "o";
	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("ShadowCubeMap: Failed to create framebuffer!\n", Reporter::ERROR);
		return false;
	}
	std::cout << "o";
	//Restore old bindings
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lastTex);
	std::cout << "o";
	_isCreated = true;

	return true;

}

void ShadowCubeMap::clear() {

	ensureContext();

	if(!isCreated()){
		Reporter::report("Unable to clear ShadowCubeMap. ShadowMap not created!", Reporter::WARNING);
		return;
	}

	//Get previous bindings
	GLint previousTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
	GLint lastFB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);

	//Clear texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
	for(int i = 0; i != 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, _resolution, _resolution, 0,
		GL_DEPTH_COMPONENT,
						GL_FLOAT, 0);

	//Clear buffers
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Restore previous bindings
	glBindTexture(GL_TEXTURE_CUBE_MAP, previousTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);

}

void ShadowCubeMap::bindAsRendertarget(const int& face) const {

	//Valid OpenGL-Context is needed
	ensureContext();

	if(!isCreated()){
		Reporter::report("Binding of uncreated ShadowCubeMap disallowed!", Reporter::WARNING);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, _cubeMap, 0);
	glViewport(0, 0, _resolution, _resolution);
}

bool ShadowCubeMap::isCreated() const {
	return _isCreated;
}

void ShadowCubeMap::onBind(const unsigned int& unit) const {

	if(!isCreated()){
		Reporter::report("Binding of uncreated ShadowCubeMap is disallowed!", Reporter::ERROR);
		return;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);

}

void ShadowCubeMap::onUnbind(const unsigned int& unit) const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void ShadowCubeMap::cleanup() {

	if(!isCreated())
		return;

	ensureContext();

	glDeleteTextures(1, &_cubeMap);
	glDeleteFramebuffers(1, &_framebuffer);

	_isCreated = false;
}

} /* namespace burn */
