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

#include <Burngine/Graphics/General/BurngineShaders.h>

namespace burn {

Shader BurngineShaders::_textureShader;
Shader BurngineShaders::_textureOneComponentShader;
Shader BurngineShaders::_pointlightShader;
Shader BurngineShaders::_spotlightShader;
Shader BurngineShaders::_dirlightShader;
Shader BurngineShaders::_singleColorShader;
Shader BurngineShaders::_fontShader;
Shader BurngineShaders::_depthShader;
Shader BurngineShaders::_skyBoxShader;
Shader BurngineShaders::_gBufferShader;
Shader BurngineShaders::_vsmDrawShader;
Shader BurngineShaders::_gaussianBlurHorizontalShader;
Shader BurngineShaders::_gaussianBlurVerticalShader;

GLuint Shader::_currentProgram = 0;

bool BurngineShaders::load(const std::string& d) {

	std::string dir = d;

	if(d[d.size() - 1] != '/' || d[d.size() - 1] != '\\')
		dir = d + "/";

	if(!_textureShader.loadFromFile(dir + "texture.vert", dir + "texture.frag")){
		return false;
	}
	if(!_textureOneComponentShader.loadFromFile(dir + "texture.vert", dir + "textureOneComponent.frag")){
		return false;
	}
	if(!_pointlightShader.loadFromFile(dir + "pointlight.vert", dir + "pointlight.frag")){
		return false;
	}
	if(!_spotlightShader.loadFromFile(dir + "spotlight.vert", dir + "spotlight.frag")){
		return false;
	}
	if(!_dirlightShader.loadFromFile(dir + "dirlight.vert", dir + "dirlight.frag")){
		return false;
	}
	if(!_singleColorShader.loadFromFile(dir + "singleColor.vert", dir + "singleColor.frag")){
		return false;
	}
	if(!_fontShader.loadFromFile(dir + "texture.vert", dir + "font.frag")){
		return false;
	}
	if(!_depthShader.loadFromFile(dir + "depth.vert", dir + "depth.frag")){
		return false;
	}
	if(!_skyBoxShader.loadFromFile(dir + "skyBox.vert", dir + "skyBox.frag")){
		return false;
	}
	if(!_gBufferShader.loadFromFile(dir + "gBuffer.vert", dir + "gBuffer.frag")){
		return false;
	}
	if(!_vsmDrawShader.loadFromFile(dir + "vsmDraw.vert", dir + "vsmDraw.frag")){
		return false;
	}
	if(!_gaussianBlurHorizontalShader.loadFromFile(dir + "texture.vert", dir + "gaussianHorizontalBlur.frag")){
		return false;
	}
	if(!_gaussianBlurVerticalShader.loadFromFile(dir + "texture.vert", dir + "gaussianVerticalBlur.frag")){
		return false;
	}

	return true;
}

const Shader& BurngineShaders::getShader(const Type& type) {
	switch (type) {
		case TEXTURE:
			//Will be returned at end of function, so it always returns.
			//This makes the compiler happy :)
			break;
		case TEXTURE_ONE_COMPONENT:
			return _textureOneComponentShader;
			break;
		case POINTLIGHT:
			return _pointlightShader;
			break;
		case SPOTLIGHT:
			return _spotlightShader;
			break;
		case DIRECTIONAL_LIGHT:
			return _dirlightShader;
			break;
		case SINGLECOLOR:
			return _singleColorShader;
			break;
		case FONT:
			return _fontShader;
			break;
		case DEPTH:
			return _depthShader;
			break;
		case SKY_BOX:
			return _skyBoxShader;
			break;
		case G_BUFFER:
			return _gBufferShader;
			break;
		case VSM_DRAW:
			return _vsmDrawShader;
			break;
		case GAUSSIAN_BLUR_HORIZONTAL:
			return _gaussianBlurHorizontalShader;
			break;
		case GAUSSIAN_BLUR_VERTICAL:
			return _gaussianBlurVerticalShader;
			break;
	}
	//See case of SOLID_COLOR above
	return _textureShader;
}

}
