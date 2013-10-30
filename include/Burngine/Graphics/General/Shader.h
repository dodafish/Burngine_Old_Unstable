/*
 * Shader.h
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <string>
#include <Burngine/System/Math.h>
#include <vector>
#include <utility>

//Vectors that store set uniforms
template class BURNGINE_API std::vector<std::pair<std::string, burn::Matrix4f> >;
template class BURNGINE_API std::vector<std::pair<std::string, burn::Vector4f> >;
template class BURNGINE_API std::vector<std::pair<std::string, burn::Vector3f> >;
template class BURNGINE_API std::vector<std::pair<std::string, burn::Vector2f> >;
template class BURNGINE_API std::vector<std::pair<std::string, int> >;
template class BURNGINE_API std::vector<std::pair<std::string, float> >;

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
	bool loadFromString(const std::string& vertexShader, const std::string& fragmentShader);

	bool loadFromFile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

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

	void setUniform(const std::string& name, const Matrix4f& value) const;
	void setUniform(const std::string& name, const Vector4f& value) const;
	void setUniform(const std::string& name, const Vector3f& value) const;
	void setUniform(const std::string& name, const Vector2f& value) const;
	void setUniform(const std::string& name, const int& value) const;
	void setUniform(const std::string& name, const float& value) const;

private:
	void uploadUniforms() const;
	GLuint _id;

	mutable std::vector<std::pair<std::string, Matrix4f> > _matrix4fUniforms;
	mutable std::vector<std::pair<std::string, Vector4f> > _vector4fUniforms;
	mutable std::vector<std::pair<std::string, Vector3f> > _vector3fUniforms;
	mutable std::vector<std::pair<std::string, Vector2f> > _vector2fUniforms;
	mutable std::vector<std::pair<std::string, int> > _intUniforms;
	mutable std::vector<std::pair<std::string, float> > _floatUniforms;
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
		COLOR, ///< Color Shader. Material colors eg.
		TEXTURE, ///< Simple 1-Texture Shader
		POINTLIGHT, ///< Renders a pointlight
		SPOTLIGHT,
		DIRECTIONAL_LIGHT,
		SINGLECOLOR, ///< Draws with only one color (internal use for depthbufferwriting)
		FONT,
		DEPTH,
		DEPTH_POINTLIGHT,
		SKY_BOX
	};

	/**
	 * @brief Loads and creates all predefined shaders.
	 *
	 * @see loadShader()
	 */
	static bool load(const std::string& directory = "/");

	static const Shader& getShader(const Type& type);

private:
	static Shader _colorShader;
	static Shader _textureShader;
	static Shader _pointlightShader;
	static Shader _spotlightShader;
	static Shader _dirlightShader;
	static Shader _singleColorShader;
	static Shader _fontShader;
	static Shader _depthShader;
	static Shader _depthPointlight;
	static Shader _skyBoxShader;
};

} /* namespace burn */
#endif /* SHADER_H_ */
