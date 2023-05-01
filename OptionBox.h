#pragma once
#ifndef OPTION_BOX_H
#define OPTION_BOX_H

#include "Global.h"
#include "Assets.h"
#include "Button.h"
#include "InputBox.h"

class OptionBox {
public:
	OptionBox();
	OptionBox(Assets* assets, sf::Vector2f positions);

	void addOption(std::string optionName);
	void addSuboption(std::string suboptionName, std::function <bool()> suboptionCondition);
	void addSuboptionInput(std::string valueName, std::vector <char> validCharacters, std::function <std::string(std::string, std::string)> validator, std::function <std::string()> generator);

	bool isFocus();
	void updateMessage();
	std::string getValue(int option, int suboption, int index);
	std::tuple <int, int, std::vector <std::string> > handleEvent(sf::RenderWindow* window, sf::Event* event);
	void draw(sf::RenderWindow* window);

private:
	Assets* assets;
	sf::Vector2f position;
	std::vector <std::string> optionName;
	std::vector <std::vector <std::string> > suboptionName;
	std::vector <std::vector <std::function<bool()> > > suboptionCondition;
	std::vector <std::vector <std::vector <InputBox> > > suboptionInput;
	int currentOption;
	std::vector <int> currentSuboption;
	std::string message;
};

#endif