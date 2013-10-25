/*
 * Model.h
 *
 *  Created on: 17.09.2013
 *      Author: Dominik
 */

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
	Model();
	~Model();

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
