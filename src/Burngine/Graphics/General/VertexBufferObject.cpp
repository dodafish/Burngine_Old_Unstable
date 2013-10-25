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
_buffer(0),
_referenceCounter(new unsigned int(1)) {
}

VertexBufferObject::VertexBufferObject(const VertexBufferObject& other) :
_lastBuffer(other._lastBuffer),
_isCreated(other._isCreated),
_isDataUploaded(other._isDataUploaded),
_buffer(other._buffer),
_data(other._data),
_referenceCounter(other._referenceCounter)
{
	++(*_referenceCounter);
}

VertexBufferObject& VertexBufferObject::operator=(const VertexBufferObject& other) {

	if(*_referenceCounter < 2){
		//cleanup();
		delete _referenceCounter;
	}else{
		--(*_referenceCounter);
	}

	_lastBuffer = other._lastBuffer;
	_isCreated = other._isCreated;
	_isDataUploaded = other._isDataUploaded;
	_buffer = other._buffer;
	_referenceCounter = other._referenceCounter;
	_data = other._data;

	++(*_referenceCounter);

	return *this;
}

VertexBufferObject::~VertexBufferObject() {
	if(*_referenceCounter < 2){
		//cleanup();
		delete _referenceCounter;
	}else{
		--(*_referenceCounter);
	}
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
	if(Window::isContextCreated() && _isCreated && _isDataUploaded)
		glBindBuffer(type, _buffer);
}

void VertexBufferObject::uploadDataToGpu(const GLint& type, const GLint& usageHint) {
	if(_data.size() == 0)
		return;

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
