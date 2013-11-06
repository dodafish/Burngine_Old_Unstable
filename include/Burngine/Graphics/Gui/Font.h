//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

class BURNGINE_API Font {
public:
	Font();
	~Font();

	bool loadFromFile(const std::string& file);

	const Character& getCharacter(const Uint32& codePoint, const unsigned int& fontSize);

	bool isLoaded() const;

	float getNextLineOffset() const;
	float getSpaceOffset() const;

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
