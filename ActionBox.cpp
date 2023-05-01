#include "ActionBox.h"

ActionBox::ActionBox() {
}

ActionBox::ActionBox(sf::RenderWindow* window, Assets* assets, sf::Vector2f position) {
	this->window = window;
	this->assets = assets;
	this->position = position;
	this->speed = X2;
	clearAllSteps();
}

void ActionBox::addNewStep() {
	numberOfFrames.push_back(120);
	drawFunction.push_back(std::vector <std::function <void(float, bool)> > ());
}

void ActionBox::abortAllSteps() {
	goToEnding();
}

void ActionBox::clearAllSteps() {
	status = CONTINUE;
	direction = FORWARD;

	numberOfFrames.clear();
	drawFunction.clear();
	currentStep = 0;
	currentFrame = 0;
}

// Node animation
void ActionBox::draw(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {
	drawFunction.back().push_back(std::bind(&Node::draw, node, window, type == CHOLLOW ? &assets->hollowCircleSprite : &assets->solidCircleSprite, circleColor, &assets->consolasBoldText, valueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::draw(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {
	draw(&nodes->begin()->next(index)->data, type, circleColor, valueColor);
}

void ActionBox::draw(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		draw(nodes, i, type, circleColor, valueColor);
	}
}

void ActionBox::drawFadeIn(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {
	drawFunction.back().push_back(std::bind(&Node::drawFadeIn, node, window, type == CHOLLOW ? &assets->hollowCircleSprite : &assets->solidCircleSprite, circleColor, &assets->consolasBoldText, valueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawFadeIn(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {
	drawFadeIn(&nodes->begin()->next(index)->data, type, circleColor, valueColor);
}

void ActionBox::drawFadeIn(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {	
	for (int i = fromIndex; i <= toIndex; i++) {
		drawFadeIn(nodes, i, type, circleColor, valueColor);
	}
}

void ActionBox::drawFadeOut(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {
	drawFunction.back().push_back(std::bind(&Node::drawFadeOut, node, window, type == CHOLLOW ? &assets->hollowCircleSprite : &assets->solidCircleSprite, circleColor, &assets->consolasBoldText, valueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawFadeOut(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {
	drawFadeOut(&nodes->begin()->next(index)->data, type, circleColor, valueColor);
}

void ActionBox::drawFadeOut(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor) {	
	for (int i = fromIndex; i <= toIndex; i++) {
		drawFadeOut(nodes, i, type, circleColor, valueColor);
	}
}

void ActionBox::drawMove(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition) {
	drawFunction.back().push_back(std::bind(&Node::drawMove, node, window, type == CHOLLOW ? &assets->hollowCircleSprite : &assets->solidCircleSprite, circleColor, &assets->consolasBoldText, valueColor, fromPosition, toPosition, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawMove(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition) {
	drawMove(&nodes->begin()->next(index)->data, type, circleColor, valueColor, fromPosition, toPosition);
}

void ActionBox::drawSlide(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, Node* fromNode, Node* toNode) {
	drawFunction.back().push_back(std::bind(&Node::drawSlide, node, window, type == CHOLLOW ? &assets->hollowCircleSprite : &assets->solidCircleSprite, circleColor, &assets->consolasBoldText, valueColor, fromNode, toNode, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawSlide(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, Node* fromNode, Node* toNode) {
	drawSlide(&nodes->begin()->next(index)->data, type, circleColor, valueColor, fromNode, toNode);
}

void ActionBox::drawUpdate(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, int fromValue, int toValue) {
	drawFunction.back().push_back(std::bind(&Node::drawUpdate, node, window, type == CHOLLOW ? &assets->hollowCircleSprite : &assets->solidCircleSprite, circleColor, &assets->consolasBoldText, valueColor, fromValue, toValue, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawUpdate(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, int fromValue, int toValue) {
	drawUpdate(&nodes->begin()->next(index)->data, type, circleColor, valueColor, fromValue, toValue);
}

void ActionBox::drawChange(Node* node, CircleSpriteType type, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Color* fromValueColor, sf::Color* toValueColor) {
	drawFunction.back().push_back(std::bind(&Node::drawChange, node, window, type == CHOLLOW ? &assets->hollowCircleSprite : &assets->solidCircleSprite, fromCircleColor, toCircleColor, &assets->consolasBoldText, fromValueColor, toValueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawChange(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Color* fromValueColor, sf::Color* toValueColor) {
	drawChange(&nodes->begin()->next(index)->data, type, fromCircleColor, toCircleColor, fromValueColor, toValueColor);
}

void ActionBox::drawChange(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Color* fromValueColor, sf::Color* toValueColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		drawChange(&nodes->begin()->next(i)->data, type, fromCircleColor, toCircleColor, fromValueColor, toValueColor);
	}
}

// Cell animation
void ActionBox::draw(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {
	drawFunction.back().push_back(std::bind(&Cell::draw, cell, window, type == CHOLLOW ? &assets->hollowSquareSprite : &assets->solidSquareSprite, squareColor, &assets->consolasBoldText, valueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::draw(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {
	draw(&cells->begin()->next(index)->data, type, squareColor, valueColor);
}

void ActionBox::draw(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		draw(cells, i, type, squareColor, valueColor);
	}
}

void ActionBox::drawFadeIn(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {
	drawFunction.back().push_back(std::bind(&Cell::drawFadeIn, cell, window, type == CHOLLOW ? &assets->hollowSquareSprite : &assets->solidSquareSprite, squareColor, &assets->consolasBoldText, valueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawFadeIn(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {
	drawFadeIn(&cells->begin()->next(index)->data, type, squareColor, valueColor);
}

void ActionBox::drawFadeIn(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {	
	for (int i = fromIndex; i <= toIndex; i++) {
		drawFadeIn(cells, i, type, squareColor, valueColor);
	}
}

void ActionBox::drawFadeOut(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {
	drawFunction.back().push_back(std::bind(&Cell::drawFadeOut, cell, window, type == CHOLLOW ? &assets->hollowSquareSprite : &assets->solidSquareSprite, squareColor, &assets->consolasBoldText, valueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawFadeOut(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {
	drawFadeOut(&cells->begin()->next(index)->data, type, squareColor, valueColor);
}

void ActionBox::drawFadeOut(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor) {	
	for (int i = fromIndex; i <= toIndex; i++) {
		drawFadeOut(cells, i, type, squareColor, valueColor);
	}
}

void ActionBox::drawMove(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition) {
	drawFunction.back().push_back(std::bind(&Cell::drawMove, cell, window, type == CHOLLOW ? &assets->hollowSquareSprite : &assets->solidSquareSprite, squareColor, &assets->consolasBoldText, valueColor, fromPosition, toPosition, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawMove(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition) {
	drawMove(&cells->begin()->next(index)->data, type, squareColor, valueColor, fromPosition, toPosition);
}

void ActionBox::drawFadeInAndUpdate(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, int fromValue, int toValue) {
	drawFunction.back().push_back(std::bind(&Cell::drawFadeInAndUpdate, cell, window, type == CHOLLOW ? &assets->hollowSquareSprite : &assets->solidSquareSprite, squareColor, &assets->consolasBoldText, valueColor, fromValue, toValue, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawFadeInAndUpdate(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, int fromValue, int toValue) {
	drawFadeInAndUpdate(&cells->begin()->next(index)->data, type, squareColor, valueColor, fromValue, toValue);
}

void ActionBox::drawChangeAndUpdate(Cell* cell, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor, int fromValue, int toValue) {
	drawFunction.back().push_back(std::bind(&Cell::drawChangeAndUpdate, cell, window, type == CHOLLOW ? &assets->hollowSquareSprite : &assets->solidSquareSprite, fromSquareColor, toSquareColor, &assets->consolasBoldText, fromValueColor, toValueColor, fromValue, toValue, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawChangeAndUpdate(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor, int fromValue, int toValue) {
	drawChangeAndUpdate(&cells->begin()->next(index)->data, type, fromSquareColor, toSquareColor, fromValueColor, toValueColor, fromValue, toValue);
}

void ActionBox::drawChange(Cell* cell, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor) {
	drawFunction.back().push_back(std::bind(&Cell::drawChange, cell, window, type == CHOLLOW ? &assets->hollowSquareSprite : &assets->solidSquareSprite, fromSquareColor, toSquareColor, &assets->consolasBoldText, fromValueColor, toValueColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawChange(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor) {
	drawChange(&cells->begin()->next(index)->data, type, fromSquareColor, toSquareColor, fromValueColor, toValueColor);
}

void ActionBox::drawChange(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		drawChange(&cells->begin()->next(i)->data, type, fromSquareColor, toSquareColor, fromValueColor, toValueColor);
	}
}

// Label animation
void ActionBox::draw(Label* label, sf::Color* color, std::string name) {
	drawFunction.back().push_back(std::bind(&Label::draw, label, window, &assets->consolasBoldText, color, name, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::draw(List <Label>* labels, int index, sf::Color* color, std::string name) {
	draw(&labels->begin()->next(index)->data, color, name);
}

void ActionBox::draw(List <Label>* labels, sf::Color* color) {
	for (int i = 0; i < labels->size(); i++) {
		draw(&labels->begin()->next(i)->data, color, std::to_string(i));
	}
}

void ActionBox::drawFadeIn(Label* label, sf::Color* color, std::string name) {
	drawFunction.back().push_back(std::bind(&Label::drawFadeIn, label, window, &assets->consolasBoldText, color, name, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawFadeIn(List <Label>* labels, int index, sf::Color* color, std::string name) {
	drawFadeIn(&labels->begin()->next(index)->data, color, name);
}

void ActionBox::drawFadeIn(List <Label>* labels, sf::Color* color) {
	for (int i = 0; i < labels->size(); i++) {
		drawFadeIn(&labels->begin()->next(i)->data, color, std::to_string(i));
	}
}

void ActionBox::drawFadeOut(Label* label, sf::Color* color, std::string name) {
	drawFunction.back().push_back(std::bind(&Label::drawFadeOut, label, window, &assets->consolasBoldText, color, name, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawFadeOut(List <Label>* labels, int index, sf::Color* color, std::string name) {
	drawFadeOut(&labels->begin()->next(index)->data, color, name);
}

void ActionBox::drawFadeOut(List <Label>* labels, sf::Color* color) {
	for (int i = 0; i < labels->size(); i++) {
		drawFadeOut(&labels->begin()->next(i)->data, color, std::to_string(i));
	}
}

void ActionBox::drawChange(Label* label, sf::Color* color, std::string fromName, std::string toName) {
	drawFunction.back().push_back(std::bind(&Label::drawChange, label, window, &assets->consolasBoldText, color, fromName, toName, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawChange(List <Label>* labels, int index, sf::Color* color, std::string fromName, std::string toName) {
	drawChange(&labels->begin()->next(index)->data, color, fromName, toName);
}

// Edge animation
void ActionBox::draw(Edge* edge, sf::Color* color) {
	drawFunction.back().push_back(std::bind(&Edge::draw, edge, window, &assets->stickSprite, &assets->arrowSprite, color, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::draw(List <Edge>* edges, int index, sf::Color* color) {
	draw(&edges->begin()->next(index)->data, color);
}

void ActionBox::draw(List <Edge>* edges, int fromIndex, int toIndex, sf::Color* color) {
	for (int i = fromIndex; i <= toIndex; i++) {
		draw(edges, i, color);
	}
}

void ActionBox::drawSlideIn(Edge* edge, sf::Color* color) {
	drawFunction.back().push_back(std::bind(&Edge::drawSlideIn, edge, window, &assets->stickSprite, &assets->arrowSprite, color, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawSlideIn(List <Edge>* edges, int index, sf::Color* color) {
	drawSlideIn(&edges->begin()->next(index)->data, color);
}

void ActionBox::drawSlideIn(List <Edge>* edges, int fromIndex, int toIndex, sf::Color* color) {
	for (int i = fromIndex; i <= toIndex; i++) {
		drawSlideIn(edges, i, color);
	}
}

void ActionBox::drawSlideOut(Edge* edge, sf::Color* color) {
	drawFunction.back().push_back(std::bind(&Edge::drawSlideOut, edge, window, &assets->stickSprite, &assets->arrowSprite, color, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawSlideOut(List <Edge>* edges, int index, sf::Color* color) {
	drawSlideOut(&edges->begin()->next(index)->data, color);
}

void ActionBox::drawSlideOut(List <Edge>* edges, int fromOutdex, int toOutdex, sf::Color* color) {
	for (int i = fromOutdex; i <= toOutdex; i++) {
		drawSlideOut(edges, i, color);
	}
}

void ActionBox::drawChange(Edge* edge, sf::Color* fromColor, sf::Color* toColor) {
	drawFunction.back().push_back(std::bind(&Edge::drawChange, edge, window, &assets->stickSprite, &assets->arrowSprite, fromColor, toColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawChange(List <Edge>* edges, int index, sf::Color* fromColor, sf::Color* toColor) {
	drawChange(&edges->begin()->next(index)->data, fromColor, toColor);
}

void ActionBox::drawChange(List <Edge>* edges, int fromIndex, int toIndex, sf::Color* fromColor, sf::Color* toColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		drawChange(&edges->begin()->next(i)->data, fromColor, toColor);
	}
}

// DoublyEdge animation
void ActionBox::draw(DoublyEdge* edge, sf::Color* leftColor, sf::Color* rightColor) {
	drawFunction.back().push_back(std::bind(&DoublyEdge::draw, edge, window, &assets->stickSprite, &assets->arrowSprite, leftColor, rightColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::draw(List <DoublyEdge>* edges, int index, sf::Color* leftColor, sf::Color* rightColor) {
	draw(&edges->begin()->next(index)->data, leftColor, rightColor);
}

void ActionBox::draw(List <DoublyEdge>* edges, int fromIndex, int toIndex, sf::Color* leftColor, sf::Color* rightColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		draw(edges, i, leftColor, rightColor);
	}
}

void ActionBox::drawSlideIn(DoublyEdge* edge, sf::Color* leftColor, sf::Color* rightColor) {
	drawFunction.back().push_back(std::bind(&DoublyEdge::drawSlideIn, edge, window, &assets->stickSprite, &assets->arrowSprite, leftColor, rightColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawSlideIn(List <DoublyEdge>* edges, int index, sf::Color* leftColor, sf::Color* rightColor) {
	drawSlideIn(&edges->begin()->next(index)->data, leftColor, rightColor);
}

void ActionBox::drawSlideIn(List <DoublyEdge>* edges, int fromIndex, int toIndex, sf::Color* leftColor, sf::Color* rightColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		drawSlideIn(edges, i, leftColor, rightColor);
	}
}

void ActionBox::drawSlideOut(DoublyEdge* edge, sf::Color* leftColor, sf::Color* rightColor) {
	drawFunction.back().push_back(std::bind(&DoublyEdge::drawSlideOut, edge, window, &assets->stickSprite, &assets->arrowSprite, leftColor, rightColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawSlideOut(List <DoublyEdge>* edges, int index, sf::Color* leftColor, sf::Color* rightColor) {
	drawSlideOut(&edges->begin()->next(index)->data, leftColor, rightColor);
}

void ActionBox::drawSlideOut(List <DoublyEdge>* edges, int fromOutdex, int toOutdex, sf::Color* leftColor, sf::Color* rightColor) {
	for (int i = fromOutdex; i <= toOutdex; i++) {
		drawSlideOut(edges, i, leftColor, rightColor);
	}
}

void ActionBox::drawChange(DoublyEdge* edge, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRightColor, sf::Color* toRightColor) {
	drawFunction.back().push_back(std::bind(&DoublyEdge::drawChange, edge, window, &assets->stickSprite, &assets->arrowSprite, fromLeftColor, toLeftColor, fromRightColor, toRightColor, std::placeholders::_1,  std::placeholders::_2));
}

void ActionBox::drawChange(List <DoublyEdge>* edges, int index, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRightColor, sf::Color* toRightColor) {
	drawChange(&edges->begin()->next(index)->data, fromLeftColor, toLeftColor, fromRightColor, toRightColor);
}

void ActionBox::drawChange(List <DoublyEdge>* edges, int fromIndex, int toIndex, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRightColor, sf::Color* toRightColor) {
	for (int i = fromIndex; i <= toIndex; i++) {
		drawChange(&edges->begin()->next(i)->data, fromLeftColor, toLeftColor, fromRightColor, toRightColor);
	}
}

// CircularEdge animation
void ActionBox::draw(CircularEdge* edge, sf::Color* color) {
	drawFunction.back().push_back(std::bind(&CircularEdge::draw, edge, window, &assets->stickSprite, &assets->arrowSprite, color, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawSlideIn(CircularEdge* edge, sf::Color* color) {
	drawFunction.back().push_back(std::bind(&CircularEdge::drawSlideIn, edge, window, &assets->stickSprite, &assets->arrowSprite, color, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawSlideOut(CircularEdge* edge, sf::Color* color) {
	drawFunction.back().push_back(std::bind(&CircularEdge::drawSlideOut, edge, window, &assets->stickSprite, &assets->arrowSprite, color, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawChange(CircularEdge* edge, sf::Color* fromColor, sf::Color* toColor) {
	drawFunction.back().push_back(std::bind(&CircularEdge::drawChange, edge, window, &assets->stickSprite, &assets->arrowSprite, fromColor, toColor, std::placeholders::_1, std::placeholders::_2));
}

// Code animtion
void ActionBox::draw(CodeBox* code) {
	drawFunction.back().push_back(std::bind(&CodeBox::draw, code, window, &assets->box680x300Sprite, &assets->box680x40Sprite, &assets->consolasText, true, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawFadeIn(CodeBox* code, int line) {
	drawFunction.back().push_back(std::bind(&CodeBox::drawFadeIn, code, window, &assets->box680x300Sprite, &assets->box680x40Sprite, &assets->consolasText, line, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawFadeOut(CodeBox* code, int line) {
	drawFunction.back().push_back(std::bind(&CodeBox::drawFadeOut, code, window, &assets->box680x300Sprite, &assets->box680x40Sprite, &assets->consolasText, line, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawMove(CodeBox* code, int fromLine, int toLine) {
	drawFunction.back().push_back(std::bind(&CodeBox::drawMove, code, window, &assets->box680x300Sprite, &assets->box680x40Sprite, &assets->consolasText, fromLine, toLine, std::placeholders::_1, std::placeholders::_2));
}

// Description animation
void ActionBox::draw(DescriptionBox* description, int line) {
	drawFunction.back().push_back(std::bind(&DescriptionBox::draw, description, window, &assets->box680x140Sprite, &assets->consolasBoldText, &assets->consolasText, line, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawFadeIn(DescriptionBox* description, int line) {
	drawFunction.back().push_back(std::bind(&DescriptionBox::drawFadeIn, description, window, &assets->box680x140Sprite, &assets->consolasBoldText, &assets->consolasText, line, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawFadeOut(DescriptionBox* description, int line) {
	drawFunction.back().push_back(std::bind(&DescriptionBox::drawFadeOut, description, window, &assets->box680x140Sprite, &assets->consolasBoldText, &assets->consolasText, line, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::drawChange(DescriptionBox* description, int fromLine, int toLine) {
	drawFunction.back().push_back(std::bind(&DescriptionBox::drawChange, description, window, &assets->box680x140Sprite, &assets->consolasBoldText, &assets->consolasText, fromLine, toLine, std::placeholders::_1, std::placeholders::_2));
}

void ActionBox::draw() {
	sf::Sprite* boxSprite = &assets->box1580x320Sprite;
	sf::Sprite* controlBoxSprite = &assets->box880x60Sprite;
	sf::Sprite* controlButtonsSprite = &assets->controlButtonsSprite;
	sf::Sprite* statusButtonsSprite = &assets->statusButtonsSprite;
	sf::Sprite* progressBarSprite = &assets->progressBarSprite;
	sf::Sprite* speedBoxSprite = &assets->skewBox100x40Sprite;
	sf::Sprite* speedSprite = &assets->speedSprite;
	
	boxSprite->setColor(assets->boxColor2);
	boxSprite->setPosition(10, 90);
	window->draw(*boxSprite);

	controlBoxSprite->setColor(assets->boxColor2);
	controlBoxSprite->setPosition(position);
	window->draw(*controlBoxSprite);

	controlButtonsSprite->setTextureRect(sf::IntRect(0, 0, 20, 30));
	controlButtonsSprite->setPosition(position + sf::Vector2f(30, 15));
	if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 10, position.y, 60, 60))) {
		controlButtonsSprite->setColor(assets->boxColor4);
	} else {
		controlButtonsSprite->setColor(assets->boxColor3);
	}

	window->draw(*controlButtonsSprite);

	controlButtonsSprite->setTextureRect(sf::IntRect(20, 0, 20, 30));
	controlButtonsSprite->setPosition(position + sf::Vector2f(90, 15));
	if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 70, position.y, 60, 60))) {
		controlButtonsSprite->setColor(assets->boxColor4);
	} else {
		controlButtonsSprite->setColor(assets->boxColor3);
	}

	window->draw(*controlButtonsSprite);

	int index = -1;
	if (status == PAUSED && direction == NONE) {
		index = 1;
	} else if (status == REPLAY) {
		index = 2;
	} else {
		index = 0;
	}

	statusButtonsSprite->setTextureRect(sf::IntRect(30 * index, 0, 30, 30));
	statusButtonsSprite->setPosition(position + sf::Vector2f(145, 15));
	if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 130, position.y, 60, 60))) {
		statusButtonsSprite->setColor(assets->boxColor4);
	} else {
		statusButtonsSprite->setColor(assets->boxColor3);
	}

	window->draw(*statusButtonsSprite);

	controlButtonsSprite->setTextureRect(sf::IntRect(40, 0, 20, 30));
	controlButtonsSprite->setPosition(position + sf::Vector2f(210, 15));
	if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 190, position.y, 60, 60))) {
		controlButtonsSprite->setColor(assets->boxColor4);
	} else {
		controlButtonsSprite->setColor(assets->boxColor3);
	}

	window->draw(*controlButtonsSprite);

	controlButtonsSprite->setTextureRect(sf::IntRect(60, 0, 20, 30));
	controlButtonsSprite->setPosition(position + sf::Vector2f(270, 15));
	if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 250, position.y, 60, 60))) {
		controlButtonsSprite->setColor(assets->boxColor4);
	} else {
		controlButtonsSprite->setColor(assets->boxColor3);
	}

	window->draw(*controlButtonsSprite);

	progressBarSprite->setTextureRect(sf::IntRect(0, 0, 400, 20));
	progressBarSprite->setColor(assets->boxColor3);
	progressBarSprite->setPosition(position + sf::Vector2f(320, 20));
	window->draw(*progressBarSprite);

	progressBarSprite->setTextureRect(sf::IntRect(0, 0, std::floor(400.f * getNumberOfFramePassed() / (120 * numberOfFrames.size())), 20));
	progressBarSprite->setColor(assets->boxColor4);
	window->draw(*progressBarSprite);

	sf::RectangleShape seperatorLine(sf::Vector2f(1, 20));
	seperatorLine.setFillColor(assets->boxColor2);
	seperatorLine.setOrigin(seperatorLine.getSize().x / 2, 0);
	for (int i = 1; i < numberOfFrames.size(); i++) {
		seperatorLine.setPosition(progressBarSprite->getPosition() + sf::Vector2f(std::floor(400.f * i / numberOfFrames.size()), 0));
		window->draw(seperatorLine);
	}

	speedBoxSprite->setColor(assets->boxColor3);
	speedBoxSprite->setPosition(position + sf::Vector2f(750, 10));
	if (positionInRect(sf::Mouse::getPosition(*window), speedBoxSprite->getGlobalBounds())) {
		speedBoxSprite->setColor(assets->boxColor4);
	} else {
		speedBoxSprite->setColor(assets->boxColor3);
	}

	window->draw(*speedBoxSprite);

	if (speed == X1) {
		index = 0;
	} else if (speed == X2) {
		index = 1;
	} else {
		index = 2;
	}

	speedSprite->setTextureRect(sf::IntRect(0, 40 * index, 100, 40));
	speedSprite->setColor(assets->boxColor4);
	speedSprite->setPosition(speedBoxSprite->getPosition());
	if (positionInRect(sf::Mouse::getPosition(*window), speedBoxSprite->getGlobalBounds())) {
		speedSprite->setColor(assets->boxColor3);
	} else {
		speedSprite->setColor(assets->boxColor4);
	}

	window->draw(*speedSprite);

	if (status == PAUSED) {
		if (direction == FORWARD) {
			if (currentFrame < numberOfFrames[currentStep]) {
				currentFrame += std::min((int)speed, numberOfFrames[currentStep] - currentFrame);
			} else {
				status = (currentStep == numberOfFrames.size() - 1) ? REPLAY : PAUSED;
				direction = NONE;
			}
		} else if (direction == BACKWARD) {
			if (currentFrame > 0) {
				currentFrame -= std::min((int)speed, currentFrame);
			} else {
				direction = NONE;
			}
		}
	} else if (status == CONTINUE) {
		direction = FORWARD;

		if (direction == FORWARD) {
			if (currentFrame < numberOfFrames[currentStep]) {
				currentFrame += std::min((int)speed, numberOfFrames[currentStep] - currentFrame);
			} else {
				if (currentStep < numberOfFrames.size() - 1) {
					currentStep++;
					currentFrame = 0;

					currentFrame += std::min((int)speed, numberOfFrames[currentStep] - currentFrame);
				} else {
					status = REPLAY;
					direction = NONE;
				}
			}
		}
	} else if (status == REPLAY) {
		assert(currentStep == numberOfFrames.size() - 1 && currentFrame == numberOfFrames[currentStep]);

		if (direction == BACKWARD) {
			status = PAUSED;
			if (currentFrame > 0) {
				currentFrame -= std::min((int)speed, currentFrame);
			} else {
				direction = NONE;
			}
		}
	}

	for (auto draw : drawFunction[currentStep]) {
		draw(currentFrame / (float)numberOfFrames[currentStep], false);
	}

	for (auto draw : drawFunction[currentStep]) {
		draw(currentFrame / (float)numberOfFrames[currentStep], true);
	}
}

int ActionBox::getNumberOfFramePassed() {
	return 120 * currentStep + currentFrame;
}

void ActionBox::goToNextStep() {
	auto go = [&]() -> void {
		for (auto draw : drawFunction[currentStep]) {
			draw(1.0f, false);
		}

		currentFrame = numberOfFrames[currentStep];
		status = (currentStep == numberOfFrames.size() - 1) ? REPLAY : PAUSED;
		direction = NONE;
	};

	if (status == PAUSED) {
		if (direction == FORWARD) {
			go();
		} else if (direction == BACKWARD) {
			direction = FORWARD;
		} else if (direction == NONE) {
			direction = FORWARD;
			if (currentFrame == numberOfFrames[currentStep] && currentStep < numberOfFrames.size() - 1) {
				currentStep++;
				currentFrame = 0;
			}
		}
	} else if (status == CONTINUE) {
		assert(direction == FORWARD);

		if (direction == FORWARD) {
			status = PAUSED;
		}
	}
}

void ActionBox::goToPrevStep() {
	auto go = [&]() -> void {
		for (auto draw : drawFunction[currentStep]) {
			draw(0.0f, false);
		}

		currentFrame = 0;
		status = PAUSED;
		direction = NONE;
	};

	if (status == PAUSED || status == REPLAY) {
		if (direction == BACKWARD) {
			go();
		} else if (direction == FORWARD) {
			direction = BACKWARD;
		} else if (direction == NONE) {
			if (currentStep > 0 || currentFrame > 0) {
				direction = BACKWARD;
			}

			if (currentFrame == 0 && currentStep > 0) {
				currentStep--;
				currentFrame = numberOfFrames[currentStep];
			}
		}
	} else if (status == CONTINUE) {
		assert(direction == FORWARD);

		if (direction == FORWARD) {
			status = PAUSED;
			direction = BACKWARD;
		}
	}
}

void ActionBox::goToEnding() {
	while (currentStep < numberOfFrames.size() - 1 || currentFrame < numberOfFrames[currentStep]) {
		goToNextStep();
	}
}

void ActionBox::goToBeginning() {
	while (currentStep > 0 || currentFrame > 0) {
		goToPrevStep();
	}
}

void ActionBox::goToSpecificFrame(int step, int frame) {
	while (currentStep < step) {
		for (auto draw : drawFunction[currentStep]) {
			draw(1.0f, false);
		}

		currentStep++;
	}

	while (currentStep > step) {
		for (auto draw : drawFunction[currentStep]) {
			draw(0.0f, false);
		}

		currentStep--;
	}

	currentFrame = frame;
}

void ActionBox::handleEvent(sf::Event* event, OptionBox* option) {
	static bool mouseButtonHolding = false;
	if (event->type == sf::Event::MouseButtonPressed) {
		if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 320, position.y + 20, 400, 20))) {
			mouseButtonHolding = true;
		}
	} else if (event->type == sf::Event::MouseButtonReleased) {
		if (!mouseButtonHolding) {
			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 10, position.y, 60, 60))) {
				goToBeginning();
			}

			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 70, position.y, 60, 60))) {
				goToPrevStep();
			}

			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 130, position.y, 60, 60))) {
				if (status == PAUSED) {
					if (direction == NONE) {
						status = CONTINUE;
					} else {
						direction = NONE;
					}
				} else if (status == CONTINUE) {
					status = PAUSED;
					direction = NONE;
				} else if (status == REPLAY) {
					goToBeginning();
					status = CONTINUE;
					direction = FORWARD;
				}
			}

			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 190, position.y, 60, 60))) {
				goToNextStep();
			}

			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 250, position.y, 60, 60))) {
				goToEnding();
			}

			if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 750, position.y + 10, 100, 40))) {
				if (speed & X1) {
					speed = X2;
				} else if (speed & X2) {
					speed = X4;
				} else {
					speed = X1;
				}
			}
		}

		mouseButtonHolding = false;
	} else if (event->type == sf::Event::KeyPressed) {
		if (!option->isFocus()) {
			if (event->key.code == sf::Keyboard::Left) {
				goToPrevStep();
			} else if (event->key.code == sf::Keyboard::Right) {
				goToNextStep();
			} else if (event->key.code == sf::Keyboard::Space) {
				if (status == PAUSED) {
					if (direction == NONE) {
						status = CONTINUE;
					} else {
						direction = NONE;
					}
				} else if (status == CONTINUE) {
					status = PAUSED;
					direction = NONE;
				} else if (status == REPLAY) {
					goToBeginning();
					status = CONTINUE;
					direction = FORWARD;
				}
			}
		}
	}

	if (mouseButtonHolding) {
		int positionX = sf::Mouse::getPosition(*window).x;
		positionX = std::max(positionX, (int)position.x + 320);
		positionX = std::min(positionX, (int)position.x + 720);
		positionX -= position.x + 320;

		assert(0 <= positionX && positionX <= 400);

		int step = positionX * numberOfFrames.size() / 400.0f;
		int frame = (positionX - step * 400.0f / numberOfFrames.size()) / (400.0f / numberOfFrames.size()) * 120;
		if (step == numberOfFrames.size()) {
			step--;
			frame = numberOfFrames[step];
		}

		goToSpecificFrame(step, frame);

		status = (step == numberOfFrames.size() - 1 && frame == numberOfFrames[step]) ? REPLAY : PAUSED;
		direction = NONE;
	}
}