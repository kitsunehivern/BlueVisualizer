#ifndef QUEUE_VISUALIZER_H
#define QUEUE_VISUALIZER_H

#include "Global.h"
#include "Assets.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class QueueVisualizer {
public:
	QueueVisualizer(sf::RenderWindow* window, Assets* assets);

	void randomQueue(int size);
	void manualQueue(std::string listOfValues);
	void createQueue();

	void peekAtTheFront();
	void peekAtTheBack();

	void enqueue(int value);

	void dequeue();

	void run();

private:
	sf::RenderWindow* window;
	Assets* assets;

	ActionBox action;
	CodeBox code;
	OptionBox option;
	DescriptionBox description;

	List <Node> nodes;
	List <Label> labels;
	List <Edge> edges;

	Node deletedNode;
	Label deletedLabel;
	Edge deletedEdge;
};

#endif