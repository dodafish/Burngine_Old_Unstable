/*
 * Character.h
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

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

	void draw() const;

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
