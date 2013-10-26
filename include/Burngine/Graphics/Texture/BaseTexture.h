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

	static GLfloat getMaxAnisotropicLevel();
	static Vector2ui calculatePow2Dimensions(const Vector2ui& dimensions);



	BaseTexture();
	BaseTexture(const BaseTexture& other);
	BaseTexture& operator=(const BaseTexture& other);
	virtual ~BaseTexture();

	virtual void bind(const unsigned int& unit) const = 0;
	bool isCreated() const = 0;

protected:
	bool createSampler();

	GLuint _sampler; ///< Sampler's ID

	unsigned int* _referenceCount;
private:
	void destroySampler();


};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
