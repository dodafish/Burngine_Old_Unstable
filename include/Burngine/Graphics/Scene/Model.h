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

#ifndef MODEL_H_
#define MODEL_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Material.h>

#include <vector>
#include <memory>

namespace burn {
class Mesh;
}

template class BURNGINE_API std::vector<std::shared_ptr<burn::Mesh>>;

namespace burn {

/**
 * @brief A model can load 3d models from file
 */
class BURNGINE_API Model {
public:

	bool loadFromFile(const std::string& file);

	size_t getMeshCount() const;
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

	void update();
	bool isUpdated() const;

private:
	std::vector<std::shared_ptr<Mesh>> _meshes;
};

} /* namespace burn */
#endif /* MODEL_H_ */
