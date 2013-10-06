/*
 * Text.cpp
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#include "Text.h"
#include "../System/Reporter.h"

namespace burn {

Text::Text() :
_face(0) {
}

Text::~Text() {
	cleanup();
}

void Text::cleanup() {
	if(_face){
		FT_Done_Face(_face);
	}

	_face = 0;
}

bool Text::loadFromFile(const std::string& fontFile) {

	//Freetype objects
	FT_Library library;

	//Initialize freetype library
	if(FT_Init_FreeType(&library) != 0){ //0 means success
		Reporter::report("Failed to load font. (Failed to init Freetype library)", Reporter::ERROR);
		return false;
	}

	//Load one face from fontfile
	if(FT_New_Face(library, fontFile.c_str(), 0, &_face) != 0){ //0 means success
		Reporter::report("Failed to load font. (Failed to load face)", Reporter::ERROR);
		return false;
	}

	// Select the unicode character map
	if(FT_Select_Charmap(_face, FT_ENCODING_UNICODE) != 0){
		Reporter::report("Failed to load font. (Failed to set Unicode-character-set)", Reporter::ERROR);
		return false;
	}

	//Font's first face is successfully loaded :)

	//Cleanup
	FT_Done_FreeType(library);

	return true;
}

void Text::setText(const std::string& text) {
	_text = text;
}

void Text::draw() {
}

} /* namespace burn */
