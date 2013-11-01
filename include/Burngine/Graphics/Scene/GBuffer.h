/*
 * GBuffer.h
 *
 *  Created on: 01.11.2013
 *      Author: Dominik
 */

#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <Burngine/Export.h>

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
		DIFFUSE = 0,
		NORMAL,

		COUNT,

		DEPTH
	};

	void setSourceBuffer(const GBufferType& buffer);

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
