#ifndef STACK_VISUALIZER
#define STACK_VISUALIZER

#include "Global.h"
#include "Assets.h"
#include "HeaderBox.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class StackVisualizer {
public:
	StackVisualizer(sf::RenderWindow* window, Assets* assets);

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

	List <Node> deletedNodes;
	List <Label> deletedLabels;
	List <Edge> deletedEdges;

	void randomStack(int size);
	void manualStack(std::string listOfValues);
	void create();

	void peek();

	void push(int value);

	void pop();

	void clear();
};

#endif