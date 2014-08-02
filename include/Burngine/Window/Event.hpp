/*
 * Event.hpp
 *
 *  Created on: 02.08.2014
 *      Author: dominik
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/Keyboard.hpp>

namespace burn {

/**
 * @brief Handles window events
 */
class BURNGINE_API_EXPORT Event {
public:

	/**
	 * @brief All event types
	 */
	enum Type{
		UNKNOWN_EVENT = 0, ///< Unkown window event (or not handled)
		CLOSED, ///< Window shall close (e.g. Alt+F4)
		KEY_PRESSED, ///< A key was pressed (data: getKey())
		KEY_RELEASED ///< A key was released (data: getKey())
	};

	/**
	 * @brief Default constructor.
	 * Event type is by default UNKNOWN
	 * Do not use this constructor, as the type
	 * can not be set after instanciation.
	 * Use Event(const Type& type);
	 */
	Event();

	/**
	 * @brief Create Event with a fixed type.
	 *
	 * @param type Event's type
	 */
	Event(const Type& type);

	const Type& getType() const;

	void setKey(const Keyboard::Key& key);

	const Keyboard::Key& getKey() const;

private:

	Type m_type; ///< Event type
	Keyboard::Key m_key; ///< The pressed/released key

};

} /* namespace burn */

#endif /* EVENT_HPP_ */