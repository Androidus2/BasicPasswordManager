#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <functional>

class Button
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	std::function<void()> onClick;
	sf::Color color;
	sf::Color hoverColor;
	sf::Color clickColor;
	bool isHovered;
	bool isClicked;

	void CenterText();
public:
	Button();

	void SetPosition(float x, float y);
	void SetSize(float width, float height);
	void SetText(const std::string& str, const sf::Font& font, unsigned int size, const sf::Color& color);
	void SetColor(const sf::Color& color);
	void SetHoverColor(const sf::Color& color);
	void SetClickColor(const sf::Color& color);
	void SetOutline(float thickness, const sf::Color& color);
	void SetOnClick(std::function<void()> onClick);

	void HandleInput(sf::Event& event);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

	bool IsHovered() const;
	bool IsClicked() const;

	~Button();
};