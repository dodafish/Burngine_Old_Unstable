/*
 * Scene.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "../Export.h"
#include "StaticMeshNode.h"

#include <vector>

template class BURNGINE_API std::vector<burn::StaticMeshNode*>;

namespace burn {

class BURNGINE_API Scene {
public:
	Scene();
	~Scene();

	void removeAllNodes();

	StaticMeshNode* createStaticMeshNode();
	void removeStaticMeshNode(StaticMeshNode* node);

private:
	std::vector<StaticMeshNode*> _staticMeshes;
};

} /* namespace burn */
#endif /* SCENE_H_ */
