/*
 * VertexBufferObject.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

VertexBufferObject::VertexBufferObject() :
_lastBuffer(0),
_isCreated(false),
_isDataUploaded(false),
_buffer(0) {
}

VertexBufferObject::~VertexBufferObject() {
	cleanup();
}

void VertexBufferObject::create() {
	if(Window::isContextCreated())
		if(!_isCreated){

			glGenBuffers(1, &_buffer);

			_isDataUploaded = false;
			_isCreated = true;
		}
}

void VertexBufferObject::cleanup() {
	if(Window::isContextCreated())
		if(_isCreated){

			glDeleteBuffers(1, &_buffer);

			_isDataUploaded = false;
			_isCreated = false;
		}
}

void VertexBufferObject::reset() {
	_data.clear();
	_isDataUploaded = false;
}

void VertexBufferObject::bind(const GLint& type) const {
	if(Window::isContextCreated())
		glBindBuffer(type, _buffer);
}

void VertexBufferObject::uploadDataToGpu(const GLint& type, const GLint& usageHint) {
	//Don't do anything, when there is no OpenGL-Context
	if(!Window::isContextCreated()){
		return;
	}

	//Ensure that we have a valid buffer
	if(!_isCreated){
		create();
	}

	//Get current buffer id
	_lastBuffer = getLastBuffer();

	//Bind this buffer
	bind(type);

	//Upload our data
	glBufferData(type, _data.size(), &_data[0], usageHint);
	_isDataUploaded = true;

	//Restore buffer bound before
	glBindBuffer(type, _lastBuffer);

}

GLint VertexBufferObject::getLastBuffer(const GLint& type) {
	if(!Window::isContextCreated())
		return 0;
	GLint last = 0;
	glGetIntegerv(type, &last);
	return last;
}

const GLuint& VertexBufferObject::getBuffer() const {
	return _buffer;
}

void VertexBufferObject::addData(const void* data, const unsigned int& size) {
	_data.insert(_data.end(), (GLbyte*)data, (GLbyte*)data + size);
}

} /* namespace burn */
