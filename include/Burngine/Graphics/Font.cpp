/*
 * Font.cpp
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#include "Font.h"
#include "../System/Reporter.h"

namespace burn {

Font::Font() :
_face(0),
_isLoaded(false) {

}

Font::~Font() {
	cleanup();
}

bool Font::loadFromFile(const std::string& file) {

	//Do some cleanup if needed
	cleanup();

	//Freetype library
	FT_Library lib;

	//Initialize Freetype
	if(FT_Init_FreeType(&lib) != 0){
		Reporter::report("Failed to load font (Unable to init Freetype)", Reporter::ERROR);
		return false;
	}

	//Load first face from file
	if(FT_New_Face(lib, file.c_str(), 0, &_face) != 0){ //0 means success
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
	FT_Done_FreeType(lib);

	_isLoaded = true;

	return _isLoaded;
}

void Font::cleanup() {

	//Free allocated memory
	for(size_t i = 0; i < _characters.size(); ++i){
		delete _characters[i];
	}

	_characters.clear();
	_isLoaded = false;

}

const Character& Font::getCharacter(const Uint32& codePoint) {

	//Return if already loaded
	for(size_t i = 0; i < _characters.size(); ++i){
		if(*(_characters[i]) == c){
			return *(_characters[i]);
		}
	}

	//Not loaded yet. Do so now and return
	return createCharacter(c);

}

const Character& Font::createCharacter(const char32_t& c) {

	//Load the glyph in face
	FT_Load_Glyph(_face, FT_Get_Char_Index(_face, charToInt(c)), FT_LOAD_DEFAULT);

	//Render it to a bitmap
	FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);

	//Shortcut to the created bitmap
	FT_Bitmap* pBitmap = &_face->glyph->bitmap;

	//Create the character
	Character* ch = new Character(c);
	ch->createFromFtBitmap(pBitmap);

	//Store in vector
	//Pushing at back should not recreate the others, so
	//the texture they hold is not destroyed automatically
	_characters.push_back(ch);

	return *ch;
}

int Font::charToInt(const char32_t& c) const {
	return static_cast<int>(c);
}

bool Font::isLoaded() const {
	return _isLoaded;
}

} /* namespace burn */
