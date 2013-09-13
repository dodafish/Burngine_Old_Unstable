/*
 * Shader.h
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#ifndef SHADER_H_
#define SHADER_H_

#include "../Export.h"
#include "OpenGL.h"
#include <string>

namespace burn {

//----------------------------------------------------------------
const std::string solidColorV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;"
		"layout(location = 1) in vec3 vertexColor;"
		"out vec3 color;"
		"void main(){"
		"color = vertexColor;"
		"gl_Position = vec4(vertexPosition, 1.0);"
		"}";
const std::string solidColorF = "#version 330\n"
		"in vec3 color;"
		"out vec3 fragmentColor;"
		"void main(){"
		"fragmentColor = color;"
		"}";
//----------------------------------------------------------------

class BURNGINE_API Shader {
public:
	Shader();
	~Shader();

	enum Type {
		SOLID_COLOR
	};

	bool loadShader(const Type& type);

	bool loadShaderFromString(const std::string& vertexShader,
			const std::string& fragmentShader);

	void activate() const;

private:
	GLuint _id;

};

} /* namespace burn */
#endif /* SHADER_H_ */
