//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef STATICMESHNODE_H_
#define STATICMESHNODE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/SceneNode.h>
#include <Burngine/Graphics/Scene/Model.h>

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
	const Model& getModel() const;

	void setFlag(const Material::Flag& flag, const bool& enabled = true);

	void update();

	bool loadFromFile(const std::string& file);

private:
	//Hide methods, that are public in SceneNode.
	//We have no need to have them public here, anymore.
	using SceneNode::addParentScene;
	using SceneNode::removeParentScene;
	////////////////////////////////////////////////////

	//Attributes:
	Model _model;
};

} /* namespace burn */
#endif /* STATICMESHNODE_H_ */
