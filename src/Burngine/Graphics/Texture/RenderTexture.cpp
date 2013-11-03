/*
 * RenderTexture.cpp
 *
 *  Created on: 22.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Texture/RenderTexture.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/System/Reporter.h>

#include <iostream>

namespace burn {

RenderTexture::RenderTexture() :
_framebuffer(0),
_depthbuffer(0),
_texture(0),
_isCreated(false) {
}

RenderTexture::~RenderTexture() {

	if(Window::isContextCreated()){
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteRenderbuffers(1, &_depthbuffer);
	}

}

bool RenderTexture::isCreated() const {
	return _isCreated;
}

void RenderTexture::onBind(const unsigned int& unit) const {

	if(!isCreated()){
		Reporter::report("Unable to bind rendertextue. Rendertexture not created!", Reporter::ERROR);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, _texture);
}

void RenderTexture::onUnbind(const unsigned int& unit) const {

	glBindTexture(GL_TEXTURE_2D, 0);

}

void RenderTexture::cleanup() {

	if(!isCreated())
		return;

	if(!Window::isContextCreated()){
		Reporter::report("Unable to cleanup rendertexture! No valid context.", Reporter::ERROR);
		return;
	}

	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_framebuffer);
	glDeleteRenderbuffers(1, &_depthbuffer);

	_isCreated = false;
	_dimensions = Vector2ui(0);

}

bool RenderTexture::create(const Vector2ui& dimensions) {

	if(!Window::isContextCreated())
		return false;

	cleanup();

	_dimensions = dimensions;

	GLint lastFB = 0;
	GLint lastRB = 0;
	GLint lastTex = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &lastRB);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

	//Framebuffer:
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	//Texture:
	glGenTextures(1, &_texture);
	//Clear texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	//Depthbuffer:
	glGenRenderbuffers(1, &_depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _dimensions.x, _dimensions.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

	//Configure:
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);
	GLenum DrawBuffers[1] = {
	GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	//Check:
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("RenderTexture: Failed to create framebuffer!\n", Reporter::ERROR);
		return false;
	}

	//Restore previous bindings
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_2D, lastTex);
	glBindRenderbuffer(GL_RENDERBUFFER, lastRB);

	_isCreated = true;

	return true;
}

void RenderTexture::bindAsTarget() const {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated()){
		Reporter::report("Unable to bind rendertexture. No valid context!", Reporter::ERROR);
		return;
	}

	if(!isCreated()){
		Reporter::report("Binding uncreated rendertexture disallowed!", Reporter::WARNING);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glViewport(0, 0, _dimensions.x, _dimensions.y);

}

void RenderTexture::clear() const {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to clear rendertexture. No valid context!", Reporter::ERROR);
		return;
	}

	if(!isCreated()){
		Reporter::report("Unable to clear rendertexture. Rendertexture not created!", Reporter::WARNING);
		return;
	}

	//Get previous bindings
	GLint previousTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
	GLint lastFB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);

	//Clear texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	//Clear buffers
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Restore previous bindings
	glBindTexture(GL_TEXTURE_2D, previousTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);

}

void RenderTexture::drawFullscreen() {

	if(!Window::isContextCreated() || !isCreated())
		return;

	Mesh mesh;
	std::vector<Vertex> v;
	v.push_back(Vertex(Vector3f(-1.f, -1.f, 0), Vector3f(), (Vector2f(0.f, 0.f))));
	v.push_back(Vertex(Vector3f(1.f, -1.f, 0), Vector3f(), (Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(-1.f, 1.f, 0), Vector3f(), (Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(-1.f, 1.f, 0), Vector3f(), (Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(1.f, -1.f, 0), Vector3f(), (Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(1.f, 1.f, 0), Vector3f(), (Vector2f(1.f, 1.f))));
	mesh.setVertices(v);
	mesh.update();

	//Get shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);

	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	//Get previous binding
	GLint previousTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);

	//Bind texture only
	bindAsSource();

	//0 = Positions
	glEnableVertexAttribArray(0);
	mesh.getPositionVbo().bind();
	glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);

	//1 = UVs
	glEnableVertexAttribArray(1);
	mesh.getUvVbo().bind();
	glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
	2,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);

	OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindTexture(GL_TEXTURE_2D, previousTexture);

}

void RenderTexture::draw(const Vector2f& p, const Vector2f& s) {

	if(!Window::isContextCreated() || !isCreated())
		return;

	Mesh mesh;
	std::vector<Vertex> v;

	v.push_back(Vertex(Vector3f(p.x, p.y - s.y, 0), Vector3f(), (Vector2f(0.f, 0.f))));
	v.push_back(Vertex(Vector3f(p.x + s.x, p.y - s.y, 0), Vector3f(), (Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(p.x, p.y, 0), Vector3f(), (Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(p.x, p.y, 0), Vector3f(), (Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(p.x + s.x, p.y - s.y, 0), Vector3f(), (Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(p.x + s.x, p.y, 0), Vector3f(), (Vector2f(1.f, 1.f))));

	mesh.setVertices(v);
	mesh.update();

	//Get shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);

	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	//Get previous binding
	GLint previousTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);

	//Bind texture only
	bindAsSource();

	//0 = Positions
	glEnableVertexAttribArray(0);
	mesh.getPositionVbo().bind();
	glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);

	//1 = UVs
	glEnableVertexAttribArray(1);
	mesh.getUvVbo().bind();
	glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
	2,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);

	OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindTexture(GL_TEXTURE_2D, previousTexture);

}

} /* namespace burn */
