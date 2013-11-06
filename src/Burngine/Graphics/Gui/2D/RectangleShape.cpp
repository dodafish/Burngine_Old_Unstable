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

#include <Burngine/Graphics/Gui/2D/RectangleShape.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

namespace burn {

RectangleShape::RectangleShape() :
_color(0.3f, 0.3f, 0.3f, 0.5f) {

}

void RectangleShape::draw() {

	//Create and set a vbo
	Vector3f vboData[4] = {
	Vector3f(0.f),
	Vector3f(_dimensions.x, 0.f, 0.f),
	Vector3f(0.f, _dimensions.y, 0.f),
	Vector3f(_dimensions.x, _dimensions.y, 0.f) };

	VertexBufferObject vbo;
	vbo.addData(&vboData[0], sizeof(Vector3f));
	vbo.addData(&vboData[1], sizeof(Vector3f));
	vbo.addData(&vboData[2], sizeof(Vector3f));
	vbo.addData(&vboData[3], sizeof(Vector3f));
	vbo.uploadDataToGpu(GL_ARRAY_BUFFER);

	//Calculate matrices
	glm::mat4 modelView = glm::translate(glm::mat4(1.0f), Vector3f(_position, 0.0f));
	modelView = glm::rotate(modelView, _rotation, Vector3f(0.f, 0.f, 1.f));
	Matrix4f ortho = Window::getOrthoMatrix();

	//Get the shader we want to use
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::SINGLECOLOR);

	//Set uniforms
	shader.setUniform("projectionMatrix", ortho);
	shader.setUniform("viewMatrix", modelView);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("color", _color);

	//Setup OpenGL for rendering
	OpenGlControl::Settings ogl;
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	ogl.setBlendMode(OpenGlControl::MIX);
	ogl.enableCulling(false);
	OpenGlControl::useSettings(ogl);

	//Draw
	vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);
	glDisableVertexAttribArray(0);

	//Restore default OGL settings
	ogl = OpenGlControl::Settings();
	OpenGlControl::useSettings(ogl);

}

void RectangleShape::setDimensions(const Vector2f& dimensions) {
	_dimensions = dimensions;
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

} /* namespace burn */
