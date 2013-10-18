/*
 * OpenGlControl.h
 *
 *  Created on: 14.10.2013
 *      Author: Dominik
 */

#ifndef OPENGLCONTROL_H_
#define OPENGLCONTROL_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>

namespace burn {

class Shader;

class OpenGlControl {
public:
	//Static only
	OpenGlControl() = delete;

	enum BlendMode {
		OVERWRITE, ADD, MULTIPLY, MIX
	};

	enum DepthtestTechnique {
		LESS = GL_LESS, EQUAL = GL_EQUAL, LEQUA = GL_LEQUAL
	};

	enum CullSide {
		OUTSIDE = GL_FRONT, INSIDE = GL_BACK
	};

	enum VertexOrder {
		COUNTER_CLOCKWISE = GL_CCW, CLOCKWISE = GL_CW
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
};

} /* namespace burn */
#endif /* OPENGLCONTROL_H_ */
