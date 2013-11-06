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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	for(size_t i = 0; i < _additionalTextures.size(); ++i){
		glActiveTexture(GL_TEXTURE0 + (_attachments[i] - GL_COLOR_ATTACHMENT0));
		glBindTexture(GL_TEXTURE_2D, _additionalTextures[i]);
	}
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

	for(size_t i = 0; i < _additionalTextures.size(); ++i){
		glDeleteTextures(1, &_additionalTextures[i]);
	}
	_additionalTextures.clear();

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
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &lastRB);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

	//Framebuffer:
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);

	//Texture:
	glGenTextures(1, &_texture);
	//Clear texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	//Depthbuffer:
	glGenRenderbuffers(1, &_depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _dimensions.x, _dimensions.y);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

	//Configure:
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);
	_attachments.push_back(GL_COLOR_ATTACHMENT0);
	glDrawBuffers(_attachments.size(), &_attachments[0]);

	//Check:
	if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("RenderTexture: Failed to create framebuffer!\n", Reporter::ERROR);
		return false;
	}

	//Restore previous bindings
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_2D, lastTex);
	glBindRenderbuffer(GL_RENDERBUFFER, lastRB);

	_isCreated = true;

	return true;
}

bool RenderTexture::addColorAttachment(const GLenum& attachment) {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated()){
		Reporter::report("Unable to add color attachment. No valid context!", Reporter::ERROR);
		return false;
	}
	if(!isCreated()){
		Reporter::report("Unable to add color attachment. Rendertexture does not exist!", Reporter::ERROR);
		return false;
	}
	if(attachment == GL_COLOR_ATTACHMENT0){
		Reporter::report("Unable to add color attachment to slot 0. Slot already taken!", Reporter::ERROR);
		return false;
	}
	for(size_t i = 0; i < _attachments.size(); ++i){
		if(_attachments[i] == attachment){
			Reporter::report("Unable to add color attachment. Slot already taken!", Reporter::ERROR);
			return false;
		}
	}

	GLint lastFB = 0;
	GLint lastTex = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

	GLuint id = 0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);

	_attachments.push_back(attachment);
	_additionalTextures.push_back(id);
	glDrawBuffers(_attachments.size(), &_attachments[0]);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_2D, lastTex);

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

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
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

	for(size_t i = 0; i < _additionalTextures.size(); ++i){
		glBindTexture(GL_TEXTURE_2D, _additionalTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}

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
