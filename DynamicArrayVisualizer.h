#ifndef DYNAMIC_ARRAY_VISUALIZER
#define DYNAMIC_ARRAY_VISUALIZER

#include "Global.h"
#include "Assets.h"
#include "HeaderBox.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class DynamicArrayVisualizer {
public:
	DynamicArrayVisualizer(sf::RenderWindow* window, Assets* assets);

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
	List <Cell> cells;
	List <Label> labels;

	List <Cell> randomCells;
	List <Label> randomLabels;

	void emptyArray();
	void zeroArray(int size);
	void randomArray(int size);
	void manualArray(std::string listOfValues);
	void create();

	void access(int index);

	void search(int value);

	void update(int index, int value);

	void insertAtTheFront(int value);
	void insertAtTheBack(int value);
	void insertAtTheMiddle(int index, int value);

	void eraseAtTheFront();
	void eraseAtTheBack();
	void eraseAtTheMiddle(int index);
};

#endif