#pragma once
#include "PasswordRow.h"
#include <vector>

class PasswordManagerUI {
private:
	std::vector<PasswordRow*> passwordRows;
	sf::RectangleShape topBar;
	sf::Text topBarText;
	Button addButton;
	Button saveButton;

	sf::Font* font;

	std::string passwordFilePath;
public:
	PasswordManagerUI(sf::Font* font);

	void AddPasswordRow();
	void RemovePasswordRow(int index);

	void SavePasswords();
	void LoadPasswords();

	void HandleInput(sf::Event& event);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

	~PasswordManagerUI();
};