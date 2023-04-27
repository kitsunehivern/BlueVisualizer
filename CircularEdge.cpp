#include "CircularEdge.h"
#include "Motion.h"

CircularEdge::CircularEdge(Node* left, Node* right) {
	this->left = left;
	this->right = right;
}

void CircularEdge::calculate() {
	length[0] = 45;
	length[1] = 72.5 + (right->position.x - left->position.x);
	length[2] = 72.5;
	length[3] = 35;

	position[0] = right->position + sf::Vector2f(30, 5);
	position[1] = right->position + sf::Vector2f(32.5, -37.5f);
	position[2] = left->position + sf::Vector2f(-37.5, -40);
	position[3] = left->position + sf::Vector2f(-40, 30);
}

void CircularEdge::draw(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	calculate();

	float total = 0;
	for (int i = 0; i < 4; i++) {
		total += length[i];
	}

	stickSprite->setColor(*color);
	arrowSprite->setColor(*color);
	for (int i = 0; i < 4; i++) {
		stickSprite->setRotation(-90 * (i + 1));
		stickSprite->setPosition(position[i]);

		arrowSprite->setRotation(-90 * (i + 1));
		arrowSprite->setPosition(position[i]);

		if (total <= length[i]) {
			stickSprite->setScale(total / CEDGE_INIT_LENGTH, 1);
			window->draw(*stickSprite);

			int width = std::max(0, (int)std::round(20 - total));
			arrowSprite->setTextureRect(sf::IntRect(width, 0, 20 - width, 15));
			float angle = -0.5 * (i + 1) * std::acos(-1);
			arrowSprite->setPosition(position[i] + sf::Vector2f((total - 13 + width) * std::cos(angle), (total - 13 + width) * std::sin(angle)));
			window->draw(*arrowSprite);

			break;
		} else {
			stickSprite->setScale(length[i] / CEDGE_INIT_LENGTH, 1);
			window->draw(*stickSprite);

			total -= length[i];
		}
	}
}

void CircularEdge::drawSlideIn(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	calculate();

	float total = 0;
	for (int i = 0; i < 4; i++) {
		total += length[i];
	}

	total *= Motion::Bezier(ratio);

	stickSprite->setColor(*color);
	arrowSprite->setColor(*color);
	for (int i = 0; i < 4; i++) {
		stickSprite->setRotation(-90 * (i + 1));
		stickSprite->setPosition(position[i]);

		arrowSprite->setRotation(-90 * (i + 1));
		arrowSprite->setPosition(position[i]);
		
		if (total <= length[i]) {
			stickSprite->setScale(total / CEDGE_INIT_LENGTH, 1);
			window->draw(*stickSprite);
			
			int width = std::max(0, (int)std::round(20 - total));
			arrowSprite->setTextureRect(sf::IntRect(width, 0, 20 - width, 15));
			float angle = -0.5 * (i + 1) * std::acos(-1);
			arrowSprite->setPosition(position[i] + sf::Vector2f((total - 13 + width) * std::cos(angle), (total - 13 + width) * std::sin(angle)));
			window->draw(*arrowSprite);

			break;
		} else {
			stickSprite->setScale(length[i] / CEDGE_INIT_LENGTH, 1);
			window->draw(*stickSprite);

			total -= length[i];
		}
	}
}

void CircularEdge::drawSlideOut(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawSlideIn(window, stickSprite, arrowSprite, color, 1 - ratio, isDrawn);
}

void CircularEdge::drawChange(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* fromColor, sf::Color* toColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	sf::Color newColor = (*fromColor);
	newColor.r += std::round(Motion::Bezier(ratio) * (toColor->r - (int)fromColor->r));
	newColor.g += std::round(Motion::Bezier(ratio) * (toColor->g - (int)fromColor->g));
	newColor.b += std::round(Motion::Bezier(ratio) * (toColor->b - (int)fromColor->b));
	newColor.a += std::round(Motion::Bezier(ratio) * (toColor->a - (int)fromColor->a));

	draw(window, stickSprite, arrowSprite, &newColor);
}