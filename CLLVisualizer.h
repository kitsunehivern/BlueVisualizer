#ifndef CLL_VISUALIZER_H
#define CLL_VISUALIZER_H

#include "Global.h"
#include "Assets.h"
#include "HeaderBox.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class CLLVisualizer {
public:
	CLLVisualizer(sf::RenderWindow* window, Assets* assets);

	void run();

private:
	sf::RenderWindow* window;
	Assets* assets;

	HeaderBox header;
	ActionBox action;
	CodeBox code;
	OptionBox option;
	DescriptionBox description;

	int maxPosition1, maxPosition2;
	List <Node> nodes;
	List <Label> labels;
	List <Edge> edges;
	CircularEdge circularEdge;

	Node randomNode;
	Edge randomEdge;
	CircularEdge randomCircularEdge;

	Node deletedNode;
	Label deletedLabel;
	Edge deletedEdge;

	void randomList(int size);
	void manualList(std::string listOfValues);
	void create();

	void search(int value);

	void update(int index, int value);

	void insertWhenEmpty(int value, bool head);
	void insertAtTheFront(int value);
	void insertAtTheBack(int value);
	void insertAtTheMiddle(int index, int value);
	void insert(int index, int value, bool head = true);

	void eraseToEmpty(bool head);
	void eraseAtTheFront();
	void eraseAtTheBack();
	void eraseAtTheMiddle(int index);
	void erase(int index, bool head = true);
};

#endif