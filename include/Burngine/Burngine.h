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

#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/Window/WindowSettings.h>

#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/Vertex.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <Burngine/Graphics/Texture/BaseTexture.h>
#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/Texture/RenderTexture.h>
#include <Burngine/Graphics/Texture/CubeMap.h>

#include <Burngine/Graphics/Scene/Scene.h>
#include <Burngine/Graphics/Scene/SceneNode.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/SpotLight.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Material.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Scene/Model.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/Graphics/Scene/SkyBox.h>
#include <Burngine/Graphics/Scene/DirectionalLight.h>
#include <Burngine/Graphics/Scene/Light.h>

#include <Burngine/Graphics/Gui/Character.h>
#include <Burngine/Graphics/Gui/Font.h>
#include <Burngine/Graphics/Gui/GuiNode.h>
#include <Burngine/Graphics/Gui/Text.h>
#include <Burngine/Graphics/Gui/Gui.h>
#include <Burngine/Graphics/Gui/Label.h>

#include <Burngine/System/Clock.h>
#include <Burngine/System/Keyboard.h>
#include <Burngine/System/Math.h>
#include <Burngine/System/Mouse.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/System/String.h>
#include <Burngine/System/Time.h>
#include <Burngine/System/Utf.h>
