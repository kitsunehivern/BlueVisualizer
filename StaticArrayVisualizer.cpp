#include "StaticArrayVisualizer.h"
#include "Random.h"

StaticArrayVisualizer::StaticArrayVisualizer(sf::RenderWindow* window, Assets* assets) {
	this->window = window;
	this->assets = assets;

	header = HeaderBox(assets, "Static Array", sf::Vector2f(10, 10));
	action = ActionBox(window, assets, sf::Vector2f(10, 430));
	option = OptionBox(assets, sf::Vector2f(10, 510));
	code = CodeBox(assets, sf::Vector2f(910, 590));
	description = DescriptionBox(assets, sf::Vector2f(910, 430));
}

void StaticArrayVisualizer::zeroArray(int capacity, int size) {
	this->size = size;

	cells.clear();
	for (int i = 0; i < capacity; i++) {
		cells.pushBack(Cell(0, sf::Vector2f()));
	}
}

void StaticArrayVisualizer::randomArray(int capacity, int size) {
	this->size = size;

	cells.clear();
	for (int i = 0; i < size; i++) {
		cells.pushBack(Cell(randInt(0, 99), sf::Vector2f()));
	}

	for (int i = size; i < capacity; i++) {
		cells.pushBack(Cell(0, sf::Vector2f()));
	}
}

void StaticArrayVisualizer::manualArray(std::string listOfValues) {
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

	this->size = cells.size();
}

void StaticArrayVisualizer::create() {
	action.clearAllSteps();

	labels.clear();
	cells.front().position = sf::Vector2f(CELL_POSITION_X, CELL_POSITION_Y);
	labels.pushBack(Label(nullptr, &cells.front()));
	for (ListNode <Cell>* iterator = cells.begin()->next(); iterator != cells.end(); iterator = iterator->next()) {
		iterator->data.position.x = iterator->prev()->data.position.x + 60 + CELL_DISTANCE;
		iterator->data.position.y = iterator ->prev()->data.position.y;
		labels.pushBack(Label(nullptr, &iterator->data));
	}

	code.update({
		"int a[" + std::to_string(cells.size()) + "], n = " + std::to_string(size) + ";"
		});

	description.newOperation("Create an array of capacity " + std::to_string(cells.size()) + ", size " + std::to_string(size));

	maxPosition1 = size - 1;
	maxPosition2 = size - 2;

	// New step: Create all cells
	action.addNewStep();

	// Description
	if (size == 0) {
		description.addDescription({ "Create " + std::to_string(cells.size()) + " consecutive cells." });
	} else {
		description.addDescription({ "Create " + std::to_string(cells.size()) + " consecutive cells.", "The first " + std::to_string(size) + " cells is highlighted." });
	}

	action.drawFadeIn(&description, description.size() - 1);

	// Cell
	action.drawFadeIn(&cells, 0, size - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
	action.drawFadeIn(&cells, size, cells.size() - 1, SHOLLOW, &assets->blurCellSquareColor, &assets->blurCellTextColor);

	// Label
	action.drawFadeIn(&labels, &assets->labelColor);

	// Code
	action.drawFadeIn(&code, 0);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "An array of capacity " + std::to_string(cells.size()) + ", size " + std::to_string(size) + " is created." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawChange(&cells, 0, size - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->normalCellSquareColor, &assets->highlightCellTextColor1, &assets->normalCellTextColor);
	action.drawFadeOut(&cells, 0, size - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);
	action.draw(&cells, size, cells.size() - 1, SHOLLOW, &assets->blurCellSquareColor, &assets->blurCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 0);
}

void StaticArrayVisualizer::access(int index) {
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
	action.draw(&cells, index + 1, size - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.draw(&cells, size, cells.size() - 1, SHOLLOW, &assets->blurCellSquareColor, &assets->blurCellTextColor);

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
	action.draw(&cells, index + 1, size - 1, SHOLLOW, &assets->normalCellSquareColor, &assets->normalCellTextColor);
	action.draw(&cells, size, cells.size() - 1, SHOLLOW, &assets->blurCellSquareColor, &assets->blurCellTextColor);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 0);
} 

void StaticArrayVisualizer::run() {
	std::vector <char> numbersCharacter;
	for (int i = 0; i <= 9; i++) {
		numbersCharacter.push_back(i + '0');
	}

	std::vector <char> numbersCommaSpaceCharacter = numbersCharacter;
	numbersCommaSpaceCharacter.push_back(',');
	numbersCommaSpaceCharacter.push_back(' ');

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

	std::function <bool()> conditionNone = []() -> bool {
		return true;
	};

	std::function <bool()> conditionSizeNotZero = [&]() -> bool {
		return size != 0;
	};

	std::function <bool()> conditionSizeNotExceedCapacity = [&]() -> bool {
		return size < cells.size();
	};

	int minCapacity = 1, maxCapacity = 15;
	int minSize = 0, maxSize[2];
	int minValue = 0, maxValue = 99;
	int minPosition1 = 0, minPosition2 = 1;

	option.addOption("Create");
	option.addSuboption("Zero", conditionNone);
	option.addSuboptionInput("c", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minCapacity, &maxCapacity), std::bind(generatorInteger, &minCapacity, &maxCapacity));
	option.addSuboptionInput("n", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize[0]), std::bind(generatorInteger, &minSize, &maxSize[0]));
	option.addSuboption("Random", conditionNone);
	option.addSuboptionInput("c", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minCapacity, &maxCapacity), std::bind(generatorInteger, &minCapacity, &maxCapacity));
	option.addSuboptionInput("n", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize[1]), std::bind(generatorInteger, &minSize, &maxSize[1]));
	option.addSuboption("Manual", conditionNone);
	option.addSuboptionInput("v[]", numbersCommaSpaceCharacter, std::bind(validatorListOfIntegers, std::placeholders::_1, std::placeholders::_2, &minCapacity, &maxCapacity, &minValue, &maxValue), std::bind(generatorListOfIntegers, &minCapacity, &maxCapacity, &minValue, &maxValue));
	option.addSuboptionInput("File browser", numbersCommaSpaceCharacter, std::bind(validatorListOfIntegers, std::placeholders::_1, std::placeholders::_2, &minCapacity, &maxCapacity, &minValue, &maxValue), generatorNone);

	option.addOption("Access");
	option.addSuboption("", conditionSizeNotZero);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition1, &maxPosition1), std::bind(generatorInteger, &minPosition1, &maxPosition1));

	option.addOption("Search");
	option.addSuboption("", conditionSizeNotZero);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Update");
	option.addSuboption("", conditionSizeNotZero);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition1, &maxPosition1), std::bind(generatorInteger, &minPosition1, &maxPosition1));
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Insert");
	option.addSuboption("Front (i = 0)", conditionSizeNotExceedCapacity);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));
	option.addSuboption("Back (i = n)", conditionSizeNotExceedCapacity);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));
	option.addSuboption("Middle (i = 1..n - 1)", conditionSizeNotExceedCapacity);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition2, &maxPosition1), std::bind(generatorInteger, &minPosition2, &maxPosition1));
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Erase");
	option.addSuboption("Front (i = 0)", conditionSizeNotZero);
	option.addSuboption("Back (i = n - 1)", conditionSizeNotZero);
	option.addSuboption("Middle (i = 1..n - 2)", conditionSizeNotZero);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition2, &maxPosition2), std::bind(generatorInteger, &minPosition2, &maxPosition2));

	randomArray(10, 7);
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
				case 0: // Zero
					zeroArray(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;

				case 1: // Random
					randomArray(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;

				case 2: // Manual
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
			}

			for (int i : {0, 1}) {
				std::string capacity = option.getValue(0, i, 0);
				if (validatorInteger("", capacity, &minCapacity, &maxCapacity).empty()) {
					maxSize[i] = std::stoi(capacity);
				} else {
					maxSize[i] = 0;
				}
			}
		}
	}
}