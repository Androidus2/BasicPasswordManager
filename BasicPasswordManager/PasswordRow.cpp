#include "PasswordRow.h"


void PasswordRow::PositionElements()
{
	background.setPosition(400, yPosition);
	descriptionField.SetPosition(110, yPosition);
	passwordField.SetPosition(370, yPosition);
	copyButton.SetPosition(580, yPosition);
	generateButton.SetPosition(660, yPosition);
	deleteButton.SetPosition(730, yPosition);
}

PasswordRow::PasswordRow(sf::Font* font, int index) : descriptionField("Enter description:"), passwordField("Enter password:")
{
	sf::Color fieldColor(200, 200, 200);
	sf::Color fieldHoverColor(150, 150, 150);
	sf::Color fieldActiveColor(100, 100, 100);

	this->font = font;
	this->index = index;
	yPosition = 0;

	background.setOrigin(400, 25);
	background.setSize(sf::Vector2f(800, 50));
	background.setFillColor(sf::Color(210, 210, 210));
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(1);

	descriptionField.SetPosition(10, 10);
	descriptionField.SetSize(200, 30);
	descriptionField.SetText("Enter description:", *font, 20, sf::Color::White);
	descriptionField.SetColor(fieldColor);
	descriptionField.SetHoverColor(fieldHoverColor);
	descriptionField.SetSelectedColor(fieldActiveColor);

	passwordField.SetPosition(220, 10);
	passwordField.SetSize(300, 30);
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

void PasswordRow::HandleInput(sf::Event& event)
{
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