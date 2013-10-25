/*
 * Mesh.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef MESH_H_
#define MESH_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/Vertex.h>
#include <Burngine/Graphics/Scene/Material.h>
#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

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
	void setVertices(const std::vector<Vertex>& vertices, bool updateImmediatly = true);

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
	const VertexBufferObject& getPositionVbo() const;

	/**
	 * @brief Returns the id of the normal-buffer.
	 * This is used mostly internally. But you can check the
	 * buffer by comparing the returned value with 0.
	 *
	 * @return Returns the id of the normal-buffer or 0 if no
	 * normal-buffer exists
	 */
	const VertexBufferObject& getNormalVbo() const;

	/**
	 * @brief Returns the id of the color-buffer.
	 * This is used mostly internally. But you can check the
	 * buffer by comparing the returned value with 0.
	 *
	 * @return Returns the id of the color-buffer or 0 if no
	 * color-buffer exists
	 */
	const VertexBufferObject& getColorVbo() const;

	/**
	 * @brief Returns the id of the UV-buffer.
	 * This is used mostly internally. But you can check the
	 * buffer by comparing the returned value with 0.
	 *
	 * @return Returns the id of the UV-buffer or 0 if no
	 * UV-buffer exists
	 */
	const VertexBufferObject& getUvVbo() const;

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

	/**
	 * @brief Returns the material that the node is using.
	 *
	 * @return The Material of the node.
	 *
	 * @see setMaterial()
	 */
	const Material& getMaterial() const;

	/**
	 * @brief Sets the material of the node. Influences the rendering
	 * behaviour.
	 *
	 * @param material The Material to use.
	 *
	 * @see getMaterial()
	 */
	void setMaterial(Material& material);

	bool update();
	void forceUpdate();

	bool isUpdated() const;

private:

	/**
	 * @brief Fills the buffers with data or creates them if needed.
	 * Uses _vertices as vertexdata. Internal use only.
	 *
	 * @see setVertices()
	 */
	bool data();

	friend Model;

	Material _material;

	std::vector<Vertex> _vertices;

	bool _needUpdate;

	VertexBufferObject _positionVbo, _colorVbo, _uvVbo, _normalVbo;

	Texture _texture;
};

} /* namespace burn */
#endif /* MESH_H_ */
