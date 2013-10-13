/*
 * ReferenceCounter.cpp
 *
 *  Created on: 13.10.2013
 *      Author: Dominik
 */

#include "ReferenceCounter.h"

namespace burn {

//Ctor
ReferenceCounter::ReferenceCounter() {
	_counter = new unsigned int(1);
}

//CopyCtor
ReferenceCounter::ReferenceCounter(const ReferenceCounter& other) {
	_counter = other._counter;
	++(*_counter);
}

//MoveCtor
ReferenceCounter::ReferenceCounter(const ReferenceCounter&& other) {
	_counter = other._counter;
}

//Dtor
ReferenceCounter::~ReferenceCounter() {

	--(*_counter);
	checkForDelete();

}

//CopyAssignment
ReferenceCounter& ReferenceCounter::operator=(const ReferenceCounter& other) {
	--(*_counter);
	checkForDelete();
	_counter = other._counter;
	++(*_counter);

	return *this;
}

//MoveAssignment
ReferenceCounter& ReferenceCounter::operator=(const ReferenceCounter&& other) {
	--(*_counter);
	checkForDelete();
	_counter = other._counter;

	return *this;
}

void ReferenceCounter::checkForDelete() {
	if(*_counter == 0){
		delete _counter;
	}
}

bool ReferenceCounter::isLastReference() const {
	return (*_counter == 1);
}

} /* namespace burn */
