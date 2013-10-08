/*
 * Font.h
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#ifndef FONT_H_
#define FONT_H_

#include "../Export.h"
#include "Character.h"

#include <vector>

template class BURNGINE_API std::vector<burn::Character*>;

namespace burn {

class BURNGINE_API Font {
public:
	Font();
	~Font();

	bool loadFromFile(const std::string& file);

	const Character& getCharacter(const char32_t& c);

private:
	void cleanup();
	const Character& createCharacter(const char32_t& c);
	int charToInt(const char32_t& c) const;

	std::vector<Character*> _characters;

	FT_Face _face;
	bool _isLoaded;
};

} /* namespace burn */
#endif /* FONT_H_ */
