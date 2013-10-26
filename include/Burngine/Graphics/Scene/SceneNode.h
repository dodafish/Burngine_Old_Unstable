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

//template class BURNGINE_API std::vector<burn::Scene*>;

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

	SceneNode(const SceneNode& other);

	SceneNode& operator=(const SceneNode& other);

	/**
	 * @brief Virtual method for rendering.
	 *
	 * @param camera Pointer to Camera to draw node correctly or
	 * nullptr for default rendermode.
	 */
	virtual void draw(const Camera& camera) = 0;

	virtual void drawSingleColor(const Camera& camera, const Vector4f& color) = 0;

	enum LightingType{
		DIFFUSE,
		SPECULAR
	};

	virtual void drawLighting(LightingType type, const Camera& camera, const std::vector<Light*>& lights, const Vector3f& ambient) = 0;

	void addParentScene(Scene* scene);
	void removeParentScene(Scene* scene);

	bool isCastingShadows() const;
	void setCastingShadows(bool enabled);

protected:

	void setMVPUniforms(const Shader& shader, const Camera& cam);

private:
	void removeAllParents();

	std::vector<Scene*> _parents;

	bool _isCastingShadows;
};

} /* namespace burn */
#endif /* SCENENODE_H_ */
