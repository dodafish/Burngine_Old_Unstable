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

#ifndef SCENERENDERSYSTEM_H_
#define SCENERENDERSYSTEM_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/GBuffer.h>

#include <Burngine/Graphics/Texture/RenderTarget.h>
#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/Texture/CubeRenderTarget.h>
#include <Burngine/Graphics/Texture/CubeMap.h>

#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <vector>

namespace burn {
class Light;
class SceneNode;
}

template class BURNGINE_API std::vector<burn::SceneNode*>;
template class BURNGINE_API std::vector<burn::Light*>;

namespace burn {

class Camera;
class Shader;
class BaseTexture;
class Window;
class SpotLight;
class DirectionalLight;
class BoundingBox;
class Material;
class SkyBox;

class BURNGINE_API SceneRenderSystem : public NonCopyable {
public:

	SceneRenderSystem();

	enum RenderMode {
		COMPOSITION,    ///< The final render result
		DIFFUSE,    ///< Diffuse pass only
		NORMAL_WS,    ///< Normals pass only
		DEPTH,    ///< Depth pass only
		POSITION_WS,    ///< Positions pass only
		LIGHTING    ///< Diffuse and specular pass
	};

	void render(const GLuint& targetFramebuffer,    ///< Window as default
				const Vector2ui& targetFramebufferDimensions,
				const Camera& camera,
				const RenderMode& mode,
				const std::vector<SceneNode*>& nodes,
				const std::vector<Light*>& lights,
				const Vector3f& ambient,
				const SkyBox& skybox,
				bool isLightingEnabled = true,
				const Matrix4f& modelMatrixOffset = Matrix4f(1.f));

	enum RenderFlag {
		POSITION = 1, NORMAL = 2, UV = 4
	};

	void setVboIndex(	const RenderFlag& flag,
						const GLuint& index);

private:
	void adjustRenderTextures(const Vector2ui& resolution);
	Vector2ui _currentRenderTexturesResolution;

	GLuint _vboIndices[3];    //Array size is elementcount of RenderFlag enum

	void renderNode(SceneNode* node,
					const int& flags,
					const Camera& camera,
					const Shader& shader,
					bool shadowMapRendering = false);
	Matrix4f _modelMatrixOffset;

	//////////////////////////////////////////////////////////////////////////////
	void drawGBuffers(	const Camera& camera,
						const std::vector<SceneNode*>& nodes,
						const SkyBox& skyBox);
	void dumpOutDepthGBuffer(	const GLuint& targetFramebuffer,    ///< Window as default
								const Vector2ui& targetFramebufferDimensions);

	//The GBuffer with several passes
	GBuffer _gBuffer;

	//Passes:
	void lightPass(	const GLuint& targetFramebuffer,    ///< Window as default
					const Vector2ui& targetFramebufferDimensions,
					const Camera& camera,
					const std::vector<SceneNode*>& nodes,
					const std::vector<Light*>& lights,
					const Vector3f& ambient,
					bool dumpLighting = false);
	//Pass-Helpers:
	void ambientPart(const Vector3f& ambient);
	void drawFullscreenQuad(const Shader& shader,
							const OpenGlControl::Settings& rendersettings) const;
	RenderTarget _renderTarget;
	VertexBufferObject _fullscreenVbo;

	struct HiMidLowResMatrices{
		Matrix4f matrices[3];
		Matrix4f view;
	};

	HiMidLowResMatrices drawShadowmap(	const DirectionalLight& dirLight,
							const std::vector<SceneNode*>& nodes,
							const Vector3f& cameraPosition);
	Matrix4f drawShadowmap(	const SpotLight& spotLight,
							const std::vector<SceneNode*>& nodes);
	//Pointlight:
	void drawShadowmap(	const Light& pointlight,
						const std::vector<SceneNode*>& nodes);
	Camera findCamera(	const int& face,
						const Light& pointlight);

	//Texture to pre render the scene
	RenderTarget _sceneTextureTarget;
	Texture _sceneTexture;

	//Textures for lighting
	Texture _diffusePartTexture, _specularPartTexture;

	//Shadowmaps:
	Texture _vsm[3]; //Array of 3 VSMs for hi- med- and low-resolution
	RenderTarget _vsmTarget[3]; //See vsm[3]
	CubeMap _vscm;
	CubeRenderTarget _vscmTarget;

};

} /* namespace burn */
#endif /* SCENERENDERSYSTEM_H_ */
