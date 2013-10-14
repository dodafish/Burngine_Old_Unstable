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
		LESS, EQUAL, LEQUAL
	};

	enum CullSide {
		OUTSIDE, INSIDE
	};

	enum VertexOrder {
		COUNTER_CLOCKWISE, CLOCKWISE
	};

	class Settings {
	public:
		Settings();

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

	private:
		bool _isBlendingEnabled;
		BlendMode _blendMode;

		bool _isCullingEnabled;
		CullSide _cullSide;
		VertexOrder _vertexOrder;

		bool _isDepthtestEnabled;
		DepthtestTechnique _depthtestTechnique;
	};

	/**
	 * @brief This is used to set the blending mode when rendering.
	 * Initially OVERWRITE is used. So everything you draw will ignore,
	 * what was already drawn.
	 * @note Setting the blendmode will only work, when an OpenGL context
	 * is created!
	 * @see isContextCreated()
	 */
	static void setBlendMode(const BlendMode& blendMode);

};

} /* namespace burn */
#endif /* OPENGLCONTROL_H_ */
