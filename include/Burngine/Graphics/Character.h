/*
 * Character.h
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "../Export.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Texture.h"

namespace burn {

class BURNGINE_API Character {
public:
	Character(const char32_t& c = '\0');
	~Character();

	void createFromFtBitmap(FT_Bitmap* bitmap);

	void draw(const Vector2i& coordinates) const;

	bool operator==(const char32_t& c) const;

private:
	Texture _texture;
	char32_t _character;
};

} /* namespace burn */
#endif /* CHARACTER_H_ */
