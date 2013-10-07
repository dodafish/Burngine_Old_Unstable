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

"out vec3 lightPosition_camspace;"
"out vec3 normal_camspace;"
"out vec3 position_camspace;"
"out vec3 cameraPosition_camspace;"

"void main(){"
	"gl_Position = " + MVP + " * vec4(vertexPosition, 1);"

	"vec3 singleLight = " + LIGHT_POSITION + ";"
	"lightPosition_camspace = (" + VIEW_MATRIX + " * vec4(singleLight,1)).xyz;"

	"cameraPosition_camspace = (" + VIEW_MATRIX + " * vec4(" + CAMERA_POSITION + ",1)).xyz;"

	"normal_camspace = (" + NORMAL_MATRIX + "* vec4(vertexNormal, 0)).xyz;"
	"position_camspace = (" + VIEW_MATRIX + "*" + MODEL_MATRIX + "* vec4(vertexPosition, 1)).xyz;"
"}";

const std::string lightingF = "#version 330\n"
"in vec3 lightPosition_camspace;"
"in vec3 normal_camspace;"
"in vec3 position_camspace;"
"in vec3 cameraPosition_camspace;"

"layout(location = 0) out vec3 color;"

"uniform vec3 " + LIGHT_COLOR + ";"
"uniform float " + LIGHT_INTENSITY + ";"
"uniform vec3 " + LIGHT_AMBIENT + ";"
"uniform vec3 " + LIGHT_SPECULAR + ";"

"uniform int " + LIGHT_ENABLED + ";"
"uniform int " + LIGHT_TYPE + ";"

"void main(){"
	"if(" + LIGHT_ENABLED + " == 1){"
		"float dist = distance(lightPosition_camspace, position_camspace);"
		"vec3 n = normalize(normal_camspace);"
		"vec3 l = normalize(lightPosition_camspace - position_camspace);"

		"if(" + LIGHT_TYPE + " == 1){"
			"float cosTheta = clamp( dot( n,l ), 0,1 );"

			"color = " + LIGHT_AMBIENT + " + " + LIGHT_COLOR + " * " + LIGHT_INTENSITY + " * cosTheta / (dist*dist);"
		"}else{"
			"vec3 E = normalize(cameraPosition_camspace - position_camspace);"
			"vec3 R = reflect(-l, n);"
			"float cosAlpha = clamp( dot( E,R ), 0,1 );"

			"color = " + LIGHT_SPECULAR + " * " + LIGHT_COLOR + " * " + LIGHT_INTENSITY
			+ " * pow(cosAlpha,5) / (dist*dist);"
		"}"
	"}"
	"else{"
		"if(" + LIGHT_TYPE + " == 1){"
			"color = vec3(1.0);"
		"}else{"
			"color = vec3(0.0);"
		"}"
	"}"
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
"layout(location = 0) out vec3 color0;"
"layout(location = 1) out vec3 color1;"
"void main(){"
	"color0 = vec3(0,0,0);"
	"color1 = vec3(0,0,0);"
"}";

}

#endif /* BURNGINESHADERCODE_H_ */
