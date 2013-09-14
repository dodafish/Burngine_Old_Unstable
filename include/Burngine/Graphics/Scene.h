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
	/**
	 * @brief The default constructor
	 */
	Scene();

	/**
	 * @brief The default destructor. When called by e.g. deleting
	 * the scene it cleans up its data. In other words it deletes all
	 * nodes/cameras/etc. it is holding.
	 */
	~Scene();

	/**
	 * @brief Draws every SceneNode.
	 */
	void drawAll();

	/**
	 * @brief Adds a StaticMeshNode to the scene.
	 *
	 * @return A pointer to the StaticMeshNode.
	 */
	StaticMeshNode* createStaticMeshNode();

	/**
	 * @brief Adds a Camera to the scene.
	 *
	 * @param active Set this to false if you don't want to be the
	 * created Camera as active.
	 *
	 * @return A pointer to the Camera.
	 *
	 * @see setCameraActive()
	 */
	Camera* createCamera(bool active = true);

	/**
	 * @brief Removes any SceneNode (except Camera) from the scene.
	 * The pointer will become a nullptr.
	 *
	 * @param node The SceneNode to delete/remove.
	 */
	void removeNode(SceneNode* node);

	/**
	 * @brief Removes all nodes from the scene.
	 */
	void removeAllNodes();

	/**
	 * @brief Removes a Camera from the scene.
	 * The pointer will become a nullptr.
	 *
	 * @param camera The Camera to delete/remove.
	 */
	void removeCamera(Camera* camera);

	/**
	 * @brief Removes all cameras from the scene.
	 */
	void removeAllCameras();

	/**
	 * @brief Sets a camera as active one. This camera will influence
	 * the rendering behaviour of the scene.
	 *
	 * @param camera The Camera that should be used.
	 *
	 * @see Camera
	 *
	 * @note You can use cameras from other scenes, but it is
	 * recommended to use cameras from the same scene only.
	 */
	void setActiveCamera(Camera* camera);

private:
	std::vector<SceneNode*> _nodes;
	std::vector<Camera*> _cameras;
	Camera* _activeCamera;
};

} /* namespace burn */
#endif /* SCENE_H_ */
