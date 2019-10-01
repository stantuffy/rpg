#include "Window.hpp"



Window::Window(const std::string title, sf::Vector2u size) {
	this->Setup(title, size);
}


Window::~Window() {
	this->Destroy();
}

void Window::BeginDraw() {
	window.clear(sf::Color::Black);
}

void Window::EndDraw() {
	window.display();
}

void Window::Update() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			isDone = true;
		}
		else if (event.type == sf::Event::GainedFocus) {
			isFocused = true;
			eventManager.SetFocus(true);
		}
		else if (event.type == sf::Event::LostFocus) {
			isFocused = false;
			eventManager.SetFocus(false);
		}
		eventManager.HandleEvent(event);
	}
	eventManager.Update();
}

void Window::Close(EventDetails * details) {
	isDone = true;
}

bool Window::IsDone() {
	return isDone;
}

bool Window::IsFullscreen() {
	return isFullscreen;
}

sf::Vector2u Window::GetWindowSize() {
	return windowSize;
}

sf::RenderWindow* Window::GetRenderWindow() {
	return &(window);
}

EventManager* Window::GetEventManager() {
	return &(eventManager);
}

void Window::ToggleFullscreen(EventDetails* details) {
	isFullscreen = !isFullscreen;
	Destroy();
	Create();
}

void Window::Draw(sf::Drawable & drawable) {
	window.draw(drawable);
}

void Window::Setup(const std::string title, const sf::Vector2u size) {
	windowTitle = title;
	windowSize = size;
	isFullscreen = false;
	isDone = false;
	isFocused = true;

	eventManager.AddCallback(StateType(0), "Fullscreen_toggle", &Window::ToggleFullscreen, this);
	eventManager.AddCallback(StateType(0), "Window_close", &Window::Close, this);
	Create();
}

void Window::Destroy() {
	window.close();
}

void Window::Create() {
	auto style = (isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	window.create( { windowSize.x, windowSize.y, 32 }, windowTitle, style);
}
