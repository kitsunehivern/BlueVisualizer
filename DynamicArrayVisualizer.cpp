#include "DynamicArrayVisualizer.h"
#include "Random.h"

DynamicArrayVisualizer::DynamicArrayVisualizer(sf::RenderWindow* window, Assets* assets) {
	this->window = window;
	this->assets = assets;

	header = HeaderBox(assets, "Dynamic Array", sf::Vector2f(10, 10));
	action = ActionBox(window, assets, sf::Vector2f(10, 430));
	option = OptionBox(assets, sf::Vector2f(10, 510));
	code = CodeBox(assets, sf::Vector2f(910, 590));
	description = DescriptionBox(assets, sf::Vector2f(910, 430));
}

void DynamicArrayVisualizer::emptyArray() {
	cells.clear();
}

void DynamicArrayVisualizer::zeroArray(int size) {
	cells.clear();
	for (int i = 0; i < size; i++) {
		cells.pushBack(Cell(0, sf::Vector2f()));
	}
}

void DynamicArrayVisualizer::randomArray(int size) {
	cells.clear();
	for (int i = 0; i < size; i++) {
		cells.pushBack(Cell(randInt(0, 99), sf::Vector2f()));
	}
}

void DynamicArrayVisualizer::manualArray(std::string listOfValues) {
	cells.clear();
	for (int i = 0; i < listOfValues.size(); i++) {
		if ('0' > listOfValues[i] || listOfValues[i] > '9') {
			continue;
		}

		int j;
		for (j = i + 1; j < listOfValues.size(); j++) {
			if ('0' > listOfValues[j] || listOfValues[j] > '9') {
				break;
			}
		}
		j--;

		cells.pushBack(Cell(std::stoi(listOfValues.substr(i, j - i + 1)), sf::Vector2f()));

		i = j;
	}
}

void DynamicArrayVisualizer::create() {
	action.clearAllSteps();

	labels.clear();
	if (!cells.empty()) {
		cells.front().position = sf::Vector2f(CELL_POSITION_X, CELL_POSITION_Y);
		labels.pushBack(Label(nullptr, &cells.front()));
		for (ListNode <Cell>* iterator = cells.begin()->next(); iterator != cells.end(); iterator = iterator->next()) {
			iterator->data.position.x = iterator->prev()->data.position.x + 60 + CELL_DISTANCE_X;
			iterator->data.position.y = iterator ->prev()->data.position.y;
			labels.pushBack(Label(nullptr, &iterator->data));
		}
	}

	code.update({
		"int n = " + std::to_string(cells.size()) + "; int* a = new int[n];"
		});

	description.newOperation("Create an array of size " + std::to_string(cells.size()));

	maxPosition1 = cells.size() - 1;
	maxPosition2 = cells.size() - 2;

	// New step: Create all cells
	action.addNewStep();

	// Description
	description.addDescription({ "Create " + std::to_string(cells.size()) + " consecutive cells." });
	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.drawFadeIn(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.drawFadeIn(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "An array of size " + std::to_string(cells.size()) + " is created." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawChange(&cells, 0, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->normalCellSquareColor, &assets->highlightCellTextColor1, &assets->normalCellTextColor);
	action.drawFadeOut(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 0);
}

void DynamicArrayVisualizer::access(int index) {
	action.clearAllSteps();

	code.update({
		"return a[i];"
		});

	description.newOperation("Access value at index " + std::to_string(index));

	// New step: return a[i]
	action.addNewStep();

	// Description
	description.addDescription({ "Return the value at index " + std::to_string(index) + ", which is " + std::to_string(cells.begin()->next(index)->data.value) + "." });
	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.draw(&cells, 0, index - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawChange(&cells, index, SHOLLOW, &assets->normalCellSquareColor, &assets->highlightCellSquareColor1, &assets->normalCellTextColor, &assets->highlightCellTextColor1);
	action.drawFadeIn(&cells, index, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
	action.draw(&cells, index + 1, cells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "So the value at index " + std::to_string(index) + " is " + std::to_string(cells.begin()->next(index)->data.value) + ".", "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.draw(&cells, 0, index - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawChange(&cells, index, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor1, &assets->normalCellTextColor);
	action.drawFadeOut(&cells, index, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
	action.draw(&cells, index + 1, cells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 0);
}

void DynamicArrayVisualizer::search(int value) {
	action.clearAllSteps();

	code.update({
		"for (int k = 0; k <= n - 1; k++)",
		"    if (a[k] == v) return k;    ",
		"return NOT_FOUND;               "
		});

	description.newOperation("Search " + std::to_string(value));

	for (int i = 0; i <= cells.size(); i++) {
		// New step: for (int k = 0; k <= n - 1; k++)
		action.addNewStep();

		// Description
		if (cells.size() == 0) {
			description.addDescription({ "Iterate k from 0 to n - 1 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to n - 1 = " + std::to_string(cells.size() - 1) + ", k is now 0.", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
			} else if (i <= cells.size() - 1) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(cells.size() - 1) + ", the loop stops." });
			}
		}

		if (i == 0) {
			action.drawFadeIn(&description, description.size() - 1);
		} else {
			action.drawChange(&description, description.size() - 2, description.size() - 1);
		}

		// Cell
		action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		if (i > 0) {
			action.drawFadeOut(&cells, i - 1, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);	
		}

		action.draw(&cells, i, cells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);

		// Label
		action.draw(&labels, &assets->labelColor);

		// Code
		if (i == 0) {
			action.drawFadeIn(&code, 0);
		} else {
			action.drawMove(&code, 1, 0);
		}

		if (i == cells.size()) {
			break;
		}

		// New step: if (a[k] == v) return k
		action.addNewStep();

		// Description
		if (cells.begin()->next(i)->data.value == value) {
			description.addDescription({ "Check if value at index " + std::to_string(i) + " equals to " + std::to_string(value) + ".", "Since it is true, k is returned." });
		} else {
			description.addDescription({ "Check if value at index " + std::to_string(i) + " equals to " + std::to_string(value) + ".", "Since it is false, the statement is ignored." });
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.drawChange(&cells, i, SHOLLOW, &assets->normalCellSquareColor, &assets->highlightCellSquareColor1, &assets->normalCellTextColor, &assets->highlightCellTextColor1);
		action.drawFadeIn(&cells, i, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
		action.draw(&cells, i + 1, cells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);

		// Label
		action.draw(&labels, &assets->labelColor);

		// Code
		action.drawMove(&code, 0, 1);

		if (cells.begin()->next(i)->data.value == value) {
			// New step: Re-layout
			action.addNewStep();

			// Description
			description.addDescription({ "So the value " + std::to_string(value) + " was found at index " + std::to_string(i) + ".", "The whole process is O(n)." });
			action.drawChange(&description, description.size() - 2, description.size() - 1);

			// Cell
			action.drawChange(&cells, 0, i, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor2, &assets->normalCellTextColor);
			action.drawFadeOut(&cells, i, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
			action.draw(&cells, i + 1, cells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);

			// Label
			action.draw(&labels, &assets->labelColor);

			// Code
			action.drawFadeOut(&code, 1);

			return;
		}
	}

	// New step: return NOT_FOUND
	action.addNewStep();

	// Description
	description.addDescription({ "NOT_FOUND is returned." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);;

	// Cell
	action.draw(&cells, 0, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 0, 2);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "So the value " + std::to_string(value) + " was not found.", "The whole process is O(n)."});
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawChange(&cells, 0, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor2, &assets->normalCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 2);
}

void DynamicArrayVisualizer::update(int index, int value) {
	action.clearAllSteps();

	code.update({
		"a[i] = v;"
		});

	description.newOperation("Update value at index " + std::to_string(index) + " to " + std::to_string(value));

	// New step: return a[i]
	action.addNewStep();

	// Description
	description.addDescription({ "Set the value at index " + std::to_string(index) + " to " + std::to_string(value) + "." });
	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.draw(&cells, 0, index - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawChangeAndUpdate(&cells, index, SHOLLOW, &assets->normalCellSquareColor, &assets->highlightCellSquareColor1, &assets->normalCellTextColor, &assets->highlightCellTextColor1, cells.begin()->next(index)->data.value, value);
	action.drawFadeInAndUpdate(&cells, index, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1, cells.begin()->next(index)->data.value, value);
	action.draw(&cells, index + 1, cells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.draw(&cells, 0, index - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawChange(&cells, index, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor1, &assets->normalCellTextColor);
	action.drawFadeOut(&cells, index, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
	action.draw(&cells, index + 1, cells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 0);
}

void DynamicArrayVisualizer::insertAtTheFront(int value) {
	action.clearAllSteps();

	randomCells.clear();
	for (int i = 0; i < cells.size(); i++) {
		randomCells.pushBack(cells.begin()->next(i)->data);
	}

	randomLabels.clear();
	for (int i = 0; i < randomCells.size(); i++) {
		randomLabels.pushBack(Label(nullptr, &randomCells.begin()->next(i)->data));
	}

	cells.pushBack(Cell(0, sf::Vector2f(CELL_POSITION_X + (60 + CELL_DISTANCE_X) * cells.size(), CELL_POSITION_Y)));
	labels.pushBack(Label(nullptr, &cells.back()));
	for (int i = 0; i < cells.size(); i++) {
		cells.begin()->next(i)->data.value = 0;
		cells.begin()->next(i)->data.position += sf::Vector2f(0, 60 + CELL_DISTANCE_Y);
	}

	maxPosition1 = cells.size() - 1;
	maxPosition2 = cells.size() - 2;

	code.update({
		"int* b = new int[++n];          ",
		"for (int k = 1; k <= n - 1; k++)",
		"    b[k] = a[k - 1];            ",
		"b[0] = v;                       ",
		"delete a; a = b;                "
		});

	description.newOperation("Insert " + std::to_string(value) + " at the front");

	// New step: int* b = new int[++n];
	action.addNewStep();

	// Description
	description.addDescription({ "Increase the size of the array by 1 and create", "a new array of size is the new size." });
	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawFadeIn(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.draw(&randomLabels, &assets->labelColor);
	action.drawFadeIn(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 1; i <= cells.size(); i++) {
		// New step: for (int k = 1; k <= n - 1; k++)
		action.addNewStep();

		// Description
		if (cells.size() == 1) {
			description.addDescription({ "Iterate k from 1 to n - 1 = 0, k is now 1.", "Since k > 0, the loop stops." });
		} else {
			if (i == 1) {
				description.addDescription({ "Iterate k from 1 to n - 1 = " + std::to_string(cells.size() - 1) + ", k is now 1.", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
			} else if (i <= cells.size() - 1) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(cells.size() - 1) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		if (i == 1) {
			action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells, 0, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
			action.drawFadeOut(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
		} else {
			action.draw(&randomCells, 0, i - 2, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
			action.drawFadeOut(&randomCells, i - 2, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
			action.draw(&randomCells, i - 1, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells.front(), SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
			action.draw(&cells, 1, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
			action.drawFadeOut(&cells, i - 1, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
			action.draw(&cells, i, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
		}

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		if (i == 1) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		if (i == cells.size()) {
			break;
		}

		// New step: b[i] = a[i - 1]
		action.addNewStep();

		// Description
		description.addDescription({ "Set the value at index " + std::to_string(i) + " in array 'b' to the", "value at index " + std::to_string(i - 1) + " in array 'a'." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);
		
		// Cell
		action.draw(&randomCells, 0, i - 2, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
		action.drawChange(&randomCells, i - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->erasedCellSquareColor, &assets->normalCellTextColor, &assets->highlightCellTextColor1);
		action.drawFadeIn(&randomCells, i - 1, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
		action.draw(&randomCells, i, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
		action.draw(&cells.front(), SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
		action.draw(&cells, 1, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.drawChangeAndUpdate(&cells, i, SHOLLOW, &assets->insertedCellSquareColor, &assets->highlightCellSquareColor1, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i - 1)->data.value);
		action.drawFadeInAndUpdate(&cells, i, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i - 1)->data.value);
		action.draw(&cells, i + 1, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		action.drawMove(&code, 1, 2);
 	}

	// New step: b[0] = v
	action.addNewStep();
	 
	// Description
	description.addDescription({ "Set the value at index 0 in array 'b' to " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
	action.drawChangeAndUpdate(&cells.front(), SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellSquareColor, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.front().value, value);
	action.drawFadeInAndUpdate(&cells.front(), SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1, cells.front().value, value);
	action.draw(&cells, 1, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);

	// Label
	action.draw(&randomLabels, &assets->labelColor);
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 1, 3);

	// New step: delete a; a = b;
	action.addNewStep();

	// Description
	description.addDescription({ "Erase array 'a' and set it to array 'b'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawFadeOut(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
	action.draw(&cells.front(), SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
	action.drawFadeOut(&cells.front(), SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
	action.draw(&cells, 1, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);

	// Label
	action.drawFadeOut(&randomLabels, &assets->labelColor);
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 3, 4);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "Re-layout the array for visualization.", "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawChange(&cells.front(), SHOLLOW, &assets->insertedCellSquareColor, &assets->normalCellSquareColor, &assets->insertedCellTextColor, &assets->normalCellTextColor);
	action.drawChange(&cells, 1, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor2, &assets->normalCellTextColor);
	for (int i = 0; i < cells.size(); i++) {
		action.drawMove(&cells, i, SHOLLOW, &assets->blankColor, &assets->blankColor, cells.begin()->next(i)->data.position, cells.begin()->next(i)->data.position + sf::Vector2f(0, -60 - CELL_DISTANCE_Y));
	}

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 4);
}

void DynamicArrayVisualizer::insertAtTheBack(int value) {
	action.clearAllSteps();

	randomCells.clear();
	for (int i = 0; i < cells.size(); i++) {
		randomCells.pushBack(cells.begin()->next(i)->data);
	}

	randomLabels.clear();
	for (int i = 0; i < randomCells.size(); i++) {
		randomLabels.pushBack(Label(nullptr, &randomCells.begin()->next(i)->data));
	}

	cells.pushBack(Cell(0, sf::Vector2f(CELL_POSITION_X + (60 + CELL_DISTANCE_X) * cells.size(), CELL_POSITION_Y)));
	labels.pushBack(Label(nullptr, &cells.back()));
	for (int i = 0; i < cells.size(); i++) {
		cells.begin()->next(i)->data.value = 0;
		cells.begin()->next(i)->data.position += sf::Vector2f(0, 60 + CELL_DISTANCE_Y);
	}

	maxPosition1 = cells.size() - 1;
	maxPosition2 = cells.size() - 2;

	code.update({
		"int* b = new int[++n];          ",
		"for (int k = 0; k <= n - 2; k++)",
		"    b[k] = a[k];                ",
		"b[n - 1] = v;                   ",
		"delete a; a = b;                "
		});

	description.newOperation("Insert " + std::to_string(value) + " at the back");

	// New step: int* b = new int[++n];
	action.addNewStep();

	// Description
	description.addDescription({ "Increase the size of the array by 1 and create", "a new array of size is the new size." });
	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawFadeIn(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.draw(&randomLabels, &assets->labelColor);
	action.drawFadeIn(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i <= cells.size() - 1; i++) {
		// New step: for (int k = 0; k <= n - 2; k++)
		action.addNewStep();

		// Description
		if (cells.size() == 1) {
			description.addDescription({ "Iterate k from 0 to n - 2 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to n - 2 = " + std::to_string(cells.size() - 2) + ", k is now 1.", "Since k <= " + std::to_string(cells.size() - 2) + ", the loop continues." });
			} else if (i <= cells.size() - 2) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(cells.size() - 2) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(cells.size() - 2) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		if (i == 0) {
			action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells, 0, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
			action.drawFadeOut(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
		} else {
			action.draw(&randomCells, 0, i - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
			action.drawFadeOut(&randomCells, i - 1, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
			action.draw(&randomCells, i, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
			action.drawFadeOut(&cells, i - 1, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
			action.draw(&cells, i, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
		}

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		if (i == cells.size() - 1) {
			break;
		}

		// New step: b[i] = a[i]
		action.addNewStep();

		// Description
		description.addDescription({ "Set the value at index " + std::to_string(i) + " in array 'b' to the", "value at index " + std::to_string(i) + " in array 'a'." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		action.draw(&randomCells, 0, i - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
		action.drawChange(&randomCells, i, SHOLLOW, &assets->normalCellSquareColor, &assets->erasedCellSquareColor, &assets->normalCellTextColor, &assets->highlightCellTextColor1);
		action.drawFadeIn(&randomCells, i, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
		action.draw(&randomCells, i + 1, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
		action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.drawChangeAndUpdate(&cells, i, SHOLLOW, &assets->insertedCellSquareColor, &assets->highlightCellSquareColor1, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i)->data.value);
		action.drawFadeInAndUpdate(&cells, i, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i)->data.value);
		action.draw(&cells, i + 1, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: b[0] = v
	action.addNewStep();

	// Description
	description.addDescription({ "Set the value at index " + std::to_string(cells.size() - 1) + " in array 'b' to " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
	action.draw(&cells, 0, cells.size() - 2, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
	action.drawChangeAndUpdate(&cells.back(), SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellSquareColor, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.back().value, value);
	action.drawFadeInAndUpdate(&cells.back(), SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1, cells.back().value, value);

	// Label
	action.draw(&randomLabels, &assets->labelColor);
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 1, 3);

	// New step: delete a; a = b;
	action.addNewStep();

	// Description
	description.addDescription({ "Erase array 'a' and set it to array 'b'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawFadeOut(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
	action.draw(&cells, 0, cells.size() - 2, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
	action.draw(&cells.back(), SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
	action.drawFadeOut(&cells.back(), SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.drawFadeOut(&randomLabels, &assets->labelColor);
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 3, 4);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "Re-layout the array for visualization.", "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);
	for (int i = 0; i < cells.size(); i++) {
		action.drawMove(&cells, i, SHOLLOW, &assets->blankColor, &assets->blankColor, cells.begin()->next(i)->data.position, cells.begin()->next(i)->data.position + sf::Vector2f(0, -60 - CELL_DISTANCE_Y));
	}

	// Cell
	action.drawChange(&cells, 0, cells.size() - 2, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor2, &assets->normalCellTextColor);
	action.drawChange(&cells.back(), SHOLLOW, &assets->insertedCellSquareColor, &assets->normalCellSquareColor, &assets->insertedCellTextColor, &assets->normalCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 4);
}

void DynamicArrayVisualizer::insertAtTheMiddle(int index, int value) {
	action.clearAllSteps();

	randomCells.clear();
	for (int i = 0; i < cells.size(); i++) {
		randomCells.pushBack(cells.begin()->next(i)->data);
	}

	randomLabels.clear();
	for (int i = 0; i < randomCells.size(); i++) {
		randomLabels.pushBack(Label(nullptr, &randomCells.begin()->next(i)->data));
	}

	cells.pushBack(Cell(0, sf::Vector2f(CELL_POSITION_X + (60 + CELL_DISTANCE_X) * cells.size(), CELL_POSITION_Y)));
	labels.pushBack(Label(nullptr, &cells.back()));
	for (int i = 0; i < cells.size(); i++) {
		cells.begin()->next(i)->data.value = 0;
		cells.begin()->next(i)->data.position += sf::Vector2f(0, 60 + CELL_DISTANCE_Y);
	}

	maxPosition1 = cells.size() - 1;
	maxPosition2 = cells.size() - 2;

	code.update({
		"int* b = new int[++n];              ",
		"for (int k = 0; k <= i - 1; k++)    ",
		"    b[k] = a[k];                    ",
		"for (int k = i + 1; k <= n - 1; k++)",
		"    b[k] = a[k - 1];                ",
		"b[i] = v;                           ",
		"delete a; a = b;                    "
		});

	description.newOperation("Insert " + std::to_string(value) + " at the back");

	// New step: int* b = new int[++n];
	action.addNewStep();

	// Description
	description.addDescription({ "Increase the size of the array by 1 and create", "a new array of size is the new size." });
	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawFadeIn(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.draw(&randomLabels, &assets->labelColor);
	action.drawFadeIn(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i <= index; i++) {
		// New step: for (int k = 0; k <= i - 1; k++)
		action.addNewStep();

		// Description
		if (i == 0) {
			description.addDescription({ "Iterate k from 0 to i - 1 = " + std::to_string(index - 1) + ", k is now 1.", "Since k <= " + std::to_string(index - 1) + ", the loop continues." });
		} else if (i <= index - 1) {
			description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(index - 1) + ", the loop continues." });
		} else {
			description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(index - 1) + ", the loop stops." });
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		if (i == 0) {
			action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells, 0, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
			action.drawFadeOut(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
		} else {
			action.draw(&randomCells, 0, i - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
			action.drawFadeOut(&randomCells, i - 1, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
			action.draw(&randomCells, i, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
			action.drawFadeOut(&cells, i - 1, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
			action.draw(&cells, i, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
		}

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		if (i == index) {
			break;
		}

		// New step: b[i] = a[i]
		action.addNewStep();

		// Description
		description.addDescription({ "Set the value at index " + std::to_string(i) + " in array 'b' to the", "value at index " + std::to_string(i) + " in array 'a'." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		action.draw(&randomCells, 0, i - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
		action.drawChange(&randomCells, i, SHOLLOW, &assets->normalCellSquareColor, &assets->erasedCellSquareColor, &assets->normalCellTextColor, &assets->highlightCellTextColor1);
		action.drawFadeIn(&randomCells, i, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
		action.draw(&randomCells, i + 1, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
		action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.drawChangeAndUpdate(&cells, i, SHOLLOW, &assets->insertedCellSquareColor, &assets->highlightCellSquareColor1, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i)->data.value);
		action.drawFadeInAndUpdate(&cells, i, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i)->data.value);
		action.draw(&cells, i + 1, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		action.drawMove(&code, 1, 2);
	}

	for (int i = index + 1; i <= cells.size(); i++) {
		// New step: for (int k = 1; k <= n - 1; k++)
		action.addNewStep();

		// Description
		if (i == index + 1) {
			description.addDescription({ "Iterate k from " + std::to_string(index + 1) + " to n - 1 = " + std::to_string(cells.size() - 1) + ", k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
		} else if (i <= cells.size() - 1) {
			description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
		} else {
			description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(cells.size() - 1) + ", the loop stops." });
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		action.draw(&randomCells, 0, i - 2, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
		if (i > index + 1) {
			action.drawFadeOut(&randomCells, i - 2, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
		}

		action.draw(&randomCells, i - 1, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
		action.draw(&cells, 0, index - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.draw(&cells, index, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
		action.draw(&cells, index + 1, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		if (i > index + 1) {
			action.drawFadeOut(&cells, i - 1, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
		}
		
		action.draw(&cells, i, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		if (i == index + 1) {
			action.drawMove(&code, 1, 3);
		} else {
			action.drawMove(&code, 4, 3);
		}

		if (i == cells.size()) {
			break;
		}

		// New step: b[i] = a[i - 1]
		action.addNewStep();

		// Description
		description.addDescription({ "Set the value at index " + std::to_string(i) + " in array 'b' to the", "value at index " + std::to_string(i - 1) + " in array 'a'." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		action.draw(&randomCells, 0, i - 2, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
		action.drawChange(&randomCells, i - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->erasedCellSquareColor, &assets->normalCellTextColor, &assets->highlightCellTextColor1);
		action.drawFadeIn(&randomCells, i - 1, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
		action.draw(&randomCells, i, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
		action.draw(&cells, 0, index - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.draw(&cells, index, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
		action.draw(&cells, index + 1, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.drawChangeAndUpdate(&cells, i, SHOLLOW, &assets->insertedCellSquareColor, &assets->highlightCellSquareColor1, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i - 1)->data.value);
		action.drawFadeInAndUpdate(&cells, i, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i - 1)->data.value);
		action.draw(&cells, i + 1, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		action.drawMove(&code, 3, 4);
	}

	// New step: b[i] = v
	action.addNewStep();

	// Description
	description.addDescription({ "Set the value at index " + std::to_string(index) + " in array 'b' to " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
	action.draw(&cells, 0, index - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
	action.drawChangeAndUpdate(&cells, index, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellSquareColor, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.begin()->next(index)->data.value, value);
	action.drawFadeInAndUpdate(&cells, index, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1, cells.begin()->next(index)->data.value, value);
	action.draw(&cells, index + 1, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);

	// Label
	action.draw(&randomLabels, &assets->labelColor);
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 3, 5);

	// New step: delete a; a = b;
	action.addNewStep();

	// Description
	description.addDescription({ "Erase array 'a' and set it to array 'b'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawFadeOut(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
	action.draw(&cells, 0, index - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
	action.draw(&cells, index, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
	action.drawFadeOut(&cells, index, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
	action.draw(&cells, index + 1, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);

	// Label
	action.drawFadeOut(&randomLabels, &assets->labelColor);
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 5, 6);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "Re-layout the array for visualization.", "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawChange(&cells, 0, index - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor2, &assets->normalCellTextColor);
	action.drawChange(&cells, index, SHOLLOW, &assets->insertedCellSquareColor, &assets->normalCellSquareColor, &assets->insertedCellTextColor, &assets->normalCellTextColor);
	action.drawChange(&cells, index + 1, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor2, &assets->normalCellTextColor);
	for (int i = 0; i < cells.size(); i++) {
		action.drawMove(&cells, i, SHOLLOW, &assets->blankColor, &assets->blankColor, cells.begin()->next(i)->data.position, cells.begin()->next(i)->data.position + sf::Vector2f(0, -60 - CELL_DISTANCE_Y));
	}

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 6);
}

void DynamicArrayVisualizer::eraseAtTheFront() {
	action.clearAllSteps();

	randomCells.clear();
	for (int i = 0; i < cells.size(); i++) {
		randomCells.pushBack(cells.begin()->next(i)->data);
	}

	randomLabels.clear();
	for (int i = 0; i < randomCells.size(); i++) {
		randomLabels.pushBack(Label(nullptr, &randomCells.begin()->next(i)->data));
	}

	cells.popBack();
	labels.popBack();
	for (int i = 0; i < cells.size(); i++) {
		cells.begin()->next(i)->data.value = 0;
		cells.begin()->next(i)->data.position += sf::Vector2f(0, 60 + CELL_DISTANCE_Y);
	}

	maxPosition1 = cells.size() - 1;
	maxPosition2 = cells.size() - 2;

	code.update({
		"int* b = new int[--n];          ",
		"for (int k = 0; k <= n - 1; k++)",
		"    b[k] = a[k + 1];            ",
		"delete a; a = b;                "
		});

	description.newOperation("Erase value at the front");

	// New step: int* b = new int[++n];
	action.addNewStep();

	// Description
	description.addDescription({ "Decrease the size of the array by 1 and create", "a new array of size is the new size." });
	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.draw(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.drawFadeIn(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.draw(&randomLabels, &assets->labelColor);
	action.drawFadeIn(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i <= cells.size(); i++) {
		// New step: for (int k = 0; k <= n - 1; k++)
		action.addNewStep();

		// Description
		if (cells.size() == 0) {
			description.addDescription({ "Iterate k from 0 to n - 1 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to n - 1 = " + std::to_string(cells.size() - 1) + ", k is now 1.", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
			} else if (i <= cells.size() - 1) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(cells.size() - 1) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(cells.size() - 1) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		if (i == 0) {
			action.drawChange(&randomCells.front(), SHOLLOW, &assets->normalCellSquareColor, &assets->erasedCellSquareColor, &assets->normalCellTextColor, &assets->erasedCellTextColor);
			action.draw(&randomCells, 1, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells, 0, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
			action.drawFadeOut(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
		} else {
			action.draw(&randomCells, 0, i, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
			action.drawFadeOut(&randomCells, i, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
			action.draw(&randomCells, i + 1, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
			action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
			action.drawFadeOut(&cells, i - 1, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1);
			action.draw(&cells, i, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);
		}

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		if (i == cells.size()) {
			break;
		}

		// New step: b[i] = a[i + 1]
		action.addNewStep();

		// Description
		description.addDescription({ "Set the value at index " + std::to_string(i) + " in array 'b' to the", "value at index " + std::to_string(i - 1) + " in array 'a'." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Cell
		action.draw(&randomCells, 0, i, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
		action.drawChange(&randomCells, i + 1, SHOLLOW, &assets->normalCellSquareColor, &assets->erasedCellSquareColor, &assets->normalCellTextColor, &assets->highlightCellTextColor1);
		action.drawFadeIn(&randomCells, i + 1, SSOLID, &assets->erasedCellSquareColor, &assets->highlightCellTextColor1);
		action.draw(&randomCells, i + 2, randomCells.size() - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
		action.draw(&cells, 0, i - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);
		action.drawChangeAndUpdate(&cells, i, SHOLLOW, &assets->insertedCellSquareColor, &assets->highlightCellSquareColor1, &assets->insertedCellTextColor, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i + 1)->data.value);
		action.drawFadeInAndUpdate(&cells, i, SSOLID, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor1, cells.begin()->next(i)->data.value, randomCells.begin()->next(i + 1)->data.value);
		action.draw(&cells, i + 1, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->insertedCellTextColor);

		// Label
		action.draw(&randomLabels, &assets->labelColor);
		action.draw(&labels, &assets->labelColor);

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: delete a; a = b;
	action.addNewStep();

	// Description
	description.addDescription({ "Erase array 'a' and set it to array 'b'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawFadeOut(&randomCells, 0, randomCells.size() - 1, SHOLLOW, &assets->erasedCellSquareColor, &assets->erasedCellTextColor);
	action.draw(&cells, 0, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->highlightCellTextColor2);

	// Label
	action.drawFadeOut(&randomLabels, &assets->labelColor);
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawMove(&code, 1, 3);

	// New step: Re-layout
	action.addNewStep();

	// Description
	if (cells.empty()) {
		description.addDescription({ "The whole process is O(n)." });
	} else {
		description.addDescription({ "Re-layout the array for visualization.", "The whole process is O(n)." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawChange(&cells, 0, cells.size() - 1, SHOLLOW, &assets->highlightCellSquareColor1, &assets->normalCellSquareColor, &assets->highlightCellTextColor2, &assets->normalCellTextColor);
	for (int i = 0; i < cells.size(); i++) {
		action.drawMove(&cells, i, SHOLLOW, &assets->blankColor, &assets->blankColor, cells.begin()->next(i)->data.position, cells.begin()->next(i)->data.position + sf::Vector2f(0, -60 - CELL_DISTANCE_Y));
	}

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 3);
}

void DynamicArrayVisualizer::run() {
	std::vector <char> numbersCharacter;
	for (int i = 0; i <= 9; i++) {
		numbersCharacter.push_back(i + '0');
	}

	std::vector <char> numbersCommaSpaceCharacter = numbersCharacter;
	numbersCommaSpaceCharacter.push_back(',');
	numbersCommaSpaceCharacter.push_back(' ');

	std::function <std::string(std::string, std::string)> validatorNone = [](std::string name, std::string value) {
		return "";
	};

	std::function <std::string(std::string, std::string, int*, int*)> validatorInteger = [](std::string name, std::string value, int* lowerBound, int* upperBound) -> std::string {
		while (value.size() > 1 && value.front() == '0') {
			value.erase(value.begin());
		}

		if (value.empty() || 
			std::to_string(*lowerBound).size() > value.size() || value.size() > std::to_string(*upperBound).size() || 
			*lowerBound > std::stoi(value) || std::stoi(value) > *upperBound) {
			return name + " must be in [" + std::to_string(*lowerBound) + ", " + std::to_string(*upperBound) + "]";
		}

		return "";
	};

	std::function <std::string(std::string, std::string, int*, int*, int*, int*)> validatorListOfIntegers = [](std::string name, std::string listOfValues, int* minSize, int* maxSize, int* lowerBound, int* upperBound) -> std::string {
		std::string tmp;
		for (auto character : listOfValues) {
			if (character != ' ') {
				tmp.push_back(character);
			}
		}
		listOfValues = tmp;

		if (!listOfValues.empty() && (listOfValues.front() == ',' || listOfValues.back() == ',')) {
			return "Invalid format";
		}

		for (int i = 0; i < (int)listOfValues.size() - 1; i++) {
			if (listOfValues[i] == ',' && listOfValues[i + 1] == ',') {
				return "Invalid format";
			}
		}

		int size = 0;
		for (int i = 0; i < (int)listOfValues.size(); i++) {
			if ('0' > listOfValues[i] || listOfValues[i] > '9') {
				continue;
			}

			int j;
			for (j = i + 1; j < (int)listOfValues.size(); j++) {
				if ('0' > listOfValues[j] || listOfValues[j] > '9') {
					break;
				}
			}
			j--;

			size++;
			if (size > *maxSize) {
				return "Size of " + name + " must be at most " + std::to_string(*maxSize);
			}

			std::string value = listOfValues.substr(i, j - i + 1);
			while (value.size() > 1 && value.front() == '0') {
				value.erase(value.begin());
			}

			if (value.empty() || 
				std::to_string(*lowerBound).size() > value.size() || value.size() > std::to_string(*upperBound).size() || 
				*lowerBound > std::stoi(value) || std::stoi(value) > *upperBound) {
				return name + " must be in [" + std::to_string(*lowerBound) + ", " + std::to_string(*upperBound) + "]";
			}

			i = j;
		}

		if (size < *minSize) {
			return "Size of " + name + " must be at least " + std::to_string(*minSize);
		}

		return "";
	};

	std::function <std::string()> generatorNone = []() -> std::string {
		return "a";
	};

	std::function <std::string(int*, int*)> generatorInteger = [](int* lowerBound, int* upperBound) -> std::string {
		if (*lowerBound > *upperBound) {
			return "";
		}

		return std::to_string(randInt(*lowerBound, *upperBound));
	};

	std::function <std::string(int*, int*, int*, int*)> generatorListOfIntegers = [](int* minSize, int* maxSize, int* lowerBound, int* upperBound) -> std::string {
		if (*lowerBound > *upperBound) {
			return "";
		}

		int size = randInt(*minSize, *maxSize);

		std::string listOfValues;
		for (int i = 0; i < size; i++) {
			listOfValues += std::to_string(randInt(*lowerBound, *upperBound));
			if (i < size - 1) {
				listOfValues += ", ";
			}
		}

		return listOfValues;
	};

	int minSize = 1, maxSize = 15;
	int minValue = 0, maxValue = 99;
	int minPosition1 = 0, minPosition2 = 1;

	std::function <bool()> conditionNone = []() -> bool {
		return true;
	};

	std::function <bool()> conditionArrayNotEmpty= [&]() -> bool {
		return cells.size() > 0;
	};

	std::function <bool()> conditionArrayNotLarge = [&]() -> bool {
		return cells.size() < maxSize;
	};

	option.addOption("Create");
	option.addSuboption("Empty", conditionNone);
	option.addSuboption("Zero", conditionNone);
	option.addSuboptionInput("n", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize), std::bind(generatorInteger, &minSize, &maxSize));
	option.addSuboption("Random", conditionNone);
	option.addSuboptionInput("n", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize), std::bind(generatorInteger, &minSize, &maxSize));
	option.addSuboption("Manual", conditionNone);
	option.addSuboptionInput("v[]", numbersCommaSpaceCharacter, std::bind(validatorListOfIntegers, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize, &minValue, &maxValue), std::bind(generatorListOfIntegers, &minSize, &maxSize, &minValue, &maxValue));
	option.addSuboptionInput("File browser", numbersCommaSpaceCharacter, validatorNone, generatorNone);

	option.addOption("Access");
	option.addSuboption("", conditionArrayNotEmpty);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition1, &maxPosition1), std::bind(generatorInteger, &minPosition1, &maxPosition1));

	option.addOption("Search");
	option.addSuboption("", conditionNone);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Update");
	option.addSuboption("", conditionArrayNotEmpty);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition1, &maxPosition1), std::bind(generatorInteger, &minPosition1, &maxPosition1));
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Insert");
	option.addSuboption("Front (i = 0)", conditionArrayNotLarge);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));
	option.addSuboption("Back (i = n)", conditionArrayNotLarge);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));
	option.addSuboption("Middle (i = 1..n - 1)", conditionArrayNotLarge);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition2, &maxPosition1), std::bind(generatorInteger, &minPosition2, &maxPosition1));
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Erase");
	option.addSuboption("Front (i = 0)", conditionArrayNotEmpty);
	option.addSuboption("Back (i = n - 1)", conditionArrayNotEmpty);
	option.addSuboption("Middle (i = 1..n - 2)", conditionArrayNotEmpty);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition2, &maxPosition2), std::bind(generatorInteger, &minPosition2, &maxPosition2));

	randomArray(10);
	create();

	while (window->isOpen()) {
		window->clear(sf::Color::White);

		window->draw(assets->backgroundSprite);
		header.draw(window);
		action.draw();
		option.draw(window);

		window->display();

		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close();
				return;
			}

			if (header.handleEvent(window, &event)) {
				return;
			}

			action.handleEvent(&event, &option);

			auto current = option.handleEvent(window, &event);
			if (std::get <0> (current) != -1) {
				action.abortAllSteps();
			}

			switch (std::get <0> (current)) {
			case 0: // Create
				switch (std::get <1> (current)) {
				case 0: // Empty
					emptyArray();
					break;
				
				case 1: // Zero
					zeroArray(std::stoi(std::get <2> (current)[0]));
					break;

				case 2: // Random
					randomArray(std::stoi(std::get <2> (current)[0]));
					break;

				case 3: // Manual
					manualArray(std::get <2> (current)[0]);
				}

				create();
				break;

			case 1: // Access
				switch (std::get <1> (current)) {
				case 0: //
					access(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 2: // Search
				switch (std::get <1> (current)) {
				case 0: //
					search(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 3: // Update
				switch (std::get <1> (current)) {
				case 0: //
					update(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 4: // Insert
				switch (std::get <1> (current)) {
				case 0: // Front
					insertAtTheFront(std::stoi(std::get <2> (current)[0]));
					break;

				case 1: // Back
					insertAtTheBack(std::stoi(std::get <2> (current)[0]));
					break;

				case 2: // Middle
					insertAtTheMiddle(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 5: // Erase
				switch (std::get <1> (current)) {
				case 0: // Front
					eraseAtTheFront();
					break;

				case 1: // Back
					//eraseAtTheBack();
					break;

				case 2: // Middle
					//eraseAtTheMiddle(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;
			}
		}
	}
}