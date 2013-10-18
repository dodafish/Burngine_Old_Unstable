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
