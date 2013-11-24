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

#ifndef VERTEXBUFFEROBJECT_H_
#define VERTEXBUFFEROBJECT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <vector>

template class BURNGINE_API std::vector<GLbyte>;

namespace burn {

class BURNGINE_API VertexBufferObject {
public:
	VertexBufferObject();
	VertexBufferObject(const VertexBufferObject& other);
	VertexBufferObject& operator=(const VertexBufferObject& other);

	~VertexBufferObject();

	bool create();
	void cleanup();

	void reset();

	void bind(const GLint& type = GL_ARRAY_BUFFER) const;
	bool uploadDataToGpu(const GLint& type = GL_ARRAY_BUFFER, const GLint& usageHint = GL_STATIC_DRAW);

	void addData(const void* data, const unsigned int& size);

	bool isCreated() const;

private:

	bool _isCreated, _isDataUploaded;
	GLuint _buffer;
	std::vector<GLbyte> _data;

	unsigned int* _referenceCount;
};

} /* namespace burn */
#endif /* VERTEXBUFFEROBJECT_H_ */
