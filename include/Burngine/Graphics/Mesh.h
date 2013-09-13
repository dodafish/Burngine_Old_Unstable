/*
 * Mesh.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef MESH_H_
#define MESH_H_

#include "../Export.h"
#include "Vertex.h"
#include "OpenGL.h"

#include <vector>

template class BURNGINE_API std::vector<burn::Vertex>;

namespace burn {

class BURNGINE_API Mesh {
public:
	Mesh();
	~Mesh();

	void setVertices(const std::vector<Vertex>& _vertices);

	size_t getVertexCount() const;

	const GLuint& getPositionBuffer();

private:
	std::vector<Vertex> _vertices;

	void data();

	bool _needUpdate;
	GLuint _vertexPositionBuffer;
};

} /* namespace burn */
#endif /* MESH_H_ */
