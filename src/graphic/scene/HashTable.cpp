#include "HashTable.hpp"

HashTable::HashTable() {
}

HashTable::HashTable(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
}

std::vector<Node*> HashTable::getNodes(int left, int right) {
    std::vector<Node*> nodes;
    for (int i = left; i <= right; i++) {
        nodes.push_back(&mTable[i]);
    }

    return nodes;
}

std::vector<Label*> HashTable::getLabels(int left, int right) {
    std::vector<Label*> labels;
    for (int i = left; i <= right; i++) {
        labels.push_back(&mLabels[i]);
    }

    return labels;
}

void HashTable::create(int capacity, int size) {
    mCapacity = capacity;
    mSize = size;
    mTable.resize(mCapacity);
    mLabels.resize(mCapacity);
    for (int i = 0; i < mCapacity; i++) {
        mTable[i] = Node("", HashTableData::position + sf::Vector2f(i % HashTableData::maxSizePerLine * HashTableData::space.x, i / HashTableData::maxSizePerLine * HashTableData::space.y));
        mLabels[i] = Label(&mTable[i], std::to_string(i));
    }

    for (int i = 0; i < mSize; i++) {
        int key = std::stoi(Randomizer::integerInRange(HashTableData::minValue, HashTableData::maxValue));
        int hashkey = key % mCapacity;
        while (mTable[hashkey].getValue() != "") {
            hashkey = (hashkey + 1) % mCapacity;
        }

        mTable[hashkey].setValue(std::to_string(key));
    }

    mCode.update({});

    addNewStep();
    drawFadeIn(getNodes(0, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
    drawFadeIn(getLabels(0, mCapacity - 1), AssetsData::Color::label);
    drawCode();
}

void HashTable::search(int key) {
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
            draw(getNodes(0, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        } else {
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            }
            drawFadeIn({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        }
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        if (count == 0) {
            drawCodeFadeIn(1);
        } else {
            drawCodeChangeLine(4, 1);
        }

        if (!(count < mCapacity && mTable[hashkey].getValue() != HashTableData::emptyValue)) {
            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        }
        draw({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        drawCodeChangeLine(1, 2);

        if (mTable[hashkey].getValue() == std::to_string(key)) {
            addNewStep();
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            }
            draw({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
            draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
            drawCodeChangeLine(2, 3);

            addNewStep();
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                drawChangeColor(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
                draw(getNodes(hashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            } else {
                drawChangeColor(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
                draw(getNodes(hashkey, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                drawChangeColor(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
            }
            drawFadeOut({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
            draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
            drawCodeFadeOut(3);

            return;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, hashkey), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            draw(getNodes(0, hashkey), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        }
        drawFadeOut({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        drawCodeChangeLine(2, 4);

        hashkey = (hashkey + 1) % mCapacity;
        count++;
    }

    addNewStep();
    if (count == mCapacity) {
        draw(getNodes(0, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
    } else {
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        }
        draw({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
    }
    draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
    drawCodeChangeLine(1, 5);

    addNewStep();
    if (count == mCapacity) {
        drawChangeColor(getNodes(0, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
    } else {
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
            draw(getNodes(hashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            drawChangeColor(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
            draw(getNodes(hashkey, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
        }
        drawFadeOut({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
    }
    draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
    drawCodeFadeOut(5);
}

void HashTable::insert(int key) {
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
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        }
        drawFadeIn({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        if (hashkey == firstHashkey) {
            drawCodeFadeIn(1);
        } else {
            drawCodeChangeLine(2, 1);
        }

        if (!(mTable[hashkey].getValue() != HashTableData::emptyValue && mTable[hashkey].getValue() != HashTableData::deletedValue)) {
            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, hashkey), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            draw(getNodes(0, hashkey), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        }
        drawFadeOut({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        drawCodeChangeLine(1, 2);

        hashkey = (hashkey + 1) % mCapacity;
    }

    addNewStep();
    if (hashkey >= firstHashkey) {
        draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
    } else {
        draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
    }
    drawChangeValue({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1, { mTable[hashkey].getValue() }, { std::to_string(key) });
    draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
    drawCodeChangeLine(1, 3);
    mSize++;

    addNewStep();
    if (hashkey >= firstHashkey) {
        draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        drawChangeColor(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
        draw(getNodes(hashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
    } else {
        drawChangeColor(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
        draw(getNodes(hashkey, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        drawChangeColor(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
    }
    drawFadeOut({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
    draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
    drawCodeFadeOut(3);
}

void HashTable::erase(int key) {
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
            draw(getNodes(0, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        } else {
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            }
            drawFadeIn({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        }
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        if (count == 0) {
            drawCodeFadeIn(1);
        } else {
            drawCodeChangeLine(4, 1);
        }

        if (!(count < mCapacity && mTable[hashkey].getValue() != HashTableData::emptyValue)) {
            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        }
        draw({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        drawCodeChangeLine(1, 2);

        if (mTable[hashkey].getValue() == std::to_string(key)) {
            addNewStep();
            if (hashkey >= firstHashkey) {
                draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            } else {
                draw(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
                draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
                draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            }
            drawChangeValue({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1, { mTable[hashkey].getValue() }, { HashTableData::deletedValue });
            draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
            drawCodeChangeLine(2, 3);
            mSize--;

            break;
        }

        addNewStep();
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, hashkey), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            draw(getNodes(0, hashkey), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
            draw(getNodes(hashkey + 1, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            draw(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus2);
        }
        drawFadeOut({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        drawCodeChangeLine(2, 4);

        hashkey = (hashkey + 1) % mCapacity;
        count++;
    }

    addNewStep();
    if (count == mCapacity) {
        drawChangeColor(getNodes(0, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        drawCodeFadeOut(1);
    } else {
        if (hashkey >= firstHashkey) {
            draw(getNodes(0, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
            draw(getNodes(hashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
        } else {
            drawChangeColor(getNodes(0, hashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
            draw(getNodes(hashkey, firstHashkey - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::node, AssetsData::Color::nodeText);
            drawChangeColor(getNodes(firstHashkey, mCapacity - 1), NodeData::Shape::square, NodeData::Type::hollow, AssetsData::Color::nodeFocus1, AssetsData::Color::node, AssetsData::Color::nodeTextFocus2, AssetsData::Color::nodeText);
        }
        drawFadeOut({ &mTable[hashkey] }, NodeData::Shape::square, NodeData::Type::filled, AssetsData::Color::nodeFocus1, AssetsData::Color::nodeTextFocus1);
        draw(getLabels(0, mCapacity - 1), AssetsData::Color::label);
        if (mTable[hashkey].getValue() == HashTableData::emptyValue) {
            drawCodeFadeOut(1);
        } else {
            drawCodeFadeOut(3);
        }
    }
}

void HashTable::run() { 
    std::function <bool()> conditionNone = []() { return true; };
    std::function <bool()> conditionSizeNotEqualCapacity = [&]() { return mSize < mCapacity; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboptionInputBox("c",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableData::minCapacity, HashTableData::maxCapacity),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableData::minCapacity, HashTableData::maxCapacity)
    );

    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("c",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableData::minCapacity, HashTableData::maxCapacity),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableData::minCapacity, HashTableData::maxCapacity)
    );
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, std::function<int()>, std::function<int()>)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, [&]() { return HashTableData::minSize; }, [&]() { return std::stoi(mOption.getValue(0, 1, 0)); }),
        std::bind(static_cast<std::string(*)(std::function<int()>, std::function<int()>)>(Randomizer::integerInRange), [&]() { return HashTableData::minSize; }, [&]() { return std::stoi(mOption.getValue(0, 1, 0)); })
    );

    mOption.addOption("Search");
    mOption.addSuboption("", conditionNone);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableData::minValue, HashTableData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableData::minValue, HashTableData::maxValue)
    );

    mOption.addOption("Insert");
    mOption.addSuboption("", conditionSizeNotEqualCapacity);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableData::minValue, HashTableData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableData::minValue, HashTableData::maxValue)
    );

    mOption.addOption("Erase");
    mOption.addSuboption("", conditionNone);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, HashTableData::minValue, HashTableData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), HashTableData::minValue, HashTableData::maxValue)
    );

    mOption.processOption();

    clearAllSteps();
    create(20, 10);

    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }

            if (handleEvent(event)) {
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
                    }

                    break;

                case 1: // Search
                    switch (option.second) {
                    case 0:
                        search(std::stoi(values[0]));
                        break;
                    }

                    break;

                case 2: // Insert
                    switch (option.second) {
                    case 0:
                        insert(std::stoi(values[0]));
                        break;
                    }

                    break;

                case 3: // Erase
                    switch (option.second) {
                    case 0:
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