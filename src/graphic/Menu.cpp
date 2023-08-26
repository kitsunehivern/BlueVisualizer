#include "Menu.hpp"

Menu::Menu(sf::RenderWindow* window, AssetsHolder* assets) {
    mWindow = window;
    mAssets = assets;

    mHashTableButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(200.f, 300.f), ButtonData::ColorSet::set4);
    mHashTableButton.setTextInside("Hash Table");
    mAVLTreeButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(650.f, 300.f), ButtonData::ColorSet::set4);
    mAVLTreeButton.setTextInside("AVL Tree");
    mBinaryHeapButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(1100.f, 300.f), ButtonData::ColorSet::set4);
    mBinaryHeapButton.setTextInside("Binary Heap");
    mTrieButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(200.f, 600.f), ButtonData::ColorSet::set4);
    mTrieButton.setTextInside("Trie");
    mBTreeButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(650.f, 600.f), ButtonData::ColorSet::set4);
    mBTreeButton.setTextInside("2-3-4 Tree");
    mGraphButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(1100.f, 600.f), ButtonData::ColorSet::set4);
    mGraphButton.setTextInside("Graph");
}

void Menu::updateState() {
    mBinaryHeapButton.updateState(mWindow);
    mHashTableButton.updateState(mWindow);
    mAVLTreeButton.updateState(mWindow);
    mTrieButton.updateState(mWindow);
    mBTreeButton.updateState(mWindow);
    mGraphButton.updateState(mWindow);
}

void Menu::handleEvent(sf::Event event) {
    if (mHashTableButton.handleEvent(mWindow, event)) {
        HashTableVisualizer visualizer(mWindow, mAssets);
        visualizer.run();
        return;
    }

    if (mAVLTreeButton.handleEvent(mWindow, event)) {
        AVLTreeVisualizer visualizer(mWindow, mAssets);
        visualizer.run();
        return;
    }

    if (mBinaryHeapButton.handleEvent(mWindow, event)) {
        BinaryHeapVisualizer visualizer(mWindow, mAssets);
        visualizer.run();
        return;
    }

    if (mTrieButton.handleEvent(mWindow, event)) {
        TrieVisualizer visualizer(mWindow, mAssets);
        visualizer.run();
        return;
    }

    if (mBTreeButton.handleEvent(mWindow, event)) {
        BTreeVisualizer visualizer(mWindow, mAssets);
        visualizer.run();
        return;
    }

    if (mGraphButton.handleEvent(mWindow, event)) {
        GraphVisualizer visualizer(mWindow, mAssets);
        visualizer.run();
        return;
    }
}

void Menu::draw() {
	sf::Sprite backgroundSprite, logoSprite;
	if (mAssets->getTheme() == AssetsData::Theme::light) {
		backgroundSprite.setTexture(*mAssets->get(AssetsData::Image::lightBackground));
        logoSprite.setTexture(*mAssets->get(AssetsData::Image::lightLogo));
	} else {
		backgroundSprite.setTexture(*mAssets->get(AssetsData::Image::darkBackground));
        logoSprite.setTexture(*mAssets->get(AssetsData::Image::darkLogo));
	}
	backgroundSprite.setPosition(sf::Vector2f(0, 0));
    logoSprite.setPosition(sf::Vector2f(400, 30));
	mWindow->draw(backgroundSprite);
    mWindow->draw(logoSprite);

    mWindow->draw(mHashTableButton);
    mWindow->draw(mAVLTreeButton);
    mWindow->draw(mBinaryHeapButton);
    mWindow->draw(mTrieButton);
    mWindow->draw(mBTreeButton);
    mWindow->draw(mGraphButton);
}

void Menu::run() {
    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
                return;
            }
            
            handleEvent(event);
        }

        mWindow->clear(sf::Color::White);

        draw();

        mWindow->display();
    }
}