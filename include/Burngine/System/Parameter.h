//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <Burngine/Export.h>
#include <memory>

#include <Burngine/System/BaseParameter.h>

namespace burn {

template<class T>
class BURNGINE_API Parameter : public BaseParameter {
public:
	Parameter(	const Name& name,
				const T& value);
	Parameter(	const std::string& name,
				const T& value);

	void setValue(const T& value);

	const T& getValue() const;

private:
	std::shared_ptr<T> _value;
};

///////////////////////////////////////////////////////////////

template<class T>
Parameter<T>::Parameter(const Name& name,
						const T& value) :
BaseParameter(name),
_value(new T(value)) {

}

template<class T>
Parameter<T>::Parameter(const std::string& name,
						const T& value) :
BaseParameter(name),
_value(new T(value)) {

}

template<class T>
void Parameter<T>::setValue(const T& value) {
	if(!_isLocked)
		_value.reset(new T(value));
}

template<class T>
const T& Parameter<T>::getValue() const {
	return (*_value);
}

} /* namespace burn */
#endif /* PARAMETER_H_ */
