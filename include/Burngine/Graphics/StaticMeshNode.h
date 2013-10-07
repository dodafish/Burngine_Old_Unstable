/*
 * StaticMeshNode.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef STATICMESHNODE_H_
#define STATICMESHNODE_H_

#include "../Export.h"
#include "SceneNode.h"
#include "Model.h"

namespace burn {

/**
 * @brief A static mesh. No animations are possible.
 *
 * @note Create a StaticMeshNode only by calling the
 * according function of a Scene! Otherwise you will
 * get undefined results.
 *
 * @see Scene::createStaticMeshNode()
 */
class BURNGINE_API StaticMeshNode : public SceneNode {
public:
	/**
	 * @brief The default constructor
	 */
	StaticMeshNode();

	/**
	 * @brief The default destructor
	 */
	~StaticMeshNode();

	/**
	 * @brief Sets the Model that the node should use
	 *
	 * @param model The Model to use
	 *
	 * @see getModel()
	 */
	void setModel(const Model& model);

	/**
	 * @brief Returns the current Model the node is using
	 *
	 * @return The current Model
	 *
	 * @see setModel()
	 */
	Model& getModel();

	bool loadFromFile(const std::string& file);

	/**
	 * @brief Renders the node to the scene. Call this
	 * indirect by calling the drawfunction of the Scene which
	 * the node belongs to.
	 *
	 * @param cam The Camera that the node is rendered to accordingly
	 * or nullptr to draw defaultposition
	 *
	 * @see Scene::drawAll()
	 */
	virtual void draw(const Camera& cam);

	virtual void drawDepthColorless(const Camera& camera);

	virtual void drawLighting(LightingType type, const Camera& camera, const std::vector<Light*>& lights, const Vector3f& ambient);

private:
	Model _model;
};

} /* namespace burn */
#endif /* STATICMESHNODE_H_ */
