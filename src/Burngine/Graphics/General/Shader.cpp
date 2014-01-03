//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/General/OpenGL.h>

#include <iostream>
#include <vector>
#include <fstream>

namespace burn {

const std::hash<std::string> strToHash;

Shader BurngineShaders::_textureShader;
Shader BurngineShaders::_textureOneComponentShader;
Shader BurngineShaders::_pointlightShader;
Shader BurngineShaders::_spotlightShader;
Shader BurngineShaders::_dirlightShader;
Shader BurngineShaders::_singleColorShader;
Shader BurngineShaders::_fontShader;
Shader BurngineShaders::_depthShader;
Shader BurngineShaders::_skyBoxShader;
Shader BurngineShaders::_gBufferShader;
Shader BurngineShaders::_vsmDrawShader;
Shader BurngineShaders::_gaussianBlurHorizontalShader;
Shader BurngineShaders::_gaussianBlurVerticalShader;

GLuint Shader::_currentProgram = 0;

bool BurngineShaders::load(const std::string& d) {

	std::string dir = d;

	if(d[d.size() - 1] != '/' || d[d.size() - 1] != '\\')
		dir = d + "/";

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
	if(!_skyBoxShader.loadFromFile(dir + "skyBox.vert", dir + "skyBox.frag")){
		return false;
	}
	if(!_gBufferShader.loadFromFile(dir + "gBuffer.vert", dir + "gBuffer.frag")){
		return false;
	}
	if(!_vsmDrawShader.loadFromFile(dir + "vsmDraw.vert", dir + "vsmDraw.frag")){
		return false;
	}
	if(!_gaussianBlurHorizontalShader.loadFromFile(dir + "texture.vert", dir + "gaussianHorizontalBlur.frag")){
		return false;
	}
	if(!_gaussianBlurVerticalShader.loadFromFile(dir + "texture.vert", dir + "gaussianVerticalBlur.frag")){
		return false;
	}

	return true;
}

const Shader& BurngineShaders::getShader(const Type& type) {
	switch (type) {
		case TEXTURE:
			//Will be returned at end of function, so it always returns.
			//This makes the compiler happy :)
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
		case SKY_BOX:
			return _skyBoxShader;
			break;
		case G_BUFFER:
			return _gBufferShader;
			break;
		case VSM_DRAW:
			return _vsmDrawShader;
			break;
		case GAUSSIAN_BLUR_HORIZONTAL:
			return _gaussianBlurHorizontalShader;
			break;
		case GAUSSIAN_BLUR_VERTICAL:
			return _gaussianBlurVerticalShader;
			break;
	}
	//See case of SOLID_COLOR above
	return _textureShader;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

Shader::Shader() :
_id(0) {

}

Shader::~Shader() {
	if(_id != 0){
		ensureContext();
		glDeleteProgram(_id);
	}
}

void Shader::setUniform(const std::string& name,
						const Matrix4f& value) const {

	ensureContext();

	if(_id != _currentProgram){

		glUseProgram(_id);

		glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1,
		GL_FALSE,
							&value[0][0]);

		glUseProgram(_currentProgram);

	}else{

		glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1,
		GL_FALSE,
							&value[0][0]);

	}

}

void Shader::setUniform(const std::string& name,
						const Vector4f& value) const {

	ensureContext();

	if(_id != _currentProgram){

		glUseProgram(_id);

		glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &(value[0]));

		glUseProgram(_currentProgram);

	}else{

		glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &(value[0]));

	}

}

void Shader::setUniform(const std::string& name,
						const Vector3f& value) const {

	ensureContext();

	if(_id != _currentProgram){

		glUseProgram(_id);

		glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &(value[0]));

		glUseProgram(_currentProgram);

	}else{

		glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &(value[0]));

	}

}

void Shader::setUniform(const std::string& name,
						const Vector2f& value) const {

	ensureContext();

	if(_id != _currentProgram){

		glUseProgram(_id);

		glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &(value[0]));

		glUseProgram(_currentProgram);

	}else{

		glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &(value[0]));

	}

}

void Shader::setUniform(const std::string& name,
						const int& value) const {

	ensureContext();

	if(_id != _currentProgram){

		glUseProgram(_id);

		glUniform1iv(glGetUniformLocation(_id, name.c_str()), 1, &value);

		glUseProgram(_currentProgram);

	}else{

		glUniform1iv(glGetUniformLocation(_id, name.c_str()), 1, &value);

	}

}

void Shader::setUniform(const std::string& name,
						const float& value) const {

	ensureContext();

	if(_id != _currentProgram){

		glUseProgram(_id);

		glUniform1fv(glGetUniformLocation(_id, name.c_str()), 1, &value);

		glUseProgram(_currentProgram);

	}else{

		glUniform1fv(glGetUniformLocation(_id, name.c_str()), 1, &value);

	}

}

void Shader::activate() const {
	ensureContext();

	glUseProgram(_id);
	_currentProgram = _id;
}

GLuint Shader::getUniformLocation(const std::string& uniformName) const {
	if(_id != 0){
		ensureContext();
		return glGetUniformLocation(_id, uniformName.c_str());
	}
	return 0;
}

bool Shader::loadFromString(const std::string& vertexShader,
							const std::string& fragmentShader) {

	ensureContext();

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

bool Shader::loadFromFile(	const std::string& vertexShaderFile,
							const std::string& fragmentShaderFile) {
	ensureContext();

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

} /* namespace burn */
