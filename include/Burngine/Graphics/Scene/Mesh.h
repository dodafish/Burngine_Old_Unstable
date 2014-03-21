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

#ifndef MESH_H_
#define MESH_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/Vertex.h>
#include <Burngine/Graphics/Scene/Material.h>
#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

#include <memory>
#include <vector>

template class BURNGINE_API std::vector<burn::Vertex>;

namespace burn {

/**
 * @brief Holds the mesh's vertices with their attributes and
 * an optional texture
 */
class BURNGINE_API Mesh {
public:
	/**
	 * @brief The default constructor creating VBOs
	 */
	Mesh();

	Mesh(const Mesh& other);
	Mesh& operator==(const Mesh& other);
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
	inline const size_t& getVertexCount() const;

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
	void setTexture(const std::shared_ptr<Texture>& texture);

	/**
	 * @brief Returns the current Texture of the mesh
	 *
	 * @return The current Texture
	 *
	 * @see setTexture()
	 */
	const Texture& getTexture() const;

	/**
	 * @brief Sets the material of the node. Influences the rendering
	 * behaviour.
	 *
	 * @param material The Material to use.
	 *
	 * @see getMaterial()
	 */
	void setMaterial(const Material& material);

	/**
	 * @brief Returns the material that the node is using.
	 *
	 * @return The Material of the node.
	 *
	 * @see setMaterial()
	 */
	const Material& getMaterial() const;

	const std::vector<Vertex>& getVertices() const;

private:

	/**
	 * @brief Fills the buffers with data or creates them if needed.
	 * Uses _vertices as vertexdata. Internal use only.
	 *
	 * @see setVertices()
	 */
	bool update();

	Material _material;
	std::shared_ptr<Texture> _texture;

	std::vector<Vertex> _vertices;
	VertexBufferObject _positionVbo, _colorVbo, _uvVbo, _normalVbo;
	size_t _verticesCount;
};

const size_t& Mesh::getVertexCount() const {
	return _verticesCount;
}

} /* namespace burn */
#endif /* MESH_H_ */
