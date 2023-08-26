#include "Menu.hpp"

Menu::Menu(sf::RenderWindow* window, AssetsHolder* assets) {
    mWindow = window;
    mAssets = assets;

    mThemeButton = Button(assets, AssetsData::Image::themeButton, VisualizerData::themeButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mHashTableButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(100.f, 300.f), ButtonData::ColorSet::set4);
    mHashTableButton.setTextInside("Hash Table");
    mAVLTreeButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(600.f, 300.f), ButtonData::ColorSet::set4);
    mAVLTreeButton.setTextInside("AVL Tree");
    mBinaryHeapButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(1100.f, 300.f), ButtonData::ColorSet::set4);
    mBinaryHeapButton.setTextInside("Binary Heap");
    mTrieButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(100.f, 600.f), ButtonData::ColorSet::set4);
    mTrieButton.setTextInside("Trie");
    mBTreeButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(600.f, 600.f), ButtonData::ColorSet::set4);
    mBTreeButton.setTextInside("2-3-4 Tree");
    mGraphButton = Button(mAssets, AssetsData::Image::titleBox, sf::Vector2f(1100.f, 600.f), ButtonData::ColorSet::set4);
    mGraphButton.setTextInside("Graph");
}

void Menu::updateState() {
    mThemeButton.updateState(mWindow);
    mBinaryHeapButton.updateState(mWindow);
    mHashTableButton.updateState(mWindow);
    mAVLTreeButton.updateState(mWindow);
    mTrieButton.updateState(mWindow);
    mBTreeButton.updateState(mWindow);
    mGraphButton.updateState(mWindow);
}

void Menu::handleEvent(sf::Event event) {
    if (mThemeButton.handleEvent(mWindow, event)) {
        mAssets->switchTheme();
        return;
    }

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

    sf::Text title("Dinh Cao Minh Quan - 22125078", *mAssets->get(AssetsData::Font::hyperspaceItalicBold), 30);
    title.setFillColor(*mAssets->get(AssetsData::Color::nodeText));
    title.setOrigin(sfhelper::getCenterPosition(title.getLocalBounds()).x, 0.f);
    title.setPosition(sf::Vector2f(800, 210));
    mWindow->draw(title);

    sf::Sprite themeBoxSprite(*mAssets->get(AssetsData::Image::themeBox));
    themeBoxSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    themeBoxSprite.setPosition(VisualizerData::themeButtonRect.getPosition() - sf::Vector2f(10.f, 10.f));
    mWindow->draw(themeBoxSprite);
    mWindow->draw(mThemeButton);
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