/*
 * Text.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "../Export.h"
#include "Transformable.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"
#include "../System/String.h"

namespace burn {

class BURNGINE_API Text : public Transformable {
public:
	Text();
	~Text();

	void setFontSize(const Uint32& size);

	void setText(const String& text);
	void draw();

	void setFont(const Font& font);

private:
	String _string;
	Uint32 _fontSize;
	Font _font;
};

} /* namespace burn */
#endif /* TEXT_H_ */
