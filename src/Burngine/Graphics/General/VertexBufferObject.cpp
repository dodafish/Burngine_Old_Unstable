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

#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/Window/Window.h>

#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

VertexBufferObject::VertexBufferObject() :
_isCreated(false),
_isDataUploaded(false),
_buffer(0),
_referenceCount(new unsigned int(1)) {
}

VertexBufferObject::VertexBufferObject(const VertexBufferObject& other) :
_isCreated(other._isCreated),
_isDataUploaded(other._isDataUploaded),
_buffer(other._buffer),
_data(other._data),
_referenceCount(other._referenceCount) {
	++(*_referenceCount);
}

VertexBufferObject& VertexBufferObject::operator=(const VertexBufferObject& other) {

	if(this == &other)
		return *this;

	if(*_referenceCount < 2){
		cleanup();
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}

	_isCreated = other._isCreated;
	_isDataUploaded = other._isDataUploaded;
	_buffer = other._buffer;
	_data = other._data;
	_referenceCount = other._referenceCount;

	++(*_referenceCount);

	return *this;

}

VertexBufferObject::~VertexBufferObject() {
	if(*_referenceCount < 2){
		cleanup();
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}
}

bool VertexBufferObject::create() {

	ensureContext();

	if(!_isCreated){

		glGenBuffers(1, &_buffer);

		_isDataUploaded = false;
		_isCreated = true;
	}

	return _isCreated;
}

void VertexBufferObject::cleanup() {
	ensureContext();

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
	ensureContext();

	glBindBuffer(type, _buffer);
}

bool VertexBufferObject::uploadDataToGpu(	const GLint& type,
											const GLint& usageHint) {

	ensureContext();

	//Ensure that we have a valid buffer
	if(!_isCreated){
		if(!create())
			return false;
	}

	//Get current buffer id
	GLint previousBuffer;
	glGetIntegerv(type, &previousBuffer);

	//Bind this buffer
	bind(type);

	//Upload our data
	glBufferData(type, _data.size(), &_data[0], usageHint);
	_isDataUploaded = true;

	//Restore buffer bound before
	glBindBuffer(type, previousBuffer);

	return true;
}

void VertexBufferObject::addData(	const void* data,
									const unsigned int& size) {
	_data.insert(_data.end(), (GLbyte*)data, (GLbyte*)data + size);
}

} /* namespace burn */
