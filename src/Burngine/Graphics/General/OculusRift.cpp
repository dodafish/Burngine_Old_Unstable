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

OculusRift::OculusRift(const Window& window) :
_window(window),
_eyeSpacing(5.f),
_cameraAspect(1.6f) {

	OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

	_pManager = *OVR::DeviceManager::Create();
	_pHMD = *_pManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

	std::cout << "test";

	const unsigned int& width = _window.getSettings().getWidth();
	const unsigned int& height = _window.getSettings().getHeight();

	assert(_leftEyeTexture.create(Vector2ui(width, height), Texture::RGB));
	assert(_rightEyeTexture.create(Vector2ui(width, height), Texture::RGB));

	assert(_leftEyeRenderTarget.create(Vector2ui(width, height), RenderTarget::NO_DEPTHBUFFER, _leftEyeTexture));
	assert(_rightEyeRenderTarget.create(Vector2ui(width, height), RenderTarget::NO_DEPTHBUFFER, _rightEyeTexture));

	//To Window:
	{
		Vector3f posData[] = {
		Vector3f(-1.f, -1.f, 0.f),
		Vector3f(0.f, -1.f, 0.f),
		Vector3f(-1.f, 1.f, 0.f),
		Vector3f(0.f, 1.f, 0.f) };
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
		Vector3f(0.f, -1.f, 0.f),
		Vector3f(1.f, -1.f, 0.f),
		Vector3f(0.f, 1.f, 0.f),
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

}

void OculusRift::clear() {
	_leftEyeRenderTarget.clear();
	_rightEyeRenderTarget.clear();
}

void OculusRift::renderScene(	Scene& scene,
								const Camera& camera,
								const SceneRenderSystem::RenderMode& rendermode) {



	_cameraAspect = camera.getAspectRatio();

	//TODO: 2 cameras
	Vector3f dir = camera.getLookAt() - camera.getPosition();

	Vector3f n1;
	n1.x = (-1.f) * dir.z;
	n1.y = 0.f;
	n1.z = dir.x;

	n1 = glm::normalize(n1);
	n1.x *= (10 * 0.5f);
	n1.z *= (10 * 0.5f);

	Camera leftCamera, rightCamera;
	/*leftCamera.setPosition(leftCamera.getPosition() + n1);
	 leftCamera.lookAt(leftCamera.getLookAt() + n1);
	 rightCamera.setPosition(rightCamera.getPosition() + n2);
	 rightCamera.lookAt(rightCamera.getLookAt() + n2);*/

	//std::cout << "N: " << n1.x << "/" << n1.y << "/" << n1.z << "\n";

	Rotation r;

	leftCamera.setPosition(camera.getPosition() - n1);
	leftCamera.lookAt(camera.getLookAt() - n1);
	leftCamera.setFov(125.871f);
	rightCamera.setPosition(camera.getPosition() + n1);
	rightCamera.lookAt(camera.getLookAt() + n1);
	rightCamera.setFov(125.871f);

	//Left Eye:
	scene.draw(_leftEyeRenderTarget, leftCamera, rendermode);
	//Right Eye:
	scene.draw(_rightEyeRenderTarget, rightCamera, rendermode);

}

void OculusRift::renderGui(const Gui& gui) {
	//TODO
}

void OculusRift::distortImages() {
	//TODO concave distortion
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

	float eyedistance = 0.064;
	float screenwidth = 0.14976;
	float screenheight = 0.0936;
	float screendist = 0.041;
	float lensdist = 0.0635;

	Vector4f hmdWarpParam(1.0f, 0.22f, 0.24f, 0.f);
	OVR::HMDInfo hmd;
	if(_pHMD){
		if(_pHMD->GetDeviceInfo(&hmd)){
			eyedistance = hmd.InterpupillaryDistance;
			screenwidth = hmd.HScreenSize;
			screenheight = hmd.VScreenSize;
			screendist = hmd.EyeToScreenDistance;
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

	_eyeSpacing = eyedistance;

	float x = 0.f;
	float y = 0.f;
	float w = 0.5f;
	float h = 1.f;
	float projshift = 0.15197f;
	float lensradius = -1.f - LensCenter;
	float lensradsq = lensradius * lensradius;

	float factor = hmdWarpParam.x + hmdWarpParam.y * lensradsq + hmdWarpParam.z * lensradsq * lensradsq
	+ hmdWarpParam.w * lensradsq * lensradsq * lensradsq;

	//factor = 0.5877112f;

	float aspect = (1280.f / 2.f) / 800.f;
	aspect = 0.8f;

	_window.bind();
	shader.setUniform("gSampler", 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Left Eye

	//Distortion parameters:
	//shader.setUniform("gLensCenter", Vector2f(x + (w + projshift) * 0.5f, y + h * 0.5f));
	//shader.setUniform("gScreenCenter", Vector2f(x + w * 0.5f, y + h * 0.5f));
	//shader.setUniform("gScale", Vector2f(w * 0.5f / factor, h * 0.5f * aspect / factor));
	//shader.setUniform("gScaleIn", Vector2f(2.f / w, 2.f / h / aspect));
	shader.setUniform("gLensCenter", Vector2f(0.5-LensCenter, 0.5f));
	shader.setUniform("gScreenCenter", Vector2f(0.5f, 0.5f));
	shader.setUniform("gScale", Vector2f(0.5f / factor, 0.5f * aspect / factor));
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
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	//Right Eye
	x = 0.5f;
	projshift = -0.15197f;

	//lensradius = -1.f - projshift;
	//lensradsq = lensradius * lensradius;

	//factor = hmdWarpParam.x + hmdWarpParam.y * lensradsq + hmdWarpParam.z * lensradsq * lensradsq
	//+ hmdWarpParam.w * lensradsq * lensradsq * lensradsq;

	//shader.setUniform("gLensCenter", Vector2f(x + (w + projshift) * 0.5f, y + h * 0.5f));
	//shader.setUniform("gScreenCenter", Vector2f(x + w * 0.5f, y + h * 0.5f));
	//shader.setUniform("gScale", Vector2f(w * 0.5f / factor, h * 0.5f * aspect / factor));
	//shader.setUniform("gScaleIn", Vector2f(2.f / w, 2.f / h / aspect));
	shader.setUniform("gLensCenter", Vector2f(0.5+LensCenter, 0.5f));
	shader.setUniform("gScreenCenter", Vector2f(0.5f, 0.5f));
	shader.setUniform("gScale", Vector2f(0.5f / factor, 0.5f * aspect / factor));
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
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void OculusRift::setEyeSpacing(const float& eyeSpacing) {
	_eyeSpacing = eyeSpacing;
}

const float& OculusRift::getEyeSpacing() const {
	return _eyeSpacing;
}

} /* namespace burn */
