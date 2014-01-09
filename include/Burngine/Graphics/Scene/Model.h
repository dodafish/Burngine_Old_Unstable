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

#ifndef MODEL_H_
#define MODEL_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Material.h>
#include <Burngine/System/BoundingBox.h>

#include <vector>
#include <memory>

namespace burn {
class Mesh;
}

template class BURNGINE_API std::vector<std::shared_ptr<burn::Mesh>>;

/**
 * @brief Holds several meshes describing a 3D model
 */
namespace burn {

/**
 * @brief A model can load 3d models from file
 */
class BURNGINE_API Model {
public:

	/**
	 * @brief Loads a 3D model from file. Supports most
	 * common formats like .obj .3ds etc.
	 *
	 * @param file The modelfile to load from
	 *
	 * @return Returns false if loading failed
	 */
	bool loadFromFile(const std::string& file);

	/**
	 * @brief Returns the count of meshes needed for describing
	 * the whole model
	 *
	 * @return The count of meshes
	 *
	 * @see getMesh()
	 */
	size_t getMeshCount() const;

	/**
	 * @brief Returns a mesh depending on the given index.
	 *
	 * @param index The mesh's index
	 *
	 * @return The mesh according to the index
	 *
	 * @note ATTENTION: When trying to get a mesh with an
	 * invalid index can result in a crash! Thus this function
	 * might (should) be deprecated in future versions.
	 *
	 * @see getMeshCount()
	 */
	const Mesh& getMesh(const size_t& index) const;

	/**
	 * @brief Changes the flags of all meshes to the given parameter.
	 * It replaces the Material::setFlag function, just that this one
	 * sets the flag to every mesh.
	 * If you want to affect only a single mesh, you have to get the
	 * mesh via getMesh()
	 *
	 * @param flag The material flag to set
	 * @param enabled Enables/Disables the flag
	 *
	 * @see Material
	 * @see getMesh()
	 */
	void setFlag(const Material::Flag& flag, const bool& enabled = true);

	/**
	 * @brief Synchronizes the vertices of each mesh when necessary
	 *
	 * @see isUpdated()
	 */
	void update();

	/**
	 * @brief Returns true when every mesh's vertices is synchronized
	 *
	 * @return Return false when a mesh is not updated
	 *
	 * @see update()
	 */
	bool isUpdated() const;

	/**
	 * @brief Recalculates the bounding box when necessary and
	 * returns it
	 *
	 * @return BoundingBox covering all vertices as tight as possible
	 */
	const BoundingBox& getBoundingBox() const;

	/**
	 * @brief Recalculates the bounding box depending on the
	 * meshes
	 */
	void recalculateBoundingBox() const;

private:
	mutable BoundingBox _bb;
	std::vector<std::shared_ptr<Mesh>> _meshes;
};

} /* namespace burn */
#endif /* MODEL_H_ */
