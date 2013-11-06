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

class BURNGINE_API Gui {
public:
	Gui();
	~Gui();

	void attachNode(GuiNode& node);
	void detachNode(GuiNode& node);
	void detachAll();

	void draw();
	void sortNodes();

private:
	std::vector<GuiNode*> _nodes;
};

} /* namespace burn */
#endif /* GUI_H_ */
