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

#include <Burngine/extern/LibOVR/Include/OVRVersion.h>
#include <Burngine/extern/LibOVR/Include/OVR.h>

namespace burn {

class Gui;

class BURNGINE_API OculusRift : public NonCopyable {
public:
	OculusRift();
	~OculusRift();

	void renderScene(	Scene& scene,
						const Camera& camera,
						const SceneRenderSystem::RenderMode& renderMode,
						const Window& window);

private:
	// Some Oculus SDK types
	OVR::Ptr<OVR::SensorDevice> _ovrSensor;
	OVR::SensorFusion* _sensorFusion;
	OVR::Util::Render::StereoConfig _stereoConfig;

	// Provides the resolution and location of the Rift
	OVR::HMDInfo _hmdInfo;
	// Calculated width and height of the per-eye rendering area used
	int _eyeWidth, _eyeHeight;
	// Calculated width and height of the frame buffer object used to contain
    // intermediate results for the multipass render
	int _fboWidth, _fboHeight;

	VertexBufferObject _quadVbo;

	Texture _stereoTexture;
	RenderTarget _stereoTextureTarget;

	bool _useTracker;
	float _aspectRatio;
};

} /* namespace burn */
#endif /* OCULUSRIFT_H_ */
