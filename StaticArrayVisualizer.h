#ifndef STATIC_ARRAY_VISUALIZER
#define STATIC_ARRAY_VISUALIZER

#include "Global.h"
#include "Assets.h"
#include "HeaderBox.h"
#include "ActionBox.h"
#include "OptionBox.h"
#include "DescriptionBox.h"

class StaticArrayVisualizer {
public:
	StaticArrayVisualizer(sf::RenderWindow* window, Assets* assets);

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
	int size;
	List <Cell> cells;
	List <Label> labels;

	void zeroArray(int capacity, int size);
	void randomArray(int capacity, int size);
	void manualArray(std::string listOfValues);
	void create();

	void access(int index);

	void search(int value);

	void update(int index, int value);

	void insertAtTheFirst(int value);
	void insertAtTheBack(int value);
	void insertAtTheMiddle(int index, int value);

	void eraseAtTheFirst();
	void eraseAtTheBack();
	void eraseAtTheMiddle(int index);
};

#endif