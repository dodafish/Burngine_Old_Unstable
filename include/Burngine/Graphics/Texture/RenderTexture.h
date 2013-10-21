/*
 * RenderTexture.h
 *
 *  Created on: 22.09.2013
 *      Author: Dominik
 */

#ifndef RENDERTEXTURE_H_
#define RENDERTEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

class BURNGINE_API RenderTexture : public BaseTexture{
public:
	RenderTexture();
	~RenderTexture();

	RenderTexture(const RenderTexture& other) = delete;
	RenderTexture& operator=(const RenderTexture& other) = delete;

	virtual bool create(const Vector2ui& dimensions);

	virtual void bind() const;
	void clear() const;

	void drawFullscreen();
	void draw(const Vector2f& position, const Vector2f& size);

private:
	GLuint _framebuffer, _depthbuffer;
};

} /* namespace burn */
#endif /* RENDERTEXTURE_H_ */
