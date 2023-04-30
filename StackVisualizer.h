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

	void randomStack(int size);
	void manualStack(std::string listOfValues);
	void createStack();

	void peek();
	
	void push(int value);
	
	void pop();

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
};

#endif