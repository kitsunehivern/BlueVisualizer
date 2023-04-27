#include "CLLVisualizer.h"
#include "Random.h"

CLLVisualizer::CLLVisualizer(sf::RenderWindow* window, Assets* assets) {
	this->window = window;
	this->assets = assets;

	action = ActionBox(window, assets, sf::Vector2f(10, 430));
	option = OptionBox(assets, sf::Vector2f(10, 510));
	code = CodeBox(sf::Vector2f(910, 590));
	description = DescriptionBox(sf::Vector2f(910, 430));
}

void CLLVisualizer::randomList(int size) {
	nodes.clear();
	for (int i = 0; i < size; i++) {
		nodes.pushBack(Node(randInt(1, 99), sf::Vector2f()));
	}
}

void CLLVisualizer::manualList(std::string listOfValues) {
	nodes.clear();
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

		nodes.pushBack(Node(std::stoi(listOfValues.substr(i, j - i + 1)), sf::Vector2f()));

		i = j;
	}
}

void CLLVisualizer::createList() {
	action.clearAllSteps();

	labels.clear();
	edges.clear();
	code.update({});
	if (nodes.empty()) {
		description.newOperation("Create an empty list");
	} else {
		description.newOperation("Create a list of size " + std::to_string(nodes.size()));
	}

	maxPosition1 = nodes.size() - 1;
	maxPosition2 = nodes.size() - 2;

	if (!nodes.empty()) {
		nodes.front().position = sf::Vector2f(NODE_POSITION_X, NODE_POSITION_Y);
		labels.pushBack(Label(&nodes.front()));
		for (ListNode <Node>* iterator = nodes.begin()->next(); iterator != nodes.end(); iterator = iterator->next()) {
			iterator->data.position.x = iterator->prev()->data.position.x + (60 + NODE_DISTANCE);
			iterator->data.position.y = iterator ->prev()->data.position.y;
			labels.pushBack(Label(&iterator->data));
			edges.pushBack(Edge(&iterator->prev()->data, &iterator->data));
		}

		circularEdge.left = &nodes.front();
		circularEdge.right = &nodes.back();
	}

	if (!nodes.empty()) {
		// New step: Create all nodes
		action.addNewStep();

		// Description
		if (nodes.size() == 1) {
			description.addDescription({ "Create only 1 node.", "'head' points to that node." });
		} else {
			description.addDescription({ "Create " + std::to_string(nodes.size()) + " nodes.", "'head' points to the first node." });
		}

		action.drawFadeIn(&description, description.size() - 1);

		// Edge

		// Node
		action.drawFadeIn(&nodes, 0, nodes.size() - 1, SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

		// Label
		if (!labels.empty()) {
			action.drawFadeIn(&labels.front(), &LABEL_COLOR, "head");
		}

		// Code
		action.draw(&code);
	}

	if (!nodes.empty()) {
		// New step: Create all edges
		action.addNewStep();

		// Description
		description.addDescription({ "Create next pointers for every nodes." });

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.drawSlideIn(&edges, 0, (int)edges.size() - 1, &INSERTED_EDGE_COLOR);
		action.drawSlideIn(&circularEdge, &INSERTED_EDGE_COLOR);

		// Node
		action.draw(&nodes, 0, nodes.size() - 1, SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

		// Label
		action.draw(&labels.front(), &LABEL_COLOR, "head");

		// Code
		action.draw(&code);
	}

	// New step: Re-layout
	action.addNewStep();

	// Description
	if (nodes.empty()) {
		description.addDescription({ "An empty list is created." });
		action.drawFadeIn(&description, description.size() - 1);
	} else {
		description.addDescription({ "A list of size " + std::to_string(nodes.size()) + " is created." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);
	}

	// Edge
	if (!edges.empty()) {
		action.drawChange(&edges, 0, edges.size() - 1, &INSERTED_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	}

	if (!nodes.empty()) {
		action.drawChange(&circularEdge, &INSERTED_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	}

	// Node
	if (!nodes.empty()) {
		action.drawChange(&nodes, 0, nodes.size() - 1, HOLLOW, &INSERTED_NODE_CIRCLE_COLOR, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1, &NORMAL_NODE_TEXT_COLOR);
		action.drawFadeOut(&nodes, 0, nodes.size() - 1, SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	}

	// Label
	if (!labels.empty()) {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
	}

	// Code
	action.draw(&code);
}

void CLLVisualizer::run() {
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

	std::function <bool()> conditionListNotEmpty = [&]() -> bool {
		return !nodes.empty();
	};

	std::function <bool()> conditionListNotLarge = [&]() -> bool {
		return nodes.size() < 10;
	};

	int minSize = 1, maxSize = 10;
	int minValue = 0, maxValue = 99;
	int minPosition1 = 0, minPosition2 = 1;

	option.addOption("Create");
	option.addSuboption("Empty", conditionNone);
	option.addSuboption("Random", conditionNone);
	option.addSuboptionInput("n", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize), std::bind(generatorInteger, &minSize, &maxSize));
	option.addSuboption("Manual", conditionNone);
	option.addSuboptionInput("v[]", numbersCommaSpaceCharacter, std::bind(validatorListOfIntegers, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize, &minValue, &maxValue), std::bind(generatorListOfIntegers, &minSize, &maxSize, &minValue, &maxValue));
	option.addSuboptionInput("File browser", numbersCommaSpaceCharacter, std::bind(validatorListOfIntegers, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize, &minValue, &maxValue), generatorNone);

	option.addOption("Search");
	option.addSuboption("", conditionNone);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Update");
	option.addSuboption("", conditionListNotEmpty);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition1, &maxPosition1), std::bind(generatorInteger, &minPosition1, &maxPosition1));
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Insert");
	option.addSuboption("Front (i = 0)", conditionListNotLarge);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));
	option.addSuboption("Back (i = n)", conditionListNotLarge);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));
	option.addSuboption("Middle (i = 1..n - 1)", conditionListNotLarge);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition2, &maxPosition1), std::bind(generatorInteger, &minPosition2, &maxPosition1));
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Erase");
	option.addSuboption("Front (i = 0)", conditionListNotEmpty);
	option.addSuboption("Back (i = n - 1)", conditionListNotEmpty);
	option.addSuboption("Middle (i = 1..n - 2)", conditionListNotEmpty);
	option.addSuboptionInput("i", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minPosition2, &maxPosition2), std::bind(generatorInteger, &minPosition2, &maxPosition2));

	randomList(7);
	createList();

	while (window->isOpen()) {
		window->clear(sf::Color(190, 230, 240));

		action.draw();
		option.updateMessage();
		window->draw(option);

		window->display();

		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close();
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
					randomList(0);
					break;

				case 1: // Random
					randomList(std::stoi(std::get <2> (current)[0]));
					break;

				case 2: // Manual
					manualList(std::get <2> (current)[0]);
				}

				createList();
				break;

			case 1: // Search
				switch (std::get <1> (current)) {
				case 0: // 
					//searchValue(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 2: // Update
				switch (std::get <1> (current)) {
				case 0: // 
					//updateValue(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 3: // Insert
				switch (std::get <1> (current)) {
				case 0: // Front
					//insertNode(0, std::stoi(std::get <2> (current)[0]));
					break;

				case 1: // Back
					//insertNode(nodes.size(), std::stoi(std::get <2>(current)[0]), false);
					break;

				case 2: // Middle
					//insertNode(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 4: // Erase
				switch (std::get <1> (current)) {
				case 0: // Front
					//eraseNode(0);
					break;

				case 1: // Back
					//eraseNode(nodes.size() - 1, false);
					break;

				case 2: // Middle
					//eraseNode(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;
			}
		}
	}
}