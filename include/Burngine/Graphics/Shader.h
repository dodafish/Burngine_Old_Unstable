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

const std::string MODEL_MATRIX = "M_";
const std::string VIEW_MATRIX = "V_";
const std::string PROJECTION_MATRIX = "P_";
const std::string MVP = "(" + PROJECTION_MATRIX + "*" + VIEW_MATRIX + "*" + MODEL_MATRIX + ")";

const std::string CAMERA_POSITION = "CAM_";

const std::string LIGHT_COUNT = "LIGHT_COUNT_";

//----------------------------------------------------------------
const std::string solidColorV = "#version 330\n"
		"layout(location = 0) in vec3 vertexPosition;"
		"layout(location = 1) in vec3 vertexColor;"
		"layout(location = 2) in vec3 vertexNormal;"

		"out vec3 fragmentColor;"
		"out vec3 normal;"
		"out vec3 eyeDir_camspace;"
		"flat out int lightCount;"
		"out mat4 viewMat;"

		"uniform mat4 " + MODEL_MATRIX + ";"
		"uniform mat4 " + VIEW_MATRIX + ";"
		"uniform mat4 " + PROJECTION_MATRIX + ";"
		"uniform vec3 " + CAMERA_POSITION + ";"
		"uniform int " + LIGHT_COUNT + ";"

		"void main(){"
		"viewMat = " + VIEW_MATRIX + ";"
		"lightCount = " + LIGHT_COUNT + ";"
		"gl_Position = " + MVP + " * vec4(vertexPosition, 1.0);"

		"vec3 vertexPosition_camspace = (" + VIEW_MATRIX + "*" + MODEL_MATRIX + "*vec4(vertexPosition, 1)).xyz;"

		"eyeDir_camspace = (" + VIEW_MATRIX + "*" + MODEL_MATRIX + "* vec4(" + CAMERA_POSITION
		+ ",1.0)).xyz - vertexPosition_camspace;"

				"fragmentColor = vertexColor;"
				"normal = (" + VIEW_MATRIX + "*" + MODEL_MATRIX + "*vec4(vertexNormal, 0)).xyz;"
				"}";

const std::string solidColorF = "#version 330\n"
		"in vec3 fragmentColor;"
		"in vec3 normal;"
		"flat in int lightCount;"
		"in vec3 eyeDir_camspace;"
		"in mat4 viewMat;"

		"uniform samplerBuffer lightPositions;"

		"out vec3 color;"

		"void main(){"
		"vec3 n = normalize(normal);"

		"color = vec3(0.0,0.0,0.0);"

		"for(int i = 0; i < lightCount; i++){"

		"vec3 lightPos;"
		"lightPos.x = texelFetch(lightPositions, i*3).r;"
		"lightPos.y = texelFetch(lightPositions, i*3+1).r;"
		"lightPos.z = texelFetch(lightPositions, i*3+2).r;"
		"lightPos = (viewMat * vec4(lightPos, 1.0)).xyz;"

		"vec3 l = normalize(lightPos + eyeDir_camspace);"
		"float cosTheta = clamp( dot( n,l ), 0,1 );"
		"color = color + (fragmentColor * cosTheta);"

		"}"
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
	bool loadFromString(const std::string& vertexShader, const std::string& fragmentShader);

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
		SOLID_COLOR, ///< Solid Color Shader
		TEXTURED ///< Simple 1-Texture Shader
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
	static GLuint getShaderUniformLocation(const Type& type, const std::string& uniformName);

private:
	static Shader _solidColorShader;
	static Shader _texturedShader;
};

} /* namespace burn */
#endif /* SHADER_H_ */
