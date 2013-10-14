/*
 * Gui.h
 *
 *  Created on: 14.10.2013
 *      Author: Dominik
 */

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
