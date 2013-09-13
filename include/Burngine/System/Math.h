/*
 * Math.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef MATH_H_
#define MATH_H_

#include "../Export.h"
#include "../extern/glm/glm.hpp"
#include "../extern/glm/gtc/matrix_transform.hpp"
#include "../extern/glm/gtx/transform.hpp"

namespace burn{

	typedef glm::vec4 Vector4f; //x,y,z,w / r,g,b,a
	typedef glm::vec3 Vector3f; //x,y,z / r,g,b
	typedef glm::vec2 Vector2f; //x,y / u,v

	typedef glm::mat4 Matrix4f;

}

#endif /* MATH_H_ */
