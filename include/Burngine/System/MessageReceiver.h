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

#ifndef MESSAGERECEIVER_H_
#define MESSAGERECEIVER_H_

#include <Burngine/Export.h>

#include <vector>
#include <functional>

namespace burn {

class Message;

class BURNGINE_API MessageReceiver {
public:
	MessageReceiver();
	MessageReceiver(const MessageReceiver& other);
	MessageReceiver& operator=(const MessageReceiver& other);
	virtual ~MessageReceiver();

	void takeMessage(const Message& msg);
	void bindReceiveFunction(const std::function<void(const Message&)>& function);

	static void sendMessageToAllReceivers(const Message& msg);
private:
	std::function<void(const Message&)> _receiveFunction;
	static std::vector<MessageReceiver*> _receivers;
};

} /* namespace burn */
#endif /* MESSAGERECEIVER_H_ */
