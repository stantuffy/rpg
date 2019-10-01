#include "Game.hpp"



Game::Game() : 
	mainWindow("Untitled Game", sf::Vector2u(1920, 1080)),
	stateManager(&shared) {
	shared.window = &mainWindow;
	shared.eventManager = mainWindow.GetEventManager();
	stateManager.SwitchTo(StateType::Intro);
	mainWindow.GetEventManager()->AddCallback(StateType(0), "Click", &Game::Click, this);
	mainWindow.GetEventManager()->AddCallback(StateType(0), "Space", &Game::HitSpace, this);
}


Game::~Game()
{
}

void Game::HandleInput() {

}

void Game::Update() {
	mainWindow.Update();
	stateManager.Update(elapsed);
}

void Game::LateUpdate() {
	stateManager.ProcessRequests();
	RestartClock();
}

void Game::Render() {
	mainWindow.BeginDraw();
	stateManager.Draw();
	//mainWindow.Draw();
	mainWindow.EndDraw();
}

void Game::RestartClock() {
	elapsed = clock.restart();
}

Window* Game::GetWindow() {
	return &(mainWindow);
}

sf::Time Game::GetElapsed() {
	return elapsed;
}
