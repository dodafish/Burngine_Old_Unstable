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

namespace burn {

//Floating
typedef glm::detail::tvec4<float> Vector4f; //x,y,z,w / r,g,b,a
typedef glm::detail::tvec3<float> Vector3f; //x,y,z / r,g,b
typedef glm::detail::tvec2<float> Vector2f; //x,y / u,v

//Double
typedef glm::detail::tvec4<double> Vector4d;
typedef glm::detail::tvec3<double> Vector3d;
typedef glm::detail::tvec2<double> Vector2d;

typedef glm::mat4 Matrix4f;

}

#endif /* MATH_H_ */
