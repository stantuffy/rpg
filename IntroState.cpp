#include "IntroState.hpp"

IntroState::~IntroState() {
	OnDestroy();
}

void IntroState::Continue(EventDetails* details) {
	if (timePassed > 5.0f) {
		stateManager->SwitchTo(StateType::MainMenu);
		stateManager->Remove(StateType::Intro);
	}
}