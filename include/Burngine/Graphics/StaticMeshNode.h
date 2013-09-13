/*
 * StaticMeshNode.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef STATICMESHNODE_H_
#define STATICMESHNODE_H_

#include "../Export.h"
#include "SceneNode.h"
#include "Mesh.h"

namespace burn {

class BURNGINE_API StaticMeshNode : public SceneNode {
public:
	StaticMeshNode();
	~StaticMeshNode();

	void setMesh(const Mesh& mesh);
	const Mesh& getMesh() const;

	virtual void draw();

private:
	Mesh _mesh;
};

} /* namespace burn */
#endif /* STATICMESHNODE_H_ */
