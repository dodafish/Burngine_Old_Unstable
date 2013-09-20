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
#include "Shader.h"

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

protected:

	void setUniforms(const BurngineShaders::Type& type, std::shared_ptr<Camera> cam);

};

} /* namespace burn */
#endif /* SCENENODE_H_ */
