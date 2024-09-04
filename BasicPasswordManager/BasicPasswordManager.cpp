#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "PasswordManager.h"
#include "SFML/Graphics.hpp"
#include "Button.h"
#include "InputField.h"

sf::Vector2f mousePos;

int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(800, 600), "Basic Password Manager");

	sf::Font font;
	if (!font.loadFromFile("Resources/Roboto-Black.ttf"))
	{
		std::cout << "Failed to load font\n";
		return 1;
	}

	sf::Text title;
	title.setFont(font);
	title.setString("Basic Password Manager");
	title.setCharacterSize(24);
	title.setFillColor(sf::Color::White);
	title.setPosition(10, 10);

	Button button;
	button.SetPosition(150, 70);
	button.SetSize(100, 50);
	button.SetText("Add", font, 24, sf::Color::White);
	button.SetColor(sf::Color::Blue);
	button.SetHoverColor(sf::Color::Green);
	button.SetClickColor(sf::Color::Red);
	button.SetOnClick([]() { std::cout << "Button clicked\n"; });

	InputField inputField;
	inputField.SetPosition(150, 150);
	inputField.SetSize(200, 50);
	inputField.SetText("Enter password", font, 24, sf::Color::White);
	inputField.SetColor(sf::Color::Blue);
	inputField.SetHoverColor(sf::Color::Green);
	inputField.SetSelectedColor(sf::Color::Red);
	inputField.SetOnEdit([](const std::string& str) { std::cout << "Input: " << str << std::endl; });
	inputField.SetOnEditStart([](const std::string& str) { std::cout << "Input started: " << str << std::endl; });
	inputField.SetOnEditEnd([](const std::string& str) { std::cout << "Input ended: " << str << std::endl; });


	while (window.isOpen())
	{
		mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			button.HandleInput(event);
			inputField.HandleInput(event);
		}

		button.Update(0.0f);
		inputField.Update(0.0f);

		window.clear(sf::Color::Black);
		window.draw(title);
		button.Draw(window);
		inputField.Draw(window);
		window.display();
	}


	/*std::string masterKey;
	std::cout << "Enter the master key: \n";
	std::getline(std::cin, masterKey);

	PasswordManager pm(masterKey);

	int command = 0;

	std::string path = "passwords.txt";

	if(pm.LoadPasswords(path))
		std::cout << "Passwords loaded successfully\n";
	else
	{
		std::cout << "Failed to load passwords\n";
		pm.GenerateKey();
	}
	
	do {

		std::cout << "Commands:\n";
		std::cout<<"0. Exit\n";
		std::cout<<"1. Add password\n";
		std::cout<<"2. Change password\n";
		std::cout<<"3. Remove password\n";
		std::cout<<"4. Get password\n";
		std::cout<<"5. Save passwords\n";

		std::cin >> command;
		std::cin.ignore();

		if (command == 1) {
			std::string password;
			std::string description;

			std::cout << "Enter the password: \n";
			std::getline(std::cin, password);

			std::cout << "Enter the description: \n";
			std::getline(std::cin, description);

			pm.AddPassword(password, description);
		}
		else if (command == 2) {
			int index;
			std::string password;

			std::cout << "Enter the index: \n";
			std::cin >> index;
			std::cin.ignore();

			std::cout << "Enter the new password: \n";
			std::getline(std::cin, password);

			pm.ChangePassword(index, password);
		}
		else if (command == 3) {
			int index;

			std::cout << "Enter the index: \n";
			std::cin >> index;
			std::cin.ignore();

			pm.RemovePassword(index);
		}
		else if (command == 4) {
			int index;

			std::cout << "Enter the index: \n";
			std::cin >> index;
			std::cin.ignore();

			std::cout << pm.GetDescription(index) << ": " << pm.GetPassword(index) << std::endl;
		}
		else if (command == 5) {
			if (pm.SavePasswords(path))
				std::cout << "Passwords saved successfully\n";
			else
				std::cout << "Failed to save passwords\n";
		}

	} while (command != 0);*/

	return 0;
}