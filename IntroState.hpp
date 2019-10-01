#pragma once
#include "BaseState.hpp"
#include "StateManager.hpp"

class IntroState: public BaseState {
public:
	~IntroState();

	void Continue(EventDetails* deatils);

	void OnCreate() override {
		timePassed = 0.0f;

		sf::Vector2u windowSize = stateManager->GetContext()->window->GetRenderWindow()->getSize();

		introTexture.loadFromFile("intro.png");
		introSprite.setTexture(introTexture);
		introSprite.setOrigin(introTexture.getSize().x / 2.0f, introTexture.getSize().y / 2.0f);
		introSprite.setPosition(windowSize.x / 2.0f, 0);

		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text.setString("Press SPACE to continue");
		text.setCharacterSize(15);
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

		EventManager* eventManager = stateManager->GetContext()->eventManager;
		eventManager->AddCallback(StateType::Intro, "Intro_Continue", &IntroState::Continue, this);

	}

	void OnDestroy() override {
		EventManager* eventManager = stateManager->GetContext()->eventManager;
		eventManager->RemoveCallback(StateType::Intro, "Intro_Continue");
	}

	void Update(const sf::Time& time) override {
		if (timePassed < 5.0) {
			timePassed += time.asSeconds();
			introSprite.setPosition
			(
				introSprite.getPosition().x,
				introSprite.getPosition().y + (48 * time.asSeconds())
			);
		}	
	}

	void Draw() override {
		sf::RenderWindow* window = stateManager->GetContext()->window->GetRenderWindow();
		window->draw(introSprite);
		if (timePassed > 5.0f) {
			window->draw(text);
		}
	}

	void Activate() override {}
	void Deactivate() override {}

private:
	sf::Texture introTexture;
	sf::Sprite introSprite;
	sf::Text text;
	sf::Font font;
	float timePassed;
};

