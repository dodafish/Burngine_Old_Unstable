/*
 * Scene.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

#include <vector>
#include <memory>

#include <Burngine/Graphics/Scene/Camera.h>

namespace burn {
class Light;
class SceneNode;
}

template class BURNGINE_API std::vector<burn::SceneNode*>;
template class BURNGINE_API std::vector<burn::Light*>;

namespace burn {

class Window;

class BURNGINE_API Scene {
public:
	/**
	 * @brief The default constructor
	 */
	Scene(const Window& parentWindow);

	/**
	 * @brief The default destructor. When called by e.g. deleting
	 * the scene it cleans up its data. In other words it deletes all
	 * nodes/cameras/etc. it is holding.
	 */
	~Scene();

	/**
	 * @brief Draws every SceneNode.
	 */
	void draw();

	void attachSceneNode(SceneNode& node);
	void detachSceneNode(SceneNode& node);

	void attachLight(Light& light);
	void detachLight(Light& light);

	void detachAll();

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
	void setCamera(Camera& camera);
	void setDefaultCamera();
	const Camera& getDefaultCamera();

	void setAmbientColor(const Vector3f& color);
	const Vector3f& getAmbientColor() const;

private:
	const Window& _window;
	Vector3f _ambientColor;

	std::vector<SceneNode*> _nodes;
	std::vector<Light*> _lights;

	Camera _defaultCamera;
	Camera& _camera;
};

} /* namespace burn */
#endif /* SCENE_H_ */
