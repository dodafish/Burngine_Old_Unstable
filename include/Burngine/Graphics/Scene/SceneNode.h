/*
 * SceneNode.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Scene/Scene.h>

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
	virtual void draw(const Camera& camera) = 0;

	virtual void drawDepthColorless(const Camera& camera) = 0;

	enum LightingType{
		DIFFUSE,
		SPECULAR
	};

	virtual void drawLighting(LightingType type, const Camera& camera, const std::vector<Light*>& lights, const Vector3f& ambient) = 0;

	void addParentScene(Scene* scene);
	void removeParentScene(Scene* scene);

protected:

	void setMVPUniforms(const BurngineShaders::Type& type, const Camera& cam);

private:
	std::vector<Scene*> _parents;
};

} /* namespace burn */
#endif /* SCENENODE_H_ */
