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

#ifndef OCULUSRIFT_H_
#define OCULUSRIFT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/RenderTarget.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/NonCopyable.h>
#include <Burngine/Graphics/Scene/Scene.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

namespace burn {

class Gui;

class BURNGINE_API OculusRift : public NonCopyable{
public:
	OculusRift(const Window& window);

	void clear();

	void renderScene(Scene& scene, const Camera& camera, const SceneRenderSystem::RenderMode& renderMode);
	void renderGui(const Gui& gui);

	void distortImages();
	void renderToWindow();

	void setEyeSpacing(const float& eyeSpacing);
	const float& getEyeSpacing() const;

private:
	const Window& _window;
	Texture _leftEyeTexture, _rightEyeTexture;
	RenderTarget _leftEyeRenderTarget, _rightEyeRenderTarget;
	float _eyeSpacing;
	VertexBufferObject _leftEyeVbo, _rightEyeVbo;
};

} /* namespace burn */
#endif /* OCULUSRIFT_H_ */
