/*
 * Material.h
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../Export.h"

namespace burn {

class BURNGINE_API Material {
public:
	Material();
	~Material();

	enum Flag {
		LIGHTING = 0,

		COUNT //Keep last!
	};

	enum Type {
		SOLID_COLOR = 0, //Default
		TEXTURED
	};

	void setFlag(Flag flag, bool enabled = true);
	void setType(Type type);

	const Type& getType() const;
	bool isFlagSet(Flag flag) const;

private:
	Type _type;
	bool _flags[COUNT];
};

} /* namespace burn */
#endif /* MATERIAL_H_ */
