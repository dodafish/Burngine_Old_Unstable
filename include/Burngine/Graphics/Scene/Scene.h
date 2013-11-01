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
#include <Burngine/Graphics/Texture/RenderTexture.h>

#include <Burngine/Graphics/Scene/SkyBox.h>

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

	enum RenderModus{
		ALL,
		COLOR,
		DIFFUSE,
		SPECULAR,
		LIGHTING
	};

	/**
	 * @brief Draws every SceneNode.
	 */
	void draw(const Camera& camera, const RenderModus& modus = ALL);

	void attachSceneNode(SceneNode& node);
	void detachSceneNode(SceneNode& node);

	void attachLight(Light& light);
	void detachLight(Light& light);

	void detachAll();

	void setSkyBox(const SkyBox& skyBox);

	void setAmbientColor(const Vector3f& color);
	const Vector3f& getAmbientColor() const;

private:
	void drawNodes(const Camera& camera);
	bool drawDiffusepart(const Camera& camera);
	bool drawSpecularpart(const Camera& camera);

	const Window& _window;
	Vector3f _ambientColor;

	std::vector<SceneNode*> _nodes;
	std::vector<Light*> _lights;

	SkyBox _skyBox;

	RenderTexture _diffuseLightTexture, _specularLightTexture;
};

} /* namespace burn */
#endif /* SCENE_H_ */
