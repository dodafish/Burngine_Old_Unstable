/*
 * BaseTexture.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#ifndef BASETEXTURE_H_
#define BASETEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>

namespace burn {

class BURNGINE_API BaseTexture {
public:

	enum MagnificationFiltering {
		MAG_NEAREST, MAG_BILINEAR
	};
	enum MinificationFiltering {
		MIN_NEAREST, MIN_BILINEAR, MIN_TRILINEAR, MIN_NEAREST_MIPMAP, MIN_BILINEAR_MIPMAP
	};

	BaseTexture();
	BaseTexture(const BaseTexture& other);
	BaseTexture& operator=(const BaseTexture& other);

	virtual ~BaseTexture();

	virtual void cleanup();

	void setFiltering(const MagnificationFiltering& mag, const MinificationFiltering& min);
	void setSamplerParameter(GLenum parameter, GLenum value);

	void unbind();
	virtual void bind() const;

	bool isCreated() const;

	const Vector2ui& getDimensions() const;
	const Vector2ui& getOriginalDimensions() const;

	void setToUnit(const unsigned int& unit);

	static GLfloat getMaxAnisotropicLevel();

	void setAnisotropicLevel(const GLfloat& level);
	const GLfloat& getAnisotropicLevel() const;

	static Vector2ui calculateDimensions(const Vector2ui& dimensions);

protected:
	void create(const Vector2ui& dimensions);

	void generate();

	GLint getCurrentBoundTexture() const;
	void updateFiltering() const;
	Vector2f mapUvCoordsToTexture(const Vector2f& uv) const;

	GLuint _texture; ///< Texture's ID
	GLuint _sampler; ///< Sampler's ID
	Vector2ui _originalDimensions; ///< Original dimension. May be no power of 2
	bool _mipmapsGenerated; ///< Whether or not mipmaps have been generated
	unsigned int _unit;
private:
	Vector2ui _dimensions; ///< Width and height. Always a power of 2

	MagnificationFiltering _magnificationFiltering; ///< Used magnification filtering method
	MinificationFiltering _minificationFiltering; ///< Used minification filtering method
	GLfloat _anisotropicLevel;

	unsigned int* _referenceCount;
};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
