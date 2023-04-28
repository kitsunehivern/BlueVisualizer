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

void CLLVisualizer::create() {
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
		action.drawSlideIn(&edges, 0, edges.size() - 1, &INSERTED_EDGE_COLOR);
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

void CLLVisualizer::insertWhenEmpty(int value, bool head) {
	nodes.pushFront(Node(value, sf::Vector2f(NODE_POSITION_X, NODE_POSITION_Y)));
	labels.pushFront(Label(&nodes.front()));
	circularEdge.left = &nodes.front();
	circularEdge.right = &nodes.back();

	code.update({
		"head = new Node(v);",
		"head->next = head; "
		});

	if (head) {
		description.newOperation("Insert " + std::to_string(value) + " at the front");
	} else {
		description.newOperation("Insert " + std::to_string(value) + " at the back");
	}

	// New step: head = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "'head' points to a new node with value " + std::to_string(value) + "." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge

	// Node
	action.drawFadeIn(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.draw(&labels.front(), &LABEL_COLOR, "head");

	// Code
	action.drawFadeIn(&code, 0);

	// New step: head->next = head;
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'head' to itself." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawSlideIn(&circularEdge, &INSERTED_EDGE_COLOR);

	// Node
	action.draw(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.draw(&labels.front(), &LABEL_COLOR, "head");

	// Code
	action.drawMove(&code, 0, 1);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&circularEdge, &INSERTED_EDGE_COLOR, &NORMAL_EDGE_COLOR);

	// Node
	action.drawChange(&nodes.front(), HOLLOW, &INSERTED_NODE_CIRCLE_COLOR, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.draw(&labels.front(), &LABEL_COLOR, "head");

	// Code
	action.drawFadeOut(&code, 1);
}

void CLLVisualizer::insertAtTheFront(int value) {
	nodes.pushFront(Node(value, sf::Vector2f(NODE_POSITION_X, NODE_POSITION_Y + 60 + NODE_DISTANCE)));
	labels.pushFront(Label(&nodes.front()));
	edges.pushFront(Edge(&nodes.begin()->data, &nodes.begin()->next()->data));
	circularEdge.left = &nodes.front();

	randomNode = nodes.begin()->next(1)->data;
	randomCircularEdge.left = &randomNode;
	randomCircularEdge.right = &nodes.back();

	code.update({
		"Node* pre = head;               ",
		"for (int k = 0; k <= n - 2; k++)",
		"    pre = pre->next;            ",
		"Node* add = new Node(v);        ",
		"add->next = head;               ",
		"pre->next = add;                ",
		"head = add;                     "
		});

	description.newOperation("Insert " + std::to_string(value) + " at the front");

	// New step: Node* pre = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'pre' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
	action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.drawChange(&nodes, 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawFadeIn(&nodes, 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, 2, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

	// Label
	action.drawChange(&labels, 1, &LABEL_COLOR, "head", "head/pre");

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i < edges.size(); i++) {
		// New step: for (int k = 0; k < n - 1; k++)
		action.addNewStep();

		// Description
		if (nodes.size() == 2) {
			description.addDescription({ "Iterate k from 0 to n - 2 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to n - 2 = " + std::to_string(nodes.size() - 3) + ", k is now 0.", "Since k <= " + std::to_string(nodes.size() - 3) + ", the loop continues." });
			} else if (i < edges.size() - 1) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(nodes.size() - 3) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(nodes.size() - 3) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 1, i, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&edges, i + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
		action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

		// Node
		action.draw(&nodes, 1, i, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
		action.draw(&nodes, i + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.draw(&nodes, i + 2, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

		// Label
		if (i == 0) {
			action.drawChange(&labels, 1, &LABEL_COLOR, "head/pre", "0/head/pre");
		} else {
			action.draw(&labels, 1, &LABEL_COLOR, "head");
			action.drawChange(&labels, i + 1, &LABEL_COLOR, "pre", std::to_string(i) + "/pre");
		}

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		// New step: pre = pre->next
		if (i == edges.size() - 1) {
			break;
		}

		action.addNewStep();

		// Description
		description.addDescription({ "Set 'pre' to the pointer of the next node." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 1, i, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&edges, i + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
		action.drawSlideIn(&edges, i + 1, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

		// Node
		action.draw(&nodes, 1, i + 1, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
		action.drawFadeOut(&nodes, i + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.drawChange(&nodes, i + 2, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.drawFadeIn(&nodes, i + 2, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.draw(&nodes, i + 3, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

		// Label
		if (i == 0) {
			action.drawChange(&labels, 1, &LABEL_COLOR, "0/head/pre", "head");
		} else {
			action.draw(&labels, 1, &LABEL_COLOR, "head");
			action.drawFadeOut(&labels, i + 1, &LABEL_COLOR, std::to_string(i) + "/pre");
		}

		action.drawFadeIn(&labels, i + 2, &LABEL_COLOR, "pre");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: Node* add = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'add' that points to a new", "node with value " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 1, edges.size() - 1, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.drawFadeIn(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, 1, nodes.size() - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, nodes.size() - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.drawChange(&labels, 1, &LABEL_COLOR, "0/head/pre", "head/pre");
	} else {
		action.draw(&labels, 1, &LABEL_COLOR, "head");
		action.drawChange(&labels.back(), &LABEL_COLOR, std::to_string(labels.size() - 2) + "/pre", "pre");
	}

	action.drawFadeIn(&labels.front(), &LABEL_COLOR, "add");

	// Code
	action.drawMove(&code, 1, 3);

	// New step: add->next = head
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'add' to 'head'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawSlideIn(&edges.front(), &INSERTED_EDGE_COLOR);
	action.draw(&edges, 1, edges.size() - 1, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.draw(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, 1, nodes.size() - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, nodes.size() - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.draw(&labels, 1, &LABEL_COLOR, "head/pre");
	} else {
		action.draw(&labels, 1, &LABEL_COLOR, "head");
		action.draw(&labels.back(), &LABEL_COLOR, "pre");
	}

	action.draw(&labels.front(), &LABEL_COLOR, "add");

	// Code
	action.drawMove(&code, 3, 4);

	// New step: pre->next = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'pre' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges.front(), &INSERTED_EDGE_COLOR);
	action.draw(&edges, 1, edges.size() - 1, &HIGHLIGHT_EDGE_COLOR);
	action.drawChange(&randomCircularEdge, &NORMAL_EDGE_COLOR, &HIGHLIGHT_EDGE_COLOR);

	// Node
	action.drawSlide(&randomNode, HOLLOW, &BLANK_COLOR, &BLANK_COLOR, &nodes.begin()->next()->data, &nodes.front());
	action.draw(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, 1, nodes.size() - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, nodes.size() - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.draw(&labels, 1, &LABEL_COLOR, "head/pre");
	} else {
		action.draw(&labels, 1, &LABEL_COLOR, "head");
		action.draw(&labels.back(), &LABEL_COLOR, "pre");
	}

	action.draw(&labels.front(), &LABEL_COLOR, "add");

	// Code
	action.drawMove(&code, 4, 5);

	// New step: head = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set 'head' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges.front(), &INSERTED_EDGE_COLOR);
	action.draw(&edges, 1, edges.size() - 1, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&circularEdge, &HIGHLIGHT_EDGE_COLOR);

	// Node
	action.draw(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, 1, nodes.size() - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, nodes.size() - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.drawChange(&labels, 1, &LABEL_COLOR, "head/pre", "pre");
	} else {
		action.drawFadeOut(&labels, 1, &LABEL_COLOR, "head");
		action.draw(&labels.back(), &LABEL_COLOR, "pre");
	}

	action.drawChange(&labels.front(), &LABEL_COLOR, "add", "head/add");

	// Code
	action.drawMove(&code, 5, 6);

	// New step: Re-layout
	action.addNewStep();

	description.addDescription({ "Re-layout the list for visualization.", "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges.front(), &INSERTED_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	action.drawChange(&edges, 1, edges.size() - 1, &HIGHLIGHT_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	action.drawChange(&circularEdge, &HIGHLIGHT_EDGE_COLOR, &NORMAL_EDGE_COLOR);

	// Node
	action.drawMove(&nodes.front(), HOLLOW, &BLANK_COLOR, &BLANK_COLOR, nodes.front().position, nodes.front().position + sf::Vector2f(0, -(60 + NODE_DISTANCE)));
	action.drawChange(&nodes.front(), HOLLOW, &INSERTED_NODE_CIRCLE_COLOR, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes.front(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	for (int i = 1; i < nodes.size(); i++) {
		action.drawMove(&nodes, i, HOLLOW, &BLANK_COLOR, &BLANK_COLOR, nodes.begin()->next(i)->data.position, nodes.begin()->next(i)->data.position + sf::Vector2f((60 + NODE_DISTANCE), 0));
	}

	action.drawChange(&nodes, 1, nodes.size() - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_2, &NORMAL_NODE_TEXT_COLOR);
	action.drawChange(&nodes, nodes.size() - 1, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes, nodes.size() - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.drawFadeOut(&labels.back(), &LABEL_COLOR, "pre");
	action.drawChange(&labels.front(), &LABEL_COLOR, "head/add", "head");

	// Code
	action.drawFadeOut(&code, 6);
}

void CLLVisualizer::insertAtTheBack(int value) {
	nodes.pushBack(Node(value, sf::Vector2f(NODE_POSITION_X + nodes.size() * (60 + NODE_DISTANCE), NODE_POSITION_Y)));
	labels.pushBack(Label(&nodes.back()));
	edges.pushBack(Edge(&nodes.rbegin()->prev()->data, &nodes.rbegin()->data));
	circularEdge.right = &nodes.back();
	code.update({
		"Node* pre = head;               ",
		"for (int k = 0; k <= n - 2; k++)",
		"    pre = pre->next;            ",
		"Node* add = new Node(v);        ",
		"pre->next = add;                ",
		"add->next = head;               "
		});

	randomNode = nodes.rbegin()->prev()->data;
	randomCircularEdge.left = &nodes.front();
	randomCircularEdge.right = &randomNode;

	description.newOperation("Insert " + std::to_string(value) + " at the back");

	// New step: Node* pre = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'pre' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &NORMAL_EDGE_COLOR);
	action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.drawChange(&nodes.front(), HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawFadeIn(&nodes.front(), SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, 1, nodes.size() - 2, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

	// Label
	action.drawChange(&labels.front(), &LABEL_COLOR, "head", "head/pre");

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i < edges.size(); i++) {
		// New step: for (int k = 0; k < n - 1; k++)
		action.addNewStep();

		// Description
		if (nodes.size() == 2) {
			description.addDescription({ "Iterate k from 0 to n - 2 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to n - 2 = " + std::to_string(nodes.size() - 3) + ", k is now 0.", "Since k <= " + std::to_string(nodes.size() - 3) + ", the loop continues." });
			} else if (i < edges.size() - 1) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(nodes.size() - 3) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(nodes.size() - 3) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&edges, i, edges.size() - 2, &NORMAL_EDGE_COLOR);
		action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
		action.draw(&nodes, i, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.draw(&nodes, i + 1, nodes.size() - 2, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &LABEL_COLOR, "head/pre", "0/head/pre");
		} else {
			action.draw(&labels.front(), &LABEL_COLOR, "head");
			action.drawChange(&labels, i, &LABEL_COLOR, "pre", std::to_string(i) + "/pre");
		}

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		// New step: pre = pre->next
		if (i == edges.size() - 1) {
			break;
		}

		action.addNewStep();

		// Description
		description.addDescription({ "Set 'pre' to the pointer of the next node." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&edges, i, edges.size() - 2, &NORMAL_EDGE_COLOR);
		action.drawSlideIn(&edges, i, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
		action.drawFadeOut(&nodes, i, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.drawChange(&nodes, i + 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.drawFadeIn(&nodes, i + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.draw(&nodes, i + 2, nodes.size() - 2, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &LABEL_COLOR, "0/head/pre", "head");
		} else {
			action.drawFadeOut(&labels, i, &LABEL_COLOR, std::to_string(i) + "/pre");
			action.draw(&labels.front(), &LABEL_COLOR, "head");
		}

		action.drawFadeIn(&labels, i + 1, &LABEL_COLOR, "pre");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: Node* add = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'add' that points to a new", "node with value " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&randomCircularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, nodes.size() - 2, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawFadeIn(&nodes.back(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.drawChange(&labels.front(), &LABEL_COLOR, "0/head/pre", "head/pre");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.drawChange(&labels, labels.size() - 2, &LABEL_COLOR, std::to_string(labels.size() - 2) + "/pre", "pre");
	}

	action.drawFadeIn(&labels.back(), &LABEL_COLOR, "add");

	// Code
	action.drawMove(&code, 1, 3);

	// New step: pre->next = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'pre' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &HIGHLIGHT_EDGE_COLOR);
	action.drawSlideOut(&randomCircularEdge, &NORMAL_EDGE_COLOR);
	action.drawSlideIn(&edges.back(), &HIGHLIGHT_EDGE_COLOR);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, nodes.size() - 2, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes.back(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.draw(&labels.front(), &LABEL_COLOR, "head/pre");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.draw(&labels, labels.size() - 2, &LABEL_COLOR, "pre");
	}

	action.draw(&labels.back(), &LABEL_COLOR, "add");

	// Code
	action.drawMove(&code, 3, 4);

	// add->next = head
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'add' to 'head'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&edges.back(), &HIGHLIGHT_EDGE_COLOR);
	action.drawSlideIn(&circularEdge, &INSERTED_EDGE_COLOR);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, nodes.size() - 2, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes.back(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.draw(&labels.front(), &LABEL_COLOR, "head/pre");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.draw(&labels, labels.size() - 2, &LABEL_COLOR, "pre");
	}

	action.draw(&labels.back(), &LABEL_COLOR, "add");

	// Code
	action.drawMove(&code, 4, 5);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, edges.size() - 2, &HIGHLIGHT_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	action.drawChange(&edges.back(), &HIGHLIGHT_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	action.drawChange(&circularEdge, &INSERTED_EDGE_COLOR, &NORMAL_EDGE_COLOR);

	// Node
	action.drawChange(&nodes, 0, nodes.size() - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_2, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes, nodes.size() - 2, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawChange(&nodes.back(), HOLLOW, &INSERTED_NODE_CIRCLE_COLOR, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes.back(), SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	if (nodes.size() == 2) {
		action.drawChange(&labels.front(), &LABEL_COLOR, "head/pre", "head");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.drawFadeOut(&labels, labels.size() - 2, &LABEL_COLOR, "pre");
	}

	action.drawFadeOut(&labels.back(), &LABEL_COLOR, "add");

	// Code
	action.drawFadeOut(&code, 5);
}

void CLLVisualizer::insertAtTheMiddle(int index, int value) {
	nodes.insert(index, Node(value, sf::Vector2f(NODE_POSITION_X + index * (60 + NODE_DISTANCE), NODE_POSITION_Y + 60 + NODE_DISTANCE)));
	labels.insert(index, Label(&nodes.begin()->next(index)->data));
	edges.insert(index, Edge(&nodes.begin()->next(index)->data, &nodes.begin()->next(index + 1)->data));
	edges.begin()->next(index - 1)->data.right = &nodes.begin()->next(index)->data;
	code.update({
		"Node* pre = head;               ",
		"for (int k = 0; k <= i - 2; k++)",
		"    pre = pre->next;            ",
		"Node* nxt = pre->next;          ",
		"Node* add = new Node(v);        ",
		"add->next = nxt;                ",
		"pre->next = add;                "
		});

	description.newOperation("Insert " + std::to_string(value) + " at index " + std::to_string(index));

	randomNode.position = nodes.begin()->next(index + 1)->data.position;
	randomEdge.left = &nodes.begin()->next(index - 1)->data;
	randomEdge.right = &randomNode;

	// New step: Node* pre = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'pre' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &NORMAL_EDGE_COLOR);
	action.draw(&randomEdge, &NORMAL_EDGE_COLOR);
	action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
	action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.draw(&nodes, 1, index - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);
	action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);
	action.drawChange(&nodes.front(), HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawFadeIn(&nodes.front(), SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.drawChange(&labels.front(), &LABEL_COLOR, "head", "head/pre");

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i < index; i++) {
		// New step: for (int k = 0; k <= i - 2; k++)
		action.addNewStep();

		// Description
		if (index == 1) {
			description.addDescription({ "Iterate k from 0 to i - 2 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to i - 2 = " + std::to_string(index - 2) + ", k is now 0.", "Since k <= " + std::to_string(index - 2) + ", the loop continues." });
			} else if (i <= index - 2) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(index - 2) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(index - 2) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&edges, i, index - 2, &NORMAL_EDGE_COLOR);
		action.draw(&randomEdge, &NORMAL_EDGE_COLOR);
		action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
		action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
		action.draw(&nodes, i, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.draw(&nodes, i + 1, index - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);
		action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &LABEL_COLOR, "head/pre", "0/head/pre");
		} else {
			action.draw(&labels.front(), &LABEL_COLOR, "head");
			action.drawChange(&labels, i, &LABEL_COLOR, "pre", std::to_string(i) + "/pre");
		}

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		if (i == index - 1) {
			break;
		}

		// New step: pre = pre->next
		action.addNewStep();

		// Description
		description.addDescription({ "Set 'pre' to the pointer of the next node." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&edges, i, index - 2, &NORMAL_EDGE_COLOR);
		action.draw(&randomEdge, &NORMAL_EDGE_COLOR);
		action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
		action.drawSlideIn(&edges, i, &HIGHLIGHT_EDGE_COLOR);
		action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
		action.drawFadeOut(&nodes, i, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.drawChange(&nodes, i + 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.drawFadeIn(&nodes, i + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
		action.draw(&nodes, i + 2, index - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);
		action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &LABEL_COLOR, "0/head/pre", "head");
		} else {
			action.drawFadeOut(&labels, i, &LABEL_COLOR, std::to_string(i) + "/pre");
			action.draw(&labels.front(), &LABEL_COLOR, "head");
		}

		action.drawFadeIn(&labels, i + 1, &LABEL_COLOR, "pre");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: Node* nxt = pre->next
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'nxt' that points to the next", "node of 'pre'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&randomEdge, &NORMAL_EDGE_COLOR);
	action.drawSlideIn(&randomEdge, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
	action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, index - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawChange(&nodes, index + 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_2, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawFadeIn(&nodes, index + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_2, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

	// Label
	if (index == 1) {
		action.drawChange(&labels.front(), &LABEL_COLOR, "0/head/pre", "head/pre");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.drawChange(&labels, index - 1, &LABEL_COLOR, std::to_string(index - 1) + "/pre", "pre");
	}

	action.drawFadeIn(&labels, index + 1, &LABEL_COLOR, "nxt");

	// Code
	action.drawMove(&code, 1, 3);

	// New step: Node* add = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'add' that points to a new", "node with value " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&randomEdge, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
	action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, index - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawFadeIn(&nodes, index, SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_2, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &LABEL_COLOR, "head/pre");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.draw(&labels, index - 1, &LABEL_COLOR, "pre");
	}

	action.drawFadeIn(&labels, index, &LABEL_COLOR, "add");
	action.draw(&labels, index + 1, &LABEL_COLOR, "nxt");

	// Code
	action.drawMove(&code, 3, 4);

	// New step: add->next = nxt
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'add' to 'nxt'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge:
	action.draw(&edges, 0, index - 2, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&randomEdge, &HIGHLIGHT_EDGE_COLOR);
	action.drawSlideIn(&edges, index, &INSERTED_EDGE_COLOR);
	action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
	action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, index - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index, SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_2, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &LABEL_COLOR, "head/pre");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.draw(&labels, index - 1, &LABEL_COLOR, "pre");
	}

	action.draw(&labels, index, &LABEL_COLOR, "add");
	action.draw(&labels, index + 1, &LABEL_COLOR, "nxt");

	// Code
	action.drawMove(&code, 4, 5);

	// New step: pre->next = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'pre' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&randomEdge, &HIGHLIGHT_EDGE_COLOR);
	action.draw(&edges, index, &INSERTED_EDGE_COLOR);
	action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
	action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.drawMove(&randomNode, HOLLOW, &BLANK_COLOR, &BLANK_COLOR, randomNode.position, randomNode.position + sf::Vector2f(0, (60 + NODE_DISTANCE)));
	action.draw(&nodes, index - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index, SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, index + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_2, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.draw(&nodes, 0, index - 2, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_2);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &LABEL_COLOR, "head/pre");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.draw(&labels, index - 1, &LABEL_COLOR, "pre");
	}

	action.draw(&labels, index, &LABEL_COLOR, "add");
	action.draw(&labels, index + 1, &LABEL_COLOR, "nxt");

	// Code
	action.drawMove(&code, 5, 6);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "Re-layout the list for visualization.", "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, index - 1, &HIGHLIGHT_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	action.drawChange(&edges, index, &INSERTED_EDGE_COLOR, &NORMAL_EDGE_COLOR);
	action.draw(&edges, index + 1, edges.size() - 1, &NORMAL_EDGE_COLOR);
	action.draw(&circularEdge, &NORMAL_EDGE_COLOR);

	// Node
	action.drawMove(&nodes, index, HOLLOW, &BLANK_COLOR, &BLANK_COLOR, nodes.begin()->next(index)->data.position, nodes.begin()->next(index)->data.position + sf::Vector2f(0, -(60 + NODE_DISTANCE)));
	action.drawChange(&nodes, 0, index - 1, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_2, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes, index - 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_1, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawChange(&nodes, index, HOLLOW, &INSERTED_NODE_CIRCLE_COLOR, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes, index, SOLID, &INSERTED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);
	action.drawMove(&nodes, index + 1, HOLLOW, &BLANK_COLOR, &BLANK_COLOR, nodes.begin()->next(index + 1)->data.position, nodes.begin()->next(index + 1)->data.position + sf::Vector2f((60 + NODE_DISTANCE), 0));
	action.drawChange(&nodes, index + 1, HOLLOW, &HIGHLIGHT_NODE_CIRCLE_COLOR_2, &NORMAL_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1, &NORMAL_NODE_TEXT_COLOR);
	action.drawFadeOut(&nodes, index + 1, SOLID, &HIGHLIGHT_NODE_CIRCLE_COLOR_2, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	for (int i = index + 2; i < nodes.size(); i++) {
		action.drawMove(&nodes, i, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR, nodes.begin()->next(i)->data.position, nodes.begin()->next(i)->data.position + sf::Vector2f((60 + NODE_DISTANCE), 0));
	}

	// Label
	if (index == 1) {
		action.drawChange(&labels.front(), &LABEL_COLOR, "head/pre", "head");
	} else {
		action.draw(&labels.front(), &LABEL_COLOR, "head");
		action.drawFadeOut(&labels, index - 1, &LABEL_COLOR, "pre");
	}

	action.drawFadeOut(&labels, index, &LABEL_COLOR, "add");
	action.drawFadeOut(&labels, index + 1, &LABEL_COLOR, "nxt");

	// Code
	action.drawFadeOut(&code, 6);
}

void CLLVisualizer::insert(int index, int value, bool head) {
	action.clearAllSteps();

	if (nodes.empty()) {
		insertWhenEmpty(value, head);
	} else if (index == 0) {
		insertAtTheFront(value);
	} else if (index == nodes.size()) {
		insertAtTheBack(value);
	} else {
		insertAtTheMiddle(index, value);
	}

	maxPosition1 = nodes.size() - 1;
	maxPosition2 = nodes.size() - 2;
}

void CLLVisualizer::eraseToEmpty(bool head) {
	deletedNode = nodes.popFront();
	labels.popFront(); deletedLabel = Label(&deletedNode);
	randomCircularEdge.left = randomCircularEdge.right = &deletedNode;

	code.update({
		"Node* del = head;",
		"head = NULL;     ",
		"delete del;      "
		});

	if (head) {
		description.newOperation("Erase value at the front");
	} else {
		description.newOperation("Erase value at the back");
	}

	// New step: Node* del = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'del' that points to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.drawChange(&randomCircularEdge, &NORMAL_EDGE_COLOR, &ERASED_EDGE_COLOR);

	// Node
	action.drawChange(&deletedNode, HOLLOW, &NORMAL_NODE_CIRCLE_COLOR, &ERASED_NODE_CIRCLE_COLOR, &NORMAL_NODE_TEXT_COLOR, &HIGHLIGHT_NODE_CIRCLE_COLOR_1);
	action.drawFadeIn(&deletedNode, SOLID, &ERASED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.drawChange(&deletedLabel, &LABEL_COLOR, "head", "head/del");

	// Code
	action.drawFadeIn(&code, 0);

	// New step: head = NULL;
	action.addNewStep();

	// Description
	description.addDescription({ "Set 'head' to NULL." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&randomCircularEdge, &ERASED_EDGE_COLOR);

	// Node
	action.draw(&deletedNode, SOLID, &ERASED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.drawChange(&deletedLabel, &LABEL_COLOR, "head/del", "del");

	// Code
	action.drawMove(&code, 0, 1);

	// New step: delete del
	action.addNewStep();

	// Description
	description.addDescription({ "Erase 'del'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawSlideOut(&randomCircularEdge, &ERASED_EDGE_COLOR);

	// Node
	action.drawFadeOut(&deletedNode, SOLID, &ERASED_NODE_CIRCLE_COLOR, &HIGHLIGHT_NODE_TEXT_COLOR_1);

	// Label
	action.drawFadeOut(&deletedLabel, &LABEL_COLOR, "del");

	// Code
	action.drawMove(&code, 1, 2);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge

	// Node

	// Label

	// Code
	action.drawFadeOut(&code, 2);
}

void CLLVisualizer::erase(int index, bool head) {
	action.clearAllSteps();

	if (nodes.size() == 1) {
		eraseToEmpty(head);
	} else if (index == 0) {
		//eraseAtTheFront();
	} else if (index == nodes.size() - 1) {
		//eraseAtTheBack(); 
	} else {
		//eraseAtTheMiddle(index);
	}

	maxPosition1 = nodes.size() - 1;
	maxPosition2 = nodes.size() - 2;
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
	create();

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

				create();
				break;

			case 1: // Search
				switch (std::get <1> (current)) {
				case 0: // 
					//search(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 2: // Update
				switch (std::get <1> (current)) {
				case 0: // 
					//update(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 3: // Insert
				switch (std::get <1> (current)) {
				case 0: // Front
					insert(0, std::stoi(std::get <2> (current)[0]));
					break;

				case 1: // Back
					insert(nodes.size(), std::stoi(std::get <2> (current)[0]), false);
					break;

				case 2: // Middle
					insert(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 4: // Erase
				switch (std::get <1> (current)) {
				case 0: // Front
					erase(0);
					break;

				case 1: // Back
					erase(nodes.size() - 1, false);
					break;

				case 2: // Middle
					//erase(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;
			}
		}
	}
}