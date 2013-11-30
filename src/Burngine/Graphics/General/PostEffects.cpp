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

#include <Burngine/Graphics/General/PostEffects.h>

#include <Burngine/Graphics/Texture/VarianceShadowMap.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

namespace burn {

void PostEffects::gaussianBlur(	const VarianceShadowMap& vsm,
								const float& blurSize = 0.001f) {

	//Return if the VSM is not created
	if(!vsm.isCreated())
		return;

	//Create a helper FBO with same size of VSM
	VarianceShadowMap helperFbo;
	if(!helperFbo.create(vsm.getDimensions()))
		return;

	//Now "Ping-Pong" horizontal and vertical blur,
	//first to the helper fbo and the back to the source
	ensureContext();

	//Create VSM-algined quad
	Vector3f quadPositions[4] = {
	Vector3f(-1.f, -1.f, 0.f),
	Vector3f(1.f, -1.f, 0.f),
	Vector3f(-1.f, 1.f, 0.f),
	Vector3f(1.f, 1.f, 0.f) };
	Vector2f quadUv[4] = {
	Vector2f(0.f, 0.f),
	Vector2f(1.f, 0.f),
	Vector2f(0.f, 1.f),
	Vector2f(1.f, 1.f) };

	VertexBufferObject vbo;
	vbo.create();
	for(int i = 0; i != 4; ++i){
		vbo.addData(&quadPositions[i], sizeof(Vector3f));
		vbo.addData(&quadUv[i], sizeof(Vector2f));
	}
	vbo.uploadDataToGpu(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	//Setup OpenGL
	OpenGlControl::Settings ogl;
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	ogl.setBlendMode(OpenGlControl::OVERWRITE);
	ogl.enableCulling(false);
	OpenGlControl::useSettings(ogl);

	//First horizontal blur
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::GAUSSIAN_BLUR_HORIZONTAL);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));
	shader.setUniform("gSamplerSource", 0);
	shader.setUniform("gBlurSize", blurSize);

	vsm.bindAsSource(0);
	helperFbo.bindAsTarget();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Now the vertical blur back to the source
	shader = BurngineShaders::getShader(BurngineShaders::GAUSSIAN_BLUR_VERTICAL);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));
	shader.setUniform("gSamplerSource", 0);
	shader.setUniform("gBlurSize", blurSize);

	vsm.bindAsTarget();
	helperFbo.bindAsSource(0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Restore default OGL settings
	OpenGlControl::useSettings(OpenGlControl::Settings());

}

} /* namespace burn */

