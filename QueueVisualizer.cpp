#include "QueueVisualizer.h"
#include "Random.h"

QueueVisualizer::QueueVisualizer(sf::RenderWindow* window, Assets *assets) {
	this->window = window;
	this->assets = assets;

	header = HeaderBox(assets, "Queue", sf::Vector2f(10, 10));
	action = ActionBox(window, assets, sf::Vector2f(10, 430));
	option = OptionBox(assets, sf::Vector2f(10, 510));
	code = CodeBox(assets, sf::Vector2f(910, 590));
	description = DescriptionBox(assets, sf::Vector2f(910, 430));
}

void QueueVisualizer::randomQueue(int size) {
	nodes.clear();
	for (int i = 0; i < size; i++) {
		nodes.pushBack(Node(randInt(0, 99), sf::Vector2f()));
	}
}

void QueueVisualizer::manualQueue(std::string listOfValues) {
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

void QueueVisualizer::create() {
	action.clearAllSteps();

	labels.clear();
	edges.clear();
	code.update({});
	if (nodes.empty()) {
		description.newOperation("Create an empty queue");
	} else {
		description.newOperation("Create a queue of size " + std::to_string(nodes.size()));
	}

	if (!nodes.empty()) {
		nodes.front().position = sf::Vector2f(NODE_POSITION_X, NODE_POSITION_Y);
		labels.pushBack(Label(&nodes.front()));
		for (ListNode <Node>* iterator = nodes.begin()->next(); iterator != nodes.end(); iterator = iterator->next()) {
			iterator->data.position.x = iterator->prev()->data.position.x + (60 + NODE_DISTANCE);
			iterator->data.position.y = iterator ->prev()->data.position.y;
			labels.pushBack(Label(&iterator->data));
			edges.pushBack(Edge(&iterator->prev()->data, &iterator->data));
		}
	}

	if (!nodes.empty()) {
		// New step: Create all nodes
		action.addNewStep();

		// Description
		if (nodes.size() == 1) {
			description.addDescription({ "Create only 1 node. Both 'head' and 'tail'", "point to that node." });
		} else {
			description.addDescription({ "Create " + std::to_string(nodes.size()) + " nodes. 'head' points to the first", "node and 'tail' points to the last node." });
		}

		action.drawFadeIn(&description, description.size() - 1);

		// Edge

		// Node
		action.drawFadeIn(&nodes, 0, nodes.size() - 1, CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

		// Label
		if (labels.size() == 1) {
			action.drawFadeIn(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.drawFadeIn(&labels.front(), &assets->labelColor, "head");
			action.drawFadeIn(&labels.back(), &assets->labelColor, "tail");
		}

		// Code
		action.draw(&code);
	}

	if (!edges.empty()) {
		// New step: Create all edges
		action.addNewStep();

		// Description
		description.addDescription({ "Create next pointers for every nodes.", "Next pointer of 'tail' points to NULL."});

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.drawSlideIn(&edges, 0, edges.size() - 1, &assets->insertedEdgeColor);

		// Node
		action.draw(&nodes, 0, nodes.size() - 1, CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

		// Label
		if (labels.size() == 1) {
			action.draw(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.back(), &assets->labelColor, "tail");
		}

		// Code
		action.draw(&code);
	}

	// New step: Re-layout
	action.addNewStep();

	// Description
	if (nodes.empty()) {
		description.addDescription({ "An empty queue is created." });
		action.drawFadeIn(&description, description.size() - 1);
	} else {
		description.addDescription({ "A queue of size " + std::to_string(nodes.size()) + " is created." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);
	}

	// Edge
	if (!edges.empty()) {
		action.drawChange(&edges, 0, edges.size() - 1, &assets->insertedEdgeColor, &assets->normalEdgeColor);
	}

	// Node
	if (!nodes.empty()) {
		action.drawChange(&nodes, 0, nodes.size() - 1, CHOLLOW, &assets->insertedNodeCircleColor, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
		action.drawFadeOut(&nodes, 0, nodes.size() - 1, CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	}

	// Label
	if (labels.size() > 0) {
		if (labels.size() == 1) {
			action.draw(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.back(), &assets->labelColor, "tail");
		}
	}

	// Code
	action.draw(&code);
}

void QueueVisualizer::peekAtTheFront() {
	action.clearAllSteps();

	code.update({
		"if (n == 0) return NOTHING;",
		"return head->data;"
		});

	description.newOperation("Peek front");

	// New step: if (n == 0) return NOTHING
	action.addNewStep();

	if (nodes.size() == 0) {
		description.addDescription({ "Check if the stack is empty.", "Since n = 0, NOT_FOUND is returned." });
	} else {
		description.addDescription({ "Check if the stack is empty.", "Since n > 0, the statement is ignored." });
	}

	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	if (nodes.size() > 0) {
		action.draw(&nodes, 0, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (labels.size() > 0) {
		if (labels.size() == 1) {
			action.draw(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.back(), &assets->labelColor, "tail");
		}
	}

	// Code
	action.drawFadeIn(&code, 0);

	if (nodes.size() == 0) {
		// New step: Re-layout
		action.addNewStep();

		// Description
		description.addDescription({ "So there is no value.", "The whole process is O(1)."});
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge

		// Node

		// Label

		// Code
		action.drawFadeOut(&code, 0);

		return;
	}

	// New step: return head->data
	action.addNewStep();

	description.addDescription({ "Return the value of 'head', which is " + std::to_string(nodes.front().value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	action.drawChange(&nodes.front(), CHOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.front(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (labels.size() == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/tail");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.back(), &assets->labelColor, "tail");
	}

	// Code
	action.drawMove(&code, 0, 1);

	// New step: Re-layout
	action.addNewStep();

	description.addDescription({ "So the value at the front is " + std::to_string(nodes.front().value) + ".", "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	action.drawChange(&nodes.front(), CHOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes.front(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 1, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (labels.size() == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/tail");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.back(), &assets->labelColor, "tail");
	}

	// Code
	action.drawFadeOut(&code, 1);
}

void QueueVisualizer::peekAtTheBack() {
	action.clearAllSteps();

	code.update({
		"if (n == 0) return NOTHING;",
		"return tail->data;"
		});

	description.newOperation("Peek back");

	// New step: if (n == 0) return NOTHING
	action.addNewStep();

	if (nodes.size() == 0) {
		description.addDescription({ "Check if the stack is empty.", "Since n = 0, NOT_FOUND is returned." });
	} else {
		description.addDescription({ "Check if the stack is empty.", "Since n > 0, the statement is ignored." });
	}

	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	if (nodes.size() > 0) {
		action.draw(&nodes, 0, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (labels.size() > 0) {
		if (labels.size() == 1) {
			action.draw(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.back(), &assets->labelColor, "tail");
		}
	}

	// Code
	action.drawFadeIn(&code, 0);

	if (nodes.size() == 0) {
		// New step: Re-layout
		action.addNewStep();

		// Description
		description.addDescription({ "So there is no value.", "The whole process is O(1)."});
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge

		// Node

		// Label

		// Code
		action.drawFadeOut(&code, 0);

		return;
	}

	// New step: return tail->data
	action.addNewStep();

	description.addDescription({ "Return the value of 'tail', which is " + std::to_string(nodes.back().value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	action.drawChange(&nodes.back(), CHOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.back(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 0, nodes.size() - 2, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (labels.size() == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/tail");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.back(), &assets->labelColor, "tail");
	}

	// Code
	action.drawMove(&code, 0, 1);

	// New step: Re-layout
	action.addNewStep();

	description.addDescription({ "So the value at the back is " + std::to_string(nodes.back().value) + ".", "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	action.drawChange(&nodes.back(), CHOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes.back(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes, 0, nodes.size() - 2, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

	// Label
	if (labels.size() == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/tail");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.back(), &assets->labelColor, "tail");
	}

	// Code
	action.drawFadeOut(&code, 1);
}

void QueueVisualizer::enqueue(int value) {
	action.clearAllSteps();

	nodes.pushBack(Node(value, sf::Vector2f(NODE_POSITION_X + nodes.size() * (60 + NODE_DISTANCE), NODE_POSITION_Y)));
	labels.pushBack(Label(&nodes.back()));
	if (nodes.size() > 1) {
		edges.pushBack(Edge(&nodes.rbegin()->prev()->data, &nodes.rbegin()->data));
	}

	code.update({
	"if (n == 0) head = tail = new Node(v);",
	"else {                                ",
	"    Node* add = new Node(v);          ",
	"    tail->next = add;                 ",
	"    tail = add;                       ",
	"}                                     "
		});

	description.newOperation("Enqueue " + std::to_string(value));

	// New step: if (n == 0) head = tail = new Node(v)
	action.addNewStep();

	// Description
	if (nodes.size() == 1) {
		description.addDescription({ "Check if the queue is empty. Since n = 0, both", "'head' and 'tail' point to a new node." });
	} else {
		description.addDescription({ "Check if the queue is empty. Since n > 0, the", "statement is ignored." });
	}

	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->normalEdgeColor);

	// Node
	if (nodes.size() == 1) {
		action.drawFadeIn(&nodes.back(), CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);
	} else {
		action.draw(&nodes, 0, nodes.size() - 2, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (labels.size() == 1) {
			action.drawFadeIn(&labels.back(), &assets->labelColor, "head/tail");
	} else {
		if (labels.size() == 2) {
			action.draw(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.rbegin()->prev()->data, &assets->labelColor, "tail");
		}
	}

	// Code
	action.drawFadeIn(&code, 0);

	if (nodes.size() == 1) {
		// New step: Re-layout
		action.addNewStep();

		// Description
		description.addDescription({ "The whole process is O(1)." });
		
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge

		// Node
		action.drawChange(&nodes.back(), CHOLLOW, &assets->insertedNodeCircleColor, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
		action.drawFadeOut(&nodes.back(), CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

		// Label
		action.draw(&labels.back(), &assets->labelColor, "head/tail");

		// Code
		action.drawFadeOut(&code, 0);

		return;
	}

	// New step: if (n == 0) head = tail = new Node(v)
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'add' that points to a new", "node with value " + std::to_string(value) + "." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 2, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.drawFadeIn(&nodes.back(), CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (labels.size() == 2) {
		action.draw(&labels.front(), &assets->labelColor, "head/tail");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.rbegin()->prev()->data, &assets->labelColor, "tail");
	}

	action.drawFadeIn(&labels.back(), &assets->labelColor, "add");

	// Code
	action.drawMove(&code, 0, 2);

	// New step: tail->next = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set the next pointer of 'tail' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->normalEdgeColor);
	action.drawSlideIn(&edges.back(), &assets->insertedEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.drawChange(&nodes.rbegin()->prev()->data, CHOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&nodes.rbegin()->prev()->data, CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes.back(), CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (labels.size() == 2) {
		action.draw(&labels.front(), &assets->labelColor, "head/tail");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.rbegin()->prev()->data, &assets->labelColor, "tail");
	}

	action.draw(&labels.back(), &assets->labelColor, "add");

	// Code
	action.drawMove(&code, 2, 3);

	// New step: tail = add

	// New step: tail->next = add
	action.addNewStep();

	// Description
	description.addDescription({ "Set 'tail' to 'add'." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->normalEdgeColor);
	action.draw(&edges.back(), &assets->insertedEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.draw(&nodes.rbegin()->prev()->data, CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.draw(&nodes.back(), CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	if (labels.size() == 2) {
		action.drawChange(&labels.front(), &assets->labelColor, "head/tail", "head");
	} else {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.drawFadeOut(&labels.rbegin()->prev()->data, &assets->labelColor, "tail");
	}

	action.drawChange(&labels.back(), &assets->labelColor, "add", "tail/add");

	// Code
	action.drawMove(&code, 3, 4);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(1)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	action.draw(&edges, 0, edges.size() - 2, &assets->normalEdgeColor);
	action.drawChange(&edges.back(), &assets->insertedEdgeColor, &assets->normalEdgeColor);

	// Node
	action.draw(&nodes, 0, nodes.size() - 3, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	action.drawChange(&nodes.rbegin()->prev()->data, CHOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes.rbegin()->prev()->data, CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
	action.drawChange(&nodes.back(), CHOLLOW, &assets->insertedNodeCircleColor, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
	action.drawFadeOut(&nodes.back(), CSOLID, &assets->insertedNodeCircleColor, &assets->highlightNodeTextColor1);

	// Label
	action.draw(&labels.front(), &assets->labelColor, "head");
	action.drawChange(&labels.back(), &assets->labelColor, "tail/add", "tail");

	// Code
	action.drawFadeOut(&code, 4);
}

void QueueVisualizer::dequeue() {
	action.clearAllSteps();

	deletedNode = nodes.popFront();
	deletedEdge = edges.popFront();
	deletedEdge.left = &deletedNode;
	deletedLabel = labels.popFront();
	deletedLabel.node = &deletedNode;
	code.update({
		"Node* del = head;             ",
		"head = head->next;            ",
		"if (head == NULL) tail = NULL;",
		"delete del;                   "
		});

	description.newOperation("Dequeue");

	// New step: Node* del = head
	action.addNewStep();

	// Description
	description.addDescription({ "Create a pointer 'del' and set it to 'head'." });
	action.drawFadeIn(&description, description.size() - 1);

	// Edge
	if (deletedEdge.right != nullptr) {
		action.draw(&deletedEdge, &assets->normalEdgeColor);
	}

	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	action.drawChange(&deletedNode, CHOLLOW, &assets->normalNodeCircleColor, &assets->erasedNodeCircleColor, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
	action.drawFadeIn(&deletedNode, CSOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.draw(&nodes, 0, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (nodes.size() == 0) {
		action.drawChange(&deletedLabel, &assets->labelColor, "head/tail", "head/tail/del");
	} else {
		action.drawChange(&deletedLabel, &assets->labelColor, "head", "head/del");
		action.draw(&labels.back(), &assets->labelColor, "tail");
	}

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
		action.draw(&deletedEdge, &assets->normalEdgeColor);
		action.drawSlideIn(&deletedEdge, &assets->erasedEdgeColor);
	}

	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	action.draw(&deletedNode, CSOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.drawChange(&nodes.front(), CHOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
		action.drawFadeIn(&nodes.front(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, 1, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (labels.size() == 0) {
		action.drawChange(&deletedLabel, &assets->labelColor, "head/tail/del", "tail/del");
	} else {
		action.drawChange(&deletedLabel, &assets->labelColor, "head/del", "del");
		if (labels.size() == 1) {
			action.drawChange(&labels.front(), &assets->labelColor, "tail", "head/tail");
		} else {
			action.drawFadeIn(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.back(), &assets->labelColor, "tail");
		}
	}

	// Code
	action.drawMove(&code, 0, 1);

	// New step: if (head == NULL) tail = NULL
	action.addNewStep();

	// Description
	description.addDescription({ "If 'head' is NULL then 'tail' is also NULL." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (deletedEdge.right != nullptr) {
		action.draw(&deletedEdge, &assets->erasedEdgeColor);
	}

	if  (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalNodeCircleColor);
	}

	// Node
	action.draw(&deletedNode, CSOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.draw(&nodes.front(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, 1, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (labels.size() == 0) {
		action.drawChange(&deletedLabel, &assets->labelColor, "tail/del", "del");
	} else {
		action.draw(&deletedLabel, &assets->labelColor, "del");
		if (labels.size() == 1) {
			action.draw(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.back(), &assets->labelColor, "tail");
		}
	}

	// Code
	action.drawMove(&code, 1, 2);

	// New step: deleted del
	action.addNewStep();

	// Description
	if (nodes.size() > 0) {
		description.addDescription({ "Erase 'del'." });
	} else {
		description.addDescription({ "Erase 'del'. Now the queue is empty." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (deletedEdge.right != nullptr) {
		action.drawSlideOut(&deletedEdge, &assets->erasedEdgeColor);
	}

	if  (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalNodeCircleColor);
	}

	// Node
	action.drawFadeOut(&deletedNode, CSOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
	if (nodes.size() > 0) {
		action.draw(&nodes.front(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		action.draw(&nodes, 1, nodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
	}

	// Label
	if (labels.size() == 0) {
		action.drawFadeOut(&deletedLabel, &assets->labelColor, "del");
	} else {
		action.drawFadeOut(&deletedLabel, &assets->labelColor, "del");
		if (labels.size() == 1) {
			action.draw(&labels.front(), &assets->labelColor, "head/tail");
		} else {
			action.draw(&labels.front(), &assets->labelColor, "head");
			action.draw(&labels.back(), &assets->labelColor, "tail");
		}
	}

	// Code
	action.drawMove(&code, 2, 3);

	// New step: Re-layout
	action.addNewStep();

	if (nodes.size() > 0) {
		description.addDescription({ "Re-layout the queue for visualization.", "The whole process is O(1)." });
	} else {
		description.addDescription({ "The whole process is O(1)." });
	}

	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge
	if (edges.size() > 0) {
		action.draw(&edges, 0, edges.size() - 1, &assets->normalEdgeColor);
	}

	// Node
	if (nodes.size() > 0) {
		action.drawMove(&nodes.front(), CHOLLOW, &assets->blankColor, &assets->blankColor, nodes.front().position, nodes.front().position + sf::Vector2f(-60 - NODE_DISTANCE, 0));
		action.drawChange(&nodes.front(), CHOLLOW, &assets->highlightNodeCircleColor1, &assets->normalNodeCircleColor, &assets->highlightNodeTextColor1, &assets->normalNodeTextColor);
		action.drawFadeOut(&nodes.front(), CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
		for (int i = 1; i < nodes.size(); i++) {
			action.drawMove(&nodes.begin()->next(i)->data, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor, nodes.begin()->next(i)->data.position, nodes.begin()->next(i)->data.position + sf::Vector2f(-60 - NODE_DISTANCE, 0));
		}
	}

	// Label
	if (labels.size() == 1) {
		action.draw(&labels.front(), &assets->labelColor, "head/tail");
	} else if (labels.size() >= 2) {
		action.draw(&labels.front(), &assets->labelColor, "head");
		action.draw(&labels.back(), &assets->labelColor, "tail");
	}

	// Code
	action.drawFadeOut(&code, 3);
}

void QueueVisualizer::clear() {
	action.clearAllSteps();

	deletedNodes.clear();
	deletedLabels.clear();
	deletedEdges.clear();
	while (!nodes.empty()) {
		deletedNodes.pushBack(nodes.popFront());
		deletedLabels.pushBack(&deletedNodes.back());
		if (deletedNodes.size() >= 2) {
			deletedEdges.pushBack(Edge(&deletedNodes.rbegin()->prev()->data, &deletedNodes.back()));
		}
	}

	labels.clear();
	edges.clear();

	code.update({
		"while (head != NULL) {",
		"    Node* del = head; ",
		"    head = head->next;",
		"    delete del;       ",
		"}                     ",
		"tail = NULL;          "
		});

	description.newOperation("Clear");

	for (int i = 0; i <= deletedNodes.size(); i++) {
		// New step: while (head != NULL)
		action.addNewStep();

		// Description
		if (i < deletedNodes.size()) {
			description.addDescription({ "Check if 'head' is not equal to NULL.", "Since it is true, the loop continues." });
		} else {
			description.addDescription({ "Check if 'head' is not equal to NULL.", "Since it is false, the loop stops." });
		}

		if (i == 0) {
			action.drawFadeIn(&description, description.size() - 1);
		} else {
			action.drawChange(&description, description.size() - 2, description.size() - 1);
		}

		// Edge
		action.draw(&deletedEdges, i, deletedEdges.size() - 1, &assets->normalEdgeColor);

		// Node
		if (i < deletedNodes.size()) {
			if (i == 0) {
				action.drawChange(&deletedNodes, i, CHOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
				action.drawFadeIn(&deletedNodes, i, CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
			} else {
				action.draw(&deletedNodes, i, CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
			}

			action.draw(&deletedNodes, i + 1, deletedNodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		}

		if (i > 0) {
			for (int j = i; j < deletedNodes.size(); j++) {
				action.drawMove(&deletedNodes, j, CHOLLOW, &assets->blankColor, &assets->blankColor, sf::Vector2f(NODE_POSITION_X + (j - i + 1) * (60 + NODE_DISTANCE), NODE_POSITION_Y), sf::Vector2f(NODE_POSITION_X + (j - i) * (60 + NODE_DISTANCE), NODE_POSITION_Y));
			}
		}

		// Label
		if (i < deletedNodes.size()) {
			if (i == deletedNodes.size() - 1) {
				action.draw(&deletedLabels, i, &assets->labelColor, "head/tail");
			} else {
				action.draw(&deletedLabels, i, &assets->labelColor, "head");
				action.draw(&deletedLabels.back(), &assets->labelColor, "tail");
			}
		}

		// Code
		if (i == 0) {
			action.drawFadeIn(&code, 0);
		} else {
			action.drawMove(&code, 3, 0);
		}

		if (i == deletedNodes.size()) {
			break;
		}

		// New step: Node* del = head
		action.addNewStep();

		// Description
		description.addDescription({ "Create a pointer 'del' and set it to 'head'." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		action.draw(&deletedEdges, i, deletedEdges.size() - 1, &assets->normalEdgeColor);

		// Node
		action.drawChange(&deletedNodes, i, CSOLID, &assets->highlightNodeCircleColor1, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1, &assets->highlightNodeTextColor1);
		action.draw(&deletedNodes, i + 1, deletedNodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);

		// Label
		if (i == deletedNodes.size() - 1) {
			action.drawChange(&deletedLabels, i, &assets->labelColor, "head/tail", "head/tail/del");
		} else {
			action.drawChange(&deletedLabels, i, &assets->labelColor, "head", "head/del");
			action.draw(&deletedLabels.back(), &assets->labelColor, "tail");
		}

		// Code
		action.drawMove(&code, 0, 1);

		// New step: head = head->next
		action.addNewStep();

		// Description
		if (i == deletedNodes.size() - 1) {
			description.addDescription({ "Set 'head' to the pointer of the next node", "(NULL)." });
		} else {
			description.addDescription({ "Set 'head' to the pointer of the next node." });
		}

		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		if (i < deletedEdges.size()) {
			action.draw(&deletedEdges, i, deletedEdges.size() - 1, &assets->normalEdgeColor);
			action.drawSlideIn(&deletedEdges, i, &assets->erasedEdgeColor);
		}

		// Node
		action.draw(&deletedNodes, i, CSOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
		if (i + 1 < deletedNodes.size()) {
			action.drawChange(&deletedNodes, i + 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->highlightNodeCircleColor1, &assets->normalNodeTextColor, &assets->highlightNodeTextColor1);
			action.drawFadeIn(&deletedNodes, i + 1, CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
			action.draw(&deletedNodes, i + 2, deletedNodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		}

		// Label
		if (i == deletedNodes.size() - 1) {
			action.drawChange(&deletedLabels, i, &assets->labelColor, "head/tail/del", "tail/del");
		} else {
			action.drawChange(&deletedLabels, i, &assets->labelColor, "head/del", "del");
			if (i + 1 < deletedLabels.size()) {
				if (i + 1 == deletedLabels.size() - 1) {
					action.drawChange(&deletedLabels.back(), &assets->labelColor, "tail", "head/tail");
				} else {
					action.drawFadeIn(&deletedLabels, i + 1, &assets->labelColor, "head");
					action.draw(&deletedLabels.back(), &assets->labelColor, "tail");
				}
			}
		}

		// Code
		action.drawMove(&code, 1, 2);

		// New step: deleted del
		action.addNewStep();

		// Description
		description.addDescription({ "Erase 'del'." });
		action.drawChange(&description, description.size() - 2, description.size() - 1);

		// Edge
		if (i < deletedEdges.size()) {
			action.draw(&deletedEdges, i + 1, deletedEdges.size() - 1, &assets->normalEdgeColor);
			action.drawSlideOut(&deletedEdges, i, &assets->erasedEdgeColor);
		}

		// Node
		action.drawFadeOut(&deletedNodes, i, CSOLID, &assets->erasedNodeCircleColor, &assets->highlightNodeTextColor1);
		if (i + 1 < deletedNodes.size()) {
			action.draw(&deletedNodes, i + 1, CSOLID, &assets->highlightNodeCircleColor1, &assets->highlightNodeTextColor1);
			action.draw(&deletedNodes, i + 2, deletedNodes.size() - 1, CHOLLOW, &assets->normalNodeCircleColor, &assets->normalNodeTextColor);
		}

		// Label
		if (i == deletedNodes.size() - 1) {
			action.drawFadeOut(&deletedLabels, i, &assets->labelColor, "tail/del");
		} else {
			action.drawFadeOut(&deletedLabels, i, &assets->labelColor, "del");
			if (i + 1 < deletedLabels.size()) {
				if (i + 1 == deletedLabels.size() - 1) {
					action.draw(&deletedLabels.back(), &assets->labelColor, "head/tail");
				} else {
					action.draw(&deletedLabels, i + 1, &assets->labelColor, "head");
					action.draw(&deletedLabels.back(), &assets->labelColor, "tail");
				}
			}
		}

		// Code
		action.drawMove(&code, 2, 3);
	}

	// New step: tail = NULL
	action.addNewStep();

	// Description
	description.addDescription({ "Set 'tail' to NULL." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge

	// Node

	// Label

	// Code
	action.drawMove(&code, 0, 5);

	// New step: Re-layout
	action.addNewStep();

	// Description
	description.addDescription({ "The whole process is O(n)." });
	action.drawChange(&description, description.size() - 2, description.size() - 1);

	// Edge

	// Node

	// Label

	// Code
	action.drawFadeOut(&code, 5);
}

void QueueVisualizer::run() {
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

	option.addOption("Create");
	option.addSuboption("Empty", conditionNone);
	option.addSuboption("Random", conditionNone);
	option.addSuboptionInput("n", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize), std::bind(generatorInteger, &minSize, &maxSize));
	option.addSuboption("Manual", conditionNone);
	option.addSuboptionInput("v[]", numbersCommaSpaceCharacter, std::bind(validatorListOfIntegers, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize, &minValue, &maxValue), std::bind(generatorListOfIntegers, &minSize, &maxSize, &minValue, &maxValue));
	option.addSuboptionInput("File browser", numbersCommaSpaceCharacter, std::bind(validatorListOfIntegers, std::placeholders::_1, std::placeholders::_2, &minSize, &maxSize, &minValue, &maxValue), generatorNone);

	option.addOption("Peek");
	option.addSuboption("Front", conditionNone);
	option.addSuboption("Back", conditionNone);

	option.addOption("Enqueue");
	option.addSuboption("", conditionListNotLarge);
	option.addSuboptionInput("v", numbersCharacter, std::bind(validatorInteger, std::placeholders::_1, std::placeholders::_2, &minValue, &maxValue), std::bind(generatorInteger, &minValue, &maxValue));

	option.addOption("Dequeue");
	option.addSuboption("", conditionListNotEmpty);

	option.addOption("Clear");
	option.addSuboption("", conditionListNotEmpty);

	randomQueue(7);
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
					randomQueue(0);
					break;

				case 1: // Random
					randomQueue(std::stoi(std::get <2> (current)[0]));
					break;

				case 2: // Manual
					manualQueue(std::get <2> (current)[0]);
				}

				create();
				break;

			case 1: // Peek
				switch (std::get <1> (current)) {
				case 0: // Front
					peekAtTheFront();
					break;

				case 1: // Back
					peekAtTheBack();
					break;
				}

				break;

			case 2: // Enqueue
				switch (std::get <1> (current)) {
				case 0: //
					enqueue(std::stoi(std::get <2> (current)[0]));
					break;
				}

				break;

			case 3: // Dequeue
				switch (std::get <1> (current)) {
				case 0: //
					dequeue();
					break;
				}

				break;

			case 4: // Clear
				switch (std::get <1> (current)) {
				case 0: //
					clear();
					break;
				}

				break;
			}
		}
	}
}