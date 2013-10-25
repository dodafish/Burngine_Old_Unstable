/*
 * VertexBufferObject.h
 *
 *  Created on: 02.10.2013
 *      Author: Dominik
 */

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

	void create();
	void cleanup();

	void reset();

	void bind(const GLint& type = GL_ARRAY_BUFFER) const;
	void uploadDataToGpu(const GLint& type = GL_ARRAY_BUFFER, const GLint& usageHint = GL_STATIC_DRAW);

	const GLuint& getBuffer() const;

	void addData(const void* data, const unsigned int& size);

	static GLint getLastBuffer(const GLint& type = GL_ARRAY_BUFFER);

private:

	GLint _lastBuffer;

	bool _isCreated, _isDataUploaded;
	GLuint _buffer;
	std::vector<GLbyte> _data;

	unsigned int* _referenceCounter;
};

} /* namespace burn */
#endif /* VERTEXBUFFEROBJECT_H_ */
