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

#include <Burngine/System/Message.h>
#include <Burngine/System/MessageReceiver.h>

namespace burn {

std::function<void(const Message&)> Message::_sendFunc = std::bind(	&MessageReceiver::sendMessageToAllReceivers,
																	std::placeholders::_1);

Message::Message(const Name& name) :
_name(name) {

}

Message::Message(const std::string& name) :
_name(name) {

}

void Message::send() {
	lock();
	_sendFunc(*this);
}

void Message::lock() {
	for(size_t i = 0; i < _parameters.size(); ++i)
		_parameters[i]->lock();
}

const Name& Message::getName() const{
	return _name;
}


} /* namespace burn */
