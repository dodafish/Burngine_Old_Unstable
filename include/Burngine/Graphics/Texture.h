/*
 * Texture.h
 *
 *  Created on: 14.09.2013
 *      Author: Dominik
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../Export.h"
#include "OpenGL.h"

namespace burn {

/**
 * @brief Holds OpenGL comfort images as textures.
 */
class BURNGINE_API Texture {
public:
	/**
	 * @brief The default constructor
	 */
	Texture();

	/**
	 * @brief The default destructor
	 */
	~Texture();

	/**
	 * @brief Loads an image from file and stores it
	 * as texture ready for use.
	 *
	 * @param file The image to load
	 *
	 * @return Returns true on success
	 */
	bool loadFromFile(const std::string& file);

	/**
	 * @brief Returns the ID of the texturebuffer
	 *
	 * @return The ID of the texturebuffer or 0 if no buffer exists
	 *
	 * @see loadFromFile()
	 */
	const GLuint& getTextureBuffer() const;

private:

	void generate();
	void erase();

	bool _isTextureGenerated;
	GLuint _texture; ///< OpenGL ID for the texture
};

} /* namespace burn */
#endif /* TEXTURE_H_ */
