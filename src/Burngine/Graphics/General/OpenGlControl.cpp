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

#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/Shader.h>

#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Reporter.h>

namespace burn {

//Members of OpenGlControl
GLuint OpenGlControl::_currentTextureBinding[];
GLuint OpenGlControl::_currentDrawBufferBinding = 0;
GLuint OpenGlControl::_currentReadBufferBinding = 0;

void OpenGlControl::draw(	const DrawingTechnique& tech,
							GLint first,
							GLsizei count,
							const Shader& shader) {

	//Activates shader and uploads uniforms
	shader.activate();

	if(tech == TRIANGLES){
		glDrawArrays(GL_TRIANGLES, first, count);
	}else{ //TRIANGLE_STRIP
		glDrawArrays(GL_TRIANGLE_STRIP, first, count);
	}

}

OpenGlControl::Settings::Settings(	bool isBlendingEnabled,
									const BlendMode& blendMode,
									bool isCullingEnabled,
									const CullSide& culledSide,
									const VertexOrder& vertexOrder,
									bool isDepthtestEnabled,
									const DepthtestTechnique& technique,
									bool isDepthbufferWritingEnabled,
									const Vector4f& clearColor) :
_isBlendingEnabled(isBlendingEnabled),
_blendMode(blendMode),
_isCullingEnabled(isCullingEnabled),
_cullSide(culledSide),
_vertexOrder(vertexOrder),
_isDepthtestEnabled(isDepthtestEnabled),
_depthtestTechnique(technique),
_isDepthbufferWritingEnabled(isDepthbufferWritingEnabled),
_clearColor(clearColor) {

}

void OpenGlControl::useSettings(const Settings& settings) {

	ensureContext();

	if(settings.isBlendingEnabled())
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	if(settings.isCullingEnabled())
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if(settings.isDepthtestEnabled())
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if(settings.isDepthbufferWritingEnabled())
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);

	switch (settings.getBlendMode()) {
		case OVERWRITE:
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		case ADD:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case MULTIPLY:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		case MIX:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
	}

	glCullFace(settings.getCulledSide());
	glFrontFace(settings.getVertexOrder());
	glDepthFunc(settings.getDepthtestTechnique());

	const Vector4f& c = settings.getClearColor();
	glClearColor(c.r, c.g, c.b, c.a);

}

void OpenGlControl::Settings::enableBlending(bool enabled) {
	_isBlendingEnabled = enabled;
}

bool OpenGlControl::Settings::isBlendingEnabled() const {
	return _isBlendingEnabled;
}

void OpenGlControl::Settings::setBlendMode(const BlendMode& blendmode) {
	_blendMode = blendmode;
}

const OpenGlControl::BlendMode& OpenGlControl::Settings::getBlendMode() const {
	return _blendMode;
}

void OpenGlControl::Settings::enableCulling(bool enabled) {
	_isCullingEnabled = enabled;
}

bool OpenGlControl::Settings::isCullingEnabled() const {
	return _isCullingEnabled;
}

void OpenGlControl::Settings::setCulledSide(const CullSide& culledSide) {
	_cullSide = culledSide;
}

const OpenGlControl::CullSide& OpenGlControl::Settings::getCulledSide() const {
	return _cullSide;
}

void OpenGlControl::Settings::setVertexOrder(const VertexOrder& order) {
	_vertexOrder = order;
}

const OpenGlControl::VertexOrder& OpenGlControl::Settings::getVertexOrder() const {
	return _vertexOrder;
}

void OpenGlControl::Settings::enableDepthtest(bool enabled) {
	_isDepthtestEnabled = enabled;
}

bool OpenGlControl::Settings::isDepthtestEnabled() const {
	return _isDepthtestEnabled;
}

void OpenGlControl::Settings::setDepthtestTechnique(const DepthtestTechnique& technique) {
	_depthtestTechnique = technique;
}

const OpenGlControl::DepthtestTechnique& OpenGlControl::Settings::getDepthtestTechnique() const {
	return _depthtestTechnique;
}

void OpenGlControl::Settings::enableDepthbufferWriting(bool enabled) {
	_isDepthbufferWritingEnabled = enabled;
}
bool OpenGlControl::Settings::isDepthbufferWritingEnabled() const {
	return _isDepthbufferWritingEnabled;
}

void OpenGlControl::Settings::setClearColor(const Vector4f& color) {
	_clearColor = color;
}

const Vector4f& OpenGlControl::Settings::getClearColor() const {
	return _clearColor;
}

/////////////////////////////////////////////////////////////////////

void OpenGlControl::bindTexture(const GLuint& textureId,
								const GLuint& unit) {
	ensureContext();

	if(unit >= MAX_TEXTURE_BINDINGS || unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS){
		Reporter::report("Unable to bind texture. Unit out of range!", Reporter::ERROR);
		exit(72);
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureId);
	_currentTextureBinding[unit] = textureId;
}

void OpenGlControl::bindCubeMap(const GLuint& cubemapId, const GLuint& unit) {

	ensureContext();

	if(unit >= MAX_TEXTURE_BINDINGS || unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS){
		Reporter::report("Unable to bind texture. Unit out of range!", Reporter::ERROR);
		exit(72);
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
	_currentTextureBinding[unit] = cubemapId;

}

const GLuint& OpenGlControl::getTextureBinding(const GLuint& unit) {

	if(unit >= MAX_TEXTURE_BINDINGS || unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS){
		Reporter::report("Unable to bind texture. Unit out of range!", Reporter::ERROR);
		exit(72);
	}

	return _currentTextureBinding[unit];
}

void OpenGlControl::bindDrawBuffer(const GLuint& drawBufferId) {
	ensureContext();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawBufferId);
	_currentDrawBufferBinding = drawBufferId;
}

const GLuint& OpenGlControl::getDrawBufferBinding() {
	return _currentDrawBufferBinding;
}

void OpenGlControl::bindReadBuffer(const GLuint& readBufferId) {
	ensureContext();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readBufferId);
	_currentReadBufferBinding = readBufferId;
}

const GLuint& OpenGlControl::getReadBufferBinding() {
	return _currentReadBufferBinding;
}

} /* namespace burn */
