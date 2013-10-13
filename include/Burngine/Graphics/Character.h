/*
 * Character.h
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "../Export.h"

#include "Texture.h"
#include "VertexBufferObject.h"

namespace burn {

class BURNGINE_API Character {
public:
	Character(const Uint32& codePoint);
	~Character();

	void createFromFtGlyph(void* glyph, void* bitmap);

	void draw() const;

	bool operator==(const Uint32& codePoint) const;

	const Vector2i& getDimensions() const;
	const Vector2i& getAdvance() const;
	const Vector2i& getBearing() const;

private:
	void removePixelErrors(GLubyte* data, const unsigned int& width, const unsigned int& height);

	Texture _texture;
	Uint32 _codePoint;
	Vector2i _dimensions, _advance, _bearing;
	VertexBufferObject _vbo;
};

} /* namespace burn */
#endif /* CHARACTER_H_ */
