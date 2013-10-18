/*
 * Shader.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/BurngineShaderCode.h>
#include <Burngine/System/Reporter.h>

#include <iostream>
#include <vector>

namespace burn {

Shader BurngineShaders::_solidColorShader;
Shader BurngineShaders::_texturedShader;
Shader BurngineShaders::_rawTextureShader;
Shader BurngineShaders::_lightingShader;
Shader BurngineShaders::_colorlessShader;
Shader BurngineShaders::_fontShader;
Shader BurngineShaders::_orthoColoredShader;

bool BurngineShaders::loadAllShaders() {

	if(!_solidColorShader.loadFromString(solidColorV, solidColorF)){
		return false;
	}
	if(!_texturedShader.loadFromString(texturedV, texturedF)){
		return false;
	}
	if(!_rawTextureShader.loadFromString(rawTextureV, rawTextureF)){
		return false;
	}
	if(!_lightingShader.loadFromString(lightingV, lightingF)){
		return false;
	}
	if(!_colorlessShader.loadFromString(colorlessV, colorlessF)){
		return false;
	}
	if(!_fontShader.loadFromString(fontV, fontF)){
		return false;
	}
	if(!_orthoColoredShader.loadFromString(orthoColoredV, orthoColoredF)){
		return false;
	}

	return true;
}

const Shader& BurngineShaders::getShader(const Type& type) {
	switch (type) {
		case SOLID_COLOR:
			//Will be returned at end of function, so it always returns.
			//This makes the compiler happy :)
			break;
		case TEXTURED:
			return _texturedShader;
			break;
		case RAW_TEXTURE:
			return _rawTextureShader;
			break;
		case LIGHTING:
			return _lightingShader;
			break;
		case COLORLESS:
			return _colorlessShader;
			break;
		case FONT:
			return _fontShader;
			break;
		case ORTHO_COLORED:
			return _orthoColoredShader;
			break;
	}
	//See case of SOLID_COLOR above
	return _solidColorShader;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

Shader::Shader() :
_id(0) {

}

Shader::~Shader() {
	if(_id != 0 && Window::isContextCreated()){
		glDeleteProgram(_id);
	}
}

void Shader::setUniform(const std::string& name, const Matrix4f& value) const {
	//Scan and set when found
	for(size_t i = 0; i < _matrix4fUniforms.size(); ++i){
		if(_matrix4fUniforms[i].first == name){
			_matrix4fUniforms[i].second = value;
			return;
		}
	}
	//Not found. Create a pair
	_matrix4fUniforms.push_back(std::pair<std::string, Matrix4f>(name, value));
}

void Shader::setUniform(const std::string& name, const Vector4f& value) const {
	//Scan and set when found
	for(size_t i = 0; i < _vector4fUniforms.size(); ++i){
		if(_vector4fUniforms[i].first == name){
			_vector4fUniforms[i].second = value;
			return;
		}
	}
	//Not found. Create a pair
	_vector4fUniforms.push_back(std::pair<std::string, Vector4f>(name, value));
}

void Shader::setUniform(const std::string& name, const Vector3f& value) const {
	//Scan and set when found
	for(size_t i = 0; i < _vector3fUniforms.size(); ++i){
		if(_vector3fUniforms[i].first == name){
			_vector3fUniforms[i].second = value;
			return;
		}
	}
	//Not found. Create a pair
	_vector3fUniforms.push_back(std::pair<std::string, Vector3f>(name, value));
}

void Shader::setUniform(const std::string& name, const Vector2f& value) const {
	//Scan and set when found
	for(size_t i = 0; i < _vector2fUniforms.size(); ++i){
		if(_vector2fUniforms[i].first == name){
			_vector2fUniforms[i].second = value;
			return;
		}
	}
	//Not found. Create a pair
	_vector2fUniforms.push_back(std::pair<std::string, Vector2f>(name, value));
}

void Shader::setUniform(const std::string& name, const int& value) const {
	//Scan and set when found
	for(size_t i = 0; i < _intUniforms.size(); ++i){
		if(_intUniforms[i].first == name){
			_intUniforms[i].second = value;
			return;
		}
	}
	//Not found. Create a pair
	_intUniforms.push_back(std::pair<std::string, int>(name, value));
}

void Shader::setUniform(const std::string& name, const float& value) const {
	//Scan and set when found
	for(size_t i = 0; i < _floatUniforms.size(); ++i){
		if(_floatUniforms[i].first == name){
			_floatUniforms[i].second = value;
			return;
		}
	}
	//Not found. Create a pair
	_floatUniforms.push_back(std::pair<std::string, float>(name, value));
}

void Shader::activate() const {
	if(!Window::isContextCreated())
		return;

	glUseProgram(_id);
	uploadUniforms();
}

void Shader::uploadUniforms() const {
	for(size_t i = 0; i < _matrix4fUniforms.size(); ++i){
		glUniformMatrix4fv(glGetUniformLocation(_id, _matrix4fUniforms[i].first.c_str()), 1,
		GL_FALSE,
							&(_matrix4fUniforms[i].second[0][0]));
	}
	for(size_t i = 0; i < _vector4fUniforms.size(); ++i){
		glUniform4fv(glGetUniformLocation(_id, _vector4fUniforms[i].first.c_str()), 1,

		&(_vector4fUniforms[i].second[0]));
	}
	for(size_t i = 0; i < _vector3fUniforms.size(); ++i){
		glUniform3fv(glGetUniformLocation(_id, _vector3fUniforms[i].first.c_str()), 1,

		&(_vector3fUniforms[i].second[0]));
	}
	for(size_t i = 0; i < _vector2fUniforms.size(); ++i){
		glUniform2fv(glGetUniformLocation(_id, _vector2fUniforms[i].first.c_str()), 1,

		&(_vector2fUniforms[i].second[0]));
	}
	for(size_t i = 0; i < _intUniforms.size(); ++i){
		glUniform1i(glGetUniformLocation(_id, _intUniforms[i].first.c_str()), (_intUniforms[i].second));
	}
	for(size_t i = 0; i < _floatUniforms.size(); ++i){
		glUniform1f(glGetUniformLocation(_id, _floatUniforms[i].first.c_str()), (_floatUniforms[i].second));
	}

	//Testcode: delete after pass
	Vector3f test;
	test.r = 0.45f;
	test.y = 0.25252f;
	if(test[0] != test.r || test[1] != test.y){
		std::cout << "FLAME";
	}
}

GLuint Shader::getUniformLocation(const std::string& uniformName) const {
	if(Window::isContextCreated() && _id != 0){
		return glGetUniformLocation(_id, uniformName.c_str());
	}
	return 0;
}

bool Shader::loadFromString(const std::string& vertexShader, const std::string& fragmentShader) {

	if(Window::isContextCreated()){

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode = vertexShader;

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode = fragmentShader;

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		Reporter::report("Compiling vertexshader...");
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		if(!Result){
			Reporter::report(&VertexShaderErrorMessage[0], Reporter::ERROR);
			return false;
		}

		// Compile Fragment Shader
		Reporter::report("Compiling fragmentshader...");
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		if(!Result){
			Reporter::report(&FragmentShaderErrorMessage[0], Reporter::ERROR);
			return false;
		}

		// Link the program
		Reporter::report("Linking program...");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		if(!Result){
			Reporter::report(&ProgramErrorMessage[0], Reporter::ERROR);
			return false;
		}

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		_id = ProgramID;

		return true;
	}

	Reporter::report("Cannot load Shader! (No valid OpenGL-Context)", Reporter::ERROR);
	return false;
}

} /* namespace burn */
