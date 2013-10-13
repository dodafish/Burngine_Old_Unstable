/*
 * Font.h
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#ifndef FONT_H_
#define FONT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Gui/Character.h>

#include <vector>
#include <memory>

template class BURNGINE_API std::vector<std::shared_ptr<burn::Character>>;

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

	void* _face;
	bool _isLoaded;
	unsigned int _loadedFontSize;

	std::vector<std::shared_ptr<burn::Character>> _characters;
	Character _emptyCharacter; ///< Will be return on errors and fails
};

} /* namespace burn */
#endif /* FONT_H_ */
