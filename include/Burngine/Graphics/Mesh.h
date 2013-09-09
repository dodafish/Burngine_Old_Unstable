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

#include <vector>

template class BURNGINE_API std::vector<burn::Vertex>;

namespace burn {

class BURNGINE_API Mesh {
public:
	Mesh();
	~Mesh();

	void setVertices(const std::vector<Vertex>& _vertices);

private:
	std::vector<Vertex> _vertices;
};

} /* namespace burn */
#endif /* MESH_H_ */
