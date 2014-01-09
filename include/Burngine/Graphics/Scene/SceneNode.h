//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
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

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Scene/Scene.h>

#include <vector>

namespace burn {

class Camera;
class Light;

/**
 * @brief Describes an object attachable to the Scene
 */
class BURNGINE_API SceneNode : public Transformable {
public:
	/**
	 * @brief Default Constructor initialzing some
	 * values
	 */
	SceneNode();

	/**
	 * @brief Default Destructor detaching the SceneNode
	 * from all Scenes
	 */
	virtual ~SceneNode();

	/**
	 * @brief Copyconstructor copying the parents of the other
	 * SceneNode and attaching itself to those scenes too
	 *
	 * @param other The other SceneNode to copy from
	 */
	SceneNode(const SceneNode& other);

	/**
	 * @brief Assignmentoperator copying the parents of the other
	 * SceneNode and attaching itself to those scenes too
	 *
	 * @param other The other SceneNode to copy from
	 *
	 * @note When this SceneNode is attached to any Scene it
	 * will get detached before!
	 */
	SceneNode& operator=(const SceneNode& other);

	/**
	 * @brief Adds a Scene as parent. This is used by Scene when
	 * attaching SceneNodes. You should use the Scene's methods for
	 * connecting SceneNodes to Scenes instead of this method
	 *
	 * @param parent The parent Scene
	 *
	 * @see removeParentScene()
	 */
	void addParentScene(Scene* scene);

	/**
	 * @brief Removes a Scene from parents. This is used by Scene when
	 * detaching SceneNodes. You should use the Scene's methods for
	 * disconnecting SceneNodes from Scenes instead of this method
	 *
	 * @param parent The parent Scene
	 *
	 * @see addParentScene()
	 */
	void removeParentScene(Scene* scene);

	/**
	 * @brief Returns true when the SceneNode is casting shadows
	 *
	 * @return Returns false when the SceneNode casts no shadows
	 *
	 * @see setCastingShadows()
	 */
	bool isCastingShadows() const;

	/**
	 * @brief Sets whether the SceneNode should cast any shadows.
	 *
	 * @param enabled Set this to true when the SceneNode should cast
	 * shadows. False otherwise.
	 *
	 * @see isCastingShadows()
	 */
	void setCastingShadows(bool enabled);

private:
	void removeAllParents();

	std::vector<Scene*> _parents;

	bool _isCastingShadows;
};

} /* namespace burn */
#endif /* SCENENODE_H_ */
