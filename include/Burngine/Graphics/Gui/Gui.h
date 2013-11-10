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

#ifndef GUI_H_
#define GUI_H_

#include <Burngine/Export.h>

#include <vector>

namespace burn {
class GuiNode;
}

template class BURNGINE_API std::vector<burn::GuiNode*>;

namespace burn {

/**
 * @brief This class cares about rendering attached GuiNodes properly
 */
class BURNGINE_API Gui {
public:
	/**
	 * @brief Default destructor detaching all attached GuiNodes
	 */
	~Gui();

	/**
	 * @brief Attach a GuiNode to the Gui for rendering
	 *
	 * @param node The GuiNode to attach
	 *
	 * @note You don't have to care about detaching. When the GuiNode or
	 * the Gui itself dies, the destructors will take care for it.
	 * - Attempts to attach a GuiNode more than once will be refused.
	 *
	 * @see detachNode()
	 */
	void attachNode(GuiNode& node);

	/**
	 * @brief Detach a GuiNode from the Gui to remove it
	 * from rendering
	 *
	 * @param node The GuiNode to detach
	 *
	 * @note This will have no effect when the GuiNode was
	 * not attached before
	 *
	 * @see attachNode()
	 */
	void detachNode(GuiNode& node);

	/**
	 * @brief This detaches all GuiNodes that have beed attached
	 * to the Gui. No nodes will be rendered by this Gui when not
	 * attached again.
	 */
	void detachAll();

	/**
	 * @brief This draws all GuiNodes that have beed attached
	 * to the Gui before. The rendering will take care of the set
	 * Z-Index of each GuiNode.
	 *
	 * @see attachNode()
	 * @see GuiNode::setZIndex()
	 */
	void draw();

	/**
	 * @brief This sorts all attached GuiNodes by their Z-Index
	 * from little (back) to high (front)
	 */
	void sortNodes();

private:
	std::vector<GuiNode*> _nodes;
};

} /* namespace burn */
#endif /* GUI_H_ */
