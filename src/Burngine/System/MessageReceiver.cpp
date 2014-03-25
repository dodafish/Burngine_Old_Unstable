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

#include <Burngine/System/MessageReceiver.h>
#include <Burngine/System/Message.h>

namespace burn {

std::vector<MessageReceiver*> MessageReceiver::_receivers;

MessageReceiver::MessageReceiver() {
	_receivers.push_back(this);
}

MessageReceiver::MessageReceiver(const MessageReceiver& other) {
	_receivers.push_back(this);
}

MessageReceiver& MessageReceiver::operator=(const MessageReceiver& other) {

	if(this == &other)
		return *this;

	//Copy if sth would be there to copy

	return *this;
}

MessageReceiver::~MessageReceiver() {
	for(unsigned int i = 0; i < _receivers.size(); ++i)
		if(_receivers[i] == this){
			_receivers.erase(_receivers.begin() + i);
			break;
		}
}

void MessageReceiver::takeMessage(const Message& msg) {
	_receiveFunction(msg);
}

void MessageReceiver::bindReceiveFunction(const std::function<void(const Message&)>& function) {
	_receiveFunction = function;
}

void MessageReceiver::sendMessageToAllReceivers(const Message& msg) {
	for(unsigned int i = 0; i < _receivers.size(); ++i)
		_receivers[i]->takeMessage(msg);
}

} /* namespace burn */
