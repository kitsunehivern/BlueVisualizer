#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "../../Global.hpp"
#include "../../core/Validator.hpp"
#include "../../core/Randomizer.hpp"
#include "../AssetsHolder.hpp"
#include "../component/OptionBox.hpp"
#include "../component/CodeBox.hpp"
#include "../object/Node.hpp"
#include "../object/Label.hpp"

namespace ControlBoxData {
    const float tableWidth = 880.f;
    const float tableHeight = 50.f;
    const sf::Vector2f controlBoxPosition = sf::Vector2f(10.f, 610.f);

    const sf::FloatRect frontButtonTextureRect = sf::FloatRect(0.f, 0.f, 20.f, 30.f);
    const sf::FloatRect prevButtonTextureRect = sf::FloatRect(20.f, 0.f, 20.f, 30.f);
    const sf::FloatRect nextButtonTextureRect = sf::FloatRect(40.f, 0.f, 20.f, 30.f);
    const sf::FloatRect backButtonTextureRect = sf::FloatRect(60.f, 0.f, 20.f, 30.f);
    const sf::FloatRect pausedButtonTextureRect = sf::FloatRect(0.f, 0.f, 30.f, 30.f);
    const sf::FloatRect continueButtonTextureRect = sf::FloatRect(30.f, 0.f, 30.f, 30.f);
    const sf::FloatRect replayButtonTextureRect = sf::FloatRect(60.f, 0.f, 30.f, 30.f);

    const sf::FloatRect frontButtonRect = sf::FloatRect(30.f, 10.f, 20.f, 30.f);
    const sf::FloatRect prevButtonRect = sf::FloatRect(90.f, 10.f, 20.f, 30.f);
    const sf::FloatRect nextButtonRect = sf::FloatRect(210.f, 10.f, 20.f, 30.f);
    const sf::FloatRect backButtonRect = sf::FloatRect(270.f, 10.f, 20.f, 30.f);
    const sf::FloatRect statusButtonRect = sf::FloatRect(145.f, 10.f, 30.f, 30.f);

    const sf::FloatRect videoBarRect = sf::FloatRect(320.f, 15.f, 400.f, 20.f);

    const sf::FloatRect speedX1TextureRect = sf::FloatRect(0.f, 0.f, 100.f, 30.f);
    const sf::FloatRect speedX2TextureRect = sf::FloatRect(100.f, 0.f, 100.f, 30.f);
    const sf::FloatRect speedX4TextureRect = sf::FloatRect(200.f, 0.f, 100.f, 30.f);
    const sf::FloatRect speedX8TextureRect = sf::FloatRect(300.f, 0.f, 100.f, 30.f);

    const sf::FloatRect speedButtonRect = sf::FloatRect(750.f, 10.f, 100.f, 30.f);
}

namespace VisualizerData {
    const int FPS = 120;
}

class Visualizer {
protected:
    Visualizer();
    Visualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void addNewStep();
    void abortAllSteps();
    void clearAllSteps();

    void draw(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor);
    void drawFadeIn(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor);
    void drawFadeOut(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor);
    void drawChangePosition(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor, std::vector<sf::Vector2f> oldPositions, std::vector<sf::Vector2f> newPositions);
    void drawChangeValue(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor, std::vector<std::string> oldValues, std::vector<std::string> newValues);
    void drawChangeColor(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, std::vector<AssetsData::Color> oldNodeColor, std::vector<AssetsData::Color> newNodeColor, std::vector<AssetsData::Color> oldValueColor, std::vector<AssetsData::Color> newValueColor);
    void drawChangeColor(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color oldNodeColor, AssetsData::Color newNodeColor, AssetsData::Color oldValueColor, AssetsData::Color newValueColor);

    void draw(std::vector<Label*> labels, AssetsData::Color color);
    void drawFadeIn(std::vector<Label*> labels, AssetsData::Color color);
    void drawFadeOut(std::vector<Label*> labels, AssetsData::Color color);
    void drawChangeName(std::vector<Label*> labels, AssetsData::Color color, std::vector<std::string> oldNames, std::vector<std::string> newNames);

    void drawCode();
    void drawCodeFadeIn(int focusLine);
    void drawCodeFadeOut(int focusLine);
    void drawCodeChangeLine(int oldFocusLine, int newFocusLine);

    void updateState();
    bool handleEvent(sf::Event event);
    void draw();

protected:
    typedef NodeData::Shape Shape;
    typedef NodeData::Type Type;
    typedef AssetsData::Color Color;

    sf::RenderWindow* mWindow;
    AssetsHolder* mAssets;
    OptionBox mOption;
    CodeBox mCode;
    
    Button mFrontButton;
    Button mPrevButton;
    Button mNextButton;
    Button mBackButton;
    Button mStatusButton;
    bool mIsVideoBarHolding;
    Button mSpeedButton;

    enum { PAUSED, CONTINUE, REPLAY } mStatus;
	enum { FORWARD, BACKWARD, NONE } mDirection;
	enum { X1 = 1, X2 = 2, X4 = 4, X8 = 8 } mSpeed;
    int mCurrentStep, mCurrentFrame;
    std::vector<std::vector<std::function<void(float, bool)>>> mDrawFunctions;

    sf::Texture* getNodeTexture(NodeData::Shape shape, NodeData::Type type);
    
	int getNumberOfFramePassed();
	void goToNextStep();
	void goToPrevStep();
	void goToEnding();
	void goToBeginning();
	void goToSpecificFrame(int step, int frame);
};

#endif // VISUALIZER_HPP