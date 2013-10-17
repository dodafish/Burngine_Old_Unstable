/*
 * RectangleShape.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/2D/RectangleShape.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

namespace burn {

RectangleShape::RectangleShape() :
_color(0.3f, 0.3f, 0.3f, 0.3f) {

}

void RectangleShape::draw() {

	//Create and set a vbo
	Vector2f vboData[4] = {
	Vector2f(0.f),
	Vector2f(_dimensions.x, 0.f),
	Vector2f(0.f, _dimensions.y),
	Vector2f(_dimensions.x, _dimensions.y) };

	VertexBufferObject vbo;
	vbo.addData(&vboData[0], sizeof(Vector2f));
	vbo.addData(&vboData[1], sizeof(Vector2f));
	vbo.addData(&vboData[2], sizeof(Vector2f));
	vbo.addData(&vboData[3], sizeof(Vector2f));
	vbo.uploadDataToGpu(GL_ARRAY_BUFFER);

	//Activate and set shader
	glm::mat4 modelView = glm::translate(glm::mat4(1.0f), Vector3f(_position, 0.0f));

	modelView = glm::rotate(modelView, _rotation, Vector3f(0.f, 0.f, 1.f));

	glm::mat4 ortho = Window::getOrthoMatrix();

	BurngineShaders::useShader(BurngineShaders::ORTHO_COLORED);
	glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::ORTHO_COLORED, PROJECTION_MATRIX), 1,
	GL_FALSE,
						&ortho[0][0]);

	glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::ORTHO_COLORED, VIEW_MATRIX), 1,
	GL_FALSE,
						&modelView[0][0]);

	glUniform4f(BurngineShaders::getShaderUniformLocation(BurngineShaders::ORTHO_COLORED, COLOR), _color.r, _color.g,
				_color.b, _color.a);

	//Setup OpenGL for textrendering
	OpenGlControl::Settings ogl;
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	ogl.setBlendMode(OpenGlControl::MIX);
	OpenGlControl::useSettings(ogl);

	//Draw
	vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_CULL_FACE);

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
