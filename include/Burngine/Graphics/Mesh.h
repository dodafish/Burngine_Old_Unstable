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
#include "Texture.h"

#include <vector>

template class BURNGINE_API std::vector<burn::Vertex>;

namespace burn {

class Model;

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
	 * @brief Loads a 3D-model into the Mesh object.
	 * It uses the assimp importer, so it supports the files
	 * that assimp does.
	 *
	 * @param file The file to load
	 *
	 * @return Returns true on load-success
	 */
	bool loadFromFile(const std::string& file);

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
	const GLuint& getPositionBuffer() const;

	/**
	 * @brief Returns the id of the color-buffer.
	 * This is used mostly internally. But you can check the
	 * buffer by comparing the returned value with 0.
	 *
	 * @return Returns the id of the color-buffer or 0 if no
	 * color-buffer exists
	 */
	const GLuint& getColorBuffer() const;

	/**
	 * @brief Returns the id of the UV-buffer.
	 * This is used mostly internally. But you can check the
	 * buffer by comparing the returned value with 0.
	 *
	 * @return Returns the id of the UV-buffer or 0 if no
	 * UV-buffer exists
	 */
	const GLuint& getUvBuffer() const;

	/**
	 * @brief Sets the Texture of the mesh
	 *
	 * @param texture The Texture to set
	 *
	 * @see getTexture()
	 */
	void setTexture(const Texture& texture);

	/**
	 * @brief Returns the current Texture of the mesh
	 *
	 * @return The current Texture
	 *
	 * @see setTexture()
	 */
	const Texture& getTexture() const;

	void setMaterialIndex(const unsigned int& index);

	const unsigned int& getMaterialIndex() const;

	/**
	 * @brief Fills the buffers with data or creates them if needed.
	 * Uses _vertices as vertexdata. Internal use only.
	 *
	 * @see setVertices()
	 */
	void data();

private:

	friend Model;

	std::vector<Vertex> _vertices;

	bool _needUpdate;
	GLuint _vertexPositionBuffer, _vertexColorBuffer, _vertexUvBuffer;

	unsigned int _materialIndex;

	Texture _texture;
};

} /* namespace burn */
#endif /* MESH_H_ */
