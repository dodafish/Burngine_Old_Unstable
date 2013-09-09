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

namespace burn {

class BURNGINE_API StaticMeshNode : public SceneNode {
public:
	StaticMeshNode();
	~StaticMeshNode();
};

} /* namespace burn */
#endif /* STATICMESHNODE_H_ */
