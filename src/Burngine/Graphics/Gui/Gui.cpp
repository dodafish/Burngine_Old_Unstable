/*
 * Gui.cpp
 *
 *  Created on: 14.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Gui/Gui.h>
#include <Burngine/Graphics/Gui/GuiNode.h>

namespace burn {

Gui::Gui() {

}

Gui::~Gui() {
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

} /* namespace burn */
