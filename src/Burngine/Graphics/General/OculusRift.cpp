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

#include <Burngine/Graphics/General/OculusRift.h>
#include <Burngine/Graphics/General/BurngineShaders.h>
#include <Burngine/Graphics/Gui/Gui.h>
#include <assert.h>

#include <iostream>

namespace burn {

const float FRAMEBUFFER_OBJECT_SCALE = 1.f;
const int EYE_LEFT = 0;

OculusRift::OculusRift() :
_sensorFusion(new OVR::SensorFusion()),
_eyeWidth(0),
_eyeHeight(0),
_fboWidth(0),
_fboHeight(0),
_useTracker(false),
_aspectRatio(0.8f) {

	//Initialize the Oculus SDK
	OVR::System::Init();

	_sensorFusion->SetGravityEnabled(false);
	_sensorFusion->SetPredictionEnabled(false);
	_sensorFusion->SetYawCorrectionEnabled(false);

	//If no Rift is present we use default values:
	_hmdInfo.HResolution = 1280;
	_hmdInfo.VResolution = 800;
	_hmdInfo.HScreenSize = 0.149759993f;
	_hmdInfo.VScreenSize = 0.0935999975f;
	_hmdInfo.VScreenCenter = 0.0467999987f;
	_hmdInfo.EyeToScreenDistance = 0.0410000011f;
	_hmdInfo.LensSeparationDistance = 0.0635000020f;
	_hmdInfo.InterpupillaryDistance = 0.0640000030f;
	_hmdInfo.DistortionK[0] = 1.00000000f;
	_hmdInfo.DistortionK[1] = 0.219999999f;
	_hmdInfo.DistortionK[2] = 0.239999995f;
	_hmdInfo.DistortionK[3] = 0.000000000f;
	_hmdInfo.ChromaAbCorrection[0] = 0.995999992f;
	_hmdInfo.ChromaAbCorrection[1] = -0.00400000019f;
	_hmdInfo.ChromaAbCorrection[2] = 1.01400006f;
	_hmdInfo.ChromaAbCorrection[3] = 0.000000000f;
	_hmdInfo.DesktopX = 0;
	_hmdInfo.DesktopY = 0;

	OVR::Ptr<OVR::DeviceManager> ovrManager = *OVR::DeviceManager::Create();

	if(ovrManager){
		_ovrSensor = *ovrManager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
		if(_ovrSensor){
			_useTracker = true;
			_sensorFusion->AttachToSensor(_ovrSensor);
		}
		OVR::Ptr<OVR::HMDDevice> ovrHmd = *ovrManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
		if(ovrHmd){
			ovrHmd->GetDeviceInfo(&_hmdInfo);
		}
		// The HMDInfo structure contains everything we need for now, so no
		// need to keep the device handle around
		ovrHmd.Clear();
	}

	// The device manager is reference counted and will be released automatically
	// when our sensorObject is destroyed.
	ovrManager.Clear();
	_stereoConfig.SetHMDInfo(_hmdInfo);

	_eyeWidth = _hmdInfo.HResolution / 2;
	_eyeHeight = _hmdInfo.VResolution;
	_fboWidth = _eyeWidth * FRAMEBUFFER_OBJECT_SCALE;
	_fboHeight = _eyeHeight * FRAMEBUFFER_OBJECT_SCALE;
	_aspectRatio = ((float)_hmdInfo.HResolution / 2.f) / (float)_hmdInfo.VResolution;

	// Create the rendertarget for both views
	_stereoTexture.create(Vector2ui(_fboWidth, _fboHeight), BaseTexture::RGB);
	_stereoTextureTarget.create(_stereoTexture.getDimensions(), RenderTarget::NO_DEPTHBUFFER, _stereoTexture);

}

OculusRift::~OculusRift() {
	delete _sensorFusion;
	_ovrSensor.Clear();
	OVR::System::Destroy();
}

void OculusRift::renderScene(	Scene& scene,
								const Camera& camera,
								const SceneRenderSystem::RenderMode& rendermode,
								const Window& window) {

	if(!window.isCreated())
		return;

	//Adjust camera
	Camera stereoCamera(camera);
	stereoCamera.setAspectRatio(_aspectRatio);
	stereoCamera.setFov(_stereoConfig.GetYFOVDegrees());

	// Calculate the camera rotation
	Rotation cameraRotation;
	if(_useTracker){
		if(_ovrSensor){
			OVR::Quatf headQuatOvr = _sensorFusion->GetOrientation();
			Quaternion headQuat = Quaternion(-headQuatOvr.z, headQuatOvr.y, -headQuatOvr.x, headQuatOvr.w);
			cameraRotation.setByQuaternion(camera.getRotation().asQuaternion() * headQuat);
		}
	}else{
		cameraRotation = camera.getRotation();
	}
	stereoCamera.setRotation(cameraRotation);

	_stereoTextureTarget.clear();

	for(int eye = 0; eye != 2; ++eye){
		_stereoTextureTarget.bind();

		Vector3f eyeProjectionOffset(-_stereoConfig.GetProjectionCenterOffset() / 2.0f, 0, 0);
		Vector3f eyeModelviewOffset = glm::vec3(-_stereoConfig.GetIPD() / 2.0f, 0, 0);

		if(eye == EYE_LEFT){    // left eye
			eyeModelviewOffset *= -1;
			eyeProjectionOffset *= -1;
		}

		scene.setModelMatrixOffset(glm::translate(Matrix4f(1.f), eyeModelviewOffset));
		stereoCamera.setProjectionMatrixOffset(glm::translate(Matrix4f(1.f), eyeProjectionOffset));

		scene.draw(_stereoTextureTarget, stereoCamera, rendermode);

		//Now render to the window
		window.bind();
		glViewport((eye == EYE_LEFT ?	0 :
										_eyeWidth),
					0, _eyeWidth, _eyeHeight);

		const Shader& shader = BurngineShaders::getShader(BurngineShaders::OVR_DISTORTION);

		// Pysical width of the viewport
		static float eyeScreenWidth = _hmdInfo.HScreenSize / 2.0f;
		// The viewport goes from -1,1.  We want to get the offset
		// of the lens from the center of the viewport, so we only
		// want to look at the distance from 0, 1, so we divide in
		// half again
		static float halfEyeScreenWidth = eyeScreenWidth / 2.0f;

		// The distance from the center of the display panel (NOT
		// the center of the viewport) to the lens axis
		static float lensDistanceFromScreenCenter = _hmdInfo.LensSeparationDistance / 2.0f;

		// Now we we want to turn the measurement from
		// millimeters into the range 0, 1
		static float lensDistanceFromViewportEdge = lensDistanceFromScreenCenter / halfEyeScreenWidth;

		// Finally, we want the distnace from the center, not the
		// distance from the edge, so subtract the value from 1
		static float lensOffset = 1.0f - lensDistanceFromViewportEdge;
		static glm::vec2 aspect(1.0, (float)_eyeWidth / (float)_eyeHeight);

		glm::vec2 lensCenter(lensOffset, 0);

		static VertexBufferObject fullVbo;
		fullVbo.reset();

		Vector3f posData[] = {
		Vector3f(-1.f, -1.f, 0.f),
		Vector3f(1.f, -1.f, 0.f),
		Vector3f(-1.f, 1.f, 0.f),
		Vector3f(1.f, 1.f, 0.f) };
		Vector2f uvData[] = {
		Vector2f(0.f, 0.f),
		Vector2f(1.f, 0.f),
		Vector2f(0.f, 1.f),
		Vector2f(1.f, 1.f), };
		for(int i = 0; i != 4; ++i){
			fullVbo.addData(&posData[i], sizeof(Vector3f));
			fullVbo.addData(&uvData[i], sizeof(Vector2f));
		}
		fullVbo.uploadDataToGpu();

		_stereoTexture.bind(0);

		Vector4f hmdWarpParam;
		for(int i = 0; i != 4; ++i)
			hmdWarpParam[i] = _hmdInfo.DistortionK[i];

		float LensCenter = 1 - 2 * _hmdInfo.LensSeparationDistance / _hmdInfo.HScreenSize;
		float lensradius = -1.f - LensCenter;
		float lensradsq = lensradius * lensradius;
		float factor = hmdWarpParam.x + hmdWarpParam.y * lensradsq + hmdWarpParam.z * lensradsq * lensradsq
		+ hmdWarpParam.w * lensradsq * lensradsq * lensradsq;

		Vector2f scale(0.5f / factor, (0.5f * _aspectRatio) / factor);

		shader.setUniform("gLensCenter", Vector2f(0.5 - LensCenter, 0.5f));
		shader.setUniform("gScreenCenter", Vector2f(0.5f, 0.5f));
		shader.setUniform("gScale", scale);
		shader.setUniform("gScaleIn", Vector2f(2.f, 2.f / _aspectRatio));
		shader.setUniform("gHmdWarpParam", hmdWarpParam);

		fullVbo.bind();
		glVertexAttribPointer(0, 3,
		GL_FLOAT,
								GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
		glVertexAttribPointer(1, 2,
		GL_FLOAT,
								GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));
		OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	}
}

} /* namespace burn */
