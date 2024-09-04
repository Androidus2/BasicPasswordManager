#include "PasswordManagerUI.h"
#include "PasswordManager.h"

extern PasswordManager* passwordManager;

PasswordManagerUI::PasswordManagerUI(sf::Font* font) {
	passwordFilePath = "passwords.txt";
	this->font = font;
	topBar.setSize(sf::Vector2f(800, 75));
	topBar.setFillColor(sf::Color(0, 0, 0, 255));
	topBar.setPosition(0, 0);
	topBarText.setFont(*font);
	topBarText.setString("Password Manager");
	topBarText.setCharacterSize(24);
	topBarText.setFillColor(sf::Color(255, 255, 255, 255));
	topBarText.setPosition(10, 10);

	addButton.SetPosition(600, 30);
	addButton.SetSize(75, 50);
	addButton.SetText("Add", *font, 20, sf::Color::White);
	addButton.SetColor(sf::Color(200, 200, 200));
	addButton.SetHoverColor(sf::Color(150, 150, 150));
	addButton.SetClickColor(sf::Color(100, 100, 100));
	addButton.SetOnClick([this]() {
		AddPasswordRow();
	});

	saveButton.SetPosition(700, 30);
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
	passwordRows.back()->SetPosition((passwordRows.size() + 1) * 50);
	passwordRows.back()->SetDeleteButtonCallback([this](int index) {
		RemovePasswordRow(index);
	});
}
void PasswordManagerUI::RemovePasswordRow(int index) {
	// if it is a valid index, remove it and update the rest of the rows
	if (index >= 0 && index < passwordRows.size()) {
		passwordRows.erase(passwordRows.begin() + index);
		for (int i = index; i < passwordRows.size(); i++) {
			passwordRows[i]->SetPosition((i + 1) * 50);
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

void PasswordManagerUI::HandleInput(sf::Event& event) {
	for (int i = 0; i < passwordRows.size(); i++) {
		passwordRows[i]->HandleInput(event);
	}
	addButton.HandleInput(event);
	saveButton.HandleInput(event);
}
void PasswordManagerUI::Update(float deltaTime) {
	for (int i = 0; i < passwordRows.size(); i++) {
		passwordRows[i]->Update(deltaTime);
	}
	addButton.Update(deltaTime);
	saveButton.Update(deltaTime);
}
void PasswordManagerUI::Draw(sf::RenderWindow& window) const {
	window.draw(topBar);
	window.draw(topBarText);
	saveButton.Draw(window);
	addButton.Draw(window);
	for (int i = passwordRows.size() - 1; i >= 0; --i) {
		passwordRows[i]->Draw(window);
	}
}

PasswordManagerUI::~PasswordManagerUI() {
	for(int i = 0; i < passwordRows.size(); i++) {
		delete passwordRows[i];
	}
	passwordRows.clear();
}