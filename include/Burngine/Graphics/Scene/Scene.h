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
#include <Burngine/Graphics/Scene/GBuffer.h>

#include <Burngine/Graphics/Scene/SkyBox.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

namespace burn {
class Light;
class SceneNode;
}

template class BURNGINE_API std::vector<burn::SceneNode*>;
template class BURNGINE_API std::vector<burn::Light*>;

namespace burn {

class Window;
class Shader;

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

	enum RenderModus {
		COMPOSITION, DIFFUSE, NORMAL_WS, DEPTH, POSITION_WS
	};

	/**
	 * @brief Draws every SceneNode.
	 *
	 * @param modus Choose a gBuffer to dump to screen
	 * or set to COMPOSITION to see final result
	 */
	void draw(const Camera& camera, const RenderModus& modus = COMPOSITION);

	void attachSceneNode(SceneNode& node);
	void detachSceneNode(SceneNode& node);

	void attachLight(Light& light);
	void detachLight(Light& light);

	void detachAll();

	void setSkyBox(const SkyBox& skyBox);

	void setAmbientColor(const Vector3f& color);
	const Vector3f& getAmbientColor() const;

private:
	void drawGBuffers(const Camera& camera);

	void dumpOutDepthGBuffer();

	const Window& _window;
	Vector3f _ambientColor;

	std::vector<SceneNode*> _nodes;
	std::vector<Light*> _lights;

	SkyBox _skyBox;

	GBuffer _gBuffer;

	//Passes:
	void lightPass(const Camera& camera);
	//Pass-Helpers:
	void ambientPart();
	void drawFullscreenQuad(const Shader& shader) const;
	RenderTexture _renderTexture, _renderTexture2;
	VertexBufferObject _fullscreenVbo;

};

} /* namespace burn */
#endif /* SCENE_H_ */
