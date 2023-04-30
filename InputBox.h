#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include "Global.h"
#include "Assets.h"

class InputBox {
public:
	InputBox();

	void setSprite(sf::Sprite* sprite);
	void setText(sf::Text* text, std::string valueNmae, int characterSize);
	void setValidator(std::function <std::string(std::string, std::string)> validator);
	void setValidCharacters(std::vector <char> validCharacters);
	void setGenerator(std::function <std::string()> generator);
	void setPosition(sf::Vector2f position);
	void setFocus(bool focus);
	bool isFocus();
	bool isValidCharacter(char character);
	void insertCharacter(char character);
	void eraseCharacter();
	void setValue(std::string value);
	void setRandomValue();
	std::string getValueName();
	std::string getValue();
	std::string getErrorMessage();
	sf::FloatRect getGlobalBounds();
	void handleEvent(sf::RenderWindow* window, sf::Event* event);
	void draw(sf::RenderWindow* window, Assets* assets);

private:
	std::string valueName;
	std::string value;
	std::vector <char> validCharacters;
	std::function <std::string(std::string, std::string)> validator;
	std::function <std::string()> generator;

	sf::Sprite* sprite;
	sf::Text* text;
	int characterSize;
	bool focus;
	sf::Clock timer;

	sf::Vector2f position;
};

#endif