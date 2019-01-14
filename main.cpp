//main.cpp

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.cpp"

#include <string>
#include <random>
#include <list>

#define SCREEN_X 450
#define SCREEN_Y 600

int main(int argc, char *argv[]){

	//command line arguments
	bool DEBUG_MODE = false;
	
	for (int i=1; i<argc; i++){
		std::string argstr(argv[i]);
		if (argstr == "-d") DEBUG_MODE = true;
	}
	
	
	//create window
	sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "Sunday Shooter");

	//load textures
	sf::Texture shipTexture;
	if (!shipTexture.loadFromFile("Assets/nutterfly.png"))
		return EXIT_FAILURE;
	sf::Sprite shipSprite(shipTexture);
	shipSprite.setScale(0.1f, 0.1f);
	
	sf::Texture pnutTexture[4];
	if (!pnutTexture[0].loadFromFile("Assets/peanut_0.png"))
		return EXIT_FAILURE;
	if (!pnutTexture[1].loadFromFile("Assets/peanut_1.png"))
		return EXIT_FAILURE;
	if (!pnutTexture[2].loadFromFile("Assets/peanut_2.png"))
		return EXIT_FAILURE;
	if (!pnutTexture[3].loadFromFile("Assets/peanut_3.png"))
		return EXIT_FAILURE;

	//text
	sf::Font basicFont;
	if (!basicFont.loadFromFile("Assets/Titillium/Titillium-Regular.otf"))
		return EXIT_FAILURE;
	
	sf::Text scoreText("Score: 99999", basicFont, 20);
	sf::Text pausedText("PAUSED", basicFont, 80);
	sf::FloatRect pausedRect = pausedText.getLocalBounds();
	pausedText.setOrigin(pausedRect.width/2.0f, pausedRect.height/2.0f);
	pausedText.setPosition(SCREEN_X/2.0f, SCREEN_Y/2.0f);

	//logic values
	float player_speed = 0.1f;
	int player_lives = 3;
	int player_score = 0;
	bool game_paused = false;
	float shotRechargeTime = 0.25f;
	
	//important objects
	std::list<Entity*> entityList;

	sf::Clock levelClock; //used to time enemy spawns
	sf::Clock shotClock; //used to enforce player shot cooldowns
	
	std::default_random_engine randGen;
	std::uniform_int_distribution<int> dist4(0,3);

	//main loop
	while (window.isOpen()){
		//Process Events
		sf::Event event;
		while (window.pollEvent(event)){
			//Event: close window
			if (event.type == sf::Event::Closed)
				window.close();
			//Event: keyboard input
			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::Escape){
					game_paused = !game_paused;
				}
			}
		}
		
		//Game logic
		if (!game_paused){
			//movement
			if (window.hasFocus()){
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					shipSprite.move(-player_speed, 0.0f);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					shipSprite.move(player_speed, 0.0f);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					shipSprite.move(0.0f, player_speed);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					shipSprite.move(0.0f, -player_speed);
				//keep player inside window
				sf::Vector2f newPos = shipSprite.getPosition();
				sf::FloatRect shipRect = shipSprite.getGlobalBounds();
				if (shipRect.left > SCREEN_X - shipRect.width) newPos.x = SCREEN_X - shipRect.width;
				else if (shipRect.left < 0) newPos.x = 0;
				if (shipRect.top > SCREEN_Y - shipRect.height) newPos.y = SCREEN_Y - shipRect.height;
				else if (shipRect.top < 0) newPos.y = 0;
				shipSprite.setPosition(newPos);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && shotClock.getElapsedTime().asSeconds() > shotRechargeTime){
					shotClock.restart();
					//fire bullet
					PlayerShot *bullet = new PlayerShot();
					bullet->sprite.setPosition(shipSprite.getPosition());
					bullet->sprite.setScale(0.05f, 0.05f);
					bullet->sprite.setTexture(pnutTexture[dist4(randGen)]);
					entityList.push_back(bullet);
				}

			}
			//collision?
			//TODO (?)

			//enemy/bullet logic (entity updates; may replace explicit collision code)
			for (std::list<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it){
				//for each entity:
				(*it)->Update();
				if ((*it)->deleteThis) entityList.erase(it);
			}


		}
		else{
			//pause menu?
			
		}



		//Clear screen
		window.clear();

		//Draw stuff
		window.draw(shipSprite);
		window.draw(scoreText);
		
		for (std::list<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it){
			window.draw((*it)->sprite);
		}

		if (game_paused) window.draw(pausedText);

		//Update Window
		window.display();

	}

}
