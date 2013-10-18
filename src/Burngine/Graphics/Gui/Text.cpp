/*
 * Text.cpp
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/Text.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

Text::Text() :
_fontSize(32),
_color(Vector4f(1.f, 1.f, 1.f, 1.f)) {
}

Text::~Text() {
}

void Text::setText(const String& text) {
	_text = text;
}

void Text::draw() {

	//Valid OpenGL context has to exist
	if(!Window::isContextCreated())
		return;

	//Draw the string to screen
	drawString();

}

void Text::drawString() {

	//The font has to be loaded obviously
	if(!_font.isLoaded())
		return;

	//Setup OpenGL for textrendering
	OpenGlControl::Settings ogl;
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	ogl.setBlendMode(OpenGlControl::MIX);
	OpenGlControl::useSettings(ogl);

	Vector2i curPosition(static_cast<int>(_position.x), static_cast<int>(_position.y));
	for(size_t i = 0; i < _text.getSize(); ++i){

		if(_text[i] == '\n'){
			curPosition.x = _position.x;
			curPosition.y -= _font.getNextLineOffset();
			continue;
		}else if(_text[i] == ' '){
			curPosition.x += _font.getSpaceOffset();
			continue;
		}

		const Character& character = _font.getCharacter(_text[i], _fontSize);
		curPosition.x += character.getBearing().x;

		glm::mat4 modelView = glm::translate(glm::mat4(1.0f),
												glm::vec3(float(curPosition.x), float(curPosition.y), 0.0f));

		modelView = glm::rotate(modelView, _rotation, Vector3f(0.f, 0.f, 1.f));

		glm::mat4 ortho = Window::getOrthoMatrix();

		BurngineShaders::useShader(BurngineShaders::FONT);
		glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::FONT, PROJECTION_MATRIX), 1,
		GL_FALSE,
							&ortho[0][0]);

		glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::FONT, VIEW_MATRIX), 1,
		GL_FALSE,
							&modelView[0][0]);

		glUniform4f(BurngineShaders::getShaderUniformLocation(BurngineShaders::FONT, FONT_COLOR), _color.r, _color.g,
					_color.b, _color.a);

		character.draw();

		curPosition.x += (character.getAdvance().x - character.getBearing().x);
	}

	//Restore default OGL settings
	ogl = OpenGlControl::Settings();
	OpenGlControl::useSettings(ogl);

}
void Text::setFontSize(const Uint32& size) {
	_fontSize = size;
}

void Text::setFont(const Font& font) {
	_font = font;
}

void Text::setColor(const Vector4f& color) {
	_color = color;
}

const Vector4f& Text::getColor() const {
	return _color;
}

} /* namespace burn */
