#include "EventManager.hpp"



EventManager::EventManager() {
	windowHasFocus = true;
	LoadBindings();
}


EventManager::~EventManager() {
	for (auto& iterator : bindings) {
		delete iterator.second;
		iterator.second = nullptr;
	}
}

bool EventManager::AddBinding(Binding* binding) {
	if (bindings.find(binding->name) != bindings.end()) {
		return false;
	}
	return bindings.emplace(binding->name, binding).second; //<iterator, bool>
}

bool EventManager::RemoveBinding(std::string name) {
	auto itr = bindings.find(name);
	if (itr == bindings.end()) {
		return false;
	}
	delete itr->second;
	bindings.erase(itr);
	return true;
}

void EventManager::SetFocus(const bool& focus) {
	windowHasFocus = focus;
}

void EventManager::HandleEvent(sf::Event& currentEvent) {
	//Handling SFML events
	for (auto& bind : bindings) {
		Binding* currentBinding = bind.second;
		for (auto& event : currentBinding->events) {
			EventType sfmlEvent = (EventType)currentEvent.type;

			if (event.first != sfmlEvent) {
				continue;
			}
			if (sfmlEvent == EventType::KeyPressed || sfmlEvent == EventType::KeyReleased) {
				if (event.second.code == currentEvent.key.code) {
					// Matching event/keystroke
					// Increase count
					if (currentBinding->details.keyCode != -1) {
						currentBinding->details.keyCode = event.second.code;
					}
					++(currentBinding->eventCounter);
					break;
				}
			}
			else if (sfmlEvent == EventType::MouseButtonPressed || sfmlEvent == EventType::MouseButtonReleased) {
				if (event.second.code == currentEvent.mouseButton.button) {
					// Matching event/keystroke
					// Increase count
					currentBinding->details.mouse.x = currentEvent.mouseButton.x;
					currentBinding->details.mouse.y = currentEvent.mouseButton.y;
					if (currentBinding->details.keyCode != -1) {
						currentBinding->details.keyCode = event.second.code;
					}
					++(currentBinding->eventCounter);
					break;
				}
			}
			else {
				if (sfmlEvent == EventType::MouseWheelMoved) {
					currentBinding->details.mouseWheelDelta = currentEvent.mouseWheel.delta;
				}
				else if (sfmlEvent == EventType::WindowResized) {
					currentBinding->details.size.x = currentEvent.size.width;
					currentBinding->details.size.y = currentEvent.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered) {
					currentBinding->details.textEntered = currentEvent.text.unicode;
				}
				++(currentBinding->eventCounter);
			}
		}
	}
}

void EventManager::Update() {
	if (!windowHasFocus)
		return;

	for (auto& bind : bindings) {
		Binding* currentBinding = bind.second;
		for (auto& event : currentBinding->events) {
			switch (event.first) {
				case EventType::Keyboard:
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(event.second.code))) {
						if (currentBinding->details.keyCode != -1) {
							currentBinding->details.keyCode = event.second.code;
						}
						++(currentBinding->eventCounter);
					}
					break;
				case EventType::Mouse:
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button(event.second.code))) {
						if (currentBinding->details.keyCode != -1) {
							currentBinding->details.keyCode = event.second.code;
						}
						++(currentBinding->eventCounter);
					}
					break;
				case EventType::Joystick:
					break;
				default:
					break;
			}

			if (currentBinding->events.size() == currentBinding->eventCounter) {
				auto stateCallbacks = callbacks.find(currentState);
				auto otherCallbacks = callbacks.find(StateType(0));

				if (stateCallbacks != callbacks.end()) {
					// Call function
					auto function = stateCallbacks->second.find(currentBinding->name);
					if (function != stateCallbacks->second.end()) {
						function->second(&currentBinding->details);
					}
				}

				if (otherCallbacks != callbacks.end()) {
					auto function = otherCallbacks->second.find(currentBinding->name);
					if (function != otherCallbacks->second.end()) {
						function->second(&currentBinding->details);
					}
				}
			}
			currentBinding->eventCounter = 0;
			currentBinding->details.Clear();
		}
	}
}

void EventManager::LoadBindings() {
	std::string delimiter = ":";

	std::ifstream bindingList;
	bindingList.open("keys.cfg");

	if (!bindingList.is_open()) {
		std::cout << "Failed opening keys.cfg." << std::endl;
		return;
	}

	std::string line;
	while (std::getline(bindingList, line)) {
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;

		Binding* binding = new Binding(callbackName);
		while (!keystream.eof()) {
			std::string keyval;
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos) {
				delete binding;
				binding = nullptr;
				break;
			}

			EventType type = EventType(stoi(keyval.substr(start, end - start)));
			int code = stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));
			EventInfo eventInfo;
			eventInfo.code = code;

			binding->BindEvent(type, eventInfo);
		}

		if (!AddBinding(binding)) {
			delete binding;
		}
		binding = nullptr;
	}
	bindingList.close();
}

void EventManager::SetCurrentState(const StateType& state) {
	currentState = state;
}

