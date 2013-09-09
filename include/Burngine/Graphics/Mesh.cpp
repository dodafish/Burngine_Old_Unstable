/*
 * Mesh.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Mesh.h"

namespace burn {

Mesh::Mesh() {
	// TODO Auto-generated constructor stub

}

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}

void Mesh::setVertices(const std::vector<Vertex>& vertices){
	_vertices = vertices;
}

} /* namespace burn */
