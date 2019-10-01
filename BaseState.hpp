#pragma once
#include "SFML/Graphics.hpp"

enum class StateType {
	Intro = 1, MainMenu, Game, Paused, GameOver, Credits
};

class StateManager;

class BaseState {
	friend class StateManager;
public:

	BaseState(StateManager* stateManager) {
		this->stateManager = stateManager;
		transparent = false;
		transcendent = false;
		OnCreate();
	}

	virtual ~BaseState() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& transparent) {
		this->transparent = transparent;
	}

	void SetTranscendent(const bool& transcendent) {
		this->transcendent = transcendent;
	}

	bool isTransparent() {
		return transparent;
	}

	bool isTranscendent() {
		return transcendent;
	}

protected:
	StateManager* stateManager;
	bool transparent;
	bool transcendent;
};

