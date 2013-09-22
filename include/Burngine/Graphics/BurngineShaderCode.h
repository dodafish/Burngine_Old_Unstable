/*
 * BurngineShaderCode.h
 *
 *  Created on: 22.09.2013
 *      Author: Dominik
 */

#ifndef BURNGINESHADERCODE_H_
#define BURNGINESHADERCODE_H_

#include "Shader.h"

namespace burn {

const std::string solidColorV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;"
		"layout(location = 1) in vec3 vertexColor;"

		"out vec3 fragmentColor;"

		"uniform mat4 " + MODEL_MATRIX + ";"
		"uniform mat4 " + VIEW_MATRIX + ";"
		"uniform mat4 " + PROJECTION_MATRIX + ";"

		"void main(){"
			"gl_Position = " + MVP + " * vec4(vertexPosition, 1.0);"
			"fragmentColor = vertexColor;"
		"}";

const std::string solidColorF = "#version 330\n"
		"in vec3 fragmentColor;"

		"out vec3 color;"

		"void main(){"
			"color = fragmentColor;"
		"}";

const std::string texturedV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;"
		"layout(location = 1) in vec2 vertexUv;"
		"layout(location = 2) in vec3 vertexNormal;\n"

		"out vec2 UV;"

		"uniform mat4 " + MODEL_MATRIX + ";"
		"uniform mat4 " + VIEW_MATRIX + ";"
		"uniform mat4 " + PROJECTION_MATRIX + ";"

		"void main(){"
			"UV = vertexUv;"
			"gl_Position = " + MVP + " * vec4(vertexPosition, 1);"
		"}";

const std::string texturedF = "#version 330\n"
		"in vec2 UV;"

		"out vec3 color;"

		"uniform sampler2D myTextureSampler;"

		"void main(){"
			"color = texture(myTextureSampler, UV).rgb;"
		"}";

}

const std::string rawTextureV = "#version 330\n"
		"in vec3 vertexPosition;"
		"in vec2 vertexUV;"
		"out vec2 UV;"

		"void main(){"
			"gl_Position = vertexPosition;"
			"UV = vertexUV;"
		"}"
		;

const std::string rawTextureF = "#version 330\n"
		"in vec2 UV;"
		"uniform sampler2D texture;"

		"out vec3 color;"

		"void main(){"
			"color = texture(texture, UV).rgb;"
		"}"
		;

#endif /* BURNGINESHADERCODE_H_ */
