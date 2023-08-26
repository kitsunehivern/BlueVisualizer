#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "../../Global.hpp"
#include "../../core/Validator.hpp"
#include "../../core/Randomizer.hpp"
#include "../AssetsHolder.hpp"
#include "../component/OptionBox.hpp"
#include "../component/CodeBox.hpp"
#include "../object/GraphicNode.hpp"
#include "../object/GraphicEdge.hpp"
#include "../object/GraphicLabel.hpp"

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

    const sf::FloatRect visualizeBoxRect = sf::FloatRect(10.f, 10.f, 1580.f, 580.f);
    const sf::Vector2f visualizeBoxCenter = sf::Vector2f(visualizeBoxRect.left + visualizeBoxRect.width / 2, visualizeBoxRect.top + visualizeBoxRect.height / 2);
    const sf::FloatRect quitButtonRect = sf::FloatRect(20.f, 20.f, 40.f, 40.f);
    const sf::FloatRect themeButtonRect = sf::FloatRect(1540.f, 20.f, 40.f, 40.f);

    enum Event {
        none,
        quit,
        confirm,
    };
}

class Visualizer {
protected:
    typedef GraphicNodeData::Shape Shape;
    typedef GraphicNodeData::Type Type;
    typedef AssetsData::Color Color;

    Visualizer();
    Visualizer(sf::RenderWindow* window, AssetsHolder* assets);
    ~Visualizer();

    void addNewStep();
    void abortAllSteps();
    void clearAllSteps();

    void draw(std::vector<GraphicNode*> nodes, Shape shape, Type type, Color nodeColor, Color valueColor);
    void drawFadeIn(std::vector<GraphicNode*> nodes, Shape shape, Type type, Color nodeColor, Color valueColor);
    void drawFadeOut(std::vector<GraphicNode*> nodes, Shape shape, Type type, Color nodeColor, Color valueColor);
    void drawChangePosition(std::vector<GraphicNode*> nodes, std::vector<sf::Vector2f> oldPositions, std::vector<sf::Vector2f> newPositions);
    void drawChangeValue(std::vector<GraphicNode*> nodes, Shape shape, Type type, Color nodeColor, Color valueColor, std::vector<std::string> oldValues, std::vector<std::string> newValues);
    void drawChangeColor(std::vector<GraphicNode*> nodes, Shape shape, Type type, std::vector<Color> oldNodeColor, std::vector<Color> newNodeColor, std::vector<Color> oldValueColor, std::vector<Color> newValueColor);
    void drawChangeColor(std::vector<GraphicNode*> nodes, Shape shape, Type type, Color oldNodeColor, Color newNodeColor, Color oldValueColor, Color newValueColor);
    void drawChangeValueColor(GraphicNode* node, Shape shape, Type type, Color oldNodeColor, Color newNodeColor, Color oldValueColor, Color newValueColor, std::string oldValues, std::string newValues);

    void drawEdge(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, Color color);
    void drawEdgeFadeIn(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, Color color);
    void drawEdgeSlideIn(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, Color color);
    void drawEdgeSlideOut(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, Color color);
    void drawEdgeChangeColor(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, Color oldColor, Color newColor);
    void drawEdgeChangeNode(std::vector<std::pair<GraphicNode*, std::pair<GraphicNode*, GraphicNode*>>> nodes, Color color);
    void drawEdgeSlideOutChangeNode(std::vector<std::pair<GraphicNode*, std::pair<GraphicNode*, GraphicNode*>>> nodes, Color color);
    void drawEdgeFixed(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, Color color);

    void drawEdgeWeight(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, std::vector<std::string> weights, Color color);
    void drawEdgeWeightFadeOut(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, std::vector<std::string> weights, Color color);
    void drawEdgeWeightSlideIn(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, std::vector<std::string> weights, Color color);
    void drawEdgeWeightChangeColor(std::vector<std::pair<GraphicNode*, GraphicNode*>> nodes, std::vector<std::string> weights, Color oldColor, Color newColor);
    
    void drawEdgeFree(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> positions, Color color);
    void drawEdgeFreeFadeIn(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> positions, Color color);
    void drawEdgeFreeSlideIn(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> positions, Color color);
    void drawEdgeFreeSlideOut(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> positions, Color color);
    void drawEdgeFreeChangeColor(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> positions, Color oldColor, Color newColor);
    void drawEdgeFreeChangePosition(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> oldPositions, std::vector<std::pair<sf::Vector2f, sf::Vector2f>> newPositions, Color color);
    void drawEdgeFreeChangePositionSlideIn(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> oldPositions, std::vector<std::pair<sf::Vector2f, sf::Vector2f>> newPositions, Color color);
    void drawEdgeFreeChangePositionSlideOut(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> oldPositions, std::vector<std::pair<sf::Vector2f, sf::Vector2f>> newPositions, Color color);

    void drawLabel(std::vector<GraphicNode*> nodes, std::vector<std::string> names, Color color);
    void drawLabelFadeIn(std::vector<GraphicNode*> nodes, std::vector<std::string> names, Color color);
    void drawLabelFadeOut(std::vector<GraphicNode*> nodes, std::vector<std::string> names, Color color);
    void drawLabelChangeName(std::vector<GraphicNode*> nodes, std::vector<std::string> oldNames, std::vector<std::string> newNames, Color color);
    void drawLabelFixed(std::vector<GraphicNode*> nodes, std::vector<std::string> names, Color color);

    void drawCode();
    void drawCodeFadeIn(int focusLine);
    void drawCodeFadeOut(int focusLine);
    void drawCodeChangeLine(int oldFocusLine, int newFocusLine);

    void updateState();
    VisualizerData::Event handleEvent(sf::Event event);
    void draw();

protected:
    sf::RenderWindow* mWindow;
    AssetsHolder* mAssets;
    OptionBox mOption;
    CodeBox mCode;
    
private:
    Button mQuitButton;
    Button mThemeButton;
    
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

    sf::Texture* getNodeTexture(Shape shape, Type type);
    
	int getNumberOfFramePassed();
	void goToNextStep();
	void goToPrevStep();
	void goToEnding();
	void goToBeginning();
	void goToSpecificFrame(int step, int frame);
};

#endif // VISUALIZER_HPP