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

namespace burn {

class OpenGlControl {

	//Static only
	OpenGlControl() = delete;

	enum BlendMode {
		OVERWRITE, ADD, MULTIPLY
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
		true);

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

	private:
		bool _isBlendingEnabled;
		BlendMode _blendMode;

		bool _isCullingEnabled;
		CullSide _cullSide;
		VertexOrder _vertexOrder;

		bool _isDepthtestEnabled;
		DepthtestTechnique _depthtestTechnique;
		bool _isDepthbufferWritingEnabled;
	};

	static void useSettings(const Settings& settings);
};

} /* namespace burn */
#endif /* OPENGLCONTROL_H_ */
