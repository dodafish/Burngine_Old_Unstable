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

void OculusRift::setIpdScale(const float& ipdScale){
	_ipdScale = ipdScale;
}

OculusRift::OculusRift(const Window& window) :
_window(window),
_renderScale(1.f),
_ipdScale(1.f) {

	std::cout << "test";

	OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

	_pManager = *OVR::DeviceManager::Create();
	_pHMD = *_pManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

	std::cout << "test";

	_width = _window.getSettings().getWidth();
	_height = _window.getSettings().getHeight();

	assert(_leftEyeTexture.create(Vector2ui(_width / 2, _height), Texture::RGB));
	assert(_rightEyeTexture.create(Vector2ui(_width / 2, _height), Texture::RGB));

	assert(_leftEyeRenderTarget.create(Vector2ui(_width / 2, _height), RenderTarget::NO_DEPTHBUFFER, _leftEyeTexture));
	assert(_rightEyeRenderTarget.create(Vector2ui(_width / 2, _height),
										RenderTarget::NO_DEPTHBUFFER,
										_rightEyeTexture));

	//To Window:
	{
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
		_leftEyeVbo.create();
		for(int i = 0; i != 4; ++i){
			_leftEyeVbo.addData(&posData[i], sizeof(Vector3f));
			_leftEyeVbo.addData(&uvData[i], sizeof(Vector2f));
		}
		_leftEyeVbo.uploadDataToGpu( GL_ARRAY_BUFFER,
		GL_STATIC_DRAW);
	}
	{
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
		_rightEyeVbo.create();
		for(int i = 0; i != 4; ++i){
			_rightEyeVbo.addData(&posData[i], sizeof(Vector3f));
			_rightEyeVbo.addData(&uvData[i], sizeof(Vector2f));
		}
		_rightEyeVbo.uploadDataToGpu( GL_ARRAY_BUFFER,
		GL_STATIC_DRAW);
	}

	_SFusion = new OVR::SensorFusion();
	std::cout << "wo";
	_pSensor = *_pHMD->GetSensor();
	std::cout << "wo";
	if(_pSensor){
		_SFusion->AttachToSensor(_pSensor);
		std::cout << "wo";
	}

}

OculusRift::~OculusRift() {
	_pSensor.Clear();
	_pHMD.Clear();
	_pManager.Clear();
	delete _SFusion;
	OVR::System::Destroy();
}

void OculusRift::clear() {
	_leftEyeRenderTarget.clear();
	_rightEyeRenderTarget.clear();
}

Matrix4f toBurngineMat(const OVR::Matrix4f& ovrMat) {
	Matrix4f result;
	for(int i = 0; i != 4; ++i)
		for(int j = 0; j != 4; ++j)
			result[i][j] = ovrMat.M[i][j];
	return result;
}

void OculusRift::renderScene(	Scene& scene,
								const Camera& camera,
								const SceneRenderSystem::RenderMode& rendermode) {

	Quaternion headQuat;
	if(_pSensor){

		OVR::Quatf headQuatOvr = _SFusion->GetOrientation();
		headQuat = Quaternion(-headQuatOvr.z, headQuatOvr.y, -headQuatOvr.x, headQuatOvr.w);
	}

	OVR::Util::Render::StereoConfig stereo;
	OVR::HMDInfo hmd;
	// Obtain setup data from the HMD and initialize StereoConfig
	// for stereo rendering.
	_pHMD->GetDeviceInfo(&hmd);
	stereo.SetFullViewport(OVR::Util::Render::Viewport(0, 0, _width, _height));
	stereo.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
	stereo.SetHMDInfo(hmd);
	stereo.SetDistortionFitPointVP(-1.f, 0.f);
	stereo.SetIPD(hmd.InterpupillaryDistance);

	_renderScale = stereo.GetDistortionScale();
	_renderScale = 1.f;

	//OVR::Util::Render::StereoEyeParams leftEye = stereo.GetEyeRenderParams(OVR::Util::Render::StereoEye_Left);
	//OVR::Util::Render::StereoEyeParams rightEye = stereo.GetEyeRenderParams(OVR::Util::Render::StereoEye_Right);

	Rotation headRot;
	headRot.setByQuaternion(camera.getRotation().asQuaternion() * headQuat);

	Camera leftCamera, rightCamera;

	// Compute Aspect Ratio. Stereo mode cuts width in half.
	float aspectRatio = float(hmd.HResolution * 0.5f) / float(hmd.VResolution);
	//aspectRatio =  ((float)_width / 2) / (float)_height;
	aspectRatio = stereo.GetAspect();

	float fov = (_renderScale * (_height / 2));
	fov = stereo.GetYFOVDegrees();

	leftCamera.setPosition(camera.getPosition());
	leftCamera.setRotation(headRot);
	leftCamera.setAspectRatio(aspectRatio);
	leftCamera.setFov(fov);

	//OVR::Matrix4f leftProjection = leftEye.Projection;
	//leftProjection.M[2][3] *= 2.0;

	{
		glm::vec3 eyeModelviewOffset = glm::vec3(-stereo.GetIPD() * 0.5f, 0, 0);
		Matrix4f view = leftCamera.getViewMatrix();
		glm::mat4 eyeModelview = glm::translate(glm::mat4(), eyeModelviewOffset) * view;
		//glm::mat4 eyeModelview = toBurngineMat(rightEye.ViewAdjust) * view;

		glm::vec3 eyeProjectionOffset(stereo.GetProjectionCenterOffset() / 2.0f, 0, 0);
		glm::mat4 eyeProjection = leftCamera.getProjectionMatrix();
		eyeProjection = glm::translate(eyeProjection, eyeProjectionOffset);
		leftCamera.setProjectionMatrix(eyeProjection);

		leftCamera.setViewMatrix(eyeModelview);
	}

	rightCamera.setPosition(camera.getPosition());
	rightCamera.setRotation(headRot);
	rightCamera.setAspectRatio(aspectRatio);
	rightCamera.setFov(fov);

	{
		glm::vec3 eyeModelviewOffset = glm::vec3(stereo.GetIPD() * 0.5f, 0, 0);
		Matrix4f view = rightCamera.getViewMatrix();
		glm::mat4 eyeModelview = glm::translate(glm::mat4(), eyeModelviewOffset) * view;
		//glm::mat4 eyeModelview = toBurngineMat(leftEye.ViewAdjust) * view;

		glm::vec3 eyeProjectionOffset(-stereo.GetProjectionCenterOffset() / 2.0f, 0, 0);
		glm::mat4 eyeProjection = rightCamera.getProjectionMatrix();
		eyeProjection = glm::translate(eyeProjection, eyeProjectionOffset);
		rightCamera.setProjectionMatrix(eyeProjection);

		rightCamera.setViewMatrix(eyeModelview);
	}

	//Left Eye:
	scene.draw(_leftEyeRenderTarget, leftCamera, rendermode);
	//Right Eye:
	scene.draw(_rightEyeRenderTarget, rightCamera, rendermode);

}

void OculusRift::renderGui(const Gui& gui) {
	//TODO
}

void OculusRift::renderToWindow() {

	OpenGlControl::Settings ogl;
	ogl.enableDepthtest(false);
	ogl.enableDepthbufferWriting(false);
	ogl.enableCulling(false);
	ogl.enableBlending(true);
	ogl.setBlendMode(OpenGlControl::OVERWRITE);
	OpenGlControl::useSettings(ogl);

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::OVR_DISTORTION);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	float screenwidth = 0.14976;
	float lensdist = 0.0635;
	Vector4f hmdWarpParam(1.0f, 0.22f, 0.24f, 0.f);

	OVR::HMDInfo hmd;
	if(_pHMD){
		if(_pHMD->GetDeviceInfo(&hmd)){
			screenwidth = hmd.HScreenSize;
			lensdist = hmd.LensSeparationDistance;

			hmdWarpParam[0] = hmd.DistortionK[0];
			hmdWarpParam[1] = hmd.DistortionK[1];
			hmdWarpParam[2] = hmd.DistortionK[2];
			hmdWarpParam[3] = hmd.DistortionK[3];
		}else{
			return;
		}
	}

	float LensCenter = 1 - 2 * lensdist / screenwidth;
	float aspect = ((float)_width / 2.f) / (float)_height;
	aspect = float(hmd.HResolution * 0.5f) / float(hmd.VResolution);

	_window.bind();
	shader.setUniform("gSampler", 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Left Eye

	//Distortion parameters:
	shader.setUniform("gLensCenter", Vector2f(0.5 - LensCenter / 2.f, 0.5f));
	shader.setUniform("gScreenCenter", Vector2f(0.5f, 0.5f));
	shader.setUniform("gScale", Vector2f(0.5f / _renderScale, 0.5f * aspect / _renderScale));
	shader.setUniform("gScaleIn", Vector2f(2.f, 2.f / aspect));
	shader.setUniform("gHmdWarpParam", hmdWarpParam);

	_rightEyeTexture.bind(0);

	_rightEyeVbo.bind();
	glVertexAttribPointer(0, 3,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	glVertexAttribPointer(1, 2,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));

	glViewport(_width / 2, 0, _width / 2, _height);
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	//Right Eye
	shader.setUniform("gLensCenter", Vector2f(0.5 + LensCenter / 2.f, 0.5f));
	shader.setUniform("gScreenCenter", Vector2f(0.5f, 0.5f));
	shader.setUniform("gScale", Vector2f(0.5f / _renderScale, 0.5f * aspect / _renderScale));
	shader.setUniform("gScaleIn", Vector2f(2.f, 2.f / aspect));
	shader.setUniform("gHmdWarpParam", hmdWarpParam);

	_leftEyeTexture.bind(0);

	_leftEyeVbo.bind();
	glVertexAttribPointer(0, 3,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	glVertexAttribPointer(1, 2,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));

	glViewport(0, 0, _width / 2, _height);
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	OpenGlControl::useSettings(OpenGlControl::Settings());

}

} /* namespace burn */
