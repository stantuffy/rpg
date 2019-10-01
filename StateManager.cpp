#include "StateManager.hpp"

StateManager::StateManager(SharedContext* shared) {
	this->shared = shared;
	RegisterState<IntroState>(StateType::Intro);
	//RegisterState<StateMainMenu>(StateType::MainMenu);
	//RegisterState<StateGame>(StateType::Game);
	//RegisterState<StatePaused>(StateType::Paused);
}

StateManager::~StateManager() {
	for (auto& state : states) {
		state.second->OnDestroy();
		delete state.second;
	}
}

// It first checks if the state container has at least one state. If it does
// it checks the most recently added one's transparency flag, as well as
// if there's more than one state on the stack, otherwise the transparency
// would be useless. If there's only one state on the stack or if the current
// state isn't transparent, simply invoke its draw method.

void StateManager::Draw() {
	if (states.empty())
		return;
	if (states.back().second->isTransparent() && states.size() > 1) {
		auto itr = states.end();
		while (itr != states.begin()) {
			if (itr != states.end()) {
				if (!itr->second->isTransparent()) {
					break;
				}
			}
			--itr;
		}
		for (; itr != states.end(); ++itr) {
			itr->second->Draw();
		}
	}
	else {
		states.back().second->Draw();
	}
}

void StateManager::Update(const sf::Time& time) {
	if (states.empty())
		return;
	if (states.back().second->isTransparent() && states.size() > 1) {
		auto itr = states.end();
		while (itr != states.begin()) {
			if (itr != states.end()) {
				if (!itr->second->isTransparent()) {
					break;
				}
			}
			--itr;
		}
		for (; itr != states.end(); ++itr) {
			itr->second->Update(time);
		}
	}
	else {
		states.back().second->Update(time);
	}
}

SharedContext* StateManager::GetContext() {
	return shared;
}

bool StateManager::HasState(const StateType& type) {
	for (auto itr = states.begin(); itr != states.end(); ++itr) {
		if (itr->first == type) {
			auto removed = std::find(toRemove.begin(), toRemove.end(), type);
			if (removed == toRemove.end()) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	return false;
}

void StateManager::Remove(const StateType& type) {
	toRemove.push_back(type);
}

void StateManager::ProcessRequests() {
	while (toRemove.begin() != toRemove.end()) {
		RemoveState(*toRemove.begin());
		toRemove.erase(toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& type) {
	shared->eventManager->SetCurrentState(type);

	for (auto itr = states.begin(); itr != states.end(); ++itr) {
		if (itr->first == type) {
			states.back().second->Deactivate();
			StateType tempType = itr->first;
			BaseState* tempState = itr->second;
			states.erase(itr);
			states.emplace_back(tempType, tempState);
			tempState->Activate();
			return;
		}
	}
	// State type was not found
	if (!states.empty()) {
		states.back().second->Deactivate();
		CreateState(type);
		states.back().second->Activate();
	}
}

void StateManager::CreateState(const StateType& type) {
	auto newState = stateFactory.find(type);
	if (newState == stateFactory.end()) {
		return;
	}
	BaseState* state = newState->second();
	states.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& type) {
	for (auto state = states.begin(); state != states.end(); ++state) {
		if (state->first == type) {
			state->second->OnDestroy();
			delete state->second;
			states.erase(state);
			return;
		}
	}
}