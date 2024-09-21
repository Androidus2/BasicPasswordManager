#include "Button.h"

extern sf::Vector2f mousePos;
extern bool blockClick;

void Button::CenterText()
{
	text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
	text.setPosition(shape.getPosition().x, shape.getPosition().y);
}

Button::Button()
{
	isHovered = false;
	isClicked = false;
}

void Button::SetPosition(float x, float y)
{
	shape.setPosition(x, y);
	text.setPosition(x, y);
}
void Button::SetSize(float width, float height)
{
	shape.setSize(sf::Vector2f(width, height));
	shape.setOrigin(width / 2, height / 2);

	CenterText();
}
void Button::SetText(const std::string& str, const sf::Font& font, unsigned int size, const sf::Color& color)
{
	text.setString(str);
	text.setFont(font);
	text.setCharacterSize(size);
	text.setFillColor(color);

	CenterText();
}
void Button::SetColor(const sf::Color& color)
{
	this->color = color;
	shape.setFillColor(color);
}
void Button::SetHoverColor(const sf::Color& color)
{
	hoverColor = color;
}
void Button::SetClickColor(const sf::Color& color)
{
	clickColor = color;
}
void Button::SetOutline(float thickness, const sf::Color& color)
{
	shape.setOutlineThickness(thickness);
	shape.setOutlineColor(color);
}
void Button::SetOnClick(std::function<void()> onClick)
{
	this->onClick = onClick;
}

void Button::HandleInput(sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		if (shape.getGlobalBounds().contains(mousePos))
			isHovered = true;
		else
			isHovered = false;
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left && isHovered)
			isClicked = true;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left && isClicked)
		{
			isClicked = false;
			if (onClick && !blockClick)
			{
				blockClick = true;
				onClick();
			}
		}
	}
}
void Button::Update(float deltaTime)
{
	if (isClicked)
		shape.setFillColor(clickColor);
	else if (isHovered)
		shape.setFillColor(hoverColor);
	else
		shape.setFillColor(color);
}
void Button::Draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}

bool Button::IsHovered() const
{
	return isHovered;
}
bool Button::IsClicked() const
{
	return isClicked;
}

Button::~Button()
{
}