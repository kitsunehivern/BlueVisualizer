#ifndef QUEUE_VISUALIZER_H
#define QUEUE_VISUALIZER_H

#include "Global.h"
#include "Assets.h"
#include "HeaderBox.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class QueueVisualizer {
public:
	QueueVisualizer(sf::RenderWindow* window, Assets* assets);

	void run();

private:
	sf::RenderWindow* window;
	Assets* assets;

	HeaderBox header;
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

	void randomQueue(int size);
	void manualQueue(std::string listOfValues);
	void create();

	void peekAtTheFront();
	void peekAtTheBack();

	void enqueue(int value);

	void dequeue();
};

#endif