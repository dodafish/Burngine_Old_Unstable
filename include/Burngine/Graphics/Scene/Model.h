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

template class BURNGINE_API std::vector<burn::Mesh>;

/**
 * @brief Holds several meshes describing a 3D model
 */
namespace burn {

/**
 * @brief A model can load 3d models from file
 */
class BURNGINE_API Model {
public:

	Model();

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
	const std::vector<Mesh>& getMeshes() const;
	std::vector<Mesh>* getMeshesPointer();

	bool isLoaded() const;

private:
	std::vector<Mesh> _meshes;

	bool _isLoaded;
};

} /* namespace burn */
#endif /* MODEL_H_ */
