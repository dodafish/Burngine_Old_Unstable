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

#include <Burngine/Graphics/Gui/2D/RectangleShape.h>

#include <Burngine/Graphics/General/BurngineShaders.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

namespace burn {

RectangleShape::RectangleShape(	const Vector4f& color,
								const Vector2f& dimensions) :
_dimensions(dimensions),
_color(color) {
	updateVbo();
}

Matrix4f RectangleShape::getViewMatrix() {
	glm::mat4 modelView = glm::translate(glm::mat4(1.0f), Vector3f(_position, 0.0f));
	return glm::rotate(modelView, _rotation, Vector3f(0.f, 0.f, 1.f));
}

Matrix4f RectangleShape::getProjectionMatrix() {
	return Window::getOrthoMatrix();
}

void RectangleShape::draw() {

	//Get the shader we want to use
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::SINGLECOLOR);

	//Set uniforms
	shader.setUniform("projectionMatrix", getProjectionMatrix());
	shader.setUniform("viewMatrix", getViewMatrix());
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("gColor", _color);

	//Setup OpenGL for rendering
	OpenGlControl::Settings ogl;
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	ogl.setBlendMode(OpenGlControl::MIX);
	ogl.enableCulling(false);
	OpenGlControl::useSettings(ogl);

	//Draw
	_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);
	glDisableVertexAttribArray(0);

	//Restore default OGL settings
	ogl = OpenGlControl::Settings();
	OpenGlControl::useSettings(ogl);

}

void RectangleShape::setDimensions(const Vector2f& dimensions) {
	_dimensions = dimensions;
	updateVbo();
}

const Vector2f& RectangleShape::getDimensions() const {
	return _dimensions;
}

void RectangleShape::setColor(const Vector4f& color) {
	_color = color;
}

const Vector4f& RectangleShape::getColor() const {
	return _color;
}

void RectangleShape::updateVbo() {

	Vector3f vboData[4] = {
	Vector3f(0.f),
	Vector3f(_dimensions.x, 0.f, 0.f),
	Vector3f(0.f, _dimensions.y, 0.f),
	Vector3f(_dimensions.x, _dimensions.y, 0.f) };

	Vector2f vboDataUv[4] = {
	Vector2f(0.f, 0.f),
	Vector2f(1.f, 0.f),
	Vector2f(0.f, 1.f),
	Vector2f(1.f, 1.f) };

	_vbo.reset();
	for(int i = 0; i != 4; ++i){
		_vbo.addData(&vboData[i], sizeof(Vector3f));
		_vbo.addData(&vboDataUv[i], sizeof(Vector2f));
	}
	_vbo.uploadDataToGpu(GL_ARRAY_BUFFER);

}

} /* namespace burn */
