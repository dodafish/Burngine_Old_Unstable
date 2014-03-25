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

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Parameter.h>

#include <vector>
#include <functional>

namespace burn {

class BURNGINE_API Message {
public:
	explicit Message(const Name& name);
	explicit Message(const std::string& name);

	template<typename T>
	void addParameter(	const std::string& name,
						const T& value);

	template<class T>
	bool getParameter(	const Name& name,
						T* var) const;

	template<typename T>
	bool getParameter(	const std::string& name,
						T* var) const;

	void send();
	void lock();

	const Name& getName() const;

private:
	Name _name;
	std::vector<std::shared_ptr<BaseParameter> > _parameters;

	static std::function<void(const Message&)> _sendFunc;
};

template<typename T>
void Message::addParameter(	const std::string& name,
							const T& value) {
	_parameters.push_back(std::shared_ptr<BaseParameter>(new Parameter<T>(name, value)));
}

template<class T>
bool Message::getParameter(	const Name& name,
							T* var) const {
	for(size_t i = 0; i < _parameters.size(); ++i){
		if(_parameters[i]->getName() == name){
			BaseParameter* bp = _parameters[i].get();
			Parameter<T>* p = static_cast<Parameter<T>*>(bp);
			*var = p->getValue();
			return true;
		}
	}
	return false;
}

template<typename T>
bool Message::getParameter(	const std::string& name,
							T* var) const {
	for(size_t i = 0; i < _parameters.size(); ++i){
		if(_parameters[i]->getName() == name){
			BaseParameter* bp = _parameters[i].get();
			Parameter<T>* p = static_cast<Parameter<T>*>(bp);
			*var = p->getValue();
			return true;
		}
	}
	return false;
}

} /* namespace burn */
#endif /* MESSAGE_H_ */
