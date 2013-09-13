/*
 * Shader.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include "Shader.h"
#include "Window.h"

#include <iostream>
#include <vector>

namespace burn {

Shader BurngineShaders::_solidColorShader;

bool BurngineShaders::loadAllShaders() {

	if(!_solidColorShader.loadFromString(solidColorV, solidColorF)){
		return false;
	}

	return true;
}

bool BurngineShaders::loadShader(const BurngineShaders::Type& type) {
	switch (type) {
		case SOLID_COLOR:
			return _solidColorShader.loadFromString(solidColorV, solidColorF);
			break;
		default:
			return false;
			break;
	}

	return false;
}

void BurngineShaders::useShader(const BurngineShaders::Type& type) {
	switch (type) {

		case SOLID_COLOR:
			_solidColorShader.activate();
			break;
		default:
			return;
			break;
	}

}

GLuint BurngineShaders::getShaderUniformLocation(const Type& type,
		const std::string& uniformName) {

	switch (type) {
		case SOLID_COLOR:
			return _solidColorShader.getUniformLocation(uniformName);
			break;
		default:
			return 0;
			break;
	}

	return 0;

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

void Shader::activate() const {
	if(Window::isContextCreated()){
		glUseProgram(_id);
	}
}

GLuint Shader::getUniformLocation(const std::string& uniformName) const {
	if(Window::isContextCreated() && _id != 0){
		return glGetUniformLocation(_id, uniformName.c_str());
	}
	return 0;
}

bool Shader::loadFromString(const std::string& vertexShader,
		const std::string& fragmentShader) {

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
		std::cout << "Compiling vertexshader...\n";
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
				&VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << "\n";

		// Compile Fragment Shader
		std::cout << "Compiling fragmentshader...\n";
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
				&FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << "\n";

		// Link the program
		std::cout << "Linking program...\n";
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
				&ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << "\n";

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		_id = ProgramID;

		return true;
	}

	return false;
}

} /* namespace burn */
