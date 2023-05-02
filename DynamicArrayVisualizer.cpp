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
			iterator->data.position.x = iterator->prev()->data.position.x + 60 + CELL_DISTANCE;
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
	description.addDescription({ "An array of capacity size " + std::to_string(cells.size()) + " is created." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Cell
	action.drawChange(&cells, 0, cells.size() - 1, SHOLLOW, &assets->insertedCellSquareColor, &assets->normalCellSquareColor, &assets->highlightCellTextColor1, &assets->normalCellTextColor);
	action.drawFadeOut(&cells, 0, cells.size() - 1, SSOLID, &assets->insertedCellSquareColor, &assets->highlightCellTextColor1);

	// Label
	action.draw(&labels, &assets->labelColor);

	// Code
	action.drawFadeOut(&code, 0);
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
		return cells.size() > minSize;
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
					//access(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 2: // Search
				switch (std::get <1> (current)) {
				case 0: //
					//search(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 3: // Update
				switch (std::get <1> (current)) {
				case 0: //
					//update(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 4: // Insert
				switch (std::get <1> (current)) {
				case 0: // Front
					//insertAtTheFront(std::stoi(std::get <2> (current)[0]));
					break;

				case 1: // Back
					//insertAtTheBack(std::stoi(std::get <2> (current)[0]));
					break;

				case 2: // Middle
					//insertAtTheMiddle(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 5: // Erase
				switch (std::get <1> (current)) {
				case 0: // Front
					//eraseAtTheFront();
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