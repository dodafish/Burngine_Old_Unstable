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

#include <Burngine/Graphics/General/PostEffects.h>

#include <Burngine/Graphics/General/BurngineShaders.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/Texture/RenderTarget.h>

namespace burn {

void PostEffects::gaussianBlur(const Texture& texture) {

	//Create a helper FBO with same size of the texture
	RenderTarget rtt, rttBack;

	//The ping-pong helper texture:
	Texture helperTexture;
	helperTexture.create(texture.getDimensions(), texture.getInternalFormat());

	if(!rtt.create(helperTexture.getDimensions(), RenderTarget::NO_DEPTHBUFFER, helperTexture))
		return;
	if(!rttBack.create(texture.getDimensions(), RenderTarget::NO_DEPTHBUFFER, texture))
		return;

	Texture::MagnificationFilter magF = texture.getMagnificationFilter();
	Texture::MinificationFilter minF = texture.getMinificationFilter();

	texture.setFiltering(BaseTexture::MAG_BILINEAR, BaseTexture::MIN_BILINEAR);
	helperTexture.setFiltering(BaseTexture::MAG_BILINEAR, BaseTexture::MIN_BILINEAR);

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
	const Shader& shaderGBH = BurngineShaders::getShader(BurngineShaders::GAUSSIAN_BLUR_HORIZONTAL);
	shaderGBH.setUniform("modelMatrix", Matrix4f(1.f));
	shaderGBH.setUniform("viewMatrix", Matrix4f(1.f));
	shaderGBH.setUniform("projectionMatrix", Matrix4f(1.f));
	shaderGBH.setUniform("gSamplerSource", 0);
	shaderGBH.setUniform("gBlurSize", 1.f / texture.getDimensions().x);

	texture.bind(0);
	rtt.bind();
	rtt.clear();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	vbo.bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shaderGBH);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Now the vertical blur back to the source
	const Shader& shaderGBV = BurngineShaders::getShader(BurngineShaders::GAUSSIAN_BLUR_VERTICAL);
	shaderGBV.setUniform("modelMatrix", Matrix4f(1.f));
	shaderGBV.setUniform("viewMatrix", Matrix4f(1.f));
	shaderGBV.setUniform("projectionMatrix", Matrix4f(1.f));
	shaderGBV.setUniform("gSamplerSource", 0);
	shaderGBV.setUniform("gBlurSize", 1.f / texture.getDimensions().y);

	helperTexture.bind(0);
	rttBack.bind();
	rttBack.clear();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	vbo.bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shaderGBV);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Restore default OGL settings
	OpenGlControl::useSettings(OpenGlControl::Settings());

	//Restore texture's filtering
	texture.setFiltering(magF, minF);

}

} /* namespace burn */

