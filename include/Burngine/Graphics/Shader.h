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

const std::string MVP_UNIFORM = "MVP";

//----------------------------------------------------------------
const std::string solidColorV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;\n"
		"layout(location = 1) in vec3 vertexColor;\n"
		"out vec3 color;\n"
		"uniform mat4 " + MVP_UNIFORM + ";\n"

		"void main(){\n"
		"color = vertexColor;\n"
		"vec4 v = vec4(vertexPosition, 1.0);\n"
		"gl_Position = " + MVP_UNIFORM + " * v;\n"
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

	bool loadFromString(const std::string& vertexShader,
			const std::string& fragmentShader);

	void activate() const;

	GLuint getUniformLocation(const std::string& uniformName) const;

private:
	GLuint _id;

};

struct BURNGINE_API BurngineShaders {
	BurngineShaders() = delete;

	enum Type {
		SOLID_COLOR
	};

	static bool loadShader(const Type& type);
	static bool loadAllShaders();

	static void useShader(const Type& type);

	static GLuint getShaderUniformLocation(const Type& type,
			const std::string& uniformName);

private:
	static Shader _solidColorShader;
};

} /* namespace burn */
#endif /* SHADER_H_ */
