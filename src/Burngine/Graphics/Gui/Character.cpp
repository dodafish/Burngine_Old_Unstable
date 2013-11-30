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

#include <Burngine/Graphics/Gui/Character.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Window/Window.h>

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

Character::Character(	const Uint32& codePoint,
						const unsigned int& size) :
_codePoint(codePoint),
_size(size) {
}

Character::~Character() {

	_vbo.cleanup();

}

void Character::createFromFtGlyph(	void* g,
									void* b) {

	FT_GlyphSlot glyph = static_cast<FT_GlyphSlot>(g);
	FT_Bitmap* bitmap = static_cast<FT_Bitmap*>(b);

	//Get dimensions of the glyph
	Vector2ui glyphDimensions(bitmap->width, bitmap->rows);

	//Create a texture so we know its final dimensions
	const Vector2ui& textureDimensions = BaseTexture::calculatePow2Dimensions(glyphDimensions);

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
	_texture.loadFromData(data, textureDimensions, 16, GL_DEPTH_COMPONENT);

	//Modify settings of the texture
	_texture.setFiltering(Sampler::MAG_BILINEAR, Sampler::MIN_BILINEAR);
	_texture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_texture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_texture.setSamplerParameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	//Get glyph metrics (Shifted to right by 6 to fit pixeldimensions)
	_dimensions = Vector2i(glyph->metrics.width >> 6, glyph->metrics.height >> 6);
	_bearing = Vector2i(glyph->metrics.horiBearingX >> 6, glyph->metrics.horiBearingY >> 6);
	_advance = Vector2i(glyph->advance.x >> 6, (glyph->metrics.height - glyph->metrics.horiBearingY) >> 6);

	//Create VBO-Data for rendering
	Vector3f quad[] = {
	Vector3f(0.0f, float(-_advance.y) + float(textureDimensions.y), 0.f),
	Vector3f(0.0f, float(-_advance.y), 0.f),
	Vector3f(float(textureDimensions.x), float(-_advance.y) + float(textureDimensions.y), 0.f),
	Vector3f(float(textureDimensions.x), float(-_advance.y), 0.f) };

	Vector2f texQuad[] = {
	Vector2f(0.0f, 1.0f),
	Vector2f(0.0f, 0.0f),
	Vector2f(1.0f, 1.0f),
	Vector2f(1.0f, 0.0f) };

	_vbo.create();

	for(int i = 0; i != 4; ++i){
		_vbo.addData(&quad[i], sizeof(Vector3f));
		_vbo.addData(&texQuad[i], sizeof(Vector2f));
	}

	_vbo.uploadDataToGpu(GL_ARRAY_BUFFER);

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

void Character::draw(	const Vector2f& position,
						const Vector4f& color) const {

	//Setup OpenGL for textrendering
	OpenGlControl::Settings ogl;
	ogl.enableDepthbufferWriting(false);
	ogl.enableDepthtest(false);
	ogl.setBlendMode(OpenGlControl::MIX);
	ogl.enableCulling(false);
	OpenGlControl::useSettings(ogl);

	//Calculate matrices
	glm::mat4 modelView = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	glm::mat4 ortho = Window::getOrthoMatrix();

	//Get shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::FONT);

	//Set uniforms
	shader.setUniform("projectionMatrix", ortho);
	shader.setUniform("viewMatrix", modelView);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("fontColor", color);

	//Bind and draw
	_texture.bindAsSource();
	_vbo.bind();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector3f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Restore default OGL settings
	ogl = OpenGlControl::Settings();
	OpenGlControl::useSettings(ogl);

}

bool Character::operator==(const Uint32& codePoint) const {
	return (_codePoint == codePoint);
}

const unsigned int& Character::getSize() const {
	return _size;
}

} /* namespace burn */
