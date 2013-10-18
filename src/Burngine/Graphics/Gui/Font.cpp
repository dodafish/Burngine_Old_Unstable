/*
 * Font.cpp
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/Font.h>
#include <Burngine/System/Reporter.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace burn {

Font::Font() :
_face(0),
_isLoaded(false),
_loadedFontSize(32),
_emptyCharacter() {

}

Font::~Font() {
	cleanup();
}

float Font::getNextLineOffset() const {
	return (_loadedFontSize * 1.4f);
}

float Font::getSpaceOffset() const {
	return (_loadedFontSize * 0.5f);
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

	if(FT_Set_Pixel_Sizes(face, 0, 32) != 0){
		Reporter::report("Failed to load font. (Failed to set Fontsize of 32)", Reporter::ERROR);
		return false;
	}

	//Font's first face is successfully loaded :)

	//Store the face in the beatiful void pointer
	_face = face;

	//Cleanup
	//FT_Done_FreeType(lib);

	_isLoaded = true;

	return _isLoaded;
}

void Font::cleanup() {

	_characters.clear();
	_isLoaded = false;

}

const Character& Font::getCharacter(const Uint32& codePoint, const unsigned int& fontSize) {

	//Update the wished fontsize
	if(!setFontSize(fontSize))
		return _emptyCharacter;

	//Return if already loaded
	for(size_t i = 0; i < _characters.size(); ++i){
		if(*(_characters[i]) == codePoint){
			if(_characters[i]->getSize() == fontSize){
				return *(_characters[i]);
			}
		}
	}

	//Not loaded yet. Do so now and return
	return createCharacter(codePoint);
}

bool Font::setFontSize(const unsigned int& fontSize) {

	//Cast voidpointer to FT_Face
	FT_Face face = static_cast<FT_Face>(_face);

	if(fontSize == _loadedFontSize)
		return true;

	if(_face != 0){
		if(FT_Set_Pixel_Sizes(face, 0, fontSize) == 0){
			_loadedFontSize = fontSize;
			return true;
		}
	}

	return false;
}

const Character& Font::createCharacter(const Uint32& codePoint) {

	FT_Face face = static_cast<FT_Face>(_face);

	//Load the glyph in face
	if(FT_Load_Glyph(face, FT_Get_Char_Index(face, codePoint), FT_LOAD_DEFAULT) != 0){
		Reporter::report("Failed to load glyph!", Reporter::ERROR);
		return _emptyCharacter;
	}

	//Get the glyph
	FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph, &glyph) != 0){
		Reporter::report("Failed get glyph!", Reporter::ERROR);
		return _emptyCharacter;
	}

	//Render to a bitmap
	if(FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1) != 0){
		Reporter::report("Failed to render glyph to bitmap!", Reporter::ERROR);
		return _emptyCharacter;
	}
	FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)(glyph);

	//Create the character
	std::shared_ptr<burn::Character> ch(new Character(codePoint, _loadedFontSize));
	ch->createFromFtGlyph(face->glyph, &bitmapGlyph->bitmap);

	//Store in vector
	//Pushing at back should not recreate the others, so
	//the texture they hold is not destroyed automatically
	_characters.push_back(ch);

	return *ch;
}

bool Font::isLoaded() const {
	return _isLoaded;
}

} /* namespace burn */
