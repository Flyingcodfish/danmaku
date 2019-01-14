//Entity.cpp

#include <SFML/Graphics.hpp>

class Entity {
public:
	sf::Sprite sprite;
	bool deleteThis = false;
	virtual void Update(){
		//pass
	};

};

class PlayerShot : public Entity {
	float speed = 0.2f;
	void Update() override {
		sprite.move(0.0f, -speed);
		if (sprite.getPosition().y < -100) deleteThis = true;
	}
};
