/*
 * Text.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "../Export.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

class BURNGINE_API Text {
public:
	Text();
	~Text();

	bool loadFromFile(const std::string& fontFile);

	void setText(const std::string& text);

	void draw();

private:

	void cleanup();

	FT_Face _face;

	std::string _text;
};

} /* namespace burn */
#endif /* TEXT_H_ */
