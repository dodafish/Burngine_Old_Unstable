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

#include "Font.h"

namespace burn {

class BURNGINE_API Text {
public:
	Text();
	~Text();

	void setText(const std::string& text);
	void draw();

private:
	std::string _text;
};

} /* namespace burn */
#endif /* TEXT_H_ */
