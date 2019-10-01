#pragma once
#include "SFML/Graphics.hpp"
#include "EventManager.hpp"

class Window {
public:
	Window(const std::string title, const sf::Vector2u size);
	~Window();

	void BeginDraw();
	void EndDraw();

	void Update();
	void Close(EventDetails* details);

	bool IsDone();
	bool IsFullscreen();
	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();
	EventManager* GetEventManager();

	void ToggleFullscreen(EventDetails* details);
	
	void Draw(sf::Drawable& drawable);

private:
	void Setup(const std::string title, const sf::Vector2u size);
	void Destroy();
	void Create();

	sf::RenderWindow window;
	sf::Vector2u windowSize;
	std::string windowTitle;
	bool isDone;
	bool isFullscreen;
	bool isFocused;

	EventManager eventManager;
};

