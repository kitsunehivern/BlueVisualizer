#ifndef DLL_VISUALIZER_H
#define DLL_VISUALIZER_H

#include "Global.h"
#include "Assets.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class DLLVisualizer {
public:
	DLLVisualizer(sf::RenderWindow* window, Assets* assets);

	void randomList(int size);
	void manualList(std::string listOfValues);
	void create();

	void search(int value);

	void update(int index, int value);

	void insertAtTheFront(int value, bool head);
	void insertAtTheBack(int value);
	void insertAtTheMiddle(int index, int value);
	void insert(int index, int value, bool head = true);

	void eraseAtTheFront(bool head);
	void eraseAtTheBack();
	void eraseAtTheMiddle(int index);
	void erase(int index, bool head = true);

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
	List <DoublyEdge> edges;

	Node randomNode1;
	Label randomLabel1;
	DoublyEdge randomEdge1;

	Node randomNode2;
	Label randomLabel2;
	DoublyEdge randomEdge2;

	DoublyEdge randomEdge;

	Node deletedNode;
	Label deletedLabel;
	DoublyEdge deletedEdge;
};

#endif