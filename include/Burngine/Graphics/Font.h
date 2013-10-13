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

	const Character& getCharacter(const Uint32& codePoint, const unsigned int& fontSize);

	bool isLoaded() const;

private:
	void cleanup();
	const Character& createCharacter(const Uint32& codePoint);
	bool setFontSize(const unsigned int& fontSize);

	std::vector<Character*> _characters;
	Character _emptyCharacter; ///< Will be return on errors and fails

	void* _face;
	bool _isLoaded;
	unsigned int _loadedFontSize;
};

} /* namespace burn */
#endif /* FONT_H_ */
