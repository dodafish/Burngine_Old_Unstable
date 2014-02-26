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

#ifndef BURNGINESHADERS_H_
#define BURNGINESHADERS_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/Shader.h>

namespace burn {

struct BURNGINE_API BurngineShaders {

	/**
	 * @brief Enum for several predefined shaders
	 *
	 * @see loadShader()
	 * @see useShader()
	 */
	enum Type {
		TEXTURE, ///< Simple 1-Texture Shader
		TEXTURE_ONE_COMPONENT, ///< Samples from a texture with a single component e.g. depth textures
		POINTLIGHT, ///< Renders a pointlight
		SPOTLIGHT, ///< Render a spotlight
		DIRECTIONAL_LIGHT, ///< Renders a directional light
		SINGLECOLOR, ///< Draws a single color
		FONT, ///< Used for font rendering
		DEPTH, ///< Outputs only the depthpart
		SKY_BOX, ///< Used for skybox rendering (z-independant cubemap rendering)
		G_BUFFER, ///< Used by the GBuffer
		VSM_DRAW, ///< Used for drawing into VSM (2-component shadowmap)
		GAUSSIAN_BLUR_HORIZONTAL,
		GAUSSIAN_BLUR_VERTICAL,
		OVR_DISTORTION
	};

	/**
	 * @brief Loads and creates all predefined shaders.
	 */
	static bool load(const std::string& directory = "/");

	static const Shader& getShader(const Type& type);

private:
	//Making ctor private avoids creating of instances
	BurngineShaders() = delete;

	static Shader _textureShader;
	static Shader _textureOneComponentShader;
	static Shader _pointlightShader;
	static Shader _spotlightShader;
	static Shader _dirlightShader;
	static Shader _singleColorShader;
	static Shader _fontShader;
	static Shader _depthShader;
	static Shader _skyBoxShader;
	static Shader _gBufferShader;
	static Shader _vsmDrawShader;
	static Shader _gaussianBlurHorizontalShader;
	static Shader _gaussianBlurVerticalShader;
	static Shader _ovrDistortionShader;
};

}

#endif /* BURNGINESHADERS_H_ */
