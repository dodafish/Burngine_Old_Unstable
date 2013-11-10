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

#ifndef MESH_H_
#define MESH_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/Vertex.h>
#include <Burngine/Graphics/Scene/Material.h>
#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/Scene/Model.h>

#include <vector>

template class BURNGINE_API std::vector<burn::Vertex>;

namespace burn {

class Model;

/**
 * @brief Holds the mesh's vertices with their attributes and
 * an optional texture
 */
class BURNGINE_API Mesh {
public:
	/**
	 * @brief The default constructor creating VBOs
	 */
	Mesh(const Model& model);

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

	/**
	 * @brief Synchronizes the vertices with the memory on the
	 * video card
	 *
	 * @return Returns false synchronizing is not necessary
	 *
	 * @see setVertices()
	 *
	 * @note This will only synchronize vertices when necessary!
	 * Use forceUpdate() to force synchronizing.
	 */
	bool update();

	/**
	 * @brief Synchronizes the vertices with the memory on the video card,
	 * even if it's not necessary.
	 *
	 * @see update()
	 */
	void forceUpdate();

	/**
	 * @brief Returns true when no update (i.e. synchronizing) is necessary.
	 *
	 * @return False when an update is necessary
	 *
	 * @see update()
	 */
	bool isUpdated() const;

	/**
	 * @brief Returns the minimum and maximum value according to the
	 * chosen axis.
	 *
	 * @return Min and Max of chosen axis
	 */
	const Vector2f& getXMinMax() const;

	/**
	 * @brief Returns the minimum and maximum value according to the
	 * chosen axis.
	 *
	 * @return Min and Max of chosen axis
	 */
	const Vector2f& getYMinMax() const;

	/**
	 * @brief Returns the minimum and maximum value according to the
	 * chosen axis.
	 *
	 * @return Min and Max of chosen axis
	 */
	const Vector2f& getZMinMax() const;

private:

	const Model& _model;

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
	Vector2f _xMinMax, _yMinMax, _zMinMax;
};

} /* namespace burn */
#endif /* MESH_H_ */
