#include "InputField.h"

extern sf::Vector2f mousePos;

void InputField::CenterText()
{
	std::cout<<"Started centering text\n";
	text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
	text.setPosition(shape.getPosition().x, shape.getPosition().y);
	std::cout<<"Finished centering text\n";
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
		text.setString(defaultValue);
	if (onEditEnd)
		onEditEnd(value);
	CenterText();
}

InputField::InputField(const std::string& defaultValue)
{
	isHovered = false;
	isSelected = false;
	this->defaultValue = defaultValue;

	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::Black);
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
void InputField::SetText(const std::string& value)
{
	this->value = value;
	text.setString(value);
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

const std::string& InputField::GetText() const
{
	return value;
}
const sf::Vector2f& InputField::GetSize() const
{
	return shape.getSize();
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

	// If the input field is selected and the user presses ctrl + v then paste the clipboard text
	if (isSelected && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
	{
		sf::Clipboard clipboard;
		value = clipboard.getString();
		text.setString(value);
		CenterText();
	}

	// If the input field is selected and the user presses ctrl + c then copy the text to the clipboard
	if (isSelected && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
	{
		sf::Clipboard clipboard;
		clipboard.setString(value);
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