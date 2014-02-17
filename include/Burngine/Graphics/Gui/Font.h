//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FONT_H_
#define FONT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Gui/Character.h>

#include <vector>
#include <memory>

template class BURNGINE_API std::vector<std::shared_ptr<burn::Character>>;

namespace burn {

/**
 * @brief Can load fontsets from TTF format
 */
class BURNGINE_API Font {
public:
	/**
	 * @brief Default constructor initializing values
	 */
	Font();


	/**
	 * @brief Default destructor cleaning up fontset
	 */
	~Font();

	/**
	 * @brief Load fontsets from file with this function and you
	 * can use them with e.g. the Text class!
	 *
	 * @param file The fontset file to load
	 */
	bool loadFromFile(const std::string& file);

	/**
	 * @brief Retrieve a character based on the loaded fontset.
	 * This is used for rendering mostly only internally.
	 *
	 * @param codePoint the character code
	 * @param fontSize the fontsize in which the character has to be
	 *
	 * @note In order to get a valid character you have to load a fontset
	 * first.
	 *
	 * @see loadFromFile()
	 */
	const std::shared_ptr<Character>& getCharacter(const Uint32& codePoint, const unsigned int& fontSize);

	/**
	 * @brief Check if a fontset is loaded
	 *
	 * @return Returns true if a fontset is loaded
	 *
	 * @see loadFromFile()
	 */
	bool isLoaded() const;

	/**
	 * @brief For rendering this will return the offset in pixels
	 * you need to correctly display a new line
	 *
	 * @return The offset in pixels
	 */
	float getNextLineOffset() const;

	/**
	 * @brief For rendering this will return the offset in pixels
	 * you need to correctly display a spacebreak
	 *
	 * @return The offset in pixels
	 */
	float getSpaceOffset() const;

private:
	void cleanup();
	const std::shared_ptr<Character>& createCharacter(const Uint32& codePoint);
	bool setFontSize(const unsigned int& fontSize);

	void* _face;
	bool _isLoaded;
	unsigned int _loadedFontSize;

	std::vector<std::shared_ptr<burn::Character>> _characters;
	std::shared_ptr<Character> _emptyCharacter; ///< Will be return on errors and fails
};

} /* namespace burn */
#endif /* FONT_H_ */
