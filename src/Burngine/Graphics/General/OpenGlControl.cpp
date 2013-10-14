/*
 * OpenGlControl.cpp
 *
 *  Created on: 14.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

void OpenGlControl::setBlendMode(const OpenGlControl::BlendMode& blendMode) {
	if(!Window::isContextCreated())
		return;

	if(blendMode == OVERWRITE){
		glBlendFunc(GL_ONE, GL_ZERO);
	}else if(blendMode == ADD){
		glBlendFunc(GL_ONE, GL_ONE);
	}else if(blendMode == MULTIPLY){
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	}

}

} /* namespace burn */
