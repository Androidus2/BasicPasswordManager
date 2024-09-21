#include "PasswordRow.h"

extern sf::Vector2i windowSize;
extern sf::Vector2f mousePos;

void PasswordRow::PositionElements()
{
	background.setPosition(windowSize.x / 2, yPosition);
	descriptionField.SetPosition(10 + descriptionField.GetSize().x / 2, yPosition);
	passwordField.SetPosition(windowSize.x / 2 - 30, yPosition);
	copyButton.SetPosition(windowSize.x - 220, yPosition);
	generateButton.SetPosition(windowSize.x - 140, yPosition);
	deleteButton.SetPosition(windowSize.x - 70, yPosition);
}

PasswordRow::PasswordRow(sf::Font* font, int index) : descriptionField("Enter description:"), passwordField("Enter password:")
{
	sf::Color fieldColor(200, 200, 200);
	sf::Color fieldHoverColor(150, 150, 150);
	sf::Color fieldActiveColor(100, 100, 100);

	this->font = font;
	this->index = index;
	yPosition = 0;

	background.setOrigin(windowSize.x / 2, 25);
	background.setSize(sf::Vector2f(windowSize.x, 50));
	background.setFillColor(sf::Color(210, 210, 210));
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(1);

	descriptionField.SetPosition(10, 10);
	descriptionField.SetSize(windowSize.x / 4, 30);
	descriptionField.SetText("Enter description:", *font, 20, sf::Color::White);
	descriptionField.SetColor(fieldColor);
	descriptionField.SetHoverColor(fieldHoverColor);
	descriptionField.SetSelectedColor(fieldActiveColor);

	passwordField.SetPosition(220, 10);
	passwordField.SetSize(windowSize.x * 3 / 8, 30);
	passwordField.SetText("Enter password:", *font, 20, sf::Color::White);
	passwordField.SetColor(fieldColor);
	passwordField.SetHoverColor(fieldHoverColor);
	passwordField.SetSelectedColor(fieldActiveColor);

	deleteButton.SetPosition(530, 10);
	deleteButton.SetSize(30, 30);
	deleteButton.SetText("X", *font, 20, sf::Color::White);
	deleteButton.SetColor(fieldColor);
	deleteButton.SetHoverColor(fieldHoverColor);
	deleteButton.SetClickColor(fieldActiveColor);
	deleteButton.SetOutline(2, sf::Color::Black);
	deleteButton.SetOnClick([]() { std::cout << "Delete button clicked" << std::endl; });

	generateButton.SetPosition(570, 10);
	generateButton.SetSize(70, 30);
	generateButton.SetText("Generate", *font, 16, sf::Color::White);
	generateButton.SetColor(fieldColor);
	generateButton.SetHoverColor(fieldHoverColor);
	generateButton.SetClickColor(fieldActiveColor);
	generateButton.SetOutline(2, sf::Color::Black);
	InputField* passwordFieldPtr = &passwordField;
	generateButton.SetOnClick([passwordFieldPtr]() {
		std::cout << "Generate button clicked: " << std::endl;
		std::string possibleChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";
		std::string newPassword;
		int length = 16;
		for (int i = 0; i < length; i++)
		{
			newPassword += possibleChars[rand() % possibleChars.size()];
		}
		std::cout<< "Generated password: " << newPassword << std::endl;
		passwordFieldPtr->SetText(newPassword);
		std::cout<< "Password field text: " << passwordFieldPtr->GetText() << std::endl;
	});

	copyButton.SetPosition(610, 10);
	copyButton.SetSize(50, 30);
	copyButton.SetText("Copy", *font, 16, sf::Color::White);
	copyButton.SetColor(fieldColor);
	copyButton.SetHoverColor(fieldHoverColor);
	copyButton.SetClickColor(fieldActiveColor);
	copyButton.SetOutline(2, sf::Color::Black);
	copyButton.SetOnClick([passwordFieldPtr]() {
		std::cout << "Copy button clicked: " << passwordFieldPtr->GetText() << std::endl;
		sf::Clipboard::setString(passwordFieldPtr->GetText());
	});

	PositionElements();
}

void PasswordRow::SetPosition(float y)
{
	yPosition = y;
	PositionElements();
}
float PasswordRow::GetPosition() const
{
	return yPosition;
}
void PasswordRow::SetDescription(const std::string& description)
{
	descriptionField.SetText(description);
}
const std::string& PasswordRow::GetDescription() const
{
	return descriptionField.GetText();
}

void PasswordRow::SetPassword(const std::string& password)
{
	passwordField.SetText(password);
}
const std::string& PasswordRow::GetPassword() const
{
	return passwordField.GetText();
}

void PasswordRow::SetDeleteButtonCallback(std::function<void(int)> callback)
{
	deleteButton.SetOnClick([this, callback]() {
		callback(index);
	});
}

int PasswordRow::GetIndex() const
{
	return index;
}

void PasswordRow::OnResize()
{
	background.setSize(sf::Vector2f(windowSize.x, 50));
	background.setOrigin(windowSize.x / 2, 25);

	descriptionField.SetSize(windowSize.x / 4, 30);
	passwordField.SetSize(windowSize.x * 3 / 8, 30);

	PositionElements();
}

void PasswordRow::HandleInput(sf::Event& event)
{
	// if the mouse is over the row, change the color of the background a bit
	if (background.getGlobalBounds().contains(mousePos))
	{
		background.setFillColor(sf::Color(230, 230, 230));
	}
	else
	{
		background.setFillColor(sf::Color(210, 210, 210));
	}

	descriptionField.HandleInput(event);
	passwordField.HandleInput(event);
	deleteButton.HandleInput(event);
	generateButton.HandleInput(event);
	copyButton.HandleInput(event);
}
void PasswordRow::Update(float deltaTime)
{
	descriptionField.Update(deltaTime);
	passwordField.Update(deltaTime);
	deleteButton.Update(deltaTime);
	generateButton.Update(deltaTime);
	copyButton.Update(deltaTime);
}
void PasswordRow::Draw(sf::RenderWindow& window) const
{
	window.draw(background);
	copyButton.Draw(window);
	generateButton.Draw(window);
	deleteButton.Draw(window);
	passwordField.Draw(window);
	descriptionField.Draw(window);
}