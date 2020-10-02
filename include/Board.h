#pragma once
#include <cuchar>
#include <functional>
#include "SFML/Graphics.hpp"
#include "Colour.h"
#include "Complex.h"
#include <thread>

//colour functions for converting types to colours for display

//class 
template <class Type, int width, int height>
class Board : public sf::Drawable
{
public:
	Board(std::function<Type(std::array<Type, 9>)>& f) : m_function(f) 
	{
		//Initialize array to zero
		for (std::size_t i = 0; i < width; ++i)
		{
			for (std::size_t j = 0; j < height; ++j)
			{
				m_board[j][i] = Complex(0,0);
			}
		}

		//Construct the shape to draw
		m_verticies.resize(width * height);
		//goes through vertically then horizontaly
 
		for (std::size_t i = 0; i < height; ++i)
		{
			for (std::size_t j = 0; j < width; ++j)
			{
				m_verticies[i * width + j].position = sf::Vector2f(j+ C::B_WIDTH, i + C::B_WIDTH);
				m_verticies[i * width + j].color = sf::Color::Black;
			}
		}
	
	}

	void update()
	{
		//Need to copy the board
		std::array<std::array<Type, width>, height>& board_buffer = m_board;

		//Spawn 4 threads to each do a quater of the screen
		std::thread one(&Board::apply_function_for_region, this, board_buffer, sf::Vector2i(1,1), sf::Vector2i(width/2, height/2));
		std::thread two(&Board::apply_function_for_region, this, board_buffer, sf::Vector2i(width/2 + 1, 1), sf::Vector2i(width / 2 - 2, height / 2));
		std::thread three(&Board::apply_function_for_region, this, board_buffer, sf::Vector2i(1, height/2 + 1), sf::Vector2i(width / 2, height / 2 - 2));
		std::thread four(&Board::apply_function_for_region, this, board_buffer, sf::Vector2i(width/2+1, height/2+1), sf::Vector2i(width / 2 - 2, height / 2 - 2));

		//Wait for threads to finish
		one.join();
		two.join();
		three.join();
		four.join();
	}

	void set_cell(sf::Vector2i location, Type value)
	{
		//No error checking
		m_board[location.y][location.x] = value;

		//Update colour even if paused
		m_verticies[location.y * width + location.x].color = sf::Color::White;
	}

	//arbitrarily update the update function
	void set_function(const std::function<Type(std::array<Type, 8>)>& f)
	{
		m_function = f;
	}

	//Draw function
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_verticies, states);
	}

private:
	//State of the board
	std::array<std::array<Type, width>, height> m_board;

	//Function that takes in each  nember and outputs another number
	std::function<Type(std::array<Type, 9>)> m_function;

	//
	void apply_function_for_region(const std::array<std::array<Type, width>, height>& board_buffer, sf::Vector2i position, sf::Vector2i size)
	{
		for (std::size_t i = position.y; i < position.y+size.y; ++i)
		{
			for (std::size_t j = position.x; j < position.x+size.x; ++j)
			{
				//Going in clockwise order from left to middle left
				// S > v       S - Start		E - End
				// E   v         
				// ^ < <

				//Create an array of surrounding cells
				std::array<Type, 9> surrounding;

				//Don't bother with a for loop just do them manually
				surrounding[0] = board_buffer[i - 1][j - 1];
				surrounding[1] = board_buffer[i][j - 1];
				surrounding[2] = board_buffer[i + 1][j - 1];

				surrounding[3] = board_buffer[i - 1][j];
				surrounding[4] = board_buffer[i + 1][j];

				surrounding[5] = board_buffer[i - 1][j + 1];
				surrounding[6] = board_buffer[i][j + 1];
				surrounding[7] = board_buffer[i + 1][j + 1];

				//Self
				surrounding[8] = board_buffer[i][j];

				//Apply function to every cell
				m_board[i][j] = m_function(surrounding);

				//For now assume the range of the function is 0 to 1 - will update to make it general later
				m_verticies[i * width + j].color = color::HSLtoRGB(m_board[i][j].angle(),
					0.75,
					m_board[i][j].abs());
			}
		}
	}

	//Vertecies to draw
	sf::VertexArray m_verticies;
};