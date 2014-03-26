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

#ifndef STATICMESHNODE_H_
#define STATICMESHNODE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/PhysicalSceneNode.h>
#include <Burngine/Graphics/Scene/Model.h>

namespace burn {

/**
 * @brief A SceneNode describing a model that has no animations
 * e.g. a building.
 */
class BURNGINE_API StaticMeshNode : public PhysicalSceneNode {
public:
	void setModel(const Model& model);
	const Model& getModel() const;
private:
	Model _model;
};

} /* namespace burn */
#endif /* STATICMESHNODE_H_ */
