/*
 * RenderTexture.cpp
 *
 *  Created on: 22.09.2013
 *      Author: Dominik
 */

#include "RenderTexture.h"
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

#include <iostream>

namespace burn {

RenderTexture::RenderTexture() :
				_framebuffer(0),
				_texture(0),
				_depthbuffer(0),
				_isCreated(false),
				_width(0),
				_height(0) {
}

RenderTexture::~RenderTexture() {
	destroy();
}

bool RenderTexture::create(const unsigned int& width, const unsigned int& height) {
	destroy(); //if needed

	_width = width;
	_height = height;

	if(Window::isContextCreated() && !_isCreated){
		//Framebuffer:
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		//Texture:
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Depthbuffer:
		glGenRenderbuffers(1, &_depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

		//Configure:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		//Check:
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			std::cout << "RenderTexture: Failed to create framebuffer!\n";
			return false;
		}

		//Unbind all:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		_isCreated = true;
		return true;
	}
	return false;
}

void RenderTexture::destroy() {
	if(Window::isContextCreated() && _isCreated){
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteTextures(1, &_texture);
		glDeleteRenderbuffers(1, &_depthbuffer);

		_isCreated = false;
	}
}

void RenderTexture::bind() const {
	if(_isCreated && Window::isContextCreated()){
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glViewport(0, 0, _width, _height);
	}
}

void RenderTexture::clear() const {
	if(_isCreated && Window::isContextCreated()){
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glViewport(0, 0, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void RenderTexture::drawFullscreen() {

	if(_isCreated && Window::isContextCreated()){

		Mesh mesh;
		std::vector<Vertex> v;
		v.push_back(Vertex(Vector3f(-1.f, -1.f, 0), Vector3f(), Vector2f(0.f, 0.f)));
		v.push_back(Vertex(Vector3f(1.f, -1.f, 0), Vector3f(), Vector2f(1.f, 0.f)));
		v.push_back(Vertex(Vector3f(-1.f, 1.f, 0), Vector3f(), Vector2f(0.f, 1.f)));
		v.push_back(Vertex(Vector3f(-1.f, 1.f, 0), Vector3f(), Vector2f(0.f, 1.f)));
		v.push_back(Vertex(Vector3f(1.f, -1.f, 0), Vector3f(), Vector2f(1.f, 0.f)));
		v.push_back(Vertex(Vector3f(1.f, 1.f, 0), Vector3f(), Vector2f(1.f, 1.f)));
		mesh.setVertices(v);
		mesh.data();

		BurngineShaders::useShader(BurngineShaders::RAW_TEXTURE);

		glBindTexture(GL_TEXTURE_2D, _texture);

		//0 = Positions
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getPositionBuffer());
		glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

		//1 = UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getUvBuffer());
		glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

}

} /* namespace burn */
