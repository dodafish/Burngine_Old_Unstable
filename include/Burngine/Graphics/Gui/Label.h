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
#include <Burngine/System/Math.h>

namespace burn {

class BURNGINE_API Label : public Text {
public:
	Label();
	~Label();

	virtual void draw();

	void setBackgroundColor(const Vector4f& color);
	const Vector4f& getBackgroundColor() const;

	void setBorder(const float& border);
	const float& getBorder() const;

private:
	Vector4f _backgroundColor;
	float _border;
};

} /* namespace burn */
#endif /* LABEL_H_ */
