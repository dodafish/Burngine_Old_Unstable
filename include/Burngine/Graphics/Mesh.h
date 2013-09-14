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
	/**
	 * @brief The default constructor
	 */
	Mesh();

	/**
	 * @brief The default destructor
	 */
	~Mesh();

	/**
	 * @brief Sets the vertices of the mesh, so that they can be
	 * used for rendering later on.
	 *
	 * @param vertices A vector with vertices
	 *
	 * @see Vertex
	 */
	void setVertices(const std::vector<Vertex>& vertices);

	/**
	 * @brief Returns the count of the vertices which the mesh
	 * is holding.
	 *
	 * @return The count of vertices
	 *
	 * @see setVertices()
	 */
	size_t getVertexCount() const;

	/**
	 * @brief Returns the id of the position-buffer.
	 * This is used mostly internally. But you can check the
	 * buffer by comparing the returned value with 0.
	 *
	 * @return Returns the id of the position-buffer or 0 if no
	 * position-buffer exists
	 */
	const GLuint& getPositionBuffer();

	/**
	 * @brief Returns the id of the color-buffer.
	 * This is used mostly internally. But you can check the
	 * buffer by comparing the returned value with 0.
	 *
	 * @return Returns the id of the color-buffer or 0 if no
	 * color-buffer exists
	 */
	const GLuint& getColorBuffer();

private:
	std::vector<Vertex> _vertices;

	/**
	 * @brief Fills the buffers with data or creates them if needed.
	 * Uses _vertices as vertexdata. Internal use only.
	 *
	 * @see setVertices()
	 */
	void data();

	bool _needUpdate;
	GLuint _vertexPositionBuffer, _vertexColorBuffer;
};

} /* namespace burn */
#endif /* MESH_H_ */
