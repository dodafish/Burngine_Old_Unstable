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

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/Texture.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

namespace burn {

class BURNGINE_API Character {
public:
	Character(const Uint32& codePoint = 0, const unsigned int& size = 0);
	~Character();

	void createFromFtGlyph(void* glyph, void* bitmap);

	void draw(const Vector2f& position, const Vector4f& color) const;

	bool operator==(const Uint32& codePoint) const;

	const Vector2i& getDimensions() const;
	const Vector2i& getAdvance() const;
	const Vector2i& getBearing() const;

	const unsigned int& getSize() const;

private:
	Texture _texture;
	Uint32 _codePoint;
	Vector2i _dimensions, _advance, _bearing;
	VertexBufferObject _vbo;
	unsigned int _size;

};

} /* namespace burn */
#endif /* CHARACTER_H_ */
