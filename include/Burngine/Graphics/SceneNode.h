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
#include "Shader.h"
#include "Scene.h"

#include <memory>
#include <vector>

template class BURNGINE_API std::vector<burn::Scene*>;

namespace burn {

class Camera;
class Light;

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
	virtual void draw(Camera* camera) = 0;

	virtual void drawDepthColorless(Camera* camera) = 0;

	virtual void drawLighting(Camera* camera, const std::vector<Light*>& lights,
			const Vector3f& ambient) = 0;

protected:

	void setMVPUniforms(const BurngineShaders::Type& type, Camera* cam);

private:
	friend void Scene::attachSceneNode(SceneNode&);
	friend void Scene::detachSceneNode(SceneNode&);
	std::vector<Scene*> _parents;
};

} /* namespace burn */
#endif /* SCENENODE_H_ */
