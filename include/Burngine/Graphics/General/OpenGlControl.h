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

#ifndef OPENGLCONTROL_H_
#define OPENGLCONTROL_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>

namespace burn {

#define MAX_TEXTURE_BINDINGS 32

class Shader;

class OpenGlControl {
public:
	//Static only
	OpenGlControl() = delete;

	enum BlendMode {
		OVERWRITE, ///< Ignores source's data
		ADD, ///< Adds to source's data
		MULTIPLY, ///< Multiplies with source's data
		MIX ///< Mixes according to alpha values
	};

	enum DepthtestTechnique {
		LESS = GL_LESS, ///< Closer vertices win depthtest
		EQUAL = GL_EQUAL, ///< Only equal vertices win depthtest
		LEQUAL = GL_LEQUAL ///< Closer and equal vertices win depthtest
	};

	enum CullSide {
		OUTSIDE = GL_FRONT, ///< Outside faces will be ignored
		INSIDE = GL_BACK ///< Inside faces will be ignored
	};

	enum VertexOrder {
		COUNTER_CLOCKWISE = GL_CCW, ///< Frontfaces have counter-clockwise vertexorder
		CLOCKWISE = GL_CW ///< Frontfaces have clockwise vertexorder
	};

	class Settings {
	public:
		Settings(bool isBlendingEnabled = true, const BlendMode& blendMode = OVERWRITE, bool isCullingEnabled = true,
		const CullSide& culledSide = INSIDE, const VertexOrder& vertexOrder = COUNTER_CLOCKWISE,
		bool isDepthtestEnabled = true, const DepthtestTechnique& technique = LESS, bool isDepthbufferWritingEnabled =
		true, const Vector4f& clearColor = Vector4f(0.1, 0.1, 0.3, 1.0));

		void enableBlending(bool enabled = true);
		bool isBlendingEnabled() const;
		void setBlendMode(const BlendMode& blendmode);
		const BlendMode& getBlendMode() const;

		void enableCulling(bool enabled = true);
		bool isCullingEnabled() const;
		void setCulledSide(const CullSide& culledSide);
		const CullSide& getCulledSide() const;
		void setVertexOrder(const VertexOrder& order);
		const VertexOrder& getVertexOrder() const;

		void enableDepthtest(bool enabled = true);
		bool isDepthtestEnabled() const;
		void setDepthtestTechnique(const DepthtestTechnique& technique);
		const DepthtestTechnique& getDepthtestTechnique() const;
		void enableDepthbufferWriting(bool enabled = true);
		bool isDepthbufferWritingEnabled() const;

		void setClearColor(const Vector4f& color);
		const Vector4f& getClearColor() const;

	private:
		bool _isBlendingEnabled;
		BlendMode _blendMode;

		bool _isCullingEnabled;
		CullSide _cullSide;
		VertexOrder _vertexOrder;

		bool _isDepthtestEnabled;
		DepthtestTechnique _depthtestTechnique;
		bool _isDepthbufferWritingEnabled;

		Vector4f _clearColor;
	};

	static void useSettings(const Settings& settings);

	enum DrawingTechnique{
		TRIANGLES,
		TRIANGLE_STRIP
	};

	static void draw(const DrawingTechnique& tech, GLint first, GLsizei count, const Shader& shader);

	//////////////////////////////////////////////
	// Binding Control
	//////////////////////////////////////////////
	static void bindTexture(const GLuint& textureId, const GLuint& unit = 0);
	static const GLuint& getTextureBinding(const GLuint& unit = 0);
	static void bindCubeMap(const GLuint& cubemapId, const GLuint& unit = 0);
	static void bindDrawBuffer(const GLuint& drawBufferId);
	static const GLuint& getDrawBufferBinding();
	static void bindReadBuffer(const GLuint& readBufferId);
	static const GLuint& getReadBufferBinding();

	//Keeping track of bindings avoid the inefficient glGet*
	static GLuint _currentTextureBinding[MAX_TEXTURE_BINDINGS];
	static GLuint _currentDrawBufferBinding;
	static GLuint _currentReadBufferBinding;

};

} /* namespace burn */
#endif /* OPENGLCONTROL_H_ */
