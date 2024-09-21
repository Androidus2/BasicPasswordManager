#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <functional>

class InputField
{
private:
	sf::RectangleShape shape;
	std::string value;
	std::string defaultValue;
	sf::Text text;
	sf::Color color;
	sf::Color hoverColor;
	sf::Color selectedColor;
	bool isHovered;
	bool isSelected;
	std::function<void(const std::string&)> onEditStart;
	std::function<void(const std::string&)> onEdit;
	std::function<void(const std::string&)> onEditEnd;

	void CenterText();
	void BeginEdit();
	void EndEdit();
public:
	InputField(const std::string& defaultValue);

	void SetPosition(float x, float y);
	void SetSize(float width, float height);
	void SetText(const std::string& str);
	void SetText(const std::string& str, const sf::Font& font, unsigned int size, const sf::Color& color);
	void SetColor(const sf::Color& color);
	void SetHoverColor(const sf::Color& color);
	void SetSelectedColor(const sf::Color& color);
	void SetOnEditStart(std::function<void(const std::string&)> onEditStart);
	void SetOnEdit(std::function<void(const std::string&)> onEdit);
	void SetOnEditEnd(std::function<void(const std::string&)> onEditEnd);

	const std::string& GetText() const;
	const sf::Vector2f& GetSize() const;

	void HandleInput(sf::Event& event);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

	bool IsHovered() const;
	bool IsSelected() const;

	~InputField();
};