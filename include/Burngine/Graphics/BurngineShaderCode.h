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

const std::string rawTextureV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;"
		"layout(location = 1) in vec2 vertexUV;"
		"out vec2 UV;"

		"void main(){"
			"gl_Position = vec4(vertexPosition, 1);"
			"UV = vertexUV;"
		"}";

const std::string rawTextureF = "#version 330\n"
		"in vec2 UV;"
		"uniform sampler2D renderedTexture;"

		"out vec3 color;"

		"void main(){"
			"color = texture(renderedTexture, UV).rgb;"
		"}";

const std::string lightingV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;"
		"layout(location = 1) in vec3 vertexNormal;"

		"uniform mat4 " + MODEL_MATRIX + ";"
		"uniform mat4 " + VIEW_MATRIX + ";"
		"uniform mat4 " + PROJECTION_MATRIX + ";"

		"uniform vec3 " + CAMERA_POSITION + ";"
		"uniform vec3 " + LIGHT_POSITION + ";"

		"out vec3 lightDir_camspace;"
		"out vec3 normal_camspace;"

		"void main(){"
			"gl_Position = " + MVP + " * vec4(vertexPosition, 1);"

			"vec3 eyeDirection_camspace = " + CAMERA_POSITION +
			" - (" + VIEW_MATRIX + "*" + MODEL_MATRIX + "* vec4(vertexPosition, 1)).xyz;"

			"vec3 singleLight = " + LIGHT_POSITION + ";"
			"vec3 lPos = (" + VIEW_MATRIX + " * vec4(singleLight,1)).xyz;"
			"lightDir_camspace = lPos + eyeDirection_camspace;"

			"normal_camspace = (" + VIEW_MATRIX + "*" + MODEL_MATRIX + "* vec4(vertexNormal, 0)).xyz;"
		"}";

const std::string lightingF = "#version 330\n"
		"in vec3 lightDir_camspace;"
		"in vec3 normal_camspace;"

		"out vec3 color;"

		"void main(){"
			"vec3 n = normalize(normal_camspace);"
			"vec3 l = normalize(lightDir_camspace);"
			"float cosTheta = clamp( dot( n,l ), 0,1 );"
			"color = vec3(1.0, 1.0, 1.0) * cosTheta;"
		"}";

}

#endif /* BURNGINESHADERCODE_H_ */
