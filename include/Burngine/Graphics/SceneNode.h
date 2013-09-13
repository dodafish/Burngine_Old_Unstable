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
	SceneNode();
	virtual ~SceneNode();

	virtual void draw(Camera* camera = nullptr) = 0;

	const Material& getMaterial() const;
	void setMaterial(const Material& material);



protected:
	Material _material;
};

} /* namespace burn */
#endif /* SCENENODE_H_ */
