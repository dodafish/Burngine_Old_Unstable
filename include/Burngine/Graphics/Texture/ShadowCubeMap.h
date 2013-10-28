/*
 * ShadowCubeMap.h
 *
 *  Created on: 28.10.2013
 *      Author: Dominik
 */

#ifndef SHADOWCUBEMAP_H_
#define SHADOWCUBEMAP_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

class BURNGINE_API ShadowCubeMap : public BaseTexture {
public:
	ShadowCubeMap();

	//Rendertargets are not copyable!
	ShadowCubeMap(const ShadowCubeMap& other) = delete;
	ShadowCubeMap& operator=(const ShadowCubeMap& other) = delete;

	~ShadowCubeMap();

	enum Resolution {
		VERY_LOW = 128, ///< 128x128
		LOW = 256, ///< 256x256
		MEDIUM = 512, ///< 512x512
		HIGH = 1024, ///< 1024x1024
		VERY_HIGH = 2048 ///< 2048x2048
	};


	bool create(const Resolution& resolution = HIGH);

	void clear();

	void bindAsRendertarget(const int& face) const;

	bool isCreated() const;

private:
	virtual void onBind(const unsigned int& unit) const;
	virtual void onUnbind(const unsigned int& unit) const;

	void cleanup();

	GLuint _framebuffer, _cubeMap;
	Resolution _resolution;
	bool _isCreated;
};

} /* namespace burn */
#endif /* SHADOWCUBEMAP_H_ */
