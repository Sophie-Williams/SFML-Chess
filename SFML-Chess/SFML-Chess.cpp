// SFML-Chess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ChessPiece.h"
#include "GameClass.h"

void draw_board_at(std::tuple<int, int, int> board_params, sf::RenderWindow& window) {
	int x = std::get<0>(board_params);
	int y = std::get<1>(board_params);
	int board_size = std::get<2>(board_params);
	
	sf::Color colors[]{ sf::Color(237, 131, 66), sf::Color(169, 224, 154) };
	float tile_size = board_size / 8;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			sf::RectangleShape r(sf::Vector2f(board_size / 8, board_size / 8));
			r.setFillColor(colors[(i + j) % 2]);
			r.setPosition(sf::Vector2f(x + i * tile_size, y + j * tile_size));
			window.draw(r);
		}
	}
}

void handle_board_mousePressed(GameClass& g, sf::RenderWindow& window, std::tuple<int, int, int> board_params) {
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
	int x = mouse_position.x;
	int y = mouse_position.y;
	int board_x = std::get<0>(board_params);
	int board_y = std::get<1>(board_params);
	int board_size = std::get<2>(board_params);
	float tile_size = board_size / 8;
	int x2 = (x - board_x) / tile_size;
	int y2 = (y - board_y) / tile_size;
	if (x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) return;
	g.select_piece_at(x2, y2);
}

void draw_killed_pieces(GameClass& g, sf::RenderWindow& window) {
	for (char color : {'W', 'B'}) {
		int zone_x = color == 'W' ? 20 : 520;
		int zone_y = 20;
		int tile_size = 50;
		std::vector<ChessPiece*> pieces = g.get_player_killed_pieces(color);
		for (int i = 0; i < pieces.size(); ++i) {
			int x = i / 8, y = i % 8;
			ChessPiece* p = pieces[i];
			p->setPosition(sf::Vector2f(zone_x + x * tile_size, zone_y + y * tile_size));
			window.draw(*p);
		}
	}
}

void draw_killed_pieces_zones(sf::RenderWindow& window) {
	for (char color : {'W', 'B'}) {
		int zone_x = color == 'W' ? 20 : 520;
		int zone_y = 20;
		sf::RectangleShape zone(sf::Vector2f(100, 400));
		zone.setFillColor(sf::Color(125, 125, 125));
		zone.setPosition(zone_x, zone_y);
		window.draw(zone);
	}
}

int main()
{
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) std::cout << "Error loading font" << std::endl;

	sf::RenderWindow window(sf::VideoMode(800, 500), "SFML Chess");
	GameClass g;
	std::tuple<int, int, int> board_params(120, 20, 400);
	sf::Text current_player_text;
	current_player_text.setFont(font);
	current_player_text.setCharacterSize(25);
	current_player_text.setPosition(sf::Vector2f(20, 430));
	sf::Text game_state_text;
	game_state_text.setFont(font);
	game_state_text.setCharacterSize(25);
	game_state_text.setPosition(sf::Vector2f(20, 460));

	while (window.isOpen())
	{
		sf::Event event;
		bool mouse_pressed = false;
;		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
				mouse_pressed = true;
				handle_board_mousePressed(g, window, board_params);
			}
		}

		window.clear(sf::Color(30, 30, 30));
		draw_board_at(board_params, window);
		draw_killed_pieces_zones(window);
		draw_killed_pieces(g, window);
		g.draw_on(window, board_params);

		current_player_text.setString(std::string() + "Current player : " + g.get_current_player());
		window.draw(current_player_text);

		game_state_text.setString(std::string() + "Game state : " + g.get_game_state());
		window.draw(game_state_text);

		window.display();
	}

	return 0;
}