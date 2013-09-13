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
#include "Camera.h"

#include <vector>

template class BURNGINE_API std::vector<burn::SceneNode*>;
template class BURNGINE_API std::vector<burn::Camera*>;

namespace burn {

class BURNGINE_API Scene {
public:
	Scene();
	~Scene();

	void drawAll();

	StaticMeshNode* createStaticMeshNode();
	Camera* createCamera(bool active = true);

	void removeNode(SceneNode* node);
	void removeAllNodes();
	void removeCamera(Camera* camera);
	void removeAllCameras();

private:
	std::vector<SceneNode*> _nodes;
	std::vector<Camera*> _cameras;
	Camera* _activeCamera;
};

} /* namespace burn */
#endif /* SCENE_H_ */
