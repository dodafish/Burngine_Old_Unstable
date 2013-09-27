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
				_texture0(0),
				_texture1(0),
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

		GLint lastFB = 0;
		GLint lastRB = 0;
		GLint lastTex = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &lastRB);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

		//Framebuffer:
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		//Textures:
		glGenTextures(1, &_texture0);
		glBindTexture(GL_TEXTURE_2D, _texture0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenTextures(1, &_texture1);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Depthbuffer:
		glGenRenderbuffers(1, &_depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

		//Configure:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, _texture1, 0);
		GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, DrawBuffers);

		//Check:
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			std::cout << "RenderTexture: Failed to create framebuffer!\n";
			return false;
		}

		//Unbind all:
		glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
		glBindTexture(GL_TEXTURE_2D, lastTex);
		glBindRenderbuffer(GL_RENDERBUFFER, lastRB);

		_isCreated = true;
		return true;
	}
	return false;
}

void RenderTexture::destroy() {
	if(Window::isContextCreated() && _isCreated){
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteTextures(1, &_texture0);
		glDeleteTextures(1, &_texture1);
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
		glBindTexture(GL_TEXTURE_2D, _texture0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		GLint lastFB = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	}
}

void RenderTexture::drawFullscreen(RenderTexture::TextureUnit tu) {

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
		mesh.update();

		BurngineShaders::useShader(BurngineShaders::RAW_TEXTURE);

		GLint lastTex = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

		if(tu == TEXTURE0)
			glBindTexture(GL_TEXTURE_2D, _texture0);
		else if(tu == TEXTURE1)
			glBindTexture(GL_TEXTURE_2D, _texture1);

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

		glBindTexture(GL_TEXTURE_2D, lastTex);

	}

}

void RenderTexture::draw(RenderTexture::TextureUnit tu, const Vector2f& p, const Vector2f& s) {

	if(_isCreated && Window::isContextCreated()){

		Mesh mesh;
		std::vector<Vertex> v;
		v.push_back(Vertex(Vector3f(p.x, p.y - s.y, 0), Vector3f(), Vector2f(0.f, 0.f)));
		v.push_back(Vertex(Vector3f(p.x + s.x, p.y - s.y, 0), Vector3f(), Vector2f(1.f, 0.f)));
		v.push_back(Vertex(Vector3f(p.x, p.y, 0), Vector3f(), Vector2f(0.f, 1.f)));
		v.push_back(Vertex(Vector3f(p.x, p.y, 0), Vector3f(), Vector2f(0.f, 1.f)));
		v.push_back(Vertex(Vector3f(p.x + s.x, p.y - s.y, 0), Vector3f(), Vector2f(1.f, 0.f)));
		v.push_back(Vertex(Vector3f(p.x + s.x, p.y, 0), Vector3f(), Vector2f(1.f, 1.f)));
		mesh.setVertices(v);
		mesh.update();

		BurngineShaders::useShader(BurngineShaders::RAW_TEXTURE);

		GLint lastTex = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

		if(tu == TEXTURE0)
			glBindTexture(GL_TEXTURE_2D, _texture0);
		else if(tu == TEXTURE1)
			glBindTexture(GL_TEXTURE_2D, _texture1);

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

		glBindTexture(GL_TEXTURE_2D, lastTex);

	}

}

} /* namespace burn */
