/*
 * Scene.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "../Export.h"
#include "SceneNode.h"
#include "StaticMeshNode.h"

#include <vector>

template class BURNGINE_API std::vector<burn::SceneNode*>;

namespace burn {

class BURNGINE_API Scene {
public:
	Scene();
	~Scene();

	void drawAll();

	StaticMeshNode* createStaticMeshNode();

	void removeNode(SceneNode* node);
	void removeAllNodes();

private:
	std::vector<SceneNode*> _nodes;
};

} /* namespace burn */
#endif /* SCENE_H_ */
