#include "Menu.h"

Menu::Menu(sf::RenderWindow* window, Assets* assets) {
	this->window = window;
	this->assets = assets;
	illustrationBoxs.clear();
}

void Menu::draw() {
	window->draw(assets->backgroundSprite);

	assets->logoSprite.setPosition(400, 30);
	window->draw(assets->logoSprite);

	assets->hyperspaceItalicText.setString("Dinh Cao Minh Quan - 22125078");
	assets->hyperspaceItalicText.setCharacterSize(30);
	assets->hyperspaceItalicText.setOrigin(assets->hyperspaceItalicText.getLocalBounds().left + assets->hyperspaceItalicText.getLocalBounds().width / 2, 0);
	assets->hyperspaceItalicText.setFillColor(assets->boxTextColor1);
	assets->hyperspaceItalicText.setPosition(sf::Vector2f(800, 210));
	window->draw(assets->hyperspaceItalicText);

	for (int i = 0; i < illustrationBoxs.size(); i++) {
		illustrationBoxs[i].draw(window, assets);
	}

	assets->box60x60Sprite.setPosition(10, 10);
	assets->box60x60Sprite.setColor(assets->boxColor2);
	window->draw(assets->box60x60Sprite);

	assets->quitButtonSprite.setPosition(20, 20);
	if (positionInRect(sf::Mouse::getPosition(*window), assets->quitButtonSprite.getGlobalBounds())) {
		assets->quitButtonSprite.setColor(assets->boxColor4);
	} else {
		assets->quitButtonSprite.setColor(assets->boxColor3);
	}

	window->draw(assets->quitButtonSprite);

	assets->box60x60Sprite.setPosition(1530, 10);
	window->draw(assets->box60x60Sprite);

	assets->modeButtonSprite.setPosition(1540, 20);
	if (positionInRect(sf::Mouse::getPosition(*window), assets->modeButtonSprite.getGlobalBounds())) {
		assets->modeButtonSprite.setColor(assets->boxColor4);
	} else {
		assets->modeButtonSprite.setColor(assets->boxColor3);
	}

	window->draw(assets->modeButtonSprite);
}

int Menu::handleEvent(sf::Event* event) {
	if (event->type == sf::Event::MouseButtonReleased) {
		if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(20, 20, 40, 40))) {
			return -2;
		}

		if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(1540, 20, 40, 40))) {
			assets->switchMode();
			return -1;
		}
	}


	for (int i = 0; i < illustrationBoxs.size(); i++) {
		if (illustrationBoxs[i].handleEvent(window, event)) {
			return i;
		}
	}

	return -1;
}

int Menu::run() {
	illustrationBoxs.emplace_back(&assets->SLLSprite, "Singly Linked List", sf::Vector2f(240, 300));
	illustrationBoxs.emplace_back(&assets->DLLSprite, "Doubly Linked List", sf::Vector2f(640, 300));
	illustrationBoxs.emplace_back(&assets->CLLSprite, "Circular Linked List", sf::Vector2f(1040, 300));
	illustrationBoxs.emplace_back(&assets->StackSprite, "Stack", sf::Vector2f(40, 600));
	illustrationBoxs.emplace_back(&assets->QueueSprite, "Queue", sf::Vector2f(440, 600));
	illustrationBoxs.emplace_back(&assets->StaticArraySprite, "Static Array", sf::Vector2f(840, 600));
	illustrationBoxs.emplace_back(&assets->DynamicArraySprite, "Dynamic Array", sf::Vector2f(1240, 600));

	while (window->isOpen()) {
		window->clear();

		draw();

		window->display();

		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close();
				return -1;
			}

			int temp = handleEvent(&event);
			if (temp == -2) {
				window->close();
				return -1;
			}

			if (temp != -1) {
				return temp;
			}
		}
	}

	return 0;
}