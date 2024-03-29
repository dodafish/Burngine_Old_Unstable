//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/GBuffer.h>

#include <Burngine/Graphics/Scene/SkyBox.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <Burngine/Graphics/Scene/SceneRenderSystem.h>

#include <Burngine/System/NonCopyable.h>

#include <Burngine/System/MessageReceiver.h>

namespace burn {

class StaticMeshNode;
class PhysicalSceneNode;
class Message;

/**
 * @brief Renders attached SceneNodes properly with attached lights.
 * Provides advanced rendering with built-in technology!
 */
class BURNGINE_API Scene : public NonCopyable {
public:
	/**
	 * @brief Constructor taking a window as the scene's parent.
	 * This relationship is needed for some techniques
	 *
	 * @param parentWindow The parent of the scene
	 */
	Scene();

	/**
	 * @brief The default destructor detaching all attached objects.
	 */
	~Scene();

	/**
	 * @brief Draws the scene with selected technology
	 *
	 * @param camera The camera which the scene is drawn
	 * relative to
	 * @param mode The mode selecting the output of the rendering
	 * @param targetTexture Reference to a texture if you want to
	 * render the scene to a texture instead of the window
	 *
	 * @see RenderMode
	 */
	void draw(	const RenderTarget& renderTarget,
				const Camera& camera,
				const SceneRenderSystem::RenderMode& mode = SceneRenderSystem::COMPOSITION);

	void draw(	const Window& renderTarget,
				const Camera& camera,
				const SceneRenderSystem::RenderMode& mode = SceneRenderSystem::COMPOSITION);

	void setModelMatrixOffset(const Matrix4f& offset);

	/**
	 * @brief Attaches a StaticMeshNode to the Scene.
	 *
	 * @param staticMeshNode The SceneNode to attach
	 *
	 * @note You don't have to care about detaching. When either the
	 * SceneNode or the Scene gets destroyed the destructors will care
	 * about deleting
	 *
	 * @see detachSceneNode()
	 */
	void attachSceneNode(StaticMeshNode& staticMeshNode);

	/**
	 * @brief Detaches a SceneNode from the Scene.
	 *
	 * @param node The SceneNode to detach
	 *
	 * @see attachSceneNode()
	 */
	void detachSceneNode(const SceneNode& node);

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
	MessageReceiver _messageReceiver;
	void onMessageReceive(const Message& msg);

	//Overall ambient color:
	Vector3f _ambientColor;

	SceneRenderSystem _renderSystem;

	bool _isLightingEnabled;    ///< Does our scene render lighting? Default: false

	//Attachable nodes:
	std::vector<Light*> _lights;
	std::vector<SceneNode*> _nodes;

	void removeSceneNodeById(const Uint64& id);
	void removeLightById(const Uint64& id);

	//Copy of a skybox which is used
	SkyBox _skyBox;

	// for ovr basically:
	Matrix4f _modelMatrixOffset;
};

} /* namespace burn */
#endif /* SCENE_H_ */
