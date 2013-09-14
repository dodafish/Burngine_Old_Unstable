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
#include "Material.h"
#include "Transformable.h"
#include "Camera.h"

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
	virtual void draw(Camera* camera = nullptr) = 0;

	/**
	 * @brief Returns the material that the node is using.
	 *
	 * @return The Material of the node.
	 *
	 * @see setMaterial()
	 */
	const Material& getMaterial() const;

	/**
	 * @brief Sets the material of the node. Influences the rendering
	 * behaviour.
	 *
	 * @param material The Material to use.
	 *
	 * @see getMaterial()
	 */
	void setMaterial(const Material& material);



protected:
	Material _material;
};

} /* namespace burn */
#endif /* SCENENODE_H_ */
