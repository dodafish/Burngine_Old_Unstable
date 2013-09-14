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
#include "Mesh.h"

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
	 * @brief Sets the Mesh that the node should use
	 *
	 * @param mesh The Mesh to use
	 *
	 * @see getMesh()
	 */
	void setMesh(const Mesh& mesh);

	/**
	 * @brief Returns the current Mesh the node is using
	 *
	 * @return The current Mesh
	 *
	 * @see setMesh()
	 */
	const Mesh& getMesh() const;

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
	virtual void draw(Camera* cam = nullptr);

private:
	Mesh _mesh;
};

} /* namespace burn */
#endif /* STATICMESHNODE_H_ */
