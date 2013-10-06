/*
 * BaseTexture.cpp
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#include "BaseTexture.h"
#include "Window.h"

#include "../System/Reporter.h"
#include <sstream>

namespace burn {

BaseTexture::BaseTexture() :
_texture(0),
_sampler(0),
_bpp(0),
_magnificationFiltering(MAG_NEAREST),
_minificationFiltering(MIN_NEAREST),
_mipmapsGenerated(false) {
}

BaseTexture::~BaseTexture() {
	cleanup();
}

bool BaseTexture::create(const Vector2ui& dimensions, const Uint8& bpp) {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated()){
		Reporter::report("Unable to create texture (No valid OpenGL-Context)!", Reporter::ERROR);
		return false;
	}

	//Generate texture and sampler
	generate();

	//Check BPP
	if(!validBpp(bpp)){
		std::stringstream ss;
		ss << bpp;
		Reporter::report("Unable to create texture (No valid BPP format)! BPP: " + ss.str(), Reporter::ERROR);
		return false;
	}
	_bpp = bpp;
	//TODO RETURN WORKING HERE

	return true;

}

bool BaseTexture::validBpp(const Uint8& bpp) const {

	if(bpp == 8 || bpp == 16 || bpp == 24 || bpp == 32 || bpp == 48 || bpp == 64 || bpp == 96 || bpp == 128)
		return true;
	return false;

}

void BaseTexture::destroy() {
	cleanup();
}

void BaseTexture::bind() {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	//If not created before, this will produce the same effect
	//as unbind()
	glBindTexture(GL_TEXTURE_2D, _texture);

}

void BaseTexture::unbind() {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	glBindTexture(GL_TEXTURE_2D, 0);

}

Uint32 BaseTexture::nextPowerOf2(const Uint32& n) const {

	//The power of 2 value to return
	Uint32 p2 = 1;

	//Look for the next greater power of 2
	while(n > p2){
		p2 << 1; //Shift one to the left. Equals p2 *= 2
	}

	return p2;

}

void BaseTexture::generate() {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	//Reset before generating new texture, sampler etc.
	cleanup();

	//Generate
	glGenTextures(1, &_texture);
	glGenSamplers(1, &_sampler);

}

void BaseTexture::cleanup() {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	//Delete texture and sampler from GPU when needed
	if(_texture != 0){
		glDeleteTextures(1, &_texture);
	}
	if(_sampler != 0){
		glDeleteSamplers(1, &_sampler);
	}

	//Reset values
	_texture = 0;
	_sampler = 0;
	_bpp = 0;
	_magnificationFiltering = MAG_NEAREST;
	_minificationFiltering = MIN_NEAREST;
	_mipmapsGenerated = false;

}

} /* namespace burn */
