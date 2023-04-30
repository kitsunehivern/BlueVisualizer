#include "InputBox.h"

InputBox::InputBox() {
	value.clear();
	focus = false;
}

void InputBox::setSprite(sf::Sprite* sprite) {
	this->sprite = sprite;
}

void InputBox::setText(sf::Text* text, std::string valueName, int characterSize) {
	this->text = text;
	this->valueName = valueName;
	this->characterSize = characterSize;
}

void InputBox::setValidator(std::function <std::string(std::string, std::string)> validator) {
	this->validator = validator;
}

void InputBox::setValidCharacters(std::vector <char> validCharacters) {
	this->validCharacters = validCharacters;
	sort(this->validCharacters.begin(), this->validCharacters.end());
}

void InputBox::setGenerator(std::function <std::string()> generator) {
	this->generator = generator;
}

void InputBox::setPosition(sf::Vector2f position) {
	this->position = position;
}

void InputBox::setFocus(bool focus) {
	if (this->focus == false && focus == true) {
		timer.restart();
	}

	this->focus = focus;
}

bool InputBox::isFocus() {
	return focus;
}

void InputBox::setValue(std::string value) {
	this->value = value;
}

void InputBox::setRandomValue() {
	value = generator();
}

std::string InputBox::getValueName() {
	return valueName;
}

std::string InputBox::getValue() {
	return value;
}

std::string InputBox::getErrorMessage() {
	if (valueName == "File browser") {
		return "";
	}

	if (generator().empty()) {
		return "Unavailable";
	}

	return validator(valueName, value);
}

bool InputBox::isValidCharacter(char character) {
	for (int left = 0, right = (int)validCharacters.size() - 1; left <= right; ) {
		int middle = (left + right) / 2;

		if (validCharacters[middle] == character) {
			return true;
		}

		if (validCharacters[middle] < character) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}

	return false;
}

void InputBox::insertCharacter(char character) {
	timer.restart();

	if (value.size() == 50) {
		return;
	}

	if (!isValidCharacter(character)) {
		return;
	}

	value.push_back(character);
}

void InputBox::eraseCharacter() {
	timer.restart();

	if (value.empty()) {
		return;
	}

	value.pop_back();
}

sf::FloatRect InputBox::getGlobalBounds() {
	sprite->setPosition(position);

	return sprite->getGlobalBounds();
}

HWND hwnd;
TCHAR szFileName[MAX_PATH];

void InputBox::handleEvent(sf::RenderWindow* window, sf::Event* event) {
	if (event->type == sf::Event::MouseButtonReleased) {
		if (valueName == "File browser") {
			if (positionInRect(sf::Mouse::getPosition(*window), getGlobalBounds())) {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = L"txt";

				if (GetOpenFileName(&ofn) == TRUE) {
					std::ifstream file(ofn.lpstrFile);
					std::string line, text;
					while (std::getline(file, line)) {
						text += line;
					}

					for (auto character : text) {
						insertCharacter(character);
					}
				}
			}
		} else if (valueName != "File") {
			if (positionInRect(sf::Mouse::getPosition(*window), getGlobalBounds())) {
				setFocus(true);
			} else {
				setFocus(false);
			}
		}
	} else if (event->type == sf::Event::KeyPressed) {
		if (isFocus()) {
			if (sf::Keyboard::Num0 <= event->key.code && event->key.code <= sf::Keyboard::Num9) {
				insertCharacter(event->key.code - sf::Keyboard::Num0 + '0');
			} else if (sf::Keyboard::Numpad0 <= event->key.code && event->key.code <= sf::Keyboard::Numpad9) {
				insertCharacter(event->key.code - sf::Keyboard::Numpad0 + '0');
			} else if (event->key.code == sf::Keyboard::Comma) {
				insertCharacter(',');
			} else if (event->key.code == sf::Keyboard::Space) {
				insertCharacter(' ');
			} else if (event->key.code == sf::Keyboard::BackSpace) {
				eraseCharacter();
			}
		}
	}
}

void InputBox::draw(sf::RenderWindow* window, Assets* assets) {
	sprite->setPosition(position);
	if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
		sprite->setColor(assets->boxColor4);
	} else {
		sprite->setColor(assets->boxColor3);
	}

	window->draw(*sprite);

	if (valueName == "File browser") {
		text->setString("Upload a file");
		text->setCharacterSize(characterSize);
		text->setOrigin(text->getLocalBounds().width / 2, 0);
		text->setPosition(position + sf::Vector2f(sprite->getGlobalBounds().width / 2, 10));
		if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
			text->setFillColor(assets->boxTextColor2);
		} else {
			text->setFillColor(assets->boxTextColor1);
		}

		window->draw(*text);
	} else {
		std::string currentString = valueName + " = " + value;
		text->setString(currentString);
		text->setCharacterSize(characterSize);
		if (text->getGlobalBounds().width > sprite->getGlobalBounds().width - 60) {
			int stringLength = -1;
			for (int left = 1, right = currentString.size(); left <= right; ) {
				int middle = (left + right) / 2;

				text->setString(currentString.substr(currentString.size() - middle, middle));
				if (text->getGlobalBounds().width <= sprite->getGlobalBounds().width - 60) {
					stringLength = middle;
					left = middle + 1;
				} else {
					right = middle - 1;
				}
			}

			text->setString(currentString.substr(currentString.size() - stringLength, stringLength));
			text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width, 0);
			text->setPosition(position + sf::Vector2f(sprite->getGlobalBounds().width - 30, 10));
			if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
				text->setFillColor(assets->boxTextColor2);
			} else {
				text->setFillColor(assets->boxTextColor1);
			}

			window->draw(*text);
		} else {
			text->setOrigin(0, 0);
			text->setPosition(position + sf::Vector2f(30, 10));
			window->draw(*text);
		}

		float currentTimer = timer.getElapsedTime().asSeconds();
		if (!focus || currentTimer - std::floor(currentTimer) > 0.5f) {
			return;
		}

		sf::RectangleShape insertionPoint(sf::Vector2f(2, text->getCharacterSize()));
		if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
			insertionPoint.setFillColor(assets->boxTextColor2);
		} else {
			insertionPoint.setFillColor(assets->boxTextColor1);
		}

		insertionPoint.setOrigin(0, insertionPoint.getLocalBounds().top + insertionPoint.getLocalBounds().height / 2);
		insertionPoint.setPosition(text->getGlobalBounds().left + text->getGlobalBounds().width, sprite->getGlobalBounds().top + sprite->getGlobalBounds().height / 2);

		window->draw(insertionPoint);
	}
}