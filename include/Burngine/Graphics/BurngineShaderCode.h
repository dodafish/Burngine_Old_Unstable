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
		"uniform mat4 " + NORMAL_MATRIX + ";"

		"uniform vec3 " + CAMERA_POSITION + ";"
		"uniform vec3 " + LIGHT_POSITION + ";"

		"out vec3 lightDir_camspace;"
		"out vec3 normal_camspace;"
		"out float dist;"
		"out vec3 eyeDir_camspace;"

		"void main(){"
			"gl_Position = " + MVP + " * vec4(vertexPosition, 1);"

			"eyeDir_camspace = " + CAMERA_POSITION +
			" - (" + VIEW_MATRIX + "*" + MODEL_MATRIX + "* vec4(vertexPosition, 1)).xyz;"

			"vec3 singleLight = " + LIGHT_POSITION + ";"
			"vec3 lPos = (" + VIEW_MATRIX + " * vec4(singleLight,1)).xyz;"
			"lightDir_camspace = lPos + eyeDir_camspace;"

			"normal_camspace = (" + NORMAL_MATRIX + "* vec4(vertexNormal, 0)).xyz;"
			"dist = distance(gl_Position.xyz, lPos);"
		"}";

const std::string lightingF = "#version 330\n"
		"in vec3 lightDir_camspace;"
		"in vec3 normal_camspace;"
		"in vec3 eyeDir_camspace;"

		"out vec3 color;"
		"in float dist;"

		"uniform vec3 " + LIGHT_COLOR + ";"
		"uniform float " + LIGHT_INTENSITY + ";"
		"uniform vec3 " + LIGHT_AMBIENT + ";"
		"uniform vec3 " + LIGHT_SPECULAR + ";"

		"void main(){"
			"vec3 n = normalize(normal_camspace);"
			"vec3 l = normalize(lightDir_camspace);"
			"vec3 E = normalize(eyeDir_camspace);"
			"vec3 R = reflect(-l, n);"

			"float cosTheta = clamp( dot( n,l ), 0,1 );"
			"float cosAlpha = clamp( dot( E,R ), 0,1 );"

			"color = " + LIGHT_AMBIENT + " + " + LIGHT_COLOR + " * " + LIGHT_INTENSITY + " * cosTheta / (dist*dist);"
			"color = color + " + LIGHT_SPECULAR + " * " + LIGHT_COLOR + " * " + LIGHT_INTENSITY + " * pow(cosAlpha,5) / (dist*dist);"
		"}";

const std::string colorlessV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;"

		"uniform mat4 " + MODEL_MATRIX + ";"
		"uniform mat4 " + VIEW_MATRIX + ";"
		"uniform mat4 " + PROJECTION_MATRIX + ";"

		"void main(){"
			"gl_Position = " + MVP + " * vec4(vertexPosition, 1);"
		"}";

const std::string colorlessF = "#version 330\n"
		"out vec3 color;"
		"void main(){"
		"color = vec3(0,0,0);"
		"}";

}

#endif /* BURNGINESHADERCODE_H_ */
