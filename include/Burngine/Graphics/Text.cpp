/*
 * Text.cpp
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#include "Text.h"
#include "../System/Reporter.h"
#include "Window.h"
#include "Shader.h"
#include <iostream>

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
	//Window::setBlendMode(Window::OVERWRITE);

	Vector2i curPosition(static_cast<int>(_position.x), static_cast<int>(_position.y));
	for(size_t i = 0; i < _text.getSize(); ++i){

		if(_text[i] == '\n'){
			curPosition.x = _position.x;
			curPosition.y -= _font.getLineHeight();
			continue;
		}
		const Character& character = _font.getCharacter(_text[i]);
		curPosition.x += character.getBearing().x;

		glm::mat4 modelView = glm::translate(glm::mat4(1.0f),
		 glm::vec3(float(curPosition.x), float(curPosition.y), 0.0f));
		//glm::mat4 modelView = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

		modelView = glm::scale(modelView, glm::vec3(1.f));

		glm::mat4 ortho = Window::getOrthoMatrix();

		BurngineShaders::useShader(BurngineShaders::FONT);
		glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::FONT, PROJECTION_MATRIX), 1,
		GL_FALSE,
							&ortho[0][0]);

		glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::FONT, VIEW_MATRIX), 1,
		GL_FALSE,
							&modelView[0][0]);

		//std::cout << "Character " << _text.toAnsiString()[i] << ". Position: " << curPosition.x << "/" << curPosition.y << "\n";

		//Vector4f endPos = ortho * modelView * Vector4f(float(curPosition.x), float(curPosition.y), 0.0f, 1.0f);
		//std::cout << "CharPos: " << endPos.x << "/" << endPos.y << "/" << endPos.z << "\n";

		character.draw();

		curPosition.x += character.getAdvance().x - character.getBearing().x;
	}

	glEnable(GL_DEPTH_TEST);

}

void Text::setFontSize(const Uint32& size) {
	_fontSize = size;
}

void Text::setFont(const Font& font) {
	_font = font;
}

void Text::setPosition(const Vector2f& pos) {
	_position = pos;
}

} /* namespace burn */
