#include "Node.h"
#include "Motion.h"

Node::Node(int value, sf::Vector2f position) {
	this->value = value;
	this->position = position;
}

void Node::draw(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	circleSprite->setColor(*circleColor);
	circleSprite->setPosition(position);
	window->draw(*circleSprite);

	valueText->setCharacterSize(25);
	valueText->setFillColor(*valueColor);
	valueText->setString(std::to_string(value));
	valueText->setOrigin(valueText->getLocalBounds().left + valueText->getLocalBounds().width / 2, valueText->getLocalBounds().top + valueText->getLocalBounds().height / 2);
	valueText->setPosition(position + sf::Vector2f(30, 30));
	window->draw(*valueText);
}

void Node::drawFadeIn(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	sf::Color newCircleColor = (*circleColor) * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255));
	sf::Color newValueColor = (*valueColor) * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255));

	draw(window, circleSprite, &newCircleColor, valueText, &newValueColor);
}

void Node::drawFadeOut(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawFadeIn(window, circleSprite, circleColor, valueText, valueColor, 1 - ratio, isDrawn);
}

void Node::drawMove(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition, float ratio, bool isDrawn) {
	position.x = fromPosition.x + Motion::Bezier(ratio) * (toPosition.x - fromPosition.x);
	position.y = fromPosition.y + Motion::Bezier(ratio) * (toPosition.y - fromPosition.y);

	if (!isDrawn) {
		return;
	}

	draw(window, circleSprite, circleColor, valueText, valueColor);
}

void Node::drawSlide(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, Node* fromNode, Node* toNode, float ratio, bool isDrawn) {
	float length = std::sqrt((toNode->position.x - fromNode->position.x) * (toNode->position.x - fromNode->position.x) + (toNode->position.y - fromNode->position.y) * (toNode->position.y - fromNode->position.y));
	float angle = std::atan2(toNode->position.y - fromNode->position.y, toNode->position.x - fromNode->position.x);

	position.x = fromNode->position.x + Motion::Bezier(ratio) * length * std::cos(angle);
	position.y = fromNode->position.y + Motion::Bezier(ratio) * length * std::sin(angle);

	if (!isDrawn) {
		return;
	}

	draw(window, circleSprite, circleColor, valueText, valueColor);
}

void Node::drawUpdate(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, int fromValue, int toValue, float ratio, bool isDrawn) {
	sf::Color newValueColor = *valueColor;
	if (ratio <= 0.5f) {
		this->value = fromValue;
		newValueColor *= sf::Color(255, 255, 255, std::round(Motion::Bezier(1.0f - 2.0f * ratio) * 255));
		draw(window, circleSprite, circleColor, valueText, &newValueColor, ratio, isDrawn);
	} else {
		this->value = toValue;
		newValueColor *= sf::Color(255, 255, 255, std::round(Motion::Bezier(2.0f * (ratio - 0.5f)) * 255));
		draw(window, circleSprite, circleColor, valueText, &newValueColor, ratio, isDrawn);
	}
}

void Node::drawChange(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Text* valueText, sf::Color* fromValueColor, sf::Color* toValueColor, float ratio, bool isDrawn) {	
	if (!isDrawn) {
		return;
	}

	sf::Color newCircleColor = *fromCircleColor;
	newCircleColor.r += std::round(Motion::Bezier(ratio) * (toCircleColor->r - (int)fromCircleColor->r));
	newCircleColor.g += std::round(Motion::Bezier(ratio) * (toCircleColor->g - (int)fromCircleColor->g));
	newCircleColor.b += std::round(Motion::Bezier(ratio) * (toCircleColor->b - (int)fromCircleColor->b));
	newCircleColor.a += std::round(Motion::Bezier(ratio) * (toCircleColor->a - (int)fromCircleColor->a));

	sf::Color newValueColor = *fromValueColor;
	newValueColor.r += std::round(Motion::Bezier(ratio) * (toValueColor->r - (int)fromValueColor->r));
	newValueColor.g += std::round(Motion::Bezier(ratio) * (toValueColor->g - (int)fromValueColor->g));
	newValueColor.b += std::round(Motion::Bezier(ratio) * (toValueColor->b - (int)fromValueColor->b));
	newValueColor.a += std::round(Motion::Bezier(ratio) * (toValueColor->a - (int)fromValueColor->a));

	draw(window, circleSprite, &newCircleColor, valueText, &newValueColor);
}