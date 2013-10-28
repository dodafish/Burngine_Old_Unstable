/*
 * Skybox.h
 *
 *  Created on: 28.10.2013
 *      Author: Dominik
 */

#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/CubeMap.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

namespace burn {

class BURNGINE_API SkyBox {
public:
	SkyBox();
	SkyBox(const SkyBox& other);
	SkyBox& operator=(const SkyBox& other);

	void setCubeMap(const CubeMap& cubeMap);
	const CubeMap& getCubeMap() const;

	void draw();

private:
	CubeMap _cubeMap;
	VertexBufferObject _boxVbo;
};

} /* namespace burn */
#endif /* SKYBOX_H_ */
