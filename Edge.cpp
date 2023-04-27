#include "Edge.h"
#include "Motion.h"

Edge::Edge(Node* left, Node* right) {
	this->left = left;
	this->right = right;
}

void Edge::calculate() {
	sf::Vector2f leftCenter(left->position.x + 30, left->position.y + 30);
	sf::Vector2f rightCenter(right->position.x + 30, right->position.y + 30);
	float distance = std::sqrt((rightCenter.x - leftCenter.x) * (rightCenter.x - leftCenter.x) + (rightCenter.y - leftCenter.y) * (rightCenter.y - leftCenter.y));
	length = distance - 60;
	angle = std::atan2(rightCenter.y - leftCenter.y, rightCenter.x - leftCenter.x);

	position.x = leftCenter.x + 25 * std::cos(angle);
	position.y = leftCenter.y + 25 * std::sin(angle);
}

void Edge::draw(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	calculate();

	stickSprite->setScale(length / EDGE_INIT_LENGTH, 1);
	stickSprite->setRotation(angle / std::acos(-1) * 180);
	stickSprite->setPosition(position);
	stickSprite->setColor(*color);

	int width = std::max(0, (int)std::round(20 - length));
	arrowSprite->setTextureRect(sf::IntRect(width, 0, 20 - width, 15));
	arrowSprite->setRotation(angle / std::acos(-1) * 180);
	arrowSprite->setPosition(position.x + (length - 13 + width) * std::cos(angle), position.y + (length - 13 + width) * std::sin(angle));
	arrowSprite->setColor(*color);

	window->draw(*stickSprite);
	window->draw(*arrowSprite);
}

void Edge::drawSlideIn(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	calculate();

	length *= Motion::Bezier(ratio);

	stickSprite->setScale(length / EDGE_INIT_LENGTH, 1);
	stickSprite->setRotation(angle / std::acos(-1) * 180);
	stickSprite->setPosition(position);
	stickSprite->setColor(*color);

	int width = std::max(0, (int)std::round(20 - length));
	arrowSprite->setTextureRect(sf::IntRect(width, 0, 20 - width, 15));
	arrowSprite->setRotation(angle / std::acos(-1) * 180);
	arrowSprite->setPosition(position.x + (length - 13 + width) * std::cos(angle), position.y + (length - 13 + width) * std::sin(angle));
	arrowSprite->setColor(*color);	

	window->draw(*stickSprite);
	window->draw(*arrowSprite);
}

void Edge::drawSlideOut(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawSlideIn(window, stickSprite, arrowSprite, color, 1 - ratio, isDrawn);
}

void Edge::drawChange(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* fromColor, sf::Color* toColor, float ratio, bool isDrawn) {
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