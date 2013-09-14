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
	/**
	 * @brief Default constructor.
	 * Default values:
	 * - ID: 0 (invalid ID)
	 */
	Shader();

	/**
	 * @brief Default destructor. Deletes shaderprogram
	 * from OpenGL if needed.
	 */
	~Shader();

	/**
	 * @brief Loads and creates shaderprograms by passing in a
	 * vertex- and fragmentshader.
	 *
	 * @param vertexShader The vertexshader.
	 * @param fragmentShader The fragmentshader.
	 *
	 * @return Returns true when shader was created successfully.
	 *
	 * @see activate()
	 */
	bool loadFromString(const std::string& vertexShader,
			const std::string& fragmentShader);

	/**
	 * @brief Activates the shader. It will be used for rendering until
	 * no other Shader had been called or this one had been destroyed.
	 *
	 * @note If no Shader was created by e.g. calling loadFromString(),
	 * this call will disable shaders at all.
	 *
	 * @see loadFromString()
	 */
	void activate() const;

	/**
	 * @brief Returns the position of a uniform inside the shaderprogram.
	 * This is useful to pass matrices for example.
	 * Internal usage mostly.
	 *
	 * @param uniformName The name of the uniform.
	 *
	 * @return Returns a value >= 0 on success or -1 when the uniform
	 * could not be found.
	 */
	GLuint getUniformLocation(const std::string& uniformName) const;

private:
	GLuint _id;

};

struct BURNGINE_API BurngineShaders {
	/**
	 * @brief Constructor is deleted, because this is a static struct
	 */
	BurngineShaders() = delete;

	/**
	 * @brief Enum for several predefined shaders
	 *
	 * @see loadShader()
	 * @see useShader()
	 */
	enum Type {
		SOLID_COLOR ///< Solid Color Shader
	};

	/**
	 * @brief Loads and creates a predefined shader.
	 *
	 * @param type The shader to load/create.
	 *
	 * @return Returns true on success.
	 *
	 * @see loadAllShaders()
	 * @see useShader()
	 * @see Type
	 */
	static bool loadShader(const Type& type);

	/**
	 * @brief Loads and creates all predefined shaders.
	 *
	 * @see loadShader()
	 */
	static bool loadAllShaders();

	/**
	 * @brief Activates a predefined shader.
	 *
	 * @param type The shader to activate.
	 *
	 * @note Ensure that the predefined shader has been loaded!
	 *
	 * @see loadShader()
	 * @see loadAllShaders()
	 */
	static void useShader(const Type& type);

	/**
	 * @brief Does the same like Shader::getUniformLocation() but on
	 * the predefined ones.
	 *
	 * @param type The shader in which the uniform will be searched
	 * @param uniformName The uniform name in the shaderprogram
	 *
	 * @return Returns the position of the uniform or -1 when failed
	 *
	 * @see Shader::getUniformLocation()
	 */
	static GLuint getShaderUniformLocation(const Type& type,
			const std::string& uniformName);

private:
	static Shader _solidColorShader;
};

} /* namespace burn */
#endif /* SHADER_H_ */