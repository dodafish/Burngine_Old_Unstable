/*
 * ReferenceCounter.h
 *
 *  Created on: 13.10.2013
 *      Author: Dominik
 */

#ifndef REFERENCECOUNTER_H_
#define REFERENCECOUNTER_H_

#include "../Export.h"

namespace burn {

class BURNGINE_API ReferenceCounter {
public:
	//Ctor
	ReferenceCounter();

	//CopyCtor
	ReferenceCounter(const ReferenceCounter& other);

	//MoveCtor
	ReferenceCounter(const ReferenceCounter&& other);

	//Dtor
	~ReferenceCounter();

	//CopyAssignment
	ReferenceCounter& operator=(const ReferenceCounter& other);

	//MoveAssignment
	ReferenceCounter& operator=(const ReferenceCounter&& other);

	bool isLastReference() const;

private:
	void checkForDelete();

	unsigned int* _counter;
};

} /* namespace burn */
#endif /* REFERENCECOUNTER_H_ */
