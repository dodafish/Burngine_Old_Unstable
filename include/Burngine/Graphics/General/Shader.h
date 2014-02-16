//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef SHADER_H_
#define SHADER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <string>
#include <Burngine/System/Math.h>

namespace burn {

//----------------------------------------------------------------
// see BurngineShaderCode.h
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
	 * @brief Loads shader from file
	 *
	 * @param vertexShaderFile File of vertex shader
	 * @param fragmentShaderFile File of fragment shader
	 *
	 * @note BurngineShaders are loaded with this function
	 */
	bool loadFromFile(	const std::string& vertexShaderFile,
						const std::string& fragmentShaderFile);

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
	 * could not be found or the uniform's name is invalid.
	 */
	GLint getUniformLocation(const std::string& uniformName) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param name Variable name in the shader
	 * @param value Value to set to the variable
	 */
	void setUniform(const std::string& name,
					const Matrix4f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param name Variable name in the shader
	 * @param value Value to set to the variable
	 */
	void setUniform(const std::string& name,
					const Vector4f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param name Variable name in the shader
	 * @param value Value to set to the variable
	 */
	void setUniform(const std::string& name,
					const Vector3f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param name Variable name in the shader
	 * @param value Value to set to the variable
	 */
	void setUniform(const std::string& name,
					const Vector2f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param name Variable name in the shader
	 * @param value Value to set to the variable
	 */
	void setUniform(const std::string& name,
					const int& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param name Variable name in the shader
	 * @param value Value to set to the variable
	 */
	void setUniform(const std::string& name,
					const float& value) const;

	////////////////////////////////////////////////////////////

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param uniformLocation Location of the uniform
	 * @param value Value to set to the variable
	 *
	 * @see getUniformLocation()
	 */
	void setUniform(const GLint& uniformLocation,
					const Matrix4f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param uniformLocation Location of the uniform
	 * @param value Value to set to the variable
	 *
	 * @see getUniformLocation()
	 */
	void setUniform(const GLint& uniformLocation,
					const Vector4f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param uniformLocation Location of the uniform
	 * @param value Value to set to the variable
	 *
	 * @see getUniformLocation()
	 */
	void setUniform(const GLint& uniformLocation,
					const Vector3f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param uniformLocation Location of the uniform
	 * @param value Value to set to the variable
	 *
	 * @see getUniformLocation()
	 */
	void setUniform(const GLint& uniformLocation,
					const Vector2f& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param uniformLocation Location of the uniform
	 * @param value Value to set to the variable
	 *
	 * @see getUniformLocation()
	 */
	void setUniform(const GLint& uniformLocation,
					const int& value) const;

	/**
	 * @brief Sets a variable in the shader program
	 *
	 * @param uniformLocation Location of the uniform
	 * @param value Value to set to the variable
	 *
	 * @see getUniformLocation()
	 */
	void setUniform(const GLint& uniformLocation,
					const float& value) const;

private:
	GLuint _id;
	static GLuint _currentProgram;
};

} /* namespace burn */
#endif /* SHADER_H_ */
