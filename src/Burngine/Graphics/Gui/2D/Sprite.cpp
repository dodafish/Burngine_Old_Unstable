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

#include <Burngine/Graphics/Gui/2D/Sprite.h>

#include <Burngine/Graphics/General/BurngineShaders.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

namespace burn {

void Sprite::setTexture(const Texture& texture) {
	_texture = texture;
}

const Texture& Sprite::getTexture() const {
	return _texture;
}

void Sprite::draw() {

	if(_texture.getId() == 0)
		return;

	//Get the shader we want to use
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);

	//Set uniforms
	shader.setUniform("projectionMatrix", getProjectionMatrix());
	shader.setUniform("viewMatrix", getViewMatrix());
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("mixColor", _color);
	shader.setUniform("gSampler", 1);

	//Setup OpenGL for rendering
	OpenGlControl::Settings ogl;
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	ogl.setBlendMode(OpenGlControl::MIX);
	ogl.enableCulling(false);
	OpenGlControl::useSettings(ogl);

	_texture.bind(1);

	//Draw
	_vbo.bind();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Restore default OGL settings
	ogl = OpenGlControl::Settings();
	OpenGlControl::useSettings(ogl);

}

} /* namespace burn */
