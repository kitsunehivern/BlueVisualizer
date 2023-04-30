#include "DLLVisualizer.h"
#include "Random.h"

DLLVisualizer::DLLVisualizer(sf::RenderWindow* window, Assets* assets) {
	this->window = window;
	this->assets = assets;

	action = ActionBox(window, assets, sf::Vector2f(10, 430));
	option = OptionBox(assets, sf::Vector2f(10, 510));
	code = CodeBox(assets, sf::Vector2f(910, 590));
	description = DescriptionBox(assets, sf::Vector2f(910, 430));
}

void DLLVisualizer::randomList(int size) {
	nodes.clear();
	for (int i = 0; i < size; i++) {
		nodes.pushBack(Node(randInt(1, 99), sf::Vector2f()));
	}
}

void DLLVisualizer::manualList(std::string listOfValues) {
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

void DLLVisualizer::create() {
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
			iterator->data.position.x = iterator->prev()->data.position.x + 60 + NODE_DISTANCE;
			iterator->data.position.y = iterator ->prev()->data.position.y;
			labels.pushBack(Label(&iterator->data));
			edges.pushBack(DoublyEdge(&iterator->prev()->data, &iterator->data));
		}
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
		action.drawFadeIn(&nodes, 0, nodes.size() - 1, SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

		// Label
		if (!labels.empty()) {
			action.drawFadeIn(&labels.front(), &assets->labelColor, "head");
		}

		// Code
		action.draw(&code);
	}

	if (!edges.empty()) {
		// New step: Create all edges
		action.addNewStep();

		// Description
		description.addDescription({ "Create next and prev pointers for every nodes."});

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.drawSlideIn(&edges, 0, edges.size() - 1, &assets->insertedEdgeColor, &assets->insertedEdgeColor);

		// Node
		action.draw(&nodes, 0, nodes.size() - 1, SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

		// Label
		action.draw(&labels.front(), &assets->labelColor, "head");

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
		action.drawChange(&edges, 0, edges.size() - 1, &assets->insertedEdgeColor, &assets->normalEdgeColor, &assets->insertedEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	if (!nodes.empty()) {
		action.drawChange(&nodes, 0, nodes.size() - 1, HOLLOW, &assets->insertedNodeCircleColor, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
		action.drawFadeOut(&nodes, 0, nodes.size() - 1, SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	}

	// Label
	if (!labels.empty()) {
		action.draw(&labels.front(), &assets->labelColor, "head");
	}

	// Code
	action.draw(&code);
}

void DLLVisualizer::search(int value) {
	action.clearAllSteps();

	code.update({
		"if (n == 0) return NOT_FOUND;     ",
		"Node* cur = head;                 ",
		"for (int k = 0; k <= n - 1; k++) {",
		"    if (cur->data == v) return k; ",
		"    cur = cur->next;              ",
		"}                                 ",
		"return NOT_FOUND;                 ",
		});

	description.newOperation("Search " + std::to_string(value));

	// New step: if (n == 0) return NOT_FOUND
	action.addNewStep();

	// Description
	if (nodes.size() == 0) {
		description.addDescription({ "Check if the list is empty.", "Since n = 0, NOT_FOUND is returned." });
	} else {
		description.addDescription({ "Check if the list is empty.", "Since n > 0, the statement is ignored." });
	}

	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	if (nodes.size() > 0) {
		action.draw(&nodes, 0, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (nodes.size() > 0) {
		action.draw(&labels.front(), &assets->labelColor, "head");
	}

	// Code
	action.drawFadeIn(&code, 0);

	if (nodes.size() == 0) {
		// New step: Re-layout
		action.addNewStep();

		// Description
		description.addDescription({ "So the value " + std::to_string(value) + " was not found.", "The whole process is O(1)."});
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge

		// Node

		// Label

		// Code
		action.drawFadeOut(&code, 0);

		return;
	}

	// New step: Node* cur = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'cur' and set it to 'head'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.drawChange(&nodes.front(), HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "head", "head/cur");

	// Code
	action.drawMove(&code, 0, 1);

	for (int i = 0; i <= nodes.size(); i++) {
		// New step: for (int k = 0; k <= n - 1; k++)
		action.addNewStep();

		// Description
		if (i == 0) {
			description.addDescription({ "Iterate k from 0 to n - 1 = " + std::to_string(nodes.size() - 1) + ", k is now 0.", "Since k <= " + std::to_string(nodes.size() - 1) + ", the loop continues." });
		} else if (i <= nodes.size() - 1) {
			description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(nodes.size() - 1) + ", the loop continues." });
		} else {
			description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(nodes.size() - 1) + ", the loop stops." });
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		if (edges.size() < i) {
			action.draw(&edges, 0, edges.size() - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		} else {
			action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		}

		action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.draw(&nodes, i + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		if (i < nodes.size()) {
			action.draw(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		}

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "head/cur", "0/head/cur");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			if (i < labels.size()) {
				action.drawChange(&labels, i, &assets->labelColor, "cur", std::to_string(i) + "/cur");
			}
		}

		// Code
		if (i == 0) {
			action.drawMove(&code, 1, 2);
		} else {
			action.drawMove(&code, 4, 2);
		}

		if (i == nodes.size()) {
			break;
		}

		// New step: if (cur->data == v) return k
		action.addNewStep();

		// Description
		if (nodes.begin()->next(i)->data.value == value) {
			description.addDescription({ "Check if value of 'cur' equals to " + std::to_string(value) + ".", "Since it is true, k is returned." });
		} else {
			description.addDescription({ "Check if value of 'cur' equals to " + std::to_string(value) + ".", "Since it is false, the statement is ignored." });
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.draw(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.draw(&labels.front(), &assets->labelColor, "0/head/cur");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels, i, &assets->labelColor, std::to_string(i) + "/cur");
		}

		// Code
		action.drawMove(&code, 2, 3);

		if (nodes.begin()->next(i)->data.value == value) {
			// New step: Re-layout
			action.addNewStep();

			// Description
			description.addDescription({ "So the value " + std::to_string(value) + " was found at index " + std::to_string(i) + ".", "The whole process is O(n)." });
			action.drawChange(&description, description.size() - 2, description.size() - 1);

			// Edge
			action.drawChange(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor);
			action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

			// Node
			action.drawChange(&nodes, 0, i, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor2, &assets->normalNodeTextColor);
			action.drawFadeOut(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
			action.draw(&nodes, i + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

			// Label
			if (i == 0) {
				action.drawChange(&labels.front(), &assets->labelColor, "0/head/cur", "head");
			} else {
				action.draw(&labels.front(), &assets->labelColor, "head");
				action.drawFadeOut(&labels, i, &assets->labelColor, std::to_string(i) + "/cur");
			}

			// Code
			action.drawFadeOut(&code, 3);

			return;
		}

		// New step: cur = pre->cur
		action.addNewStep();

		// Description
		if (i < nodes.size()) {
			description.addDescription({ "Set 'cur' to the pointer of the next node." });
		} else {
			description.addDescription({ "Set 'cur' to the pointer of the next node", "(NULL)." });
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		if (i < edges.size()) {
			action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
			action.drawSlideIn(&edges, i, &assets->highlightEdgeColor, &assets->blankColor);
		}

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.drawFadeOut(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		if (i + 1 < nodes.size()) {
			action.drawChange(&nodes, i + 1, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
			action.drawFadeIn(&nodes, i + 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		}

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "0/head/cur", "head");
		} else {
			action.drawFadeOut(&labels, i, &assets->labelColor, std::to_string(i) + "/cur");
			action.draw(&labels.front(), &assets->labelColor, "head");
		}

		if (i + 1 < labels.size()) {
			action.drawFadeIn(&labels, i + 1, &assets->labelColor, "cur");
		}

		// Code
		action.drawMove(&code, 3, 4);
	}

	// New step: return NOT_FOUND
	action.addNewStep();

	// Description
	description.addDescription({ "NOT_FOUND is returned." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);

	// Label
	action.draw(&labels.front(), &assets->labelColor, "head");

	// Code
	action.drawMove(&code, 2, 6);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "So the value " + std::to_string(value) + " was not found.", "The whole process is O(n)."});
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, edges.size() - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes, 0, nodes.size() - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor2, &assets->normalNodeTextColor);

	// Label
	action.draw(&labels.front(), &assets->labelColor, "head");

	// Code
	action.drawFadeOut(&code, 6);
}

void DLLVisualizer::update(int index, int value) {
	action.clearAllSteps();

	code.update({
		"Node* upd = head;                 ",
		"for (int k = 0; k <= i - 1; k++) {",
		"    upd = upd->next;              ",
		"upd->data = v;                    ",
		});

	description.newOperation("Update value at index " + std::to_string(index) + " to " + std::to_string(value));

	// New step: Node* upd = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'upd' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes.front(), HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "head", "head/upd");

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i <= index; i++) {
		// New step: for (int k = 0; k <= i - 1; k++)
		action.addNewStep();

		// Description
		if (nodes.size() == 1) {
			description.addDescription({ "Iterate k from 0 to i - 1 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to n - 1 = " + std::to_string(index - 1) + ", k is now 0.", "Since k <= " + std::to_string(index - 1) + ", the loop continues." });
			} else if (i <= index - 1) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(index - 1) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(index - 1) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.draw(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "head/upd", "0/head/upd");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.drawChange(&labels, i, &assets->labelColor, "upd", std::to_string(i) + "/upd");
		}

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		if (i == index) {
			break;
		}

		// New step: pre = pre->next
		action.addNewStep();

		// Description
		description.addDescription({ "Set 'upd' to the pointer of the next node." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.drawSlideIn(&edges, i, &assets->highlightEdgeColor, &assets->blankColor);

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.drawFadeOut(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.drawChange(&nodes, i + 1, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
		action.drawFadeIn(&nodes, i + 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "0/head/upd", "head");
		} else {
			action.drawFadeOut(&labels, i, &assets->labelColor, std::to_string(i) + "/upd");
			action.draw(&labels.front(), &assets->labelColor, "head");
		}

		action.drawFadeIn(&labels, i + 1, &assets->labelColor, "upd");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: upd->data = v
	action.addNewStep();

	// Description
	description.addDescription({ "Set the value of 'upd' to " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.drawUpdate(&nodes, index, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1, nodes.begin()->next(index)->data.value, value);
	action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 0) {
		action.drawChange(&labels.front(), &assets->labelColor, "0/head/upd", "head/upd");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawChange(&labels, index, &assets->labelColor, std::to_string(index) + "/upd", "upd");
	}

	// Code
	action.drawMove(&code, 1, 3);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, index - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes, 0, index, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor2, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes, index, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 0) {
		action.drawChange(&labels.front(), &assets->labelColor, "head/upd", "head");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawFadeOut(&labels, index, &assets->labelColor, "upd");
	}

	// Code
	action.drawFadeOut(&code, 3);
}

void DLLVisualizer::insertAtTheFront(int value, bool head) {
	nodes.pushFront(Node(value, sf::Vector2f(NODE_POSITION_X, NODE_POSITION_Y + 60 + NODE_DISTANCE)));
	labels.pushFront(Label(&nodes.front()));
	if (nodes.size() > 1) {
		edges.pushFront(DoublyEdge(&nodes.begin()->data, &nodes.begin()->next()->data));
	}

	code.update({
		"Node* add = new Node(v);           ",
		"add->next = head;                  ",
		"if (head != NULL) head->prev = add;",
		"head = add;                        "
		});

	if (head) {
		description.newOperation("Insert " + std::to_string(value) + " at the front");
	} else {
		description.newOperation("Insert " + std::to_string(value) + " at the back");
	}

	// New step: Node* add = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'add' that points to a new", "node with value " + std::to_string(value) + "." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawFadeIn(&nodes.front(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.drawFadeIn(&labels.front(), &assets->labelColor, "add");
	if (labels.size() > 1) {
		action.draw(&labels, 1, &assets->labelColor, "head");
	}

	// Code
	action.drawFadeIn(&code, 0);

	// New step: add->next = head
	action.addNewStep();

	// Description
	if (nodes.size() > 1) {
		description.addDescription({ "Set the next pointer of 'add' to 'head'." });
	} else {
		description.addDescription({ "Set the next pointer of 'add' to 'head' (NULL)." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.drawSlideIn(&edges.front(), &assets->insertedEdgeColor, &assets->blankColor);
		action.draw(&edges, 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.draw(&nodes.front(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.draw(&labels.front(), &assets->labelColor, "add");
	if (labels.size() > 1) {
		action.draw(&labels, 1, &assets->labelColor, "head");
	}

	// Code
	action.drawMove(&code, 0, 1);

	// New step: if (head != NULL) head->prev = add
	action.addNewStep();

	// Description
	if (nodes.size() == 1) {
		description.addDescription({ "Check if 'head' is not NULL. Since it is false,", "the statement is ignored." });
	} else {
		description.addDescription({ "Check if 'head' is not NULL. Since it is true,", "set the prev pointer of 'head' to 'add'." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.drawSlideIn(&edges.front(), &assets->blankColor, &assets->insertedEdgeColor);
		action.draw(&edges.front(), &assets->insertedEdgeColor, &assets->blankColor);
		action.draw(&edges, 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.draw(&nodes.front(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.draw(&labels.front(), &assets->labelColor, "add");
	if (labels.size() > 1) {
		action.draw(&labels, 1, &assets->labelColor, "head");
	}

	// Code
	action.drawMove(&code, 1, 2);

	// New step: head = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set 'head' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges.front(), &assets->insertedEdgeColor, &assets->insertedEdgeColor);
		action.draw(&edges, 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.draw(&nodes.front(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "add", "head/add");
	if (labels.size() > 1) {
		action.drawFadeOut(&labels, 1, &assets->labelColor, "head");
	}

	// Code
	action.drawMove(&code, 2, 3);

	// New step: Re-layout
	action.addNewStep();

	description.addDescription({ "Re-layout the list for visualization.", "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.drawChange(&edges.front(), &assets->insertedEdgeColor, &assets->normalEdgeColor, &assets->insertedEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.drawMove(&nodes.front(), HOLLOW, &assets->blankColor, &assets->blankColor, nodes.front().position, nodes.front().position + sf::Vector2f(0, -(60 + NODE_DISTANCE)));
	action.drawChange(&nodes.front(), HOLLOW, &assets->insertedNodeCircleColor, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes.front(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	for (int i = 1; i < nodes.size(); i++) {
		action.drawMove(&nodes, i, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeCircleColor, nodes.begin()->next(i)->data.position, nodes.begin()->next(i)->data.position + sf::Vector2f((60 + NODE_DISTANCE), 0));
	}

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "head/add", "head");

	// Code
	action.drawFadeOut(&code, 3);
}

void DLLVisualizer::insertAtTheBack(int value) {
	nodes.pushBack(Node(value, sf::Vector2f(NODE_POSITION_X + nodes.size() * (60 + NODE_DISTANCE), NODE_POSITION_Y)));
	labels.pushBack(Label(&nodes.back()));
	edges.pushBack(DoublyEdge(&nodes.rbegin()->prev()->data, &nodes.rbegin()->data));
	code.update({
		"Node* pre = head;                ",
		"for (int k = 0; k <= n - 2; k++) ",
		"    pre = pre->next;             ",
		"Node* add = new Node(v);         ",
		"pre->next = add; add->prev = pre;"
		});

	description.newOperation("Insert " + std::to_string(value) + " at the back");

	// New step: Node* pre = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'pre' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes.front(), HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 2, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "head", "head/pre");

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
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, edges.size() - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.draw(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 1, nodes.size() - 2, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "0/head/pre");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.drawChange(&labels, i, &assets->labelColor, "pre", std::to_string(i) + "/pre");
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
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, edges.size() - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.drawSlideIn(&edges, i, &assets->highlightEdgeColor, &assets->blankColor);

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.drawFadeOut(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.drawChange(&nodes, i + 1, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
		action.drawFadeIn(&nodes, i + 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 2, nodes.size() - 2, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head");
		} else {
			action.drawFadeOut(&labels, i, &assets->labelColor, std::to_string(i) + "/pre");
			action.draw(&labels.front(), &assets->labelColor, "head");
		}

		action.drawFadeIn(&labels, i + 1, &assets->labelColor, "pre");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: Node* add = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'add' that points to a new", "node with value " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, nodes.size() - 2, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.back(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (nodes.size() == 2) {
		action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawChange(&labels, labels.size() - 2, &assets->labelColor, std::to_string(labels.size() - 2) + "/pre", "pre");
	}

	action.drawFadeIn(&labels.back(), &assets->labelColor, "add");

	// Code
	action.drawMove(&code, 1, 3);

	// New step: pre->next = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'pre' to 'add' and set", "the prev pointer of 'add' to 'pre'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.drawSlideIn(&edges.back(), &assets->insertedEdgeColor, &assets->insertedEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, nodes.size() - 2, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes.back(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (nodes.size() == 2) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels, labels.size() - 2, &assets->labelColor, "pre");
	}

	action.draw(&labels.back(), &assets->labelColor, "add");

	// Code
	action.drawMove(&code, 3, 4);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, edges.size() - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.drawChange(&edges.back(), &assets->insertedEdgeColor, &assets->normalEdgeColor, &assets->insertedEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes, 0, nodes.size() - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor2, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes, nodes.size() - 2, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawChange(&nodes.back(), HOLLOW, &assets->insertedNodeCircleColor, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes.back(), SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (nodes.size() == 2) {
		action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "head");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawFadeOut(&labels, labels.size() - 2, &assets->labelColor, "pre");
	}

	action.drawFadeOut(&labels.back(), &assets->labelColor, "add");

	// Code
	action.drawFadeOut(&code, 4);
}

void DLLVisualizer::insertAtTheMiddle(int index, int value) {
	nodes.insert(index, Node(value, sf::Vector2f(NODE_POSITION_X + index * (60 + NODE_DISTANCE), NODE_POSITION_Y + 60 + NODE_DISTANCE)));
	labels.insert(index, Label(&nodes.begin()->next(index)->data));
	edges.insert(index, DoublyEdge(&nodes.begin()->next(index)->data, &nodes.begin()->next(index + 1)->data));
	edges.begin()->next(index - 1)->data.right = &nodes.begin()->next(index)->data;
	code.update({
		"Node* pre = head;                ",
		"for (int k = 0; k <= i - 2; k++) ",
		"    pre = pre->next;             ",
		"Node* nxt = pre->next;           ",
		"Node* add = new Node(v);         ",
		"add->next = nxt; nxt->prev = add;",
		"pre->next = add; add->prev = pre;"
		});

	description.newOperation("Insert " + std::to_string(value) + " at index " + std::to_string(index));

	randomNode1.position = nodes.begin()->next(index + 1)->data.position;
	randomLabel1.node = &randomNode1;
	randomEdge1.left = &nodes.begin()->next(index - 1)->data;
	randomEdge1.right = &randomNode1;

	randomNode2.position = nodes.begin()->next(index - 1)->data.position;
	randomLabel2.node = &randomNode2;
	randomEdge2.left = &randomNode2;
	randomEdge2.right = &nodes.begin()->next(index + 1)->data;

	// New step: Node* pre = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'pre' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 1, index - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.drawChange(&nodes.front(), HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "head", "head/pre");

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
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, index - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.draw(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 1, index - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "0/head/pre");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.drawChange(&labels, i, &assets->labelColor, "pre", std::to_string(i) + "/pre");
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
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, index - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.drawSlideIn(&edges, i, &assets->highlightEdgeColor, &assets->blankColor);

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.drawFadeOut(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.drawChange(&nodes, i + 1, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
		action.drawFadeIn(&nodes, i + 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 2, index - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head");
		} else {
			action.drawFadeOut(&labels, i, &assets->labelColor, std::to_string(i) + "/pre");
			action.draw(&labels.front(), &assets->labelColor, "head");
		}

		action.drawFadeIn(&labels, i + 1, &assets->labelColor, "pre");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: Node* nxt = pre->next
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'nxt' that points to the next", "node of 'pre'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.drawSlideIn(&randomEdge1, &assets->highlightEdgeColor, &assets->blankColor);
	action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawChange(&nodes, index + 1, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor2, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes, index + 1, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawChange(&labels, index - 1, &assets->labelColor, std::to_string(index - 1) + "/pre", "pre");
	}

	action.drawFadeIn(&labels, index + 1, &assets->labelColor, "nxt");

	// Code
	action.drawMove(&code, 1, 3);

	// New step: Node* add = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'add' that points to a new", "node with value " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes, index, SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 1, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.drawFadeIn(&labels, index, &assets->labelColor, "add");
	action.draw(&labels, index + 1, &assets->labelColor, "nxt");

	// Code
	action.drawMove(&code, 3, 4);

	// New step: add->next = nxt
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'add' to 'nxt' and set", "the prev pointer of 'nxt' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge:
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->highlightEdgeColor, &assets->blankColor);
	action.drawChange(&randomEdge2, &assets->blankColor, &assets->blankColor, &assets->normalEdgeColor, &assets->highlightEdgeColor);
	action.drawSlideIn(&edges, index, &assets->insertedEdgeColor, &assets->blankColor);
	action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawMove(&randomNode2, HOLLOW, &assets->blankColor, &assets->blankColor, randomNode2.position, randomNode2.position + sf::Vector2f((60 + NODE_DISTANCE), (60 + NODE_DISTANCE)));
	action.draw(&nodes, index, SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 1, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.draw(&labels, index, &assets->labelColor, "add");
	action.draw(&labels, index + 1, &assets->labelColor, "nxt");

	// Code
	action.drawMove(&code, 4, 5);

	// New step: pre->next = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'pre' to 'add' and set", "the prev pointer of 'add' to 'pre'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->highlightEdgeColor, &assets->blankColor);
	action.draw(&randomEdge2, &assets->highlightEdgeColor, &assets->blankColor);
	action.drawSlideIn(&edges, index - 1, &assets->blankColor, &assets->insertedEdgeColor);
	action.draw(&edges, index, &assets->insertedEdgeColor, &assets->highlightEdgeColor);
	action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawMove(&randomNode1, HOLLOW, &assets->blankColor, &assets->blankColor, randomNode1.position, randomNode1.position + sf::Vector2f(0, (60 + NODE_DISTANCE)));
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index, SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 1, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.draw(&labels, index, &assets->labelColor, "add");
	action.draw(&labels, index + 1, &assets->labelColor, "nxt");

	// Code
	action.drawMove(&code, 5, 6);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "Re-layout the list for visualization.", "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.drawChange(&edges, index - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->insertedEdgeColor, &assets->normalEdgeColor);
	action.drawChange(&edges, index, &assets->insertedEdgeColor, &assets->normalEdgeColor, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index + 1, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawMove(&nodes, index, HOLLOW, &assets->blankColor, &assets->blankColor, nodes.begin()->next(index)->data.position, nodes.begin()->next(index)->data.position + sf::Vector2f(0, -(60 + NODE_DISTANCE)));
	action.drawChange(&nodes, 0, index - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor2, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawChange(&nodes, index, HOLLOW, &assets->insertedNodeCircleColor, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes, index, SOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.drawMove(&nodes, index + 1, HOLLOW, &assets->blankColor, &assets->blankColor, nodes.begin()->next(index + 1)->data.position, nodes.begin()->next(index + 1)->data.position + sf::Vector2f((60 + NODE_DISTANCE), 0));
	action.drawChange(&nodes, index + 1, HOLLOW, &assets->highlightNodeCircleColor2, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes, index + 1, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);

	for (int i = index + 2; i < nodes.size(); i++) {
		action.drawMove(&nodes, i, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor, nodes.begin()->next(i)->data.position, nodes.begin()->next(i)->data.position + sf::Vector2f((60 + NODE_DISTANCE), 0));
	}

	// Label
	if (index == 1) {
		action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "head");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawFadeOut(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.drawFadeOut(&labels, index, &assets->labelColor, "add");
	action.drawFadeOut(&labels, index + 1, &assets->labelColor, "nxt");

	// Code
	action.drawFadeOut(&code, 6);
}

void DLLVisualizer::insert(int index, int value, bool head) {
	action.clearAllSteps();

	if (index == 0) {
		insertAtTheFront(value, head);
	} else if (index == nodes.size()) {
		insertAtTheBack(value);
	} else {
		insertAtTheMiddle(index, value);
	}

	maxPosition1 = nodes.size() - 1;
	maxPosition2 = nodes.size() - 2;
}

void DLLVisualizer::eraseAtTheFront(bool head) {
	deletedNode = nodes.popFront();
	deletedEdge = edges.popFront();
	deletedEdge.left = &deletedNode;
	deletedLabel = labels.popFront();
	deletedLabel.node = &deletedNode;
	code.update({
		"Node* del = head;                   ",
		"head = head->next;                  ",
		"if (head != NULL) head->prev = NULL;",
		"delete del;                         "
		});

	if (head) {
		description.newOperation("Erase value at the front");
	} else {
		description.newOperation("Erase value at the back");
	}

	// New step: Node* del = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'del' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	if (deletedEdge.right != nullptr) {
		action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.drawChange(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->erasedNodeCircleColor, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.draw(&nodes, 0, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	action.drawChange(&deletedLabel, &assets->labelColor, "head", "head/del");

	// Code
	action.drawFadeIn(&code, 0);

	// New step: head = head->next
	action.addNewStep();

	// Description
	if (nodes.size() == 0) {
		description.addDescription({ "Set 'head' to the pointer of the next node", "(NULL)." });
	} else {
		description.addDescription({ "Set 'head' to the pointer of the next node." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (deletedEdge.right != nullptr) {
		action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.drawSlideIn(&deletedEdge, &assets->erasedEdgeColor, &assets->blankColor);
		action.drawChange(&deletedEdge, &assets->blankColor, &assets->blankColor, &assets->normalEdgeColor, &assets->erasedEdgeColor);
	}

	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.draw(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.drawChange(&nodes.front(), HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
		action.drawFadeIn(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	action.drawChange(&deletedLabel, &assets->labelColor, "head/del", "del");
	if (labels.size() > 0) {
		action.drawFadeIn(&labels.front(), &assets->labelColor, "head");
	}

	// Code
	action.drawMove(&code, 0, 1);

	// New step: if (head != NULL) head->prev = NULL;
	action.addNewStep();

	// Description
	if (nodes.size() == 1) {
		description.addDescription({ "Check if 'head' is not NULL. Since it is false,", "the statement is ignored." });
	} else {
		description.addDescription({ "Check if 'head' is not NULL. Since it is true,", "set the prev pointer of 'head' to NULL." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (deletedEdge.right != nullptr) {
		action.draw(&deletedEdge, &assets->erasedEdgeColor, &assets->blankColor);
		action.drawSlideOut(&deletedEdge, &assets->blankColor, &assets->erasedEdgeColor);
	}

	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	action.draw(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.draw(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	action.draw(&deletedLabel, &assets->labelColor, "del");
	if (labels.size() > 0) {
		action.draw(&labels.front(), &assets->labelColor, "head");
	}

	// Code
	action.drawMove(&code, 1, 2);

	// New step: delete add
	action.addNewStep();

	// Description
	if (nodes.size() > 0) {
		description.addDescription({ "Erase 'del'." });
	} else {
		description.addDescription({ "Erase 'del'. Now the list is empty." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (deletedEdge.right != nullptr) {
		action.drawSlideOut(&deletedEdge, &assets->erasedEdgeColor, &assets->blankColor);
	}

	if  (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalNodeCircleColor, &assets->normalEdgeColor);
	}

	// Node
	action.drawFadeOut(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.draw(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	action.drawFadeOut(&deletedLabel, &assets->labelColor, "del");
	if (labels.size() > 0) {
		action.draw(&labels.front(), &assets->labelColor, "head");
	}

	// Code
	action.drawMove(&code, 2, 3);

	// New step: Re-layout
	action.addNewStep();

	if (nodes.size() > 0) {
		description.addDescription({ "Re-layout the list for visualization.", "The whole process is O(1)." });
	} else {
		description.addDescription({ "The whole process is O(1)." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	if (nodes.size() > 0) {
		action.drawMove(&nodes.front(), HOLLOW, &assets->blankColor, &assets->blankColor, nodes.front().position, nodes.front().position + sf::Vector2f(-(60 + NODE_DISTANCE), 0));
		action.drawChange(&nodes.front(), HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
		action.drawFadeOut(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		for (int i = 1; i < nodes.size(); i++) {
			action.drawMove(&nodes.begin()->next(i)->data, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor, nodes.begin()->next(i)->data.position, nodes.begin()->next(i)->data.position + sf::Vector2f(-(60 + NODE_DISTANCE), 0));
		}
	}

	// Label
	if (labels.size() > 0) {
		action.draw(&labels.front(), &assets->labelColor, "head");
	}

	// Code
	action.drawFadeOut(&code, 3);
}

void DLLVisualizer::eraseAtTheBack() {
	deletedNode = nodes.popBack();
	deletedEdge = edges.popBack();
	deletedEdge.right = &deletedNode;
	deletedLabel = labels.popBack();
	deletedLabel = &deletedNode;

	code.update({
		"Node* pre = head;               ",
		"for (int k = 0; k <= n - 3; k++)",
		"    pre = pre->next;            ",
		"Node* del = pre->next;          ",
		"delete del; pre->next = NULL;   "
		});

	description.newOperation("Erase value at the back");

	// New step: Node* pre = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'pre' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes.front(), HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.draw(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "head", "head/pre");

	// Code
	action.drawFadeIn(&code, 0);

	for (int i = 0; i <= edges.size(); i++) {
		// New step: for (int k = 0; k <= n - 3; k++)
		action.addNewStep();

		if (nodes.size() == 2) {
			description.addDescription({ "Iterate k from 0 to n - 3 = -1, k is now 0.", "Since k > -1, the loop stops." });
		} else {
			if (i == 0) {
				description.addDescription({ "Iterate k from 0 to n - 3 = " + std::to_string(nodes.size() - 2) + ", k is now 0.", "Since k <= " + std::to_string(nodes.size() - 2) + ", the loop continues." });
			} else if (i <= nodes.size() - 2) {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k <= " + std::to_string(nodes.size() - 2) + ", the loop continues." });
			} else {
				description.addDescription({ "Increase k by 1, k is now " + std::to_string(i) + ".", "Since k > " + std::to_string(nodes.size() - 2) + ", the loop stops." });
			}
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.draw(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		action.draw(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "0/head/pre");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.drawChange(&labels, i, &assets->labelColor, "pre", std::to_string(i) + "/pre");
		}

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		// New step: pre = pre->next
		if (i == edges.size()) {
			break;
		}

		action.addNewStep();

		// Description
		description.addDescription({ "Set 'pre' to the pointer of the next node." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.drawSlideIn(&edges, i, &assets->highlightEdgeColor, &assets->blankColor);
		action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.drawFadeOut(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.drawChange(&nodes, i + 1, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
		action.drawFadeIn(&nodes, i + 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		action.draw(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head");
		} else {
			action.drawFadeOut(&labels, i, &assets->labelColor, std::to_string(i) + "/pre");
			action.draw(&labels.front(), &assets->labelColor, "head");
		}

		action.drawFadeIn(&labels, i + 1, &assets->labelColor, "pre");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: Node* del = pre->next
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'del' that points to the next", "node of 'pre'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.drawSlideIn(&deletedEdge, &assets->erasedEdgeColor, &assets->blankColor);
	action.drawChange(&deletedEdge, &assets->blankColor, &assets->blankColor, &assets->normalEdgeColor, &assets->erasedEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes.back(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawChange(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->erasedNodeCircleColor, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (nodes.size() == 1) {
		action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawChange(&labels.back(), &assets->labelColor, std::to_string(labels.size() - 1) + "/pre", "pre");
	}

	action.drawFadeIn(&deletedLabel, &assets->labelColor, "del");

	// Code
	action.drawMove(&code, 1, 3);

	// New step: delete del; pre->next = NULL
	action.addNewStep();

	// Description
	description.addDescription({ "Erase 'del'.", "Set the next pointer of 'pre' to NULL." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.drawSlideOut(&deletedEdge, &assets->erasedEdgeColor, &assets->erasedEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes.back(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawFadeOut(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (nodes.size() == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.back(), &assets->labelColor, "pre");
	}

	action.drawFadeOut(&deletedLabel, &assets->labelColor, "del");

	// Code
	action.drawMove(&code, 3, 4);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, edges.size() - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes, 0, nodes.size() - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor2, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes.back(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);

	// Label
	if (nodes.size() == 1) {
		action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "head");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawFadeOut(&labels.back(), &assets->labelColor, "pre");
	}

	// Code
	action.drawFadeOut(&code, 4);
}

void DLLVisualizer::eraseAtTheMiddle(int index) {
	deletedNode = nodes.erase(index);
	deletedEdge = edges.erase(index);
	deletedEdge.left = &deletedNode;
	deletedLabel = labels.erase(index);
	deletedLabel.node = &deletedNode;
	edges.begin()->next(index - 1)->data.right = &nodes.begin()->next(index)->data;
	if (index != edges.size()) {
		edges.begin()->next(index)->data.left = &nodes.begin()->next(index)->data;
	}

	code.update({
		"Node* pre = head;                ",
		"for (int k = 0; k <= i - 2; k++) ",
		"    pre = pre->next;             ",
		"Node* del = pre->next;           ",
		"Node* nxt = del->next;           ",
		"pre->next = nxt; nxt->prev = pre;",
		"delete del;                      "
		});

	description.newOperation("Erase value at index " + std::to_string(index));

	randomNode1.position = deletedNode.position;
	randomLabel1.node = &randomNode1;
	randomEdge1.left = &nodes.begin()->next(index - 1)->data;
	randomEdge1.right = &randomNode1;

	randomNode2.position = deletedNode.position;
	randomLabel2.node = &randomNode2;
	randomEdge2.left = &randomNode2;
	randomEdge2.right = &nodes.begin()->next(index)->data;

	randomEdge.left = &nodes.begin()->next(index - 1)->data;
	randomEdge.right = &deletedNode;

	// New step: Node* pre = head
	action.addNewStep();

	description.addDescription({ "Create a pointer 'pre' that points to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes.front(), HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.front(), SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.draw(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	action.drawChange(&labels.front(), &assets->labelColor, "head", "head/pre");

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
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, index - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.draw(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		action.draw(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "0/head/pre");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.drawChange(&labels, i, &assets->labelColor, "pre", std::to_string(i) + "/pre");
		}

		// Code
		if (i == 0) {
			action.drawMove(&code, 0, 1);
		} else {
			action.drawMove(&code, 2, 1);
		}

		// New step: pre = pre->next
		if (i == index - 1) {
			break;
		}

		action.addNewStep();

		// Description
		description.addDescription({ "Set 'pre' to the pointer of the next node." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&edges, 0, i - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, i, index - 2, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.drawSlideIn(&edges, i, &assets->highlightEdgeColor, &assets->blankColor);
		action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
		action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

		// Node
		action.draw(&nodes, 0, i, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
		action.drawFadeOut(&nodes, i, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.drawChange(&nodes, i + 1, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
		action.drawFadeIn(&nodes, i + 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, i + 2, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		action.draw(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == 0) {
			action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head");
		} else {
			action.drawFadeOut(&labels, i, &assets->labelColor, std::to_string(i) + "/pre");
			action.draw(&labels.front(), &assets->labelColor, "head");
		}

		action.drawFadeIn(&labels, i + 1, &assets->labelColor, "pre");

		// Code
		action.drawMove(&code, 1, 2);
	}

	// New step: Node* del = pre->next
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'del' that points to the next", "node of 'pre'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.drawSlideIn(&randomEdge1, &assets->highlightEdgeColor, &assets->blankColor);
	action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawChange(&deletedNode, HOLLOW, &assets->normalNodeCircleColor, &assets->erasedNodeCircleColor, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.drawChange(&labels.front(), &assets->labelColor, "0/head/pre", "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawChange(&labels, index - 1, &assets->labelColor, std::to_string(index - 1) + "/pre", "pre");
	}

	action.drawFadeIn(&deletedLabel, &assets->labelColor, "del");

	// Code
	action.drawMove(&code, 1, 3);

	// New step: Node* nxt = add->next
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'nxt' that points to the next", "node of 'del'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&randomEdge1, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&deletedEdge, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.drawSlideIn(&deletedEdge, &assets->erasedEdgeColor, &assets->blankColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.drawChange(&nodes, index, HOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor2, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes, index, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.draw(&deletedLabel, &assets->labelColor, "del");
	action.drawFadeIn(&labels, index, &assets->labelColor, "nxt");

	// Code
	action.drawMove(&code, 3, 4);

	// New step: pre->next = nxt
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'pre' to 'nxt' and set", "the prev pointer of 'nxt' to 'prev'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.drawChange(&randomEdge2, &assets->blankColor, &assets->blankColor, &assets->normalEdgeColor, &assets->highlightEdgeColor);
	action.draw(&randomEdge1, &assets->highlightEdgeColor, &assets->blankColor);
	action.drawChange(&randomEdge, &assets->blankColor, &assets->blankColor, &assets->normalEdgeColor, &assets->erasedEdgeColor);
	action.draw(&deletedEdge, &assets->erasedEdgeColor, &assets->blankColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawMove(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1, deletedNode.position, deletedNode.position + sf::Vector2f(0, (60 + NODE_DISTANCE)));
	action.drawSlide(&randomNode1, HOLLOW, &assets->blankColor, &assets->blankColor, &deletedNode, &nodes.begin()->next(index)->data);
	action.drawSlide(&randomNode2, HOLLOW, &assets->blankColor, &assets->blankColor, &deletedNode, &nodes.begin()->next(index - 1)->data);
	action.draw(&nodes, index, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.draw(&deletedLabel, &assets->labelColor, "del");
	action.draw(&labels, index, &assets->labelColor, "nxt");

	// Code
	action.drawMove(&code, 4, 5);

	// New step: delete del
	action.addNewStep();

	// Description
	description.addDescription({ "Erase 'del'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index - 1, &assets->highlightEdgeColor, &assets->highlightEdgeColor);
	action.drawSlideOut(&randomEdge, &assets->blankColor, &assets->erasedEdgeColor);
	action.drawSlideOut(&deletedEdge, &assets->erasedEdgeColor, &assets->blankColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor2);
	action.draw(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawFadeOut(&deletedNode, SOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	action.draw(&nodes, index + 1, nodes.size() - 1, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (index == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/pre");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.drawFadeOut(&deletedLabel, &assets->labelColor, "del");
	action.draw(&labels, index, &assets->labelColor, "nxt");

	// Code
	action.drawMove(&code, 5, 6);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "Re-layout the list for visualization.", "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.drawChange(&edges, 0, index - 2, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor, &assets->normalEdgeColor);
	action.drawChange(&edges, index - 1, &assets->highlightEdgeColor, &assets->normalEdgeColor, &assets->highlightEdgeColor, &assets->normalEdgeColor);
	action.draw(&edges, index, edges.size() - 1, &assets->normalEdgeColor, &assets->normalEdgeColor);

	// Node
	action.drawChange(&nodes, 0, index - 2, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor2, &assets->normalNodeTextColor);
	action.drawChange(&nodes, index - 1, HOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes, index - 1, SOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawMove(&nodes, index, HOLLOW, &assets->blankColor, &assets->blankColor, nodes.begin()->next(index)->data.position, nodes.begin()->next(index)->data.position + sf::Vector2f(-(60 + NODE_DISTANCE), 0));
	action.drawChange(&nodes, index, HOLLOW, &assets->highlightNodeCircleColor2, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes, index, SOLID, &assets->highlightNodeCircleColor2, &assets->highlightNodeTextColor1);
	for (int i = index + 1; i < nodes.size(); i++) {
		action.drawMove(&nodes, i, HOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor, nodes.begin()->next(i)->data.position, nodes.begin()->next(i)->data.position + sf::Vector2f(-(60 + NODE_DISTANCE), 0));
	}

	// Label
	if (index == 1) {
		action.drawChange(&labels.front(), &assets->labelColor, "head/pre", "head");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawFadeOut(&labels, index - 1, &assets->labelColor, "pre");
	}

	action.drawFadeOut(&labels, index, &assets->labelColor, "nxt");

	// Code
	action.drawFadeOut(&code, 6);
}

void DLLVisualizer::erase(int index, bool head) {
	action.clearAllSteps();

	if (index == 0) {
		eraseAtTheFront(head);
	} else if (index == nodes.size() - 1) {
		eraseAtTheBack(); 
	} else {
		eraseAtTheMiddle(index);
	}

	maxPosition1 = nodes.size() - 1;
	maxPosition2 = nodes.size() - 2;
}

void DLLVisualizer::run() {
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
		option.draw(window);

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
					search(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 2: // Update
				switch (std::get <1> (current)) {
				case 0: // 
					update(std::stoi(std::get <2> (current)[0]), std::stoi(std::get <2> (current)[1]));
					break;
				}

				break;

			case 3: // Insert
				switch (std::get <1> (current)) {
				case 0: // Front
					insert(0, std::stoi(std::get <2> (current)[0]));
					break;

				case 1: // Back
					insert(nodes.size(), std::stoi(std::get <2>(current)[0]), false);
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
					erase(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;
			}
		}
	}
}