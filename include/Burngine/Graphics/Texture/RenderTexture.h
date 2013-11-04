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
#include <vector>

namespace burn {

class BURNGINE_API RenderTexture : public BaseTexture{
public:
	RenderTexture();
	~RenderTexture();

	//Rendertargets are not copyable
	RenderTexture(const RenderTexture& other) = delete;
	RenderTexture& operator=(const RenderTexture& other) = delete;

	bool create(const Vector2ui& dimensions);

	void bindAsTarget() const;

	void clear() const;

	void drawFullscreen();
	void draw(const Vector2f& position, const Vector2f& size);

	bool isCreated() const;

	bool addColorAttachment(const GLenum& attachment);

private:
	virtual void onBind(const unsigned int& unit) const;
	virtual void onUnbind(const unsigned int& unit) const;

	void cleanup();

	GLuint _framebuffer, _depthbuffer, _texture;
	bool _isCreated;
	Vector2ui _dimensions;
	std::vector<GLenum> _attachments;
	std::vector<GLuint> _additionalTextures;
};

} /* namespace burn */
#endif /* RENDERTEXTURE_H_ */
