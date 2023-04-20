#include "DoublyEdge.h"
#include "Motion.h"

DoublyEdge::DoublyEdge(Node* left, Node* right) {
	this->left = left;
	this->right = right;
}

void DoublyEdge::calculate() {
	sf::Vector2f leftCenter(left->position.x + 30, left->position.y + 30);
	sf::Vector2f rightCenter(right->position.x + 30, right->position.y + 30);
	float distance = std::sqrt((rightCenter.x - leftCenter.x) * (rightCenter.x - leftCenter.x) + (rightCenter.y - leftCenter.y) * (rightCenter.y - leftCenter.y));
	length = distance - 58;
	angle = std::atan2(rightCenter.y - leftCenter.y, rightCenter.x - leftCenter.x);

	position.x = leftCenter.x + 27 * std::cos(angle);
	position.y = leftCenter.y + 27 * std::sin(angle);
}

void DoublyEdge:: draw(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* leftColor, sf::Color* rightColor, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	auto drawEdge = [&](sf::Color* color) -> void {
		calculate();

		stickSprite->setScale(length / 100, 1);
		stickSprite->setRotation(angle / std::acos(-1) * 180);
		stickSprite->setPosition(position + sf::Vector2f(7.5f * std::sin(angle), -7.5f * std::cos(angle)));
		stickSprite->setColor(*color);

		int width = std::max(0, (int)std::round(20 - length));
		arrowSprite->setTextureRect(sf::IntRect(width, 0, 20 - width, 15));
		arrowSprite->setRotation(angle / std::acos(-1) * 180);
		arrowSprite->setPosition(stickSprite->getPosition() + sf::Vector2f((length - 15 + width) * std::cos(angle), (length - 15 + width) * std::sin(angle)));
		arrowSprite->setColor(*color);


		window->draw(*stickSprite);
		window->draw(*arrowSprite);
	};

	drawEdge(leftColor);
	std::swap(left, right);
	drawEdge(rightColor);
	std::swap(left, right);
}

void DoublyEdge::drawSlideIn(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* leftColor, sf::Color* rightColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	auto drawEdge = [&](sf::Color* color) -> void {
		calculate();

		length *= Motion::Bezier(ratio);

		stickSprite->setScale(length / 100, 1);
		stickSprite->setRotation(angle / std::acos(-1) * 180);
		stickSprite->setPosition(position + sf::Vector2f(7.5f * std::sin(angle), -7.5f * std::cos(angle)));
		stickSprite->setColor(*color);

		int width = std::max(0, (int)std::round(20 - length));
		arrowSprite->setTextureRect(sf::IntRect(width, 0, 20 - width, 15));
		arrowSprite->setRotation(angle / std::acos(-1) * 180);
		arrowSprite->setPosition(stickSprite->getPosition() + sf::Vector2f((length - 15 + width) * std::cos(angle), (length - 15 + width) * std::sin(angle)));
		arrowSprite->setColor(*color);


		window->draw(*stickSprite);
		window->draw(*arrowSprite);
	};

	drawEdge(leftColor);
	std::swap(left, right);
	drawEdge(rightColor);
	std::swap(left, right);
}

void DoublyEdge::drawSlideOut(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* leftColor, sf::Color* rightColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawSlideIn(window, stickSprite, arrowSprite, leftColor, rightColor, 1 - ratio, isDrawn);
}

void DoublyEdge::drawChange(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRightColor, sf::Color* toRightColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	sf::Color newLeftColor = (*fromLeftColor);
	newLeftColor.r += std::round(Motion::Bezier(ratio) * (toLeftColor->r - (int)fromLeftColor->r));
	newLeftColor.g += std::round(Motion::Bezier(ratio) * (toLeftColor->g - (int)fromLeftColor->g));
	newLeftColor.b += std::round(Motion::Bezier(ratio) * (toLeftColor->b - (int)fromLeftColor->b));
	newLeftColor.a += std::round(Motion::Bezier(ratio) * (toLeftColor->a - (int)fromLeftColor->a));

	sf::Color newRightColor = (*fromRightColor);
	newRightColor.r += std::round(Motion::Bezier(ratio) * (toRightColor->r - (int)fromRightColor->r));
	newRightColor.g += std::round(Motion::Bezier(ratio) * (toRightColor->g - (int)fromRightColor->g));
	newRightColor.b += std::round(Motion::Bezier(ratio) * (toRightColor->b - (int)fromRightColor->b));
	newRightColor.a += std::round(Motion::Bezier(ratio) * (toRightColor->a - (int)fromRightColor->a));

	draw(window, stickSprite, arrowSprite, &newLeftColor, &newRightColor);
}