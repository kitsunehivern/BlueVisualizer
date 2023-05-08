#include "OptionBox.h"

OptionBox::OptionBox() {
}

OptionBox::OptionBox(Assets* assets, sf::Vector2f position) {
	this->position = position;
	this->assets = assets;
	currentOption = -1;
}

void OptionBox::addOption(std::string optionName) {
	this->optionName.push_back(optionName);
	this->suboptionName.push_back(std::vector <std::string> ());
	this->suboptionCondition.push_back(std::vector <std::function <bool()> > ());
	this->suboptionInput.push_back(std::vector <std::vector <InputBox> > ());
	currentSuboption.push_back(0);
}

void OptionBox::addSuboption(std::string suboptionName, std::function <bool()> suboptionCondition) {
	this->suboptionName.back().push_back(suboptionName);
	this->suboptionCondition.back().push_back(suboptionCondition);
	this->suboptionInput.back().push_back(std::vector <InputBox> ());
}

void OptionBox::addSuboptionInput(std::string valueName, std::vector <char> validCharacters, std::function <std::string(std::string, std::string)> validator, std::function <std::string()> generator) {
	this->suboptionInput.back().back().push_back(InputBox());
	if (valueName == "File browser") {
		this->suboptionInput.back().back().back().setSprite(&assets->skewBox560x60Sprite);
	} else {
		this->suboptionInput.back().back().back().setSprite(&assets->skewBox400x60Sprite);
	}

	this->suboptionInput.back().back().back().setText(&assets->consolasBoldText, valueName, 30);
	this->suboptionInput.back().back().back().setValidCharacters(validCharacters);
	this->suboptionInput.back().back().back().setValidator(validator);
	this->suboptionInput.back().back().back().setGenerator(generator);
	for (int i = 0; i < this->suboptionInput.back().back().size(); i++) {
		this->suboptionInput.back().back()[i].setPosition(position + sf::Vector2f(300, 200 - 40 * this->suboptionInput.back().back().size() + 80 * i));
	}
}

void OptionBox::updateMessage() {
	message.clear();
	if (currentOption != -1) {
		for (int i = 0; i < suboptionInput[currentOption][currentSuboption[currentOption]].size(); i++) {
			if (message.empty()) {
				message = suboptionInput[currentOption][currentSuboption[currentOption]][i].getErrorMessage();
			}
		}

		if (!suboptionCondition[currentOption][currentSuboption[currentOption]]()) {
			message = "Unavailable";
		}
	}
}

bool OptionBox::isFocus() {
	if (currentOption != -1) {
		for (int i = 0; i < suboptionInput[currentOption][currentSuboption[currentOption]].size(); i++) {
			if (suboptionInput[currentOption][currentSuboption[currentOption]][i].isFocus()) {
				return true;
			}
		}
	}

	return false;
}

std::string OptionBox::getValue(int option, int suboption, int index) {
	return suboptionInput[option][suboption][index].getValue();
}

void OptionBox::draw(sf::RenderWindow* window) {
	sf::Sprite* boxSprite = &assets->box880x380Sprite;
	sf::Text* optionText = &assets->consolasBoldText;
	sf::Sprite* suboptionBoxSprite = &assets->skewBox400x60Sprite;
	sf::Text* suboptionText = &assets->consolasBoldText;

	boxSprite->setTextureRect(sf::IntRect(280, 0, 600, 380));
	boxSprite->setColor(assets->boxColor2);
	boxSprite->setPosition(position + sf::Vector2f(280, 0));
	window->draw(*boxSprite);

	int lastHeight = 0;
	for (int i = 0; i < optionName.size(); i++) {
		int height;
		if (i == optionName.size() - 1) {
			height = 380 - lastHeight;
		} else {
			height = 380 / optionName.size();
		}

		boxSprite->setTextureRect(sf::IntRect(0, lastHeight, 280, 5));
		boxSprite->setPosition(position.x, position.y + lastHeight);
		boxSprite->setColor(assets->boxColor1);
		window->draw(*boxSprite);

		boxSprite->setTextureRect(sf::IntRect(0, lastHeight + 5, 280, height - 10));
		boxSprite->setPosition(position.x, position.y + lastHeight + 5);
		if (positionInRect(sf::Mouse::getPosition(*window), boxSprite->getGlobalBounds())) {
			boxSprite->setColor(assets->boxColor2);
			assets->setCursor(2);
		}
		
		if (i == currentOption) {
			boxSprite->setColor(assets->boxColor2);
		} else {
			boxSprite->setColor(assets->boxColor1);
		}

		window->draw(*boxSprite);

		optionText->setString(optionName[i]);
		optionText->setFillColor(assets->boxTextColor1);
		optionText->setCharacterSize(30);
		optionText->setOrigin(optionText->getLocalBounds().left + optionText->getLocalBounds().width / 2, optionText->getLocalBounds().top + optionText->getLocalBounds().height / 2);
		optionText->setPosition(boxSprite->getGlobalBounds().left + boxSprite->getGlobalBounds().width / 2, boxSprite->getGlobalBounds().top + boxSprite->getGlobalBounds().height / 2);
		window->draw(*optionText);

		boxSprite->setTextureRect(sf::IntRect(0, lastHeight + height - 5, 280, 5));
		boxSprite->setPosition(position.x, position.y + lastHeight + height - 5);
		boxSprite->setColor(assets->boxColor1);
		window->draw(*boxSprite);

		if (i < optionName.size() - 1) {
			sf::RectangleShape seperatorLine(sf::Vector2f(200, 2));
			seperatorLine.setFillColor(assets->seperatorLineColor1);
			seperatorLine.setOrigin(0, seperatorLine.getSize().y / 2);
			seperatorLine.setPosition(position.x + 40, position.y + lastHeight + height);
			window->draw(seperatorLine);
		}

		lastHeight += height;
	}

	if (currentOption != -1) {
		assets->prevButtonSprite.setPosition(position + sf::Vector2f(300, 20));
		if (positionInRect(sf::Mouse::getPosition(*window), assets->prevButtonSprite.getGlobalBounds())) {
			assets->prevButtonSprite.setColor(assets->boxColor4);
			assets->setCursor(2);
		} else {
			assets->prevButtonSprite.setColor(assets->boxColor3);
		}

		window->draw(assets->prevButtonSprite);

		assets->nextButtonSprite.setPosition(position + sf::Vector2f(800, 20));
		if (positionInRect(sf::Mouse::getPosition(*window), assets->nextButtonSprite.getGlobalBounds())) {
			assets->nextButtonSprite.setColor(assets->boxColor4);
			assets->setCursor(2);
		} else {
			assets->nextButtonSprite.setColor(assets->boxColor3);
		}

		window->draw(assets->nextButtonSprite);

		suboptionBoxSprite->setPosition(position + sf::Vector2f(380, 20));
		suboptionBoxSprite->setColor(assets->boxColor3);
		window->draw(*suboptionBoxSprite);

		suboptionText->setString(suboptionName[currentOption][currentSuboption[currentOption]]);
		suboptionText->setFillColor(assets->boxTextColor1);
		suboptionText->setCharacterSize(30);
		suboptionText->setOrigin(suboptionText->getLocalBounds().left + suboptionText->getLocalBounds().width / 2, 0);
		suboptionText->setPosition(position + sf::Vector2f(580, 30));
		window->draw(*suboptionText);

		for (int i = 0; i < suboptionInput[currentOption][currentSuboption[currentOption]].size(); i++) {
			suboptionInput[currentOption][currentSuboption[currentOption]][i].draw(window, assets);

			if (suboptionName[currentOption][currentSuboption[currentOption]] != "Manual" || i != 1) {
				Button randomButton;
				randomButton.setSprite(&assets->skewBox160x60Sprite);
				randomButton.setText(&assets->consolasBoldText, "Random", 30);
				randomButton.setPosition(position + sf::Vector2f(700, 200 - 40 * suboptionInput[currentOption][currentSuboption[currentOption]].size() + 80 * i));
				randomButton.draw(window, assets);
			}
		}

		updateMessage();

		Button messageButton;
		messageButton.setSprite(&assets->skewBox560x60Sprite);
		if (message.empty()) {
			messageButton.setText(&assets->consolasBoldText, "GO", 30);
		} else {
			messageButton.setText(&assets->consolasBoldText, message, 30);
		}

		messageButton.setPosition(position + sf::Vector2f(300, 300));
		messageButton.draw(window, assets);
		
		sf::RectangleShape seperatorLine(sf::Vector2f(500, 2));
		seperatorLine.setFillColor(assets->seperatorLineColor2);
		seperatorLine.setOrigin(0, seperatorLine.getSize().y / 2);
		seperatorLine.setPosition(position.x + 330, position.y + 100);
		window->draw(seperatorLine);

		seperatorLine.setPosition(position.x + 330, position.y + 280);
		window->draw(seperatorLine);
	}
}

std::tuple <int, int, std::vector <std::string> > OptionBox::handleEvent(sf::RenderWindow* window, sf::Event* event) {
	if (currentOption != -1) {
		for (int i = 0; i < suboptionInput[currentOption][currentSuboption[currentOption]].size(); i++) {
			suboptionInput[currentOption][currentSuboption[currentOption]][i].handleEvent(window, event);
		}

		if (suboptionName[currentOption][currentSuboption[currentOption]] == "Manual") {
			if (!suboptionInput[currentOption][currentSuboption[currentOption]][1].getValue().empty()) {
				suboptionInput[currentOption][currentSuboption[currentOption]][0].setValue(suboptionInput[currentOption][currentSuboption[currentOption]][1].getValue());
				suboptionInput[currentOption][currentSuboption[currentOption]][1].setValue("");
			}
		}
	}

	if (event->type == sf::Event::MouseButtonReleased) {
		int lastHeight = 0;
		for (int i = 0; i < optionName.size(); i++) {
			int height;
			if (i == optionName.size() - 1) {
				height = 380 - lastHeight;
			} else {
				height = 380 / optionName.size();
			}

			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x, position.y + lastHeight + 5, 280, height - 10))) {
				currentOption = i;
			}

			lastHeight += height;
		}

		if (currentOption != -1) {
			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 300, position.y + 20, 60, 60))) {
				currentSuboption[currentOption]--;
				if (currentSuboption[currentOption] < 0) {
					currentSuboption[currentOption] = suboptionName[currentOption].size() - 1;
				}
			}

			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 800, position.y + 20, 60, 60))) {
				currentSuboption[currentOption]++;
				if (currentSuboption[currentOption] > suboptionName[currentOption].size() - 1) {
					currentSuboption[currentOption] = 0;
				}
			}

			for (int i = 0; i < suboptionInput[currentOption][currentSuboption[currentOption]].size(); i++) {
				if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 700, position.y + 200 - 40 * suboptionInput[currentOption][currentSuboption[currentOption]].size() + 80 * i, 160, 60))) {
					suboptionInput[currentOption][currentSuboption[currentOption]][i].setRandomValue();
				}
			}

			if (message.empty() && positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 300, position.y + 300, 560, 60))) {
				std::vector <std::string> values;
				for (int i = 0; i < suboptionInput[currentOption][currentSuboption[currentOption]].size(); i++) {
					values.push_back(suboptionInput[currentOption][currentSuboption[currentOption]][i].getValue());
				}

				return { currentOption, currentSuboption[currentOption], values };
			}
		}
	} else if (event->type == sf::Event::KeyPressed) {
		if (currentOption != -1 && message.empty() && event->key.code == sf::Keyboard::Enter) {
			std::vector <std::string> values;
			for (int i = 0; i < suboptionInput[currentOption][currentSuboption[currentOption]].size(); i++) {
				values.push_back(suboptionInput[currentOption][currentSuboption[currentOption]][i].getValue());
				suboptionInput[currentOption][currentSuboption[currentOption]][i].setFocus(false);
			}

			return { currentOption, currentSuboption[currentOption], values };
		}
	}

	updateMessage();

	return { -1, -1, {} };
}