/*
 * Label.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/Label.h>
#include <Burngine/System/Rectangle.h>
#include <Burngine/Graphics/Gui/2D/RectangleShape.h>

namespace burn {

Label::Label() :
_border(0.f) {
}

Label::~Label() {
}

void Label::draw() {

	//Calculate the text's box
	IntRect rect;
	rect.left = _position.x;
	rect.bottom = _position.y;
	float finalHeight = 0.f;
	int finalWidth = 0;
	bool firstLine = true;
	int highestAdvanceY = 0;
	for(size_t i = 0; i < _text.getSize(); ++i){
		//Handle special characters
		if(_text[i] == '\n'){
			finalHeight -= _font.getNextLineOffset();

			if(firstLine){
				rect.bottom += rect.height;
				finalHeight -= rect.height;
				firstLine = false;
			}

			rect.width = 0;
			highestAdvanceY = 0;
			continue;
		}else if(_text[i] == ' '){
			rect.width += _font.getSpaceOffset();
			continue;
		}

		//Default character
		rect.width += (_font.getCharacter(_text[i], _fontSize).getAdvance().x
		- _font.getCharacter(_text[i], _fontSize).getBearing().x)
		+ _font.getCharacter(_text[i], _fontSize).getBearing().x;
		rect.height = std::max(_font.getCharacter(_text[i], _fontSize).getDimensions().y, rect.height);
		highestAdvanceY = std::max(highestAdvanceY, _font.getCharacter(_text[i], _fontSize).getAdvance().y);
		finalWidth = std::max(finalWidth, rect.width);
	}
	if(!firstLine){
		rect.height = static_cast<int>(finalHeight);
		rect.width = finalWidth;
	}else{
		rect.bottom += rect.height;
		rect.height *= -1;
	}
	rect.height -= highestAdvanceY;

	//Draw the label's background
	RectangleShape shape;
	shape.setColor(_backgroundColor);
	shape.setPosition(Vector2f(static_cast<float>(rect.left) - _border, static_cast<float>(rect.bottom) + _border));
	shape.setDimensions(
	Vector2f(static_cast<float>(rect.width) + _border * 2.f, static_cast<float>(rect.height) - _border * 2.f));
	shape.setRotation(_rotation);
	shape.draw();

	//Draw the string on it
	drawString();

}

void Label::setBackgroundColor(const Vector4f& color) {
	_backgroundColor = color;
}

const Vector4f& Label::getBackgroundColor() const {
	return _backgroundColor;
}

void Label::setBorder(const float& border) {
	_border = border;
}

const float& Label::getBorder() const {
	return _border;
}

} /* namespace burn */
