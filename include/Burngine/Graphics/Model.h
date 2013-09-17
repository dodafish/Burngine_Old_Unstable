/*
 * Model.h
 *
 *  Created on: 17.09.2013
 *      Author: Dominik
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "../Export.h"
#include "Mesh.h"
#include <vector>

template class BURNGINE_API std::vector<burn::Mesh*>;

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

	void update();

private:
	std::vector<Mesh*> _meshes;
};

} /* namespace burn */
#endif /* MODEL_H_ */
