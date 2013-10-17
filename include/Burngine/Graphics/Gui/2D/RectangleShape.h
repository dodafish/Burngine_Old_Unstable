/*
 * RectangleShape.h
 *
 *  Created on: 17.10.2013
 *      Author: Dominik
 */

#ifndef RECTANGLESHAPE_H_
#define RECTANGLESHAPE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>
#include <Burngine/Graphics/Gui/GuiNode.h>

namespace burn {

class BURNGINE_API RectangleShape : public GuiNode {
public:
	RectangleShape();

	void setDimensions(const Vector2f& dimensions);
	const Vector2f& getDimensions() const;

	void setColor(const Vector4f& color);
	const Vector4f& getColor() const;

	void draw();

private:
	Vector2f _dimensions;
	Vector4f _color;
};

} /* namespace burn */
#endif /* RECTANGLESHAPE_H_ */
