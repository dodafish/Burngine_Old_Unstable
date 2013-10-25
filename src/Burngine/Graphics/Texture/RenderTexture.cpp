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

#include <iostream>

namespace burn {

RenderTexture::RenderTexture() :
_framebuffer(0),
_depthbuffer(0) {
}

RenderTexture::~RenderTexture() {

	if(Window::isContextCreated()){
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteRenderbuffers(1, &_depthbuffer);
	}

}

bool RenderTexture::create(const Vector2ui& dimensions) {

	if(!Window::isContextCreated())
		return false;

	//Generate texture and sampler. Does cleanup before if needed
	generate(dimensions);

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
	clear(); //Make it black

	//Depthbuffer:
	glGenRenderbuffers(1, &_depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, getDimensions().x, getDimensions().y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

	//Configure:
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);
	GLenum DrawBuffers[1] = {
	GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	//Check:
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "RenderTexture: Failed to create framebuffer!\n";
		return false;
	}

	//Set filtering
	updateFiltering();

	//Unbind all:
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_2D, lastTex);
	glBindRenderbuffer(GL_RENDERBUFFER, lastRB);

	return true;
}

void RenderTexture::bindAsRendertarget() const {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	if(!isCreated())
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glViewport(0, 0, getOriginalDimensions().x, getOriginalDimensions().y);

}

void RenderTexture::clear() const {

	if(!Window::isContextCreated())
		return;

	GLint previousTexture = getCurrentBoundTexture();
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, getDimensions().x, getDimensions().y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, previousTexture);

	GLint lastFB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &lastFB);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);

}

void RenderTexture::drawFullscreen() {

	if(!Window::isContextCreated())
		return;

	Mesh mesh;
	std::vector<Vertex> v;
	v.push_back(Vertex(Vector3f(-1.f, -1.f, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(0.f, 0.f))));
	v.push_back(Vertex(Vector3f(1.f, -1.f, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(-1.f, 1.f, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(-1.f, 1.f, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(1.f, -1.f, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(1.f, 1.f, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(1.f, 1.f))));
	mesh.setVertices(v);
	mesh.update();

	//Get shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);

	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	GLint lastTex = getCurrentBoundTexture();

	//Bind texture only
	bind();

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

	glBindTexture(GL_TEXTURE_2D, lastTex);

}

void RenderTexture::draw(const Vector2f& p, const Vector2f& s) {

	if(!Window::isContextCreated())
		return;

	Mesh mesh;
	std::vector<Vertex> v;

	v.push_back(Vertex(Vector3f(p.x, p.y - s.y, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(0.f, 0.f))));
	v.push_back(Vertex(Vector3f(p.x + s.x, p.y - s.y, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(p.x, p.y, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(p.x, p.y, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(0.f, 1.f))));
	v.push_back(Vertex(Vector3f(p.x + s.x, p.y - s.y, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(1.f, 0.f))));
	v.push_back(Vertex(Vector3f(p.x + s.x, p.y, 0), Vector3f(), mapUvCoordsToTexture(Vector2f(1.f, 1.f))));

	mesh.setVertices(v);
	mesh.update();

	//Get shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);

	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	GLint lastTex = getCurrentBoundTexture();

	//Bind texture only
	bind();

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

	glBindTexture(GL_TEXTURE_2D, lastTex);

}

} /* namespace burn */
