#ifndef CLL_VISUALIZER_H
#define CLL_VISUALIZER_H

#include "Global.h"
#include "Assets.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class CLLVisualizer {
public:
	CLLVisualizer(sf::RenderWindow* window, Assets* assets);

	void randomList(int size);
	void manualList(std::string listOfValues);
	void createList();

	void searchValue(int value);

	void updateValue(int index, int value);

	void insertAtTheFront(int value, bool head);
	void insertAtTheBack(int value);
	void insertAtTheMiddle(int index, int value);
	void insertNode(int index, int value, bool head = true);

	void eraseAtTheFront(bool head);
	void eraseAtTheBack();
	void eraseAtTheMiddle(int index);
	void eraseNode(int index, bool head = true);

	void run();

private:
	sf::RenderWindow* window;
	Assets* assets;

	ActionBox action;
	CodeBox code;
	OptionBox option;
	DescriptionBox description;

	int maxPosition1, maxPosition2;
	List <Node> nodes;
	List <Label> labels;
	List <Edge> edges;
	CircularEdge circularEdge;

	Node deletedNode;
	Label deletedLabel;
	Edge deletedEdge;
};

#endif