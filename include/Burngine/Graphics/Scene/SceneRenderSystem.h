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

#ifndef SCENERENDERSYSTEM_H_
#define SCENERENDERSYSTEM_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

class SceneNode;
class Camera;
class Shader;
class BaseTexture;

class BURNGINE_API SceneRenderSystem {
public:

	//Static only
	SceneRenderSystem() = delete;

	enum RenderFlag{
		POSITION = 1,
		NORMAL = 2,
		UV = 4
	};

	static void render(SceneNode* node, const int& flags, const Camera& camera, const Shader& shader, bool onlyShadowCasters = false);

	static void setVboIndex(const RenderFlag& flag, const GLuint& index);

	static void renderTextureToFramebuffer(const BaseTexture& source);

private:
	static GLuint _vboIndices[3]; //Array size is elementcount of RenderFlag enum
};

//Shortcut for easy access
typedef SceneRenderSystem::RenderFlag RF;

} /* namespace burn */
#endif /* SCENERENDERSYSTEM_H_ */
