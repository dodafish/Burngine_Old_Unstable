/*
 * Font.cpp
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#include "Font.h"
#include "../System/Reporter.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace burn {

Font::Font() :
_face(0),
_isLoaded(false),
_lineHeight(0) {

}

Font::~Font() {
	cleanup();
}

bool Font::loadFromFile(const std::string& file) {

	//Do some cleanup if needed
	cleanup();

	//Freetype library
	FT_Library lib;
	FT_Face face;

	//Initialize Freetype
	if(FT_Init_FreeType(&lib) != 0){
		Reporter::report("Failed to load font (Unable to init Freetype)", Reporter::ERROR);
		return false;
	}

	//Load first face from file
	if(FT_New_Face(lib, file.c_str(), 0, &face) != 0){ //0 means success
		Reporter::report("Failed to load font. (Failed to load face)", Reporter::ERROR);
		return false;
	}

	// Select the unicode character map
	if(FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0){
		Reporter::report("Failed to load font. (Failed to set Unicode-character-set)", Reporter::ERROR);
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 32);

	//Font's first face is successfully loaded :)

	//Store the face in the beatiful void pointer
	_face = face;

	//Cleanup
	//FT_Done_FreeType(lib);

	_isLoaded = true;

	return _isLoaded;
}

void Font::cleanup() {

	//Free allocated memory
	for(size_t i = 0; i < _characters.size(); ++i){
		//delete _characters[i];
	}

	_characters.clear();
	_isLoaded = false;

}

const Character& Font::getCharacter(const Uint32& codePoint) {

	//Return if already loaded
	for(size_t i = 0; i < _characters.size(); ++i){
		if(*(_characters[i]) == codePoint){
			return *(_characters[i]);
		}
	}

	//Not loaded yet. Do so now and return
	return createCharacter(codePoint);

}

const Character& Font::createCharacter(const Uint32& codePoint) {

	FT_Face face = static_cast<FT_Face>(_face);

	//Load the glyph in face
	FT_Load_Glyph(face, FT_Get_Char_Index(face, codePoint), FT_LOAD_DEFAULT);

	//Render it to a bitmap
	//FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	//Shortcut to the created glyph
	//FT_GlyphSlot glyph = face->glyph;

	FT_Glyph glyph;
	FT_Get_Glyph(face->glyph, &glyph);

	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//Update lineheight information if needed
	_lineHeight = std::max(_lineHeight, static_cast<int>(face->glyph->metrics.height >> 6));

	//Create the character
	Character* ch = new Character(codePoint);
	ch->createFromFtGlyph(face->glyph, &bitmap_glyph->bitmap);

	//Store in vector
	//Pushing at back should not recreate the others, so
	//the texture they hold is not destroyed automatically
	_characters.push_back(ch);

	return *ch;
}

const int& Font::getLineHeight() const {
	return _lineHeight;
}

bool Font::isLoaded() const {
	return _isLoaded;
}

} /* namespace burn */
