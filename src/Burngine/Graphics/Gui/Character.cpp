/*
 * Character.cpp
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/Character.h>
#include <Burngine/Graphics/General/Shader.h>

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

Character::Character(const Uint32& codePoint, const unsigned int& size) :
_codePoint(codePoint),
_size(size) {
}

Character::~Character() {

	_vbo.cleanup();
	_texture.cleanup();

}

void Character::createFromFtGlyph(void* g, void* b) {

	FT_GlyphSlot glyph = static_cast<FT_GlyphSlot>(g);
	FT_Bitmap* bitmap = static_cast<FT_Bitmap*>(b);

	//Get dimensions of the glyph
	Vector2ui glyphDimensions(bitmap->width, bitmap->rows);

	//Create a texture so we know its final dimensions
	_texture.create(glyphDimensions);
	const Vector2ui& textureDimensions = _texture.getDimensions();

	//Create an array to store glyph data
	GLubyte* data = new GLubyte[textureDimensions.x * textureDimensions.y];

	//Fill this array with data
	for(unsigned int i = 0; i != textureDimensions.y; ++i){
		for(unsigned int j = 0; j != textureDimensions.x; ++j){
			//No bufferdata in bitmap, store 0 (transparent)
			if(i >= glyphDimensions.y || j >= glyphDimensions.x){
				data[i * textureDimensions.x + j] = 0;
			}else{
				data[i * textureDimensions.x + j] = bitmap->buffer[(glyphDimensions.y - i - 1) * glyphDimensions.x + j];
			}
		}
	}

	//Fill texture with data
	_texture.loadFromData(data, glyphDimensions, 16, GL_DEPTH_COMPONENT);

	//Modify settings of the texture
	_texture.setFiltering(Texture::MAG_BILINEAR, Texture::MIN_BILINEAR);
	_texture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_texture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_texture.setSamplerParameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	//Get glyph metrics (Shifted to right by 6 to fit pixeldimensions)
	_dimensions = Vector2i(glyph->metrics.width >> 6, glyph->metrics.height >> 6);
	_bearing = Vector2i(glyph->metrics.horiBearingX >> 6, glyph->metrics.horiBearingY >> 6);
	_advance = Vector2i(glyph->advance.x >> 6, (glyph->metrics.height - glyph->metrics.horiBearingY) >> 6);

	//Create VBO-Data for rendering
	Vector2f quad[] = {
	Vector2f(0.0f, float(-_advance.y) + float(textureDimensions.y)),
	Vector2f(0.0f, float(-_advance.y)),
	Vector2f(float(textureDimensions.x), float(-_advance.y) + float(textureDimensions.y)),
	Vector2f(float(textureDimensions.x), float(-_advance.y)) };

	Vector2f texQuad[] = {
	Vector2f(0.0f, 1.0f),
	Vector2f(0.0f, 0.0f),
	Vector2f(1.0f, 1.0f),
	Vector2f(1.0f, 0.0f) };

	_vbo.create();

	std::cout << "Quad data: ";
	for(int i = 0; i != 4; ++i){
		std::cout << quad[i].x << "/" << quad[i].y << " ";
		_vbo.addData(&quad[i], sizeof(Vector2f));
		_vbo.addData(&texQuad[i], sizeof(Vector2f));
	}
	std::cout << "\n";

	_vbo.uploadDataToGpu(GL_ARRAY_BUFFER);

	_texture.unbind();

	//Free allocated memory
	delete[] data;
}

const Vector2i& Character::getDimensions() const {
	return _dimensions;
}
const Vector2i& Character::getAdvance() const {
	return _advance;
}
const Vector2i& Character::getBearing() const {
	return _bearing;
}

void Character::draw() const {

	if(!_texture.isCreated())
		return;

//std::cout << "VBO ID: " << _vbo.getBuffer() << "\n";

	_texture.bind();

	glEnableVertexAttribArray(0);
	_vbo.bind();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) * 2, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) * 2, (void*)sizeof(Vector2f));

	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_CULL_FACE);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

bool Character::operator==(const Uint32& codePoint) const {
	return (_codePoint == codePoint);
}

const unsigned int& Character::getSize() const {
	return _size;
}

} /* namespace burn */
