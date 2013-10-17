/*
 * Label.h
 *
 *  Created on: 17.10.2013
 *      Author: Dominik
 */

#ifndef LABEL_H_
#define LABEL_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Gui/Text.h>

namespace burn {

class BURNGINE_API Label : public Text {
public:
	Label();
	~Label();

	virtual void draw();

private:

};

} /* namespace burn */
#endif /* LABEL_H_ */
