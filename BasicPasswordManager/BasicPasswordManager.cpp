#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "PasswordManager.h"
#include "SFML/Graphics.hpp"
#include "Button.h"
#include "InputField.h"
#include "PasswordRow.h"
#include "PasswordManagerUI.h"

sf::Vector2f mousePos;
PasswordManager* passwordManager;
bool blockClick = false;
std::string possibleChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";
sf::Vector2i windowSize;
bool exitProgram = false;

int main()
{
	srand(time(NULL));

	std::string masterKey;
	std::cout << "Enter the master key: \n";
	std::getline(std::cin, masterKey);

	sf::RenderWindow window(sf::VideoMode(800, 600), "Basic Password Manager");
	windowSize = window.getViewport(window.getDefaultView()).getSize();

	sf::Font font;
	if (!font.loadFromFile("Resources/Roboto-Black.ttf"))
	{
		std::cout << "Failed to load font\n";
		return 1;
	}

	passwordManager = new PasswordManager(masterKey);
	PasswordManagerUI passwordManagerUI(&font);

	if (exitProgram)
	{
		std::cout << "Invalid master key!\n";
		return 1;
	}

	while (window.isOpen())
	{
		blockClick = false;
		mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				windowSize = window.getViewport(window.getDefaultView()).getSize();
				passwordManagerUI.OnResize();
			}

			passwordManagerUI.HandleInput(event);
		}

		passwordManagerUI.Update(0.0f);

		window.clear(sf::Color::Black);
		//window.draw(title);
		passwordManagerUI.Draw(window);
		window.display();
	}

	return 0;
}