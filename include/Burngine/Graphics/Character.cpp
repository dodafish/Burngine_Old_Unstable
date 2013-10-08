/*
 * Character.cpp
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#include "Character.h"

namespace burn {

Character::Character(const char32_t& c) :
_character(c) {
}

Character::~Character() {
}

void Character::createFromFtBitmap(FT_Bitmap* bitmap) {

}

void Character::draw(const Vector2i& coordinates) const {

}

bool Character::operator==(const char32_t& c) const {
	return (_character == c);
}

} /* namespace burn */
