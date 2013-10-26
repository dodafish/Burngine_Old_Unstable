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
#include <Burngine/Graphics/Texture/Sampler.h>

namespace burn {

class BURNGINE_API BaseTexture {
public:
	static Vector2ui calculatePow2Dimensions(const Vector2ui& dimensions);

	BaseTexture();
	virtual ~BaseTexture();

	void bind(const unsigned int& unit = 0) const;
	void unbind(const unsigned int& unit = 0) const;

protected:

	virtual void onBind(const unsigned int& unit) const = 0;
	virtual void onUnbind(const unsigned int& unit) const = 0;

private:
	Sampler _sampler;
};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
