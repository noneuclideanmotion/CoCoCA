#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "WindowOptions.h"
#include "Board.h"
#include "Complex.h"
#include <array>
#include <cmath>

int main()
{
	//Set up Window
	sf::RenderWindow window(sf::VideoMode(C::W_WIDTH, C::W_HEIGHT), "", sf::Style::None);
	window.setFramerateLimit(30);

	//The function (which is applied to every cell)
	//This function just averages them but it can be anything
	std::function<Complex(std::array<Complex, 9>)> a_function = [](std::array<Complex, 9> input) -> Complex
	{
		Complex x = Complex(0, 0);
		for (int i = 0; i < 8; ++i)
		{
			x = x + input[i];
		}
		//Rotate each number by i to create a 4 cycle
		//Input 8 is required
		//conways game of life rule
			/*return input[8]*exp(-3*(x - 3) * (x - 3)*(x-2)*(x-2))
				+(1- input[8])*exp(- 3*(x - 3) * (x - 3));
			*/
		/*
		return input[8] * exp(-(x - 3) * (x - 3) * (x - 5) * (x - 5))
			+ (1 - input[8]) * exp(-0.5*(x - 4) * (x - 4));
		*/
		Complex emx = Complex(x.real()-3, x.imaginary());
		//Complex result = (x * emx * emx) * (0.01265);
		Complex result = input[8]*exp(emx.abs() * emx.abs() * -1);
		return result;
	};
	//Start off paused
	bool play = false;
	bool outoffocus = false;

	//Radius of mouse click
	int radius = 3;

	//Create the board object
	Board<Complex, C::W_WIDTH-2*C::B_WIDTH, C::W_HEIGHT - 2 * C::B_WIDTH> b(a_function);

	//Set up Window border
	sf::RectangleShape border(sf::Vector2f(C::W_WIDTH - 2 * C::B_WIDTH, C::W_HEIGHT - 2 * C::B_WIDTH));
	border.setPosition(sf::Vector2f(C::B_WIDTH, C::B_WIDTH));
	border.setFillColor(sf::Color(0, 0, 0, 0));
	border.setOutlineColor(sf::Color(255, 255, 255, 125));
	border.setOutlineThickness(C::B_WIDTH);

	//Set up close button
	WindowOptions windowoptions(window);

	//Event handler
	sf::Event event;

	//Old and new mouse position for calculating delta
	sf::Vector2f mouse_old_position = sf::Vector2f(-1, -1);
	sf::Vector2f mouse_new_position = sf::Vector2f(-1, -1);

	//To check if a click is new
	bool is_already_clicked = false;

	while (window.isOpen())
	{
		//std::cout << "frame" << std::endl;
		while (window.pollEvent(event))
		{
			//Pause if focus lost
			if (event.type == sf::Event::LostFocus)
			{
				play = false;
				outoffocus = true;
			}
			if (event.type == sf::Event::GainedFocus)
			{
				outoffocus = false;
			}

			bool clicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);

			//Stop the program from closing if dragging
			if (clicked == true && is_already_clicked == false)
			{
				windowoptions.event_clicked(sf::Mouse::getPosition(window), true);
			}
			else
			{
				windowoptions.event_clicked(sf::Mouse::getPosition(window), false);
			}
			mouse_new_position = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

			//Update 
			if (clicked)
			{
				sf::Vector2i position = sf::Mouse::getPosition(window);
				//Update cell that was clicked;
				if (!outoffocus && position.x - radius > 0 && position.y - radius > 0 && position.x < C::W_WIDTH && position.y < C::W_HEIGHT)
				{
					for (int i = -radius; i < radius; ++i)
					{
						for (int j = -radius; j < radius; ++j)
						{
							b.set_cell(sf::Vector2i(position.x + i, position.y + j), Complex(0.98,0.199));
						}
					}

					//Make glider on click
						
						/*b.set_cell(sf::Vector2i(position.x, position.y), Complex(1, 0));
						b.set_cell(sf::Vector2i(position.x + 1, position.y), Complex(1, 0));
						b.set_cell(sf::Vector2i(position.x + 2, position.y), Complex(1, 0));
						b.set_cell(sf::Vector2i(position.x + 2, position.y - 1), Complex(1, 0));
						b.set_cell(sf::Vector2i(position.x + 1, position.y - 2), Complex(1, 0));
							*/
				}
			}
			//Update click state
			is_already_clicked = clicked;

			//Check for space press
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				play = !play;
			}
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left))
			{
				radius--;
			}
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right))
			{
				radius++;
			}
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::F))
			{
				play = false;
				b.update();
			}
		}
		mouse_old_position = mouse_new_position;

		if (play)
		{
			b.update();
		}
		//Draw objects
		window.draw(b);
		window.draw(windowoptions);
		window.draw(border);

		//Display and clear
		window.display();

		window.clear();
	}
	return 0;
}