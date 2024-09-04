#pragma once
#include "Button.h"
#include "InputField.h"
#include "SFML/Graphics.hpp"

class PasswordRow {
private:
	sf::Font* font;
	sf::RectangleShape background;
	InputField descriptionField;
	InputField passwordField;
	Button generateButton;
	Button copyButton;
	Button deleteButton;
	float yPosition;

	int index;

	void PositionElements();

public:
	PasswordRow(sf::Font* font, int index);

	void SetPosition(float y);

	void SetDescription(const std::string& text);
	const std::string& GetDescription() const;

	void SetPassword(const std::string& text);
	const std::string& GetPassword() const;

	void SetDeleteButtonCallback(std::function<void(int)> callback);

	int GetIndex() const;

	void HandleInput(sf::Event& event);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;
};