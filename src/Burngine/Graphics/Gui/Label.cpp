/*
 * Label.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/Label.h>
#include <Burngine/System/Rectangle.h>

namespace burn {

Label::Label() {
}

Label::~Label() {
}

void Label::draw() {

	//Calculate the text's box
	IntRect rect;
	rect.left = _position.x;
	rect.bottom = _position.y;
	for(size_t i = 0; i < _text.getSize(); ++i){
		rect.width += _font.getCharacter(_text[i], _fontSize).getDimensions().x;
		rect.height = std::max(_font.getCharacter(_text[i], _fontSize).getDimensions().y, rect.height);
	}

	//Draw the label's background


	//Draw the string on it
	drawString();

}

} /* namespace burn */
