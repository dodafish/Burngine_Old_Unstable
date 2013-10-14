/*
 * GuiNode.h
 *
 *  Created on: 13.10.2013
 *      Author: Dominik
 */

#ifndef GUINODE_H_
#define GUINODE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

#include <vector>

namespace burn{
class Gui;
}

template class BURNGINE_API std::vector<burn::Gui*>;

namespace burn {

class BURNGINE_API GuiNode {
public:
	GuiNode();
	virtual ~GuiNode();

	virtual void draw() = 0;

	void setPosition(const Vector2f& position);
	const Vector2f& getPosition() const;

	void setRotation(const float& rotation);
	const float& getRotation() const;

	void rotate(const float& angle);
	void move(const Vector2f& offset);
	void move(const float& offsetX, const float& offsetY);

	void setZIndex(const Int32& zIndex);
	const Int32& getZIndex() const;

	void addParentGui(Gui* parent);
	void removeParentGui(Gui* parent);

protected:
	Vector2f _position;
	float _rotation;
	Int32 _zIndex;
	std::vector<Gui*> _parents;
};

} /* namespace burn */
#endif /* GUINODE_H_ */
