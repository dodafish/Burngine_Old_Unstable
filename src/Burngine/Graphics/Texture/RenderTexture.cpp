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
#include <Burngine/Graphics/General/OpenGL.h>

#include <iostream>

namespace burn {

RenderTexture::RenderTexture() :
_framebuffer(0),
_depthbuffer(0),
_texture(0),
_isCreated(false) {
}

RenderTexture::~RenderTexture() {

	for(size_t i = 0; i < _additionalAttachments.size(); ++i){
		glDeleteTextures(1, &_additionalAttachments[i].texture);
	}

	ensureContext();

	glDeleteFramebuffers(1, &_framebuffer);
	glDeleteRenderbuffers(1, &_depthbuffer);

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

	for(size_t i = 0; i < _additionalAttachments.size(); ++i){

		//Texture has filterung without sampler.
		Sampler::unbind(_additionalAttachments[i].attachment);

		glActiveTexture(GL_TEXTURE0 + _additionalAttachments[i].attachment);
		glBindTexture(GL_TEXTURE_2D, _additionalAttachments[i].texture);
	}
}

void RenderTexture::onUnbind(const unsigned int& unit) const {

	glBindTexture(GL_TEXTURE_2D, 0);

}

void RenderTexture::cleanup() {

	if(!isCreated())
		return;

	ensureContext();

	for(size_t i = 0; i < _additionalAttachments.size(); ++i){
		glDeleteTextures(1, &_additionalAttachments[i].texture);
	}
	_additionalAttachments.clear();

	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_framebuffer);
	glDeleteRenderbuffers(1, &_depthbuffer);

	_isCreated = false;
	_dimensions = Vector2ui(0);

}

bool RenderTexture::create(const Vector2ui& dimensions) {

	ensureContext();

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
	GLenum drawBuffers[1] = {
	GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

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

bool RenderTexture::addColorAttachment(const unsigned int& attachment) {

	//Valid OpenGL-Context is needed
	ensureContext();

	if(!isCreated()){
		Reporter::report("Unable to add color attachment. Rendertexture does not exist!", Reporter::ERROR);
		return false;
	}
	if(attachment == 0){
		Reporter::report("Unable to add color attachment to slot 0. Slot already taken!", Reporter::ERROR);
		return false;
	}
	if(attachment >= GL_MAX_COLOR_ATTACHMENTS){
		Reporter::report("Unable to add color attachment. Attachment ID out of range!", Reporter::ERROR);
		return false;
	}
	for(size_t i = 0; i < _additionalAttachments.size(); ++i){
		if(_additionalAttachments[i].attachment == attachment){
			Reporter::report("Unable to add color attachment. Slot already taken!", Reporter::ERROR);
			return false;
		}
	}

	//Ok, we can create the new attachment:
	///////////////////////////////////////

	//For restoring
	GLint lastFB = 0;
	GLint lastTex = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &lastFB);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

	AdditionalAttachment aa;
	aa.attachment = attachment;

	//Create:
	glGenTextures(1, &aa.texture);
	glBindTexture(GL_TEXTURE_2D, aa.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	//Samplerless filtering!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Reconfigure framebuffer:
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	//Connect texture to framebuffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + aa.attachment, GL_TEXTURE_2D, aa.texture, 0);

	_additionalAttachments.push_back(aa);

	//Tell the framebuffer the new drawbufferset
	std::vector<GLenum> drawBuffers;
	drawBuffers.push_back(GL_COLOR_ATTACHMENT0); //The default one that is no additional attachment
	for(size_t i = 0; i < _additionalAttachments.size(); ++i){
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + _additionalAttachments[i].attachment);
	}
	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

	//Check:
	if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Reporter::report("RenderTexture: Failed to add color attachment!\n", Reporter::ERROR);
		return false;
	}

	//Restore
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lastFB);
	glBindTexture(GL_TEXTURE_2D, lastTex);

	return true;
}

void RenderTexture::bindAsTarget() const {

	//Valid OpenGL-Context is needed
	ensureContext();

	if(!isCreated()){
		Reporter::report("Binding uncreated rendertexture disallowed!", Reporter::WARNING);
		return;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glViewport(0, 0, _dimensions.x, _dimensions.y);

}

void RenderTexture::clear() const {

	ensureContext();

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

	//Clear additional textures
	for(size_t i = 0; i < _additionalAttachments.size(); ++i){
		glBindTexture(GL_TEXTURE_2D, _additionalAttachments[i].texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _dimensions.x, _dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}

	//Clear buffers
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Restore previous bindings
	glBindTexture(GL_TEXTURE_2D, previousTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, lastFB);

}

/*void RenderTexture::drawFullscreen() {

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

 }*/

} /* namespace burn */
