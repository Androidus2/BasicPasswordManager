#include "PasswordManagerUI.h"
#include "PasswordManager.h"

extern PasswordManager* passwordManager;
extern sf::Vector2i windowSize;
extern std::string possibleChars;
extern bool exitProgram;

PasswordManagerUI::PasswordManagerUI(sf::Font* font) {
	passwordFilePath = "passwords.txt";
	this->font = font;
	topBar.setSize(sf::Vector2f(windowSize.x, 75));
	topBar.setFillColor(sf::Color(0, 0, 0, 255));
	topBar.setPosition(0, 0);
	topBarText.setFont(*font);
	topBarText.setString("Password Manager");
	topBarText.setCharacterSize(24);
	topBarText.setFillColor(sf::Color(255, 255, 255, 255));
	topBarText.setPosition(10, 10);

	addButton.SetPosition(windowSize.x - 200, 30);
	std::cout << windowSize.x << std::endl;
	addButton.SetSize(75, 50);
	addButton.SetText("Add", *font, 20, sf::Color::White);
	addButton.SetColor(sf::Color(200, 200, 200));
	addButton.SetHoverColor(sf::Color(150, 150, 150));
	addButton.SetClickColor(sf::Color(100, 100, 100));
	addButton.SetOnClick([this]() {
		AddPasswordRow();
	});

	saveButton.SetPosition(windowSize.x - 100, 30);
	saveButton.SetSize(75, 50);
	saveButton.SetText("Save", *font, 20, sf::Color::White);
	saveButton.SetColor(sf::Color(200, 200, 200));
	saveButton.SetHoverColor(sf::Color(150, 150, 150));
	saveButton.SetClickColor(sf::Color(100, 100, 100));
	saveButton.SetOnClick([this]() {
		SavePasswords();
	});

	LoadPasswords();
}

void PasswordManagerUI::AddPasswordRow() {
	passwordRows.push_back(new PasswordRow(font, passwordRows.size()));

	float position = 50;
	if (passwordRows.size() > 1) {
		position = passwordRows[passwordRows.size() - 2]->GetPosition() + 50;
	}
	passwordRows.back()->SetPosition(position);

	passwordRows.back()->SetDeleteButtonCallback([this](int index) {
		RemovePasswordRow(index);
	});
}
void PasswordManagerUI::RemovePasswordRow(int index) {
	// if it is a valid index, remove it and update the rest of the rows
	if (index >= 0 && index < passwordRows.size()) {
		delete passwordRows[index];
		passwordRows.erase(passwordRows.begin() + index);
		for (int i = index; i < passwordRows.size(); i++) {
			passwordRows[i]->SetPosition(passwordRows[i]->GetPosition() - 50);
		}
	}
}

void PasswordManagerUI::SavePasswords() {
	passwordManager->RemoveAllPasswords();
	for (int i = 0; i < passwordRows.size(); i++) {
		passwordManager->AddPassword(passwordRows[i]->GetPassword(), passwordRows[i]->GetDescription());
	}
	if (passwordManager && passwordManager->SavePasswords(passwordFilePath)) {
		std::cout << "Passwords saved successfully" << std::endl;
	}
	else {
		std::cout << "Failed to save passwords" << std::endl;
	}
}
void PasswordManagerUI::LoadPasswords() {
	if (passwordManager && passwordManager->LoadPasswords(passwordFilePath)) {
		bool ok = true;
		for (int i = 0; i < passwordManager->GetPasswordsCount(); i++)
		{
			const std::string& password = passwordManager->GetPassword(i);
			const std::string& description = passwordManager->GetDescription(i);
			//std::cout << "Password: " << password << " " << password.size() << std::endl;
			for (int j = 0; j < password.size(); j++)
			{
				if (possibleChars.find(password[j]) == std::string::npos)
				{
					std::cout<<(int)password[j]<<std::endl;
					ok = false;
					break;
				}
			}
			for (int j = 0; j < description.size(); j++)
			{
				if (description[j] > 128 || description[j] < 32)
				{
					ok = false;
					break;
				}
			}
		}

		if (!ok)
		{
			std::cout << "Invalid master key!" << std::endl;
			exitProgram = true;
			return;
		}

		for (int i = 0; i < passwordManager->GetPasswordsCount(); i++) {
			passwordRows.push_back(new PasswordRow(font, i));
			passwordRows.back()->SetPosition((passwordRows.size() + 1) * 50);
			passwordRows.back()->SetDeleteButtonCallback([this](int index) {
				RemovePasswordRow(index);
			});
			passwordRows.back()->SetPassword(passwordManager->GetPassword(i));
			passwordRows.back()->SetDescription(passwordManager->GetDescription(i));
		}
		std::cout << "Number of passwords: " << passwordManager->GetPasswordsCount() << std::endl;
		std::cout << "Passwords loaded successfully" << std::endl;
	}
	else {
		std::cout << "Failed to load passwords, generating key!" << std::endl;
		passwordManager->GenerateKey();
	}
}

void PasswordManagerUI::OnResize() {
	topBar.setSize(sf::Vector2f(windowSize.x, 75));
	addButton.SetPosition(windowSize.x - 200, 30);
	saveButton.SetPosition(windowSize.x - 100, 30);
	for (int i = 0; i < passwordRows.size(); i++) {
		passwordRows[i]->OnResize();
	}
}

void PasswordManagerUI::HandleInput(sf::Event& event) {

	bool ok = false;
	float delta = 0;
	// if the user scrolls, move the rows
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		delta = event.mouseWheelScroll.delta * 10;
		ok = true;
	}

	// if the user presses the up or down arrow keys, move the rows
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Up)
		{
			ok = true;
			delta = 10;
		}
		if (event.key.code == sf::Keyboard::Down)
		{
			ok = true;
			delta = -10;
		}
	}

	if (ok)
	{
		if (passwordRows.size() > 0 && passwordRows[0]->GetPosition() + delta <= 100 && passwordRows.back()->GetPosition() + delta >= 100)
		{
			for (int i = 0; i < passwordRows.size(); i++)
				passwordRows[i]->SetPosition(passwordRows[i]->GetPosition() + delta);
		}
		std::cout << passwordRows[0]->GetPosition() + delta << std::endl;
		std::cout << passwordRows.back()->GetPosition() + delta << std::endl;
	}


	addButton.HandleInput(event);
	saveButton.HandleInput(event);

	for (int i = 0; i < passwordRows.size(); i++) {
		passwordRows[i]->HandleInput(event);
	}
}
void PasswordManagerUI::Update(float deltaTime) {
	for (int i = 0; i < passwordRows.size(); i++) {
		passwordRows[i]->Update(deltaTime);
	}
	addButton.Update(deltaTime);
	saveButton.Update(deltaTime);
}
void PasswordManagerUI::Draw(sf::RenderWindow& window) const {
	for (int i = passwordRows.size() - 1; i >= 0; --i) {
		passwordRows[i]->Draw(window);
	}
	window.draw(topBar);
	window.draw(topBarText);
	saveButton.Draw(window);
	addButton.Draw(window);
}

PasswordManagerUI::~PasswordManagerUI() {
	for(int i = 0; i < passwordRows.size(); i++) {
		delete passwordRows[i];
	}
	passwordRows.clear();
}