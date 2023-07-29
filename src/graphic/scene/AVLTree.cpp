#include "AVLTree.hpp"

AVLTree::AVLTree() {
}

AVLTree::AVLTree(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
}

void AVLTree::run() {
    std::function <bool()> conditionNone = []() { return true; };

    mOption.addOption("Create");
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "n");
    mOption.addSuboption("Manual", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v[]");
    mOption.addSuboptionInputBox(InputBoxData::InputType::file, "file");
    
    mOption.addOption("Search");
    mOption.addSuboption("Value", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");

    mOption.addOption("Update");
    mOption.addSuboption("Value", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");

    mOption.addOption("Insert");
    mOption.addSuboption("Front", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");
    mOption.addSuboption("Back", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");
    mOption.addSuboption("Middle", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "i");
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");

    mOption.addOption("Delete");
    mOption.addSuboption("Front", conditionNone);
    mOption.addSuboption("Back", conditionNone);
    mOption.addSuboption("Middle", conditionNone);
    mOption.addSuboptionInputBox(InputBoxData::InputType::keyboard, "i");

    mOption.processOption();

    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }

            mOption.handleEvent(mWindow, event);
            mControl.handleEvent(mWindow, event);
        }

        mWindow->clear(sf::Color::White);
        mWindow->draw(mOption);
        mWindow->draw(mControl);
        mWindow->display();
    }
}