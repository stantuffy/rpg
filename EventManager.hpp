#pragma once
#include "SFML/Graphics.hpp"
#include "BaseState.hpp"
#include <unordered_map>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>

// The event manager must have:
// -The ability to couple any mix of keys, buttons or events with desired functionality identified by a string
// -Binding of the said functionalities to methods that get called if all the conditions
//  (such as key being pressed, the left mouse being clicked...) for the binding are satisfied
// -A way through which the event manager can deal with actual SFML events being polled
// -Loading the bindings from a configuration file

enum class EventType;
struct EventInfo;
struct EventDetails;
struct Binding;

// Using a vector for duplicates
using Events = std::vector<std::pair<EventType, EventInfo>>;
// Using a map guarantees that there will only be one binding per action
using Bindings = std::unordered_map<std::string, Binding*>;
// Always one callback per action
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
// With this there is only one CallbackContainer per state in addition to only one callback
// function per name. Without this, the same key or event will be needed by two or more states.
// Callbacks from all states will be invoked at the same time and create weird behaviour.
using Callbacks = std::unordered_map<StateType, CallbackContainer>;


// I'm only interested in a few keys that the sf::Event union provides
enum class EventType {
	KeyPressed = sf::Event::KeyPressed,
	KeyReleased = sf::Event::KeyReleased,
	MouseButtonPressed = sf::Event::MouseButtonPressed,
	MouseButtonReleased = sf::Event::MouseButtonReleased,
	MouseWheelMoved = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

// In order to bind to a key, I need both the event type
// and the code for the key that I am interested in.
struct EventInfo {
	EventInfo() {
		code = 0;
	}

	EventInfo(int code) {
		this->code = code;
	}

	union {
		int code;
	};
};

struct EventDetails {
	EventDetails(const std::string bindName) {
		this->bindName = bindName;
		Clear();
	}

	std::string bindName;
	sf::Vector2i size;
	sf::Uint32 textEntered;
	sf::Vector2i mouse;
	int mouseWheelDelta;
	int keyCode;

	void Clear() {
		size = sf::Vector2i(0, 0);
		textEntered = 0;
		mouse = sf::Vector2i(0, 0);
		mouseWheelDelta = 0;
		keyCode = -1;
	}
};

struct Binding {

	Binding(const std::string name) : name(name), details(name), eventCounter(0) {}

	void BindEvent(EventType eventType, EventInfo eventInfo = EventInfo()) {
		events.emplace_back(eventType, eventInfo);
	}

	Events events;
	std::string name;
	int eventCounter;

	EventDetails details;
};


class EventManager
{
public:
	EventManager();
	~EventManager();

	bool AddBinding(Binding* binding);
	bool RemoveBinding(std::string name);

	void SetFocus(const bool& focus);

	template <class T>
	bool AddCallback(StateType state, const std::string& name, void(T::*function)(EventDetails*), T* instance) {
		auto itr = callbacks.emplace(state, CallbackContainer()).first;
		auto func = std::bind(function, instance, std::placeholders::_1);
		return itr->second.emplace(name, func).second;
	}

	bool RemoveCallback(StateType state, const std::string& name) {
		auto itr = callbacks.find(state);
		if (itr == callbacks.end())
			return false;

		auto itr2 = itr->second.find(name);
		if (itr2 == itr->second.end())
			return false;
		itr->second.erase(name);
		return true;
	}

	void HandleEvent(sf::Event& event);
	void Update();

	sf::Vector2i GetMousePosition(sf::RenderWindow* window = nullptr) {
		return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition());
	}

	void SetCurrentState(const StateType& state);

private:
	// Load the information about the bindings in a file
	void LoadBindings();

	Bindings bindings;
	Callbacks callbacks;
	StateType currentState;
	bool windowHasFocus;
};

