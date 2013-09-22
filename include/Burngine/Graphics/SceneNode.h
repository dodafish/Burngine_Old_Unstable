/*
 * SceneNode.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include "../Export.h"
#include "../System/Math.h"
#include "Transformable.h"
#include "Camera.h"
#include <memory>
#include <vector>
#include "Shader.h"
#include "Light.h"

namespace burn {

class BURNGINE_API SceneNode : public Transformable {
public:
	/**
	 * @brief Default Constructor.
	 */
	SceneNode();

	/**
	 * @brief Default Destructor
	 */
	virtual ~SceneNode();

	/**
	 * @brief Virtual method for rendering.
	 *
	 * @param camera Pointer to Camera to draw node correctly or
	 * nullptr for default rendermode.
	 */
	virtual void draw(std::shared_ptr<Camera> camera) = 0;

	virtual void drawDepthColorless(std::shared_ptr<Camera> camera) = 0;

	virtual void drawLighting(std::shared_ptr<Camera> camera, const std::vector<std::shared_ptr<Light>>& lights,
			const Vector3f& ambient) = 0;

protected:

	void setMVPUniforms(const BurngineShaders::Type& type, std::shared_ptr<Camera> cam);

};

} /* namespace burn */
#endif /* SCENENODE_H_ */
