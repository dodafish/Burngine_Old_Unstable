//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef SCENE_H_
#define SCENE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

#include <vector>
#include <memory>

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Texture/RenderTexture.h>
#include <Burngine/Graphics/Scene/GBuffer.h>
#include <Burngine/Graphics/Texture/ShadowMap.h>
#include <Burngine/Graphics/Texture/ShadowCubeMap.h>

#include <Burngine/Graphics/Scene/SkyBox.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

namespace burn {
class Light;
class SceneNode;
}

template class BURNGINE_API std::vector<burn::SceneNode*>;
template class BURNGINE_API std::vector<burn::Light*>;

namespace burn {

class Window;
class Shader;
class SpotLight;
class DirectionalLight;
class BoundingBox;
class Material;

/**
 * @brief Renders attached SceneNodes properly with attached lights.
 * Provides advanced rendering with built-in technology!
 */
class BURNGINE_API Scene {
public:
	/**
	 * @brief Constructor taking a window as the scene's parent.
	 * This relationship is needed for some techniques
	 *
	 * @param parentWindow The parent of the scene
	 */
	Scene(	const Window& parentWindow,
			const ShadowMap::Resolution& shadowmapRes = ShadowMap::MEDIUM,
			const ShadowCubeMap::Resolution& shadowcubemapRes = ShadowCubeMap::MEDIUM);

	//Scenes are not copyable!
	Scene(const Scene& other) = delete;
	Scene& operator=(const Scene& other) = delete;

	/**
	 * @brief The default destructor detaching all attached objects.
	 */
	~Scene();

	enum RenderMode {
		COMPOSITION, ///< The final render result
		DIFFUSE, ///< Diffuse pass only
		NORMAL_WS, ///< Normals pass only
		DEPTH, ///< Depth pass only
		POSITION_WS, ///< Positions pass only
		LIGHTING ///< Diffuse and specular pass
	};

	/**
	 * @brief Draws the scene with selected technology
	 *
	 * @param camera The camera which the scene is drawn
	 * relative to
	 * @param mode The mode selecting the output of the rendering
	 *
	 * @see RenderMode
	 */
	void draw(	const Camera& camera,
				const RenderMode& mode = COMPOSITION);

	/**
	 * @brief Attaches a SceneNode to the Scene.
	 *
	 * @param node The SceneNode to attach
	 *
	 * @note You don't have to care about detaching. When either the
	 * SceneNode or the Scene gets destroyed the destructors will care
	 * about deleting
	 *
	 * @see detachSceneNode()
	 */
	void attachSceneNode(SceneNode& node);

	/**
	 * @brief Detaches a SceneNode from the Scene.
	 *
	 * @param node The SceneNode to detach
	 *
	 * @see attachSceneNode()
	 */
	void detachSceneNode(SceneNode& node);

	/**
	 * @brief Attaches a Light to the Scene.
	 *
	 * @param node The Light to attach
	 *
	 * @note You don't have to care about detaching. When either the
	 * Light or the Scene gets destroyed the destructors will care
	 * about deleting
	 *
	 * @see detachLight()
	 */
	void attachLight(Light& light);

	/**
	 * @brief Detaches a Light from the Scene.
	 *
	 * @param node The Light to detach
	 *
	 * @see attachLight()
	 */
	void detachLight(Light& light);

	/**
	 * @brief Detaches everything from the Scene that is attached
	 */
	void detachAll();

	/**
	 * @brief Sets the Skybox which should be used to cover the Scene
	 *
	 * @param skyBox The used SkyBox
	 *
	 * @note SkyBoxes are not attached, but only set! This means that
	 * the skybox will get copied, so you can destroy the original one
	 */
	void setSkyBox(const SkyBox& skyBox);

	/**
	 * @brief Sets the ambient part of the scene. E.g. while daytime
	 * the light gets scattered over the whole scene. So this part will
	 * fake the major indirect lighting
	 *
	 * @param color The ambient part's color
	 *
	 * @note When you want to make the scene half lit by
	 * default just use this: Vector3f(1.f, 1.f, 1.f)
	 *
	 * @see getAmbientColor()
	 */
	void setAmbientColor(const Vector3f& color);

	/**
	 * @brief Returns the ambient part's color
	 *
	 * @return The ambient color
	 *
	 * @see setAmbientColor()
	 */
	const Vector3f& getAmbientColor() const;

	/**
	 * @brief Enables or disables lighting
	 */
	void setLightingEnabled(bool enabled = true);

	/**
	 * @brief Returns true when lighting is enabled
	 */
	bool isLightingEnabled() const;

private:
	void drawGBuffers(const Camera& camera);
	void dumpOutDepthGBuffer();

	//Parent window and overall ambient color:
	const Window& _window;
	Vector3f _ambientColor;

	bool _isLightingEnabled; ///< Does our scene render lighting? Default: false

	//Attachable nodes:
	std::vector<SceneNode*> _nodes;
	std::vector<Light*> _lights;

	//Copy of a skybox which is used
	SkyBox _skyBox;

	//The GBuffer with several passes
	GBuffer _gBuffer;

	//Passes:
	void lightPass(	const Camera& camera,
					bool dumpLighting = false);
	//Pass-Helpers:
	void ambientPart();
	void drawFullscreenQuad(const Shader& shader,
							const OpenGlControl::Settings& rendersettings) const;
	RenderTexture _renderTexture;
	VertexBufferObject _fullscreenVbo;

	//Shadow:
	Matrix4f drawShadowmap(const DirectionalLight& dirLight);
	Matrix4f drawShadowmap(const SpotLight& spotLight);
	//Pointlight:
	void drawShadowmap(const Light& pointlight);
	Camera findCamera(const int& face,
						const Light& pointlight);
	//ShadowMaps:
	ShadowMap _shadowMap;
	ShadowCubeMap _shadowCubeMap;

};

} /* namespace burn */
#endif /* SCENE_H_ */
