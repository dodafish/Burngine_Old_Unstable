/*
 * Character.cpp
 *
 *  Created on: 08.10.2013
 *      Author: Dominik
 */

#include "Character.h"
#include "Shader.h"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

Character::Character(const Uint32& codePoint) :
_codePoint(codePoint) {
}

Character::~Character() {

}

void Character::createFromFtGlyph(void* g) {

	FT_GlyphSlot glyph = static_cast<FT_GlyphSlot>(g);

	//Get dimensions of the glyph
	Vector2ui glyphDimensions(glyph->bitmap.width, glyph->bitmap.rows);

	//Create a texture so we know its final dimensions
	_texture.create(glyphDimensions);
	const Vector2ui& textureDimensions = _texture.getDimensions();

	//Create an array to store glyph data
	GLubyte* data = new GLubyte[textureDimensions.x * textureDimensions.y];

	//Fill this array with data
	for(unsigned int i = 0; i != textureDimensions.y; ++i){
		for(unsigned int j = 0; j != textureDimensions.x; ++j){
			//No bufferdata in bitmap, store 0 (transparent)
			if(i > glyphDimensions.y || j > glyphDimensions.x){
				data[i * textureDimensions.x + j] = 0;
			}else{
				data[i * textureDimensions.x + j] = glyph->bitmap.buffer[(glyphDimensions.y - i - 1) * glyphDimensions.x
				+ j];
			}
		}
	}

	//Remove pixel errors
	removePixelErrors(data, textureDimensions.x, textureDimensions.y);

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

	//Create VBO for rendering
	Vector2f quad[] = {
	Vector2f(0.0f, float(-_advance.y + textureDimensions.y)),
	Vector2f(0.0f, float(-_advance.y)),
	Vector2f(float(textureDimensions.x), float(-_advance.y + textureDimensions.y)),
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

void Character::removePixelErrors(GLubyte* data, const unsigned int& width, const unsigned int& height) {

	//Pixels which have at least 2 neighbours being not null are no
	//errors.
	//Neighbours are one unit up, left, down and right. Not diagonal.

	unsigned int size = width * height;
	for(unsigned int i = 0; i != size; ++i){
		//Cast unsigned to signed for proper testing
		int pixel = static_cast<int>(i);

		//Counts the number of neighbours being not 0
		int neighbours = 0;

		//To the top
		if(pixel - width >= 0){
			if(data[pixel - width] != 0){
				++neighbours;
			}
		}
		//To the bottom
		if(pixel + width < size){
			if(data[pixel + width] != 0){
				++neighbours;
			}
		}
		//To the left
		if(pixel - 1 >= 0){
			if(data[pixel - 1] != 0){
				++neighbours;
			}
		}
		//To the right
		if(static_cast<unsigned int>(pixel + 1) < size){
			if(data[pixel + 1] != 0){
				++neighbours;
			}
		}

		//Update pixel
		if(neighbours < 2){
			data[i] = 0;
		}

	}

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

} /* namespace burn */
