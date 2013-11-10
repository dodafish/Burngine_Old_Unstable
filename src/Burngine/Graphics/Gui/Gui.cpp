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

#include <Burngine/Graphics/Gui/Gui.h>
#include <Burngine/Graphics/Gui/GuiNode.h>

namespace burn {

Gui::~Gui() {
	detachAll();
}

void Gui::attachNode(GuiNode& node) {
	//Ensure storing a node only once
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node)
			return;
	}

	//Store node
	_nodes.push_back(&node);
	node.addParentGui(this);

	//Sort all nodes by Z-Index for proper rendering
	sortNodes();
}

void Gui::detachNode(GuiNode& node) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node){
			node.removeParentGui(this);
			_nodes.erase(_nodes.begin() + i);
			return;
		}
	}
}

void Gui::detachAll() {
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->removeParentGui(this);
	}
	_nodes.clear();
}

void Gui::draw() {
	//They are sorted by Z-Index
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->draw();
	}
}

void Gui::sortNodes() {

	//Return if no need for sorting
	if(_nodes.size() < 2)
		return;

	bool sorted = false;
	while(!sorted){
		sorted = true;
		for(size_t i = 0; i < (_nodes.size() - 1); ++i){
			if(_nodes[i]->getZIndex() > _nodes[i + 1]->getZIndex()){
				GuiNode* temp = _nodes[i];
				_nodes[i] = _nodes[i + 1];
				_nodes[i + 1] = temp;
				sorted = false;
			}
		}
	}

}

} /* namespace burn */
