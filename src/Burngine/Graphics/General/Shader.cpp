/*
 * Shader.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>

#include <iostream>
#include <vector>
#include <fstream>

namespace burn {

Shader BurngineShaders::_colorShader;
Shader BurngineShaders::_textureShader;
Shader BurngineShaders::_textureOneComponentShader;
Shader BurngineShaders::_pointlightShader;
Shader BurngineShaders::_spotlightShader;
Shader BurngineShaders::_dirlightShader;
Shader BurngineShaders::_singleColorShader;
Shader BurngineShaders::_fontShader;
Shader BurngineShaders::_depthShader;
Shader BurngineShaders::_skyBoxShader;
Shader BurngineShaders::_depthPointlight;
Shader BurngineShaders::_gBufferShader;

bool BurngineShaders::load(const std::string& d) {

	std::string dir = d;

	if(d[d.size() - 1] != '/' || d[d.size() - 1] != '\\')
		dir = d + "/";

	if(!_colorShader.loadFromFile(dir + "color.vert", dir + "color.frag")){
		return false;
	}
	if(!_textureShader.loadFromFile(dir + "texture.vert", dir + "texture.frag")){
		return false;
	}
	if(!_textureOneComponentShader.loadFromFile(dir + "texture.vert", dir + "textureOneComponent.frag")){
		return false;
	}
	if(!_pointlightShader.loadFromFile(dir + "pointlight.vert", dir + "pointlight.frag")){
		return false;
	}
	if(!_spotlightShader.loadFromFile(dir + "spotlight.vert", dir + "spotlight.frag")){
		return false;
	}
	if(!_dirlightShader.loadFromFile(dir + "dirlight.vert", dir + "dirlight.frag")){
		return false;
	}
	if(!_singleColorShader.loadFromFile(dir + "singleColor.vert", dir + "singleColor.frag")){
		return false;
	}
	if(!_fontShader.loadFromFile(dir + "texture.vert", dir + "font.frag")){
		return false;
	}
	if(!_depthShader.loadFromFile(dir + "depth.vert", dir + "depth.frag")){
		return false;
	}
	if(!_depthPointlight.loadFromFile(dir + "pointlightDepth.vert", dir + "pointlightDepth.frag")){
		return false;
	}
	if(!_skyBoxShader.loadFromFile(dir + "skyBox.vert", dir + "skyBox.frag")){
		return false;
	}
	if(!_gBufferShader.loadFromFile(dir + "gBuffer.vert", dir + "gBuffer.frag")){
		return false;
	}

	return true;
}

const Shader& BurngineShaders::getShader(const Type& type) {
	switch (type) {
		case COLOR:
			//Will be returned at end of function, so it always returns.
			//This makes the compiler happy :)
			break;
		case TEXTURE:
			return _textureShader;
			break;
		case TEXTURE_ONE_COMPONENT:
			return _textureOneComponentShader;
			break;
		case POINTLIGHT:
			return _pointlightShader;
			break;
		case SPOTLIGHT:
			return _spotlightShader;
			break;
		case DIRECTIONAL_LIGHT:
			return _dirlightShader;
			break;
		case SINGLECOLOR:
			return _singleColorShader;
			break;
		case FONT:
			return _fontShader;
			break;
		case DEPTH:
			return _depthShader;
			break;
		case DEPTH_POINTLIGHT:
			return _depthPointlight;
			break;
		case SKY_BOX:
			return _skyBoxShader;
			break;
		case G_BUFFER:
			return _gBufferShader;
			break;
	}
	//See case of SOLID_COLOR above
	return _colorShader;
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

bool Shader::loadFromFile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
	if(Window::isContextCreated()){

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertexShaderFile, std::ios::in);
		if(VertexShaderStream.is_open()){
			std::string Line = "";
			while(getline(VertexShaderStream, Line))
				VertexShaderCode += Line + "\n";
			VertexShaderStream.close();
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragmentShaderFile, std::ios::in);
		if(FragmentShaderStream.is_open()){
			std::string Line = "";
			while(getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}

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
