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
	void drawFullscreenQuad(const Shader& shader, const OpenGlControl::Settings& rendersettings) const;
	RenderTexture _renderTexture;
	VertexBufferObject _fullscreenVbo;

	//Shadow:
	Matrix4f drawShadowmap(const DirectionalLight& dirLight);
	Matrix4f drawShadowmap(const SpotLight& spotLight);
	//Pointlight:
	void drawShadowmap(const Light& pointlight);
	Matrix4f findViewMatrix(const int& face, const Light& pointlight);
	//ShadowMaps:
	ShadowMap _shadowMap;
	ShadowCubeMap _shadowCubeMap;

};

} /* namespace burn */
#endif /* SCENE_H_ */
