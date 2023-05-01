#ifndef GRAPH_VISUALIZER_H
#define GRAPH_VISUALIZER_H

#include "Global.h"

#include "List.h"
#include "Assets.h"
#include "Node.h"
#include "Cell.h"
#include "Label.h"
#include "Edge.h"
#include "DoublyEdge.h"
#include "CircularEdge.h"
#include "CodeBox.h"
#include "DescriptionBox.h"
#include "OptionBox.h"

enum CircleSpriteType { CHOLLOW, CSOLID };
enum SquareSpriteType { SHOLLOW, SSOLID };

class ActionBox {
public:
	ActionBox();
	ActionBox(sf::RenderWindow* window, Assets* assets, sf::Vector2f position);

	void addNewStep();
	void abortAllSteps();
	void clearAllSteps();

	// Node animation
	void draw(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);
	void draw(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);
	void draw(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);

	void drawFadeIn(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);
	void drawFadeIn(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);
	void drawFadeIn(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);

	void drawFadeOut(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);
	void drawFadeOut(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);
	void drawFadeOut(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor);

	void drawMove(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition);
	void drawMove(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition);

	void drawSlide(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, Node* fromNode, Node* toNode);
	void drawSlide(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, Node* fromNode, Node* toNode);

	void drawUpdate(Node* node, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, int fromValue, int toValue);
	void drawUpdate(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* circleColor, sf::Color* valueColor, int fromValue, int toValue);

	void drawChange(Node* node, CircleSpriteType type, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Color* fromValueColor, sf::Color* toValueColor);
	void drawChange(List <Node>* nodes, int index, CircleSpriteType type, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Color* fromValueColor, sf::Color* toValueColor);
	void drawChange(List <Node>* nodes, int fromIndex, int toIndex, CircleSpriteType type, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Color* fromValueColor, sf::Color* toValueColor);

	// Cell animation
	void draw(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);
	void draw(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);
	void draw(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);

	void drawFadeIn(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);
	void drawFadeIn(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);
	void drawFadeIn(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);

	void drawFadeOut(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);
	void drawFadeOut(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);
	void drawFadeOut(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor);

	void drawMove(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition);
	void drawMove(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition);

	void drawFadeInAndUpdate(Cell* cell, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, int fromValue, int toValue);
	void drawFadeInAndUpdate(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* squareColor, sf::Color* valueColor, int fromValue, int toValue);

	void drawChangeAndUpdate(Cell* cell, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor, int fromValue, int toValue);
	void drawChangeAndUpdate(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor, int fromValue, int toValue);

	void drawChange(Cell* cell, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor);
	void drawChange(List <Cell>* cells, int index, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor);
	void drawChange(List <Cell>* cells, int fromIndex, int toIndex, SquareSpriteType type, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Color* fromValueColor, sf::Color* toValueColor);

	// Label animation
	void draw(Label* label, sf::Color* color, std::string name);
	void draw(List <Label>* labels, int index, sf::Color* color, std::string name);
	void draw(List <Label>* labels, sf::Color* color);

	void drawFadeIn(Label* label, sf::Color* color, std::string name);
	void drawFadeIn(List <Label>* labels, int index, sf::Color* color, std::string name);
	void drawFadeIn(List <Label>* labels, sf::Color* color);

	void drawFadeOut(Label* label, sf::Color* color, std::string name);
	void drawFadeOut(List <Label>* labels, int index, sf::Color* color, std::string name);
	void drawFadeOut(List <Label>* labels, sf::Color* color);

	void drawChange(Label* label, sf::Color* color, std::string fromName, std::string toName);
	void drawChange(List <Label>* labels, int index, sf::Color* color, std::string fromName, std::string toName);

	// Edge animation
	void draw(Edge* edge, sf::Color* color);
	void draw(List <Edge>* edges, int index, sf::Color* color);
	void draw(List <Edge>* edges, int fromIndex, int toIndex, sf::Color* color);

	void drawSlideIn(Edge* edge, sf::Color* color);
	void drawSlideIn(List <Edge>* edges, int index, sf::Color* color);
	void drawSlideIn(List <Edge>* edges, int fromIndex, int toIndex, sf::Color* color);

	void drawSlideOut(Edge* edge, sf::Color* color);
	void drawSlideOut(List <Edge>* edges, int index, sf::Color* color);
	void drawSlideOut(List <Edge>* edges, int fromIndex, int toIndex, sf::Color* color);

	void drawChange(Edge* edge, sf::Color* fromColor, sf::Color* toColor);
	void drawChange(List <Edge>* edges, int index, sf::Color* fromColor, sf::Color* toColor);
	void drawChange(List <Edge>* edges, int fromIndex, int toIndex, sf::Color* fromColor, sf::Color* toColor);

	// DoublyEdge animation
	void draw(DoublyEdge* edge, sf::Color* leftColor, sf::Color* rightColor);
	void draw(List <DoublyEdge>* edges, int index, sf::Color* leftColor, sf::Color* rightColor);
	void draw(List <DoublyEdge>* edges, int fromIndex, int toIndex, sf::Color* leftColor, sf::Color* rightColor);

	void drawSlideIn(DoublyEdge* edge, sf::Color* leftColor, sf::Color* rightColor);
	void drawSlideIn(List <DoublyEdge>* edges, int index, sf::Color* leftColor, sf::Color* rightColor);
	void drawSlideIn(List <DoublyEdge>* edges, int fromIndex, int toIndex, sf::Color* leftColor, sf::Color* rightColor);

	void drawSlideOut(DoublyEdge* edge, sf::Color* leftColor, sf::Color* rightColor);
	void drawSlideOut(List <DoublyEdge>* edges, int index, sf::Color* leftColor, sf::Color* rightColor);
	void drawSlideOut(List <DoublyEdge>* edges, int fromIndex, int toIndex, sf::Color* leftColor, sf::Color* rightColor);

	void drawChange(DoublyEdge* edge, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRIghtColor, sf::Color* toRightColor);
	void drawChange(List <DoublyEdge>* edges, int index, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRIghtColor, sf::Color* toRightColor);
	void drawChange(List <DoublyEdge>* edges, int fromIndex, int toIndex, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRIghtColor, sf::Color* toRightColor);

	// CircularEdge animation
	void draw(CircularEdge* edge, sf::Color* color);

	void drawSlideIn(CircularEdge* edge, sf::Color* color);

	void drawSlideOut(CircularEdge* edge, sf::Color* color);

	void drawChange(CircularEdge* edge, sf::Color* fromColor, sf::Color* toColor);

	// Cell animtion

	// Code animation
	void draw(CodeBox* code);

	void drawFadeIn(CodeBox* code, int line);

	void drawFadeOut(CodeBox* code, int line);

	void drawMove(CodeBox* code, int fromLine, int toLine);

	// Description animation
	void draw(DescriptionBox* description, int line);

	void drawFadeIn(DescriptionBox* description, int line);

	void drawFadeOut(DescriptionBox* description, int line);

	void drawChange(DescriptionBox* description, int froLine, int toLine);

	void draw();

	void handleEvent(sf::Event* event, OptionBox* option);

private:
	sf::RenderWindow* window;
	Assets* assets;
	sf::Vector2f position;

	enum { PAUSED, CONTINUE, REPLAY } status;
	enum { FORWARD, BACKWARD, NONE } direction;
	enum { X1 = 1, X2 = 2, X4 = 4 } speed;
	int currentStep, currentFrame;
	std::vector <int> numberOfFrames; 
	std::vector <std::vector <std::function <void(float, bool)> > > drawFunction;

	int getNumberOfFramePassed();
	void goToNextStep();
	void goToPrevStep();
	void goToEnding();
	void goToBeginning();
	void goToSpecificFrame(int step, int frame);
};

#endif