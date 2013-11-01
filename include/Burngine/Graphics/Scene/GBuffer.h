/*
 * GBuffer.h
 *
 *  Created on: 01.11.2013
 *      Author: Dominik
 */

#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>

namespace burn {

class BURNGINE_API GBuffer {
public:
	GBuffer();
	~GBuffer();

	//GBuffer is not copyable!
	GBuffer(const GBuffer& other) = delete;
	GBuffer& operator=(const GBuffer& other) = delete;

	bool create(const Vector2ui& dimensions);

	void clear();

	void bindAsTarget() const;
	void bindAsSource() const;

	enum GBufferType {
		DIFFUSE = 0, NORMAL,

		COUNT
	};

	void setSourceBuffer(const GBufferType& buffer);
	void bindDepthBufferAsSourceTexture() const;

	const Vector2ui& getDimensions() const;

private:

	bool _isCreated;
	GLuint _textures[COUNT];
	GLuint _depthTexture;
	GLuint _framebuffer;
	Vector2ui _dimensions;
};

} /* namespace burn */
#endif /* GBUFFER_H_ */
