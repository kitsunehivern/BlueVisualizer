#include "GraphVisualizer.hpp"

GraphVisualizer::GraphVisualizer() {
}

GraphVisualizer::GraphVisualizer(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
    mNodes = std::vector<GraphicNode>();
    mAdjacencyMatrix = std::vector<std::vector<int>>();
    mSelectedNode = std::make_pair(-1, sf::Vector2f());
}

sf::Vector2f GraphVisualizer::springForce(sf::Vector2f fixedPosition, sf::Vector2f movingPosition, float idealLength) {
    sf::Vector2f direction = movingPosition - fixedPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance == 0) {
        return sf::Vector2f(0, 0);
    }

    float force = GraphVisualizerData::springConstant * (distance - idealLength);

    return sf::Vector2f(force * direction.x / distance, force * direction.y / distance);
}

sf::Vector2f GraphVisualizer::rubberbandForce(sf::Vector2f fixedPosition, sf::Vector2f movingPosition) {
    sf::Vector2f direction = movingPosition - fixedPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance == 0) {
        return sf::Vector2f(0, 0);
    }

    float force = GraphVisualizerData::springConstant * distance;

    return sf::Vector2f(force * direction.x / distance, force * direction.y / distance);
}

void GraphVisualizer::updateGraphState() {
    std::vector<sf::Vector2f> totalForce(mNodes.size(), sf::Vector2f(0, 0));

    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                sf::Vector2f force = springForce(mNodes[i].getCenter(), mNodes[j].getCenter(), GraphVisualizerData::idealLength);
                totalForce[i] += force;
                totalForce[j] -= force;
            } else {
                float distance = std::sqrt(std::pow(mNodes[i].getCenter().x - mNodes[j].getCenter().x, 2) + std::pow(mNodes[i].getCenter().y - mNodes[j].getCenter().y, 2));
                if (distance < GraphVisualizerData::idealLength) {
                    sf::Vector2f force = springForce(mNodes[i].getCenter(), mNodes[j].getCenter(), GraphVisualizerData::idealLength);
                    totalForce[i] += force;
                    totalForce[j] -= force;
                }
            }
        }
    }

    for (int i = 0; i < (int)mNodes.size() - 1; i++) {
        for (int j = i + 1; j < (int)mNodes.size(); j++) {
            if (!mAdjacencyMatrix[i][j]) {
                continue;
            }

            sf::Vector2f midpoint = (mNodes[i].getCenter() + mNodes[j].getCenter()) / 2.f;
            for (int k = 0; k < (int)mNodes.size(); k++) {
                float distance = std::sqrt(std::pow(midpoint.x - mNodes[k].getCenter().x, 2) + std::pow(midpoint.y - mNodes[k].getCenter().y, 2));
                if (distance < GraphVisualizerData::idealLength) {
                    sf::Vector2f force = springForce(midpoint, mNodes[k].getCenter(), GraphVisualizerData::idealLength);
                    totalForce[i] += force;
                    totalForce[j] += force;
                    totalForce[k] -= force;
                }
            }
        }
    }

    for (int i = 0; i < (int)mNodes.size(); i++) {
        sf::Vector2f forceLeft = rubberbandForce(sf::Vector2f(VisualizerData::visualizeBoxRect.getPosition().x, mNodes[i].getCenter().y), mNodes[i].getCenter());
        sf::Vector2f forceRight = rubberbandForce(sf::Vector2f(VisualizerData::visualizeBoxRect.getPosition().x + VisualizerData::visualizeBoxRect.getSize().x, mNodes[i].getCenter().y), mNodes[i].getCenter());
        sf::Vector2f forceTop = rubberbandForce(sf::Vector2f(mNodes[i].getCenter().x, VisualizerData::visualizeBoxRect.getPosition().y), mNodes[i].getCenter());
        sf::Vector2f forceBottom = rubberbandForce(sf::Vector2f(mNodes[i].getCenter().x, VisualizerData::visualizeBoxRect.getPosition().y + VisualizerData::visualizeBoxRect.getSize().y), mNodes[i].getCenter());
        totalForce[i] -= (forceLeft + forceRight + forceTop + forceBottom) * 0.1f;
    }

    for (int i = 0; i < (int)mNodes.size(); ++i) {
        mNodes[i].setPosition(mNodes[i].getPosition() + totalForce[i]);
    }

    if (mSelectedNode.first != -1) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*mWindow);
        sf::Vector2f newPosition = sf::Vector2f(mousePosition.x, mousePosition.y) - mSelectedNode.second;
        mNodes[mSelectedNode.first].setPosition(newPosition);
    }

    for (int i = 0; i < (int)mNodes.size(); ++i) {
        if (mNodes[i].getPosition().x < VisualizerData::visualizeBoxRect.left) {
            mNodes[i].setPosition(sf::Vector2f(VisualizerData::visualizeBoxRect.left, mNodes[i].getPosition().y));
        } else if (mNodes[i].getPosition().x > VisualizerData::visualizeBoxRect.left + VisualizerData::visualizeBoxRect.width - GraphicNodeData::nodeSize.x) {
            mNodes[i].setPosition(sf::Vector2f(VisualizerData::visualizeBoxRect.left + VisualizerData::visualizeBoxRect.width - GraphicNodeData::nodeSize.x, mNodes[i].getPosition().y));
        }

        if (mNodes[i].getPosition().y < VisualizerData::visualizeBoxRect.top) {
            mNodes[i].setPosition(sf::Vector2f(mNodes[i].getPosition().x, VisualizerData::visualizeBoxRect.top));
        } else if (mNodes[i].getPosition().y > VisualizerData::visualizeBoxRect.top + VisualizerData::visualizeBoxRect.height - GraphicNodeData::nodeSize.y) {
            mNodes[i].setPosition(sf::Vector2f(mNodes[i].getPosition().x, VisualizerData::visualizeBoxRect.top + VisualizerData::visualizeBoxRect.height - GraphicNodeData::nodeSize.y));
        }
    }
}

void GraphVisualizer::handleGraphEvent(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        for (int i = 0; i < (int)mNodes.size(); ++i) {
            if (sfhelper::isMouseOver(mWindow, mNodes[i].getPosition(), GraphicNodeData::nodeSize)) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*mWindow);
                mSelectedNode = std::make_pair(i, sf::Vector2f(mousePosition.x, mousePosition.y) - mNodes[i].getPosition());
                break;
            }
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        mSelectedNode = std::make_pair(-1, sf::Vector2f());
    }
}

void GraphVisualizer::drawAllNode() {
    for (int i = 0; i < (int)mNodes.size(); ++i) {
        draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
    }
}

void GraphVisualizer::drawAllEdge() {
    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
            }
        }
    }
}

void GraphVisualizer::create(int numNode, int numEdge) {
    mNodes.clear();
    mAdjacencyMatrix.clear();

    for (int i = 0; i < numNode; ++i) {
        sf::Vector2f randomPosition = sf::Vector2f(
            Randomizer::randomFloat(VisualizerData::visualizeBoxRect.getPosition().x, VisualizerData::visualizeBoxRect.getPosition().x + VisualizerData::visualizeBoxRect.getSize().x - GraphicNodeData::nodeSize.x),
            Randomizer::randomFloat(VisualizerData::visualizeBoxRect.getPosition().y, VisualizerData::visualizeBoxRect.getPosition().y + VisualizerData::visualizeBoxRect.getSize().y - GraphicNodeData::nodeSize.y)
        );

        mNodes.push_back(GraphicNode(std::to_string(i), randomPosition));
    }

    mAdjacencyMatrix.assign(numNode, std::vector<int>(numNode, 0));
    for (int i = 0; i < numEdge; ++i) {
        int u, v;
        do {
            u = Randomizer::random<int>(0, numNode - 1);
            v = Randomizer::random<int>(0, numNode - 1);
        } while (mAdjacencyMatrix[u][v] > 0 || u >= v);

        mAdjacencyMatrix[u][v] = mAdjacencyMatrix[v][u] = Randomizer::random<int>(1, 99);
    }

    mCode.update({});

    addNewStep();

    drawAllNode();
    drawAllEdge();
    drawCode();
}

void GraphVisualizer::createFromAdjacencyMatrix(std::string input) {
    mNodes.clear();
    mAdjacencyMatrix.clear();

    std::stringstream ss(input);
    std::string tmp;
    ss >> tmp;
    int numNode = std::stoi(tmp);
    for (int i = 0; i < numNode; ++i) {
        sf::Vector2f randomPosition = sf::Vector2f(
            Randomizer::randomFloat(VisualizerData::visualizeBoxRect.getPosition().x, VisualizerData::visualizeBoxRect.getPosition().x + VisualizerData::visualizeBoxRect.getSize().x - GraphicNodeData::nodeSize.x),
            Randomizer::randomFloat(VisualizerData::visualizeBoxRect.getPosition().y, VisualizerData::visualizeBoxRect.getPosition().y + VisualizerData::visualizeBoxRect.getSize().y - GraphicNodeData::nodeSize.y)
        );

        mNodes.push_back(GraphicNode(std::to_string(i), randomPosition));
    }

    mAdjacencyMatrix.assign(numNode, std::vector<int>(numNode, 0));
    for (int i = 0; i < numNode; ++i) {
        for (int j = 0; j < numNode; ++j) {
            ss >> tmp;
            mAdjacencyMatrix[i][j] = std::stoi(tmp);
        }
    }

    mCode.update({});

    addNewStep();

    drawAllNode();
    drawAllEdge();
    drawCode();
}

void GraphVisualizer::BFS() {
    mCode.update({
        "for s from 0 to n - 1: if !CC[s]:",
        "    Q = {s}, CC[s] = ++curCC",
        "    while !Q.empty():",
        "        u = Q.front(), Q.pop()",
        "        for each v is adjacent to u: if !CC[v]:",
        "            CC[v] = curCC, Q.push(v)"
    });

    std::vector<bool> visited(mNodes.size(), false);
    std::vector<std::vector<bool>> usedEdge(mNodes.size(), std::vector<bool>(mNodes.size(), false));
    std::vector<int> CC(mNodes.size(), -1);
    std::queue<int> Q;

    int lastU = -1, numCC = 0;
    int lastCodeLine = -1;
    for (int s = 0; s < (int)mNodes.size(); ++s) {
        if (visited[s]) {
            continue;
        }

        CC[s] = ++numCC;

        addNewStep();
        for (int i = 0; i < (int)mNodes.size(); ++i) {
            if (i == s) {
                drawChangeColor({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
            } else if (visited[i]) {
                draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            } else {
                draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            }

            if (i == lastU) {
                drawFadeOut({ &mNodes[i] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            }
        }

        for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
            for (int j = i + 1; j < (int)mNodes.size(); ++j) {
                if (mAdjacencyMatrix[i][j] > 0) {
                    if (usedEdge[i][j]) {
                        drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus);
                    } else {
                        drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                    }
                }
            }
        }

        for (int i = 0; i < (int)mNodes.size(); ++i) {
            if (i == s) {
                drawLabelFadeIn({ &mNodes[s] }, { std::to_string(numCC) }, Color::label);
            } else if (CC[i] != -1) {
                drawLabel({ &mNodes[i] }, { std::to_string(CC[i]) }, Color::label);
            }
        }
        
        if (lastCodeLine == -1) {
            drawCodeFadeIn(1);
        } else {
            drawCodeChangeLine(lastCodeLine, 1);
        }
        lastCodeLine = 1;

        Q.push(s);
        visited[s] = true;
        lastU = -1;

        while (!Q.empty()) {
            int u = Q.front(); Q.pop();

            addNewStep();
            for (int i = 0; i < (int)mNodes.size(); ++i) {
                if (visited[i]) {
                    draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                } else {
                    draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                }

                if (i == u) {
                    drawFadeIn({ &mNodes[i] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                }

                if (i == lastU) {
                    drawFadeOut({ &mNodes[i] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                }
            }

            for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
                for (int j = i + 1; j < (int)mNodes.size(); ++j) {
                    if (mAdjacencyMatrix[i][j] > 0) {
                        if (usedEdge[i][j]) {
                            drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus);
                        } else {
                            drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                        }
                    }
                }
            }

            for (int i = 0; i < (int)mNodes.size(); i++) {
                if (CC[i] != -1) {
                    drawLabel({ &mNodes[i] }, { std::to_string(CC[i]) }, Color::label);
                }
            }

            drawCodeChangeLine(lastCodeLine, 3);
            lastCodeLine = 3;

            for (int v = 0; v < (int)mNodes.size(); ++v) {
                if (mAdjacencyMatrix[u][v] > 0 && !visited[v]) {
                    addNewStep();
                    for (int i = 0; i < (int)mNodes.size(); ++i) {
                        if (i == u) {
                            draw({ &mNodes[i] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                        } else if (i == v) {
                            drawChangeColor({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
                        } else if (visited[i]) {
                            draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        } else {
                            draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                        }
                    }

                    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
                        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
                            if (mAdjacencyMatrix[i][j] > 0) {
                                if (usedEdge[i][j]) {
                                    drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus);
                                } else {
                                    drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                                }

                                if (i == u && j == v) {
                                    drawEdgeSlideIn({ std::make_pair(&mNodes[i], &mNodes[j]) }, Color::edgeFocus);
                                } else if (j == u && i == v) {
                                    drawEdgeSlideIn({ std::make_pair(&mNodes[j], &mNodes[i]) }, Color::edgeFocus);
                                }
                            }
                        }
                    }

                    for (int i = 0; i < (int)mNodes.size(); i++) {
                        if (i == v) {
                            drawLabelFadeIn({ &mNodes[v] }, { std::to_string(numCC) }, Color::label);
                        } else if (CC[i] != -1) {
                            drawLabel({ &mNodes[i] }, { std::to_string(CC[i]) }, Color::label);
                        }
                    }

                    drawCodeChangeLine(lastCodeLine, 5);
                    lastCodeLine = 5;

                    CC[v] = numCC;
                    usedEdge[u][v] = usedEdge[v][u] = true;

                    if (!visited[v]) {
                        visited[v] = true;
                        Q.push(v);
                    }
                }
            }

            lastU = u;
        }
    }

    addNewStep();
    for (int i = 0; i < (int)mNodes.size(); ++i) {
        drawChangeColor({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        if (i == lastU) {
            drawFadeOut({ &mNodes[i] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
    }

    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                if (usedEdge[i][j]) {
                    drawEdgeWeightChangeColor({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus, Color::edge);
                } else {
                    drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                }
            }
        }   
    }

    for (int i = 0; i < (int)mNodes.size(); ++i) {
        drawLabel({ &mNodes[i] }, { std::to_string(CC[i]) }, Color::label);
    }

    drawCodeFadeOut(lastCodeLine);
}

void GraphVisualizer::Krukal() {
    mCode.update({
        "sort edges by weight",
        "for each edge (u, v) in sorted edges:",
        "    if !same(u, v):",
        "        union(u, v), add edge (u, v) to MST",
        "    else:",
        "        add edge (u, v) to discarded edges"
    });

    std::vector<int> parent(mNodes.size());
    for (int u = 0; u < (int)mNodes.size(); ++u) {
        parent[u] = u;
    }

    std::function<int(int)> findSet = [&](int u) {
        if (parent[u] == u) {
            return u;
        }

        return parent[u] = findSet(parent[u]);
    };

    std::function<bool(int, int)> isSameSet = [&](int u, int v) {
        return findSet(u) == findSet(v);
    };

    std::function<void(int, int)> unionSet = [&](int u, int v) {
        parent[findSet(u)] = findSet(v);
    };

    std::vector<std::pair<int, std::pair<int, int>>> edges;
    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                edges.push_back(std::make_pair(mAdjacencyMatrix[i][j], std::make_pair(i, j)));
            }
        }
    }
    std::sort(edges.begin(), edges.end());

    std::vector<int> usedNode(mNodes.size(), 0);
    std::vector<std::vector<int>> usedEdge(mNodes.size(), std::vector<int>(mNodes.size(), 0));

    int lastCodeLine = -1;
    for (auto edge : edges) {
        int u = edge.second.first, v = edge.second.second;

        if (usedNode[u] == 0) {
            usedNode[u] = 1;
        }

        if (usedNode[v] == 0) {
            usedNode[v] = 1;
        }

        addNewStep();
        for (int i = 0; i < (int)mNodes.size(); ++i) {
            if (usedNode[i] == 0) {
                draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            } else if (usedNode[i] == 1) {
                drawChangeColor({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
            } else {
                draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            }
        }

        for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
            for (int j = i + 1; j < (int)mNodes.size(); ++j) {
                if (mAdjacencyMatrix[i][j] > 0) {
                    if (usedEdge[i][j] == 1) {
                        drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus);
                    } else if (usedEdge[i][j] == 2) {

                    } else {
                        if (i == u && j == v) {
                            if (isSameSet(u, v)) {
                                drawEdgeWeightFadeOut({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                            } else {
                                drawEdgeWeightChangeColor({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge, Color::edgeFocus);
                            }
                        } else {
                            drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                        }
                    }
                }
            }
        }

        if (isSameSet(u, v)) {
            drawCodeChangeLine(lastCodeLine, 5);
            lastCodeLine = 5;
        } else {
            if (lastCodeLine == -1) {
                drawCodeFadeIn(3);
            } else {
                drawCodeChangeLine(lastCodeLine, 3);
            }
            lastCodeLine = 3;
        }

        if (usedNode[u] == 1) {
            usedNode[u] = 2;
        }

        if (usedNode[v] == 1) {
            usedNode[v] = 2;
        }

        if (isSameSet(u, v)) {
            usedEdge[u][v] = usedEdge[v][u] = 2;
        } else {
            usedEdge[u][v] = usedEdge[v][u] = 1;
            unionSet(u, v);
        }
    }

    addNewStep();
    for (int i = 0; i < (int)mNodes.size(); i++) {
        if (usedNode[i] == 0) {
            draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        } else {
            drawChangeColor({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        }
    }

    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                if (usedEdge[i][j] == 1) {
                    drawEdgeWeightChangeColor({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus, Color::edge);
                }
            }
        }
    }

    drawCodeFadeOut(lastCodeLine);
}

void GraphVisualizer::Dijkstra(int s) {
    mCode.update({
        "dist[s] = 0, PQ = {(0, s)}",
        "while !PQ.empty():",
        "    du, u = Q.front(), Q.pop()",
        "    if du > dist[u]: continue",
        "    for each v is adjacent to u with weight w:",
        "        if minimize(dist[v], dist[u] + w):",
        "            PQ.push((dist[v], v))"
    });

    std::vector<int> dist(mNodes.size(), INT_MAX);
    std::vector<std::vector<bool>> usedEdge(mNodes.size(), std::vector<bool>(mNodes.size(), false));
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> PQ;

    int lastU = -1;
    int lastCodeLine = -1;

    addNewStep();
    for (int i = 0; i < (int)mNodes.size(); ++i) {
        if (i == s) {
            drawChangeColor({ &mNodes[s] }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
        } else {
            draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        }
    }

    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                if (usedEdge[i][j]) {
                    drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus);
                } else {
                    drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                }
            }
        }
    }

    drawLabelFadeIn({ &mNodes[s] }, { std::to_string(0) }, Color::label);

    if (lastCodeLine == -1) {
        drawCodeFadeIn(0);
    } else {
        drawCodeChangeLine(lastCodeLine, 0);
    }
    lastCodeLine = 0;

    dist[s] = 0;
    PQ.push(std::make_pair(0, s));
    lastU = -1;

    while (!PQ.empty()) {
        int du = PQ.top().first, u = PQ.top().second; 
        PQ.pop();

        addNewStep();
        for (int i = 0; i < (int)mNodes.size(); ++i) {
            if (dist[i] != INT_MAX) {
                draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            } else {
                draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            }

            if (i == u) {
                drawFadeIn({ &mNodes[i] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            }

            if (i == lastU) {
                drawFadeOut({ &mNodes[i] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            }
        }

        for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
            for (int j = i + 1; j < (int)mNodes.size(); ++j) {
                if (mAdjacencyMatrix[i][j] > 0) {
                    if (usedEdge[i][j]) {
                        drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus);
                    } else {
                        drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                    }
                }
            }
        }

        for (int i = 0; i < (int)mNodes.size(); i++) {
            if (dist[i] != INT_MAX) {
                drawLabel({ &mNodes[i] }, { std::to_string(dist[i]) }, Color::label);
            }
        }

        drawCodeChangeLine(lastCodeLine, 2);
        lastCodeLine = 2;

        if (du > dist[u]) {
            lastU = u;
            continue;
        }

        for (int v = 0; v < (int)mNodes.size(); ++v) {
            if (mAdjacencyMatrix[u][v] > 0 && dist[v] > dist[u] + mAdjacencyMatrix[u][v]) {
                addNewStep();
                for (int i = 0; i < (int)mNodes.size(); ++i) {
                    if (i == u) {
                        draw({ &mNodes[u] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    } else if (i == v) {
                        if (dist[i] != INT_MAX) {
                            draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        } else {
                            drawChangeColor({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
                        }
                    } else if (dist[i] != INT_MAX) {
                        draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                    } else {
                        draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                    }
                }
                

                for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
                    for (int j = i + 1; j < (int)mNodes.size(); ++j) {
                        if (mAdjacencyMatrix[i][j] > 0) {
                            if (usedEdge[i][j]) {
                                drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus);
                            } else {
                                drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                            }

                            if (i == u && j == v) {
                                drawEdgeSlideIn({ std::make_pair(&mNodes[i], &mNodes[j]) }, Color::edgeFocus);
                            } else if (j == u && i == v) {
                                drawEdgeSlideIn({ std::make_pair(&mNodes[j], &mNodes[i]) }, Color::edgeFocus);
                            }
                        }
                    }
                }

                for (int i = 0; i < (int)mNodes.size(); i++) {
                    if (i == v) {
                        if (dist[v] != INT_MAX) {
                            drawLabelChangeName({ &mNodes[v] }, { std::to_string(dist[v]) }, { std::to_string(dist[u] + mAdjacencyMatrix[u][v]) }, Color::label);
                        } else {
                            drawLabelFadeIn({ &mNodes[v] }, { std::to_string(dist[u] + mAdjacencyMatrix[u][v]) }, Color::label);
                        }
                    } else if (dist[i] != INT_MAX) {
                        drawLabel({ &mNodes[i] }, { std::to_string(dist[i]) }, Color::label);
                    }
                }

                drawCodeChangeLine(lastCodeLine, 6);
                lastCodeLine = 6;

                dist[v] = dist[u] + mAdjacencyMatrix[u][v];
                usedEdge[u][v] = usedEdge[v][u] = true;
                PQ.push(std::make_pair(dist[v], v));
            }
        }

        lastU = u;
    }

    addNewStep();
    for (int i = 0; i < (int)mNodes.size(); ++i) {
        if (dist[i] != INT_MAX) {
            drawChangeColor({ &mNodes[i] }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        } else {
            draw({ &mNodes[i] }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        }

        if (i == lastU) {
            drawFadeOut({ &mNodes[lastU] }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
    }

    for (int i = 0; i < (int)mNodes.size() - 1; ++i) {
        for (int j = i + 1; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                if (usedEdge[i][j]) {
                    drawEdgeWeightChangeColor({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edgeFocus, Color::edge);
                } else {
                    drawEdgeWeight({ std::make_pair(&mNodes[i], &mNodes[j]) }, { std::to_string(mAdjacencyMatrix[i][j]) }, Color::edge);
                }
            }
        }   
    }

    for (int i = 0; i < (int)mNodes.size(); ++i) {
        if (dist[i] != INT_MAX) {
            drawLabel({ &mNodes[i] }, { std::to_string(dist[i]) }, Color::label);
        }
    }

    drawCodeFadeOut(lastCodeLine);
}

void GraphVisualizer::run() {
    std::function<bool()> conditionNone = [&]() { return true; };

    std::function<int(int)> getNumEdgeLimit = [](int numNode) {
        return std::min(numNode * (numNode - 1) / 2, 2 * numNode);
    };

    std::function<std::string(std::string, std::string)> adjacencyMatrixValidator = [&](std::string value, std::string name) -> std::string {
        name.clear();
        
        std::stringstream ss(value);
        std::vector<std::string> tokens;
        for (std::string token; ss >> token; ) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            return "Matrix must not be empty";
        }

        std::string error = Validator::isIntegerInRange(tokens[0], "n", GraphVisualizerData::minNode, GraphVisualizerData::maxNode);
        if (error != "") {
            return error;
        }

        int numNode = std::stoi(tokens[0]);
        if ((int)tokens.size() != numNode * numNode + 1) {
            return "Matrix must have " + std::to_string(numNode * numNode) + " elements";
        }

        std::vector<std::vector<int>> adjacencyMatrix(numNode, std::vector<int>(numNode, 0));
        for (int i = 1; i < (int)tokens.size(); ++i) {
            error = Validator::isIntegerInRange(tokens[i], "w", 0, GraphVisualizerData::maxWeight);
            if (error != "") {
                return error;
            }

            adjacencyMatrix[(i - 1) / numNode][(i - 1) % numNode] = std::stoi(tokens[i]);
        }

        for (int i = 0; i < numNode; i++) {
            if (adjacencyMatrix[i][i] != 0) {
                return "Matrix must have 0 on the main diagonal";
            }
        }

        for (int i = 0; i < numNode; ++i) {
            for (int j = 0; j < numNode; ++j) {
                if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i]) {
                    return "Matrix must be symmetric";
                }
            }
        }

        return "";
    };

    mOption.addOption("Create");
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, GraphVisualizerData::minNode, GraphVisualizerData::maxNode),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), GraphVisualizerData::minNode, GraphVisualizerData::maxNode)
    );
    mOption.addSuboptionInputBox("m",
        std::bind(static_cast<std::string(*)(std::string, std::string, std::function<int()>, std::function<int()>)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, [&]() { return 0; }, [&]() { return getNumEdgeLimit(std::stoi(mOption.getValue(0, 0, 0))); }),
        std::bind(static_cast<std::string(*)(std::function<int()>, std::function<int()>)>(Randomizer::integerInRange), [&]() { return 0; }, [&]() { return getNumEdgeLimit(std::stoi(mOption.getValue(0, 0, 0))); })
    );

    mOption.addSuboption("Matrix", conditionNone);
    mOption.addSuboptionEditorBox(adjacencyMatrixValidator);

    mOption.addSuboption("File", conditionNone);
    mOption.addSuboptionFileBox("", adjacencyMatrixValidator);

    mOption.addOption("Find CC");
    mOption.addSuboption("BFS", conditionNone);

    mOption.addOption("Find MST");
    mOption.addSuboption("Kruskal", conditionNone);

    mOption.addOption("Find SP");
    mOption.addSuboption("Dijkstra", conditionNone);
    mOption.addSuboptionInputBox("s",
        std::bind(static_cast<std::string(*)(std::string, std::string, std::function<int()>, std::function<int()>)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, [&]() { return 0; }, [&]() { return (int)mNodes.size() - 1; }),
        std::bind(static_cast<std::string(*)(std::function<int()>, std::function<int()>)>(Randomizer::integerInRange), [&]() { return 0; }, [&]() { return (int)mNodes.size() - 1; })
    );

    mOption.processOption();

    clearAllSteps();
    create(GraphVisualizerData::defaultNode, GraphVisualizerData::defaultEdge);

    while (mWindow->isOpen()) {
        updateGraphState();
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }

            handleGraphEvent(event);

            VisualizerData::Event action = handleEvent(event);
            if (action == VisualizerData::Event::quit) {
                return;
            }

            if (action == VisualizerData::Event::confirm) {
                abortAllSteps();
                clearAllSteps();

                std::pair<int, int> option = mOption.getOption();
                std::vector<std::string> values = mOption.getValues();

                switch (option.first) {
                case 0: // Create
                    switch (option.second) {
                    case 0: // Random
                        create(stoi(values[0]), stoi(values[1]));
                        break;

                    case 1: // Matrix
                        createFromAdjacencyMatrix(values[0]);
                        break;

                    case 2: // File
                        createFromAdjacencyMatrix(values[0]);
                        break;
                    }


                    break;

                case 1: // Find CC
                    switch (option.second) {
                    case 0: // BFS
                        BFS();
                        break;
                    }
                    break;

                case 2: // Find MST
                    switch (option.second) {
                    case 0: // Kruskal
                        Krukal();
                        break;
                    }

                    break;

                case 3: // Find SP
                    switch (option.second) {
                    case 0: // Dijkstra
                        Dijkstra(stoi(values[0]));
                        break;
                    }

                    break;
                }
            }
        }

        mWindow->clear(sf::Color::White);
        draw();
        mWindow->display();
    }
}