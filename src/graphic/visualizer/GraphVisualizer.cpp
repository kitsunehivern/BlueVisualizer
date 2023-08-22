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

    float force = -GraphVisualizerData::springConstant * (distance - idealLength);

    return sf::Vector2f(force * direction.x / distance, force * direction.y / distance);
}

void GraphVisualizer::updateGraphState() {
    std::vector<sf::Vector2f> totalForce(mNodes.size(), sf::Vector2f(0, 0));

    for (int i = 0; i < (int)mNodes.size(); ++i) {
        for (int j = 0; j < (int)mNodes.size(); ++j) {
            if (mAdjacencyMatrix[i][j] > 0) {
                sf::Vector2f force = springForce(mNodes[i].getPosition(), mNodes[j].getPosition(), GraphVisualizerData::idealLength);
                totalForce[i] -= force;
                totalForce[j] += force;
            } else {
                float distance = std::sqrt(std::pow(mNodes[i].getPosition().x - mNodes[j].getPosition().x, 2) + std::pow(mNodes[i].getPosition().y - mNodes[j].getPosition().y, 2));
                if (distance < GraphVisualizerData::idealLength) {
                    sf::Vector2f force = springForce(mNodes[i].getPosition(), mNodes[j].getPosition(), GraphVisualizerData::idealLength);
                    totalForce[i] -= force;
                    totalForce[j] += force;
                }
            }
        }
    }

    sf::Vector2f graphCenter = sf::Vector2f(0, 0);
    for (int i = 0; i < (int)mNodes.size(); ++i) {
        graphCenter += mNodes[i].getPosition();
    }
    graphCenter /= (float)mNodes.size();

    sf::Vector2f force = springForce(VisualizerData::visualizeBoxCenter, graphCenter, 0.f);
    for (int i = 0; i < (int)mNodes.size(); ++i) {
        totalForce[i] += force;
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
    for (int i = 0; i < (int)mNodes.size(); ++i) {
        for (int j = 0; j < (int)mNodes.size(); ++j) {
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
        } while (mAdjacencyMatrix[u][v] == 1 || u >= v);

        mAdjacencyMatrix[u][v] = 1;
    }

    mCode.update({});

    addNewStep();

    drawAllNode();
    drawAllEdge();
    drawCode();
}

void GraphVisualizer::run() {
    std::function<bool()> conditionNone = [&]() { return true; };

    std::function<int(int)> getNumEdgeLimit = [](int numNode) {
        return std::min(numNode * (numNode - 1) / 2, 2 * numNode);
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

            if (handleEvent(event)) {
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