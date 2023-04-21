#ifndef CIRCULAR_EDGE_H
#define CIRCULAR_EDGE_H

#include "Global.h"
#include "Node.h"

struct CircularEdge {
	Node* left;
	Node* right;

	CircularEdge(Node* left, Node* right);
};

#endif