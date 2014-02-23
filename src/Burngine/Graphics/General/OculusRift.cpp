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

namespace burn {

OculusRift::OculusRift(const Window& window) :
_window(window),
_eyeSpacing(2.5f) {

	const unsigned int& width = _window.getSettings().getWidth() / 2;
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

	//TODO: 2 cameras
	Vector3f dir = glm::normalize(camera.getLookAt() - camera.getPosition());

	Vector3f n1;
	n1.x = (-1.f) * dir.z;
	n1.y = 0.f;
	n1.z = /*(-1.f) */ dir.x;

	n1 *= (_eyeSpacing * 0.5f);

	Vector3f n2 = -1.f * n1;

	Camera leftCamera, rightCamera;
	/*leftCamera.setPosition(leftCamera.getPosition() + n1);
	 leftCamera.lookAt(leftCamera.getLookAt() + n1);
	 rightCamera.setPosition(rightCamera.getPosition() + n2);
	 rightCamera.lookAt(rightCamera.getLookAt() + n2);*/

	leftCamera.setPosition(camera.getPosition() + n2);
	leftCamera.lookAt(camera.getLookAt() + n2);
	rightCamera.setPosition(camera.getPosition() + n1);
	rightCamera.lookAt(camera.getLookAt() + n1);

	//Left Eye:
	scene.draw(leftCamera, rendermode, &_leftEyeRenderTarget);
	//Right Eye:
	scene.draw(rightCamera, rendermode, &_rightEyeRenderTarget);

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

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));
	shader.setUniform("mixColor", Vector3f(1.f));

	_window.bind();
	shader.setUniform("gSampler", 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Left Eye
	_leftEyeTexture.bind(0);

	_leftEyeVbo.bind();
	glVertexAttribPointer(0, 3,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	glVertexAttribPointer(1, 2,
	GL_FLOAT,
							GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	//Right Eye
	_rightEyeTexture.bind(0);

	_rightEyeVbo.bind();
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
