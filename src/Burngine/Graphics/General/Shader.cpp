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

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Log.h>
#include <Burngine/Graphics/General/OpenGL.h>

#include <iostream>
#include <vector>
#include <fstream>

namespace burn {

const std::hash<std::string> strToHash;

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

////////////////////////////////////////////////////////////////////////////////
void Shader::activate() const {
	ensureContext();

	glUseProgram(_id);
	_currentProgram = _id;
}

GLint Shader::getUniformLocation(const std::string& uniformName) const {
	if(_id != 0){
		ensureContext();
		return glGetUniformLocation(_id, uniformName.c_str());
	}
	return -1;
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
	Log::log("Compiling vertexshader...");
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	if(!Result){
		Log::log(&VertexShaderErrorMessage[0], Log::ERROR);
		return false;
	}

// Compile Fragment Shader
	Log::log("Compiling fragmentshader...");
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	if(!Result){
		Log::log(&FragmentShaderErrorMessage[0], Log::ERROR);
		return false;
	}

// Link the program
	Log::log("Linking program...");
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
		Log::log(&ProgramErrorMessage[0], Log::ERROR);
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
	Log::log("Compiling vertexshader...");
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	if(!Result){
		Log::log("Failed to load/compile shader: " + vertexShaderFile, Log::ERROR);
		Log::log(&VertexShaderErrorMessage[0], Log::ERROR);
		return false;
	}

// Compile Fragment Shader
	Log::log("Compiling fragmentshader...");
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	if(!Result){
		Log::log("Failed to load/compile shader: " + fragmentShaderFile, Log::ERROR);
		Log::log(&FragmentShaderErrorMessage[0], Log::ERROR);
		return false;
	}

// Link the program
	Log::log("Linking program...");
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
		Log::log("Failed to link shaders: " + vertexShaderFile + " & " + fragmentShaderFile, Log::ERROR);
		Log::log(&ProgramErrorMessage[0], Log::ERROR);
		return false;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	_id = ProgramID;

	return true;

}

} /* namespace burn */
