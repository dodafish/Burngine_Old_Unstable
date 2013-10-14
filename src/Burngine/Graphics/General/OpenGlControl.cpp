/*
 * OpenGlControl.cpp
 *
 *  Created on: 14.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

OpenGlControl::Settings::Settings(bool isBlendingEnabled, const BlendMode& blendMode, bool isCullingEnabled,
const CullSide& culledSide, const VertexOrder& vertexOrder, bool isDepthtestEnabled,
const DepthtestTechnique& technique, bool isDepthbufferWritingEnabled, const Vector4f& clearColor) :
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

	if(!Window::isContextCreated())
		return;

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

} /* namespace burn */
