#pragma once
#include "SharedContext.hpp"
#include "StateManager.hpp"

class Game
{
public:
	Game();
	~Game();

	void HandleInput();
	void Update();
	void LateUpdate();
	void Render();
	void RestartClock();
	Window* GetWindow();
	sf::Time GetElapsed();

	//test
	void Click(EventDetails* details) {
		sf::Vector2i mousepos = mainWindow.GetEventManager()->GetMousePosition(mainWindow.GetRenderWindow());
		std::cout << "MOVING TO:" << mousepos.x << ":" << mousepos.y << std::endl;
	}

	//test
	void HitSpace(EventDetails* details) {
		std::cout << "SPACE BAR HIT!";
	}

private:
	Window mainWindow;
	StateManager stateManager;
	SharedContext shared;

	sf::Clock clock;
	sf::Time elapsed;
};

