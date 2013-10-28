/*
 * ShadowMap.h
 *
 *  Created on: 25.10.2013
 *      Author: Dominik
 */

#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

class BURNGINE_API ShadowMap : public BaseTexture {
public:
	ShadowMap();

	//Rendertargets are not copyable!
	ShadowMap(const ShadowMap& other) = delete;
	ShadowMap& operator=(const ShadowMap& other) = delete;

	~ShadowMap();

	enum Resolution{
		VERY_LOW = 128, ///< 128x128
		LOW = 256, ///< 256x256
		MEDIUM = 512, ///< 512x512
		HIGH = 1024, ///< 1024x1024
		VERY_HIGH = 2048 ///< 2048x2048
	};

	bool create(const Resolution& resolution = HIGH);

	void clear();
	void bindAsRendertarget() const;

	bool isCreated() const;

private:
	void onBind(const unsigned int& unit) const;
	void onUnbind(const unsigned int& unit) const;

	void cleanup();

	GLuint _framebuffer, _texture;
	Resolution _resolution;
	bool _isCreated;
};

} /* namespace burn */
#endif /* SHADOWMAP_H_ */
