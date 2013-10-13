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
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

Text::Text() :
_fontSize(32) {
}

Text::~Text() {
}

void Text::setText(const String& text) {
	_text = text;
}

void Text::draw() {

	if(!_font.isLoaded())
		return;

	if(!Window::isContextCreated())
		return;

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Vector2i curPosition(static_cast<int>(_position.x), static_cast<int>(_position.y));
	for(size_t i = 0; i < _text.getSize(); ++i){

		if(_text[i] == '\n'){
			curPosition.x = _position.x;
			curPosition.y -= (_fontSize * 1.2f);
			continue;
		}else if(_text[i] == ' '){
			curPosition.x += 10.f;
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

		character.draw();

		curPosition.x += (character.getAdvance().x - character.getBearing().x);
	}

	glEnable(GL_DEPTH_TEST);

}

void Text::setFontSize(const Uint32& size) {
	_fontSize = size;
}

void Text::setFont(const Font& font) {
	_font = font;
}

} /* namespace burn */