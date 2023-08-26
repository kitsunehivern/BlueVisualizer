#include "HashTableVisualizer.hpp"

HashTableVisualizer::HashTableVisualizer() {
}

HashTableVisualizer::HashTableVisualizer(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
}

std::vector<GraphicNode*> HashTableVisualizer::getNodes(int left, int right) {
    std::vector<GraphicNode*> nodes;
    for (int i = left; i <= right; i++) {
        nodes.push_back(&mTable[i]);
    }

    return nodes;
}

void HashTableVisualizer::drawAllLabels() {
    for (int i = 0; i < mCapacity; i++) {
        drawLabel({ &mTable[i] }, { std::to_string(i) }, Color::label);
    }
}

void HashTableVisualizer::drawAllLabelsFadeIn() {
    for (int i = 0; i < mCapacity; i++) {
        drawLabelFadeIn({ &mTable[i] }, { std::to_string(i) }, Color::label);
    }
}

void HashTableVisualizer::create(int capacity, int size) {
    mCapacity = capacity;
    mSize = size;
    mTable.resize(mCapacity);
    for (int i = 0; i < mCapacity; i++) {
        mTable[i] = GraphicNode("", HashTableVisualizerData::position + sf::Vector2f(i % HashTableVisualizerData::maxSizePerLine * HashTableVisualizerData::space.x, i / HashTableVisualizerData::maxSizePerLine * HashTableVisualizerData::space.y));
    }

    for (int i = 0; i < mSize; i++) {
        int key = std::stoi(Randomizer::integerInRange(HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue));
        int hashkey = key % mCapacity;
        while (mTable[hashkey].getValue() != "") {
            hashkey = (hashkey + 1) % mCapacity;
        }

        mTable[hashkey].setValue(std::to_string(key));
    }

    mCode.update({});

    addNewStep();
    drawFadeIn(getNodes(0, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
    drawAllLabelsFadeIn();
    drawCode();
}

void HashTableVisualizer::createFromList(std::string input) {
    for (auto& character : input) {
        if (!(character == '-' || (character >= '0' && character <= '9'))) {
            character = ' ';
        }
    }
    
    std::stringstream ss(input);
    std::vector<int> keys;
    for (std::string token; ss >> token; ) {
        keys.push_back(std::stoi(token));
    }

    mCapacity = keys.size();
    mSize = keys.size();
    mTable.resize(mCapacity);
    for (int i = 0; i < mCapacity; i++) {
        mTable[i] = GraphicNode("", HashTableVisualizerData::position + sf::Vector2f(i % HashTableVisualizerData::maxSizePerLine * HashTableVisualizerData::space.x, i / HashTableVisualizerData::maxSizePerLine * HashTableVisualizerData::space.y));
    }

    for (auto key : keys) {
        int hashkey = key % mCapacity;
        while (mTable[hashkey].getValue() != "") {
            hashkey = (hashkey + 1) % mCapacity;
        }

        mTable[hashkey].setValue(std::to_string(key));
    }

    mCode.update({});

    addNewStep();
    drawFadeIn(getNodes(0, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
    drawAllLabelsFadeIn();
    drawCode();
}

void HashTableVisualizer::search(int key) {
    mCode.update({
        "i = v mod c, k = 0",
        "while k < c and a[i] != EMPTY:",
        "    if a[i] == key:",
        "        return FOUND",
        "    i = (i + 1) mod c, k += 1",
        "return NOT_FOUND"
    });

    int hashkey = key % mCapacity, firstHashkey = hashkey, count = 0;
    while (true) {
        addNewStep();
        if (count == mCapacity) {
            draw(getNodes(0, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        } else {
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            }
            drawFadeIn({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        }
        drawAllLabels();
        if (count == 0) {
            drawCodeFadeIn(1);
        } else {
            drawCodeChangeLine(4, 1);
        }

        if (!(count < mCapacity && mTable[hashkey].getValue() != HashTableVisualizerData::emptyValue)) {
            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        }
        draw({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        drawAllLabels();
        drawCodeChangeLine(1, 2);

        if (mTable[hashkey].getValue() == std::to_string(key)) {
            addNewStep();
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            }
            draw({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
            drawAllLabels();
            drawCodeChangeLine(2, 3);

            addNewStep();
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                drawChangeColor(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
                draw(getNodes(hashkey, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            } else {
                drawChangeColor(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
                draw(getNodes(hashkey, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                drawChangeColor(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
            }
            drawFadeOut({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
            drawAllLabels();
            drawCodeFadeOut(3);

            return;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, hashkey), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            draw(getNodes(0, hashkey), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        }
        drawFadeOut({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        drawAllLabels();
        drawCodeChangeLine(2, 4);

        hashkey = (hashkey + 1) % mCapacity;
        count++;
    }

    addNewStep();
    if (count == mCapacity) {
        draw(getNodes(0, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
    } else {
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        }
        draw({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
    }
    drawAllLabels();
    drawCodeChangeLine(1, 5);

    addNewStep();
    if (count == mCapacity) {
        drawChangeColor(getNodes(0, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
    } else {
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
            draw(getNodes(hashkey, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            drawChangeColor(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
            draw(getNodes(hashkey, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
        }
        drawFadeOut({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
    }
    drawAllLabels();
    drawCodeFadeOut(5);
}

void HashTableVisualizer::insert(int key) {
    mCode.update({
        "i = v mod c",
        "while a[i] != EMPTY and a[i] != DELETED:",
        "    i = (i + 1) mod c",
        "a[i] = v"
    });

    int hashkey = key % mCapacity, firstHashkey = hashkey;
    while (true) {
        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        }
        drawFadeIn({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        drawAllLabels();
        if (hashkey == firstHashkey) {
            drawCodeFadeIn(1);
        } else {
            drawCodeChangeLine(2, 1);
        }

        if (!(mTable[hashkey].getValue() != HashTableVisualizerData::emptyValue && mTable[hashkey].getValue() != HashTableVisualizerData::deletedValue)) {
            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, hashkey), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            draw(getNodes(0, hashkey), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        }
        drawFadeOut({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        drawAllLabels();
        drawCodeChangeLine(1, 2);

        hashkey = (hashkey + 1) % mCapacity;
    }

    addNewStep();
    if (hashkey >= firstHashkey) {
        draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
    } else {
        draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
    }
    drawChangeValue({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1, { mTable[hashkey].getValue() }, { std::to_string(key) });
    drawAllLabels();
    drawCodeChangeLine(1, 3);
    mSize++;

    addNewStep();
    if (hashkey >= firstHashkey) {
        draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        drawChangeColor(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
        draw(getNodes(hashkey, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
    } else {
        drawChangeColor(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
        draw(getNodes(hashkey, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        drawChangeColor(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
    }
    drawFadeOut({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
    drawAllLabels();
    drawCodeFadeOut(3);
}

void HashTableVisualizer::erase(int key) {
    mCode.update({
        "i = v mod c, k = 0",
        "while k < c and a[i] != EMPTY:",
        "    if a[i] == key:",
        "        a[i] = DELETED, break",
        "    i = (i + 1) mod c, k += 1",
    });

    int hashkey = key % mCapacity, firstHashkey = hashkey, count = 0;
    while (true) {
        addNewStep();
        if (count == mCapacity) {
            draw(getNodes(0, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        } else {
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            }
            drawFadeIn({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        }
        drawAllLabels();
        if (count == 0) {
            drawCodeFadeIn(1);
        } else {
            drawCodeChangeLine(4, 1);
        }

        if (!(count < mCapacity && mTable[hashkey].getValue() != HashTableVisualizerData::emptyValue)) {
            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        }
        draw({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        drawAllLabels();
        drawCodeChangeLine(1, 2);

        if (mTable[hashkey].getValue() == std::to_string(key)) {
            addNewStep();
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            }
            drawChangeValue({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1, { mTable[hashkey].getValue() }, { HashTableVisualizerData::deletedValue });
            drawAllLabels();
            drawCodeChangeLine(2, 3);
            mSize--;

            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, hashkey), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            draw(getNodes(0, hashkey), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::nodeTextFocus2);
        }
        drawFadeOut({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        drawAllLabels();
        drawCodeChangeLine(2, 4);

        hashkey = (hashkey + 1) % mCapacity;
        count++;
    }

    addNewStep();
    if (count == mCapacity) {
        drawChangeColor(getNodes(0, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
        drawAllLabels();
        drawCodeFadeOut(1);
    } else {
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
            draw(getNodes(hashkey, mCapacity - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
        } else {
            drawChangeColor(getNodes(0, hashkey - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
            draw(getNodes(hashkey, firstHashkey - 1), Shape::square, Type::hollow,Color::node,Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, mCapacity - 1), Shape::square, Type::hollow,Color::nodeFocus1,Color::node,Color::nodeTextFocus2,Color::nodeText);
        }
        drawFadeOut({ &mTable[hashkey] }, Shape::square, Type::filled,Color::nodeFocus1,Color::nodeTextFocus1);
        drawAllLabels();
        if (mTable[hashkey].getValue() == HashTableVisualizerData::emptyValue) {
            drawCodeFadeOut(1);
        } else {
            drawCodeFadeOut(3);
        }
    }
}

void HashTableVisualizer::run() { 
    std::function <bool()> conditionNone = []() { return true; };
    std::function <bool()> conditionSizeNotEqualCapacity = [&]() { return mSize < mCapacity; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboptionInputBox("c",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableVisualizerData::minCapacity, HashTableVisualizerData::maxCapacity),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableVisualizerData::minCapacity, HashTableVisualizerData::maxCapacity)
    );

    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("c",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableVisualizerData::minCapacity, HashTableVisualizerData::maxCapacity),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableVisualizerData::minCapacity, HashTableVisualizerData::maxCapacity)
    );
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, std::function<int()>, std::function<int()>)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, [&]() { return HashTableVisualizerData::minSize; }, [&]() { return std::stoi(mOption.getValue(0, 1, 0)); }),
        std::bind(static_cast<std::string(*)(std::function<int()>, std::function<int()>)>(Randomizer::integerInRange), [&]() { return HashTableVisualizerData::minSize; }, [&]() { return std::stoi(mOption.getValue(0, 1, 0)); })
    );

    mOption.addSuboption("File", conditionNone);
    mOption.addSuboptionFileBox("v", std::bind(Validator::isListOfIntegerInRange, std::placeholders::_1, std::placeholders::_2, HashTableVisualizerData::minSize, HashTableVisualizerData::maxSize, HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue));

    mOption.addOption("Search");
    mOption.addSuboption("Value", conditionNone);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue)
    );

    mOption.addOption("Insert");
    mOption.addSuboption("Value", conditionSizeNotEqualCapacity);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue)
    );

    mOption.addOption("Erase");
    mOption.addSuboption("Value", conditionNone);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableVisualizerData::minValue, HashTableVisualizerData::maxValue)
    );

    mOption.processOption();

    clearAllSteps();
    create(HashTableVisualizerData::defaultCapacity, HashTableVisualizerData::defaultSize);

    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }

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
                    case 0: // Empty
                        create(std::stoi(values[0]), 0);
                        break;

                    case 1: // Random
                        create(std::stoi(values[0]), std::stoi(values[1]));
                        break;

                    case 2: // File
                        createFromList(values[0]);
                        break;
                    }

                    break;

                case 1: // Search
                    switch (option.second) {
                    case 0: // Value
                        search(std::stoi(values[0]));
                        break;
                    }

                    break;

                case 2: // Insert
                    switch (option.second) {
                    case 0: // Value
                        insert(std::stoi(values[0]));
                        break;
                    }

                    break;

                case 3: // Erase
                    switch (option.second) {
                    case 0: // Value
                        erase(std::stoi(values[0]));
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