#include "InputField.h"

extern sf::Vector2f mousePos;

void InputField::CenterText()
{
	text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
	text.setPosition(shape.getPosition().x, shape.getPosition().y);
}
void InputField::BeginEdit()
{
	isSelected = true;
	text.setString(value);
	if (onEditStart)
		onEditStart(value);
}
void InputField::EndEdit()
{
	isSelected = false;
	if(value.size() > 0)
		text.setString(value);
	else
		text.setString("Enter password");
	if (onEditEnd)
		onEditEnd(value);
}

InputField::InputField()
{
	isHovered = false;
	isSelected = false;
}

void InputField::SetPosition(float x, float y)
{
	shape.setPosition(x, y);
	text.setPosition(x, y);
}
void InputField::SetSize(float width, float height)
{
	shape.setSize(sf::Vector2f(width, height));
	shape.setOrigin(width / 2, height / 2);

	CenterText();
}
void InputField::SetText(const std::string& str, const sf::Font& font, unsigned int size, const sf::Color& color)
{
	text.setString(str);
	text.setFont(font);
	text.setCharacterSize(size);
	text.setFillColor(color);

	CenterText();
}
void InputField::SetColor(const sf::Color& color)
{
	this->color = color;
	shape.setFillColor(color);
}
void InputField::SetHoverColor(const sf::Color& color)
{
	hoverColor = color;
}
void InputField::SetSelectedColor(const sf::Color& color)
{
	selectedColor = color;
}
void InputField::SetOnEditStart(std::function<void(const std::string&)> onEditStart)
{
	this->onEditStart = onEditStart;
}
void InputField::SetOnEdit(std::function<void(const std::string&)> onEdit)
{
	this->onEdit = onEdit;
}
void InputField::SetOnEditEnd(std::function<void(const std::string&)> onEditEnd)
{
	this->onEditEnd = onEditEnd;
}

void InputField::HandleInput(sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		if (shape.getGlobalBounds().contains(mousePos))
			isHovered = true;
		else
			isHovered = false;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (shape.getGlobalBounds().contains(mousePos))
			BeginEdit();
		else if(isSelected)
			EndEdit();
	}
	else if (event.type == sf::Event::TextEntered)
	{
		if (isSelected)
		{
			if (event.text.unicode == 8)
			{
				if (value.size() > 0)
					value.pop_back();
				if (onEdit)
					onEdit(value);
			}
			else if (event.text.unicode == 13)
			{
				EndEdit();
			}
			else
			{
				value += event.text.unicode;
				if (onEdit)
					onEdit(value);
			}

			text.setString(value);
			CenterText();
		}
	}
}
void InputField::Update(float deltaTime)
{
	if (isSelected)
		shape.setFillColor(selectedColor);
	else if (isHovered)
		shape.setFillColor(hoverColor);
	else
		shape.setFillColor(color);
}
void InputField::Draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}

bool InputField::IsHovered() const
{
	return isHovered;
}
bool InputField::IsSelected() const
{
	return isSelected;
}

InputField::~InputField()
{
}