#include "Visualizer.hpp"

Visualizer::Visualizer() {
}

Visualizer::Visualizer(sf::RenderWindow* window, AssetsHolder* assets) {
    mWindow = window;
    mAssets = assets;
    mOption = OptionBox(mAssets);

	mThemeButton = Button(assets, AssetsData::Image::themeButton, VisualizerData::themeButtonRect.getPosition(), ButtonData::ColorSet::set1);

    mFrontButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::frontButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mFrontButton.setImageRect(ControlBoxData::frontButtonTextureRect);

    mPrevButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::prevButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mPrevButton.setImageRect(ControlBoxData::prevButtonTextureRect);

    mNextButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::nextButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mNextButton.setImageRect(ControlBoxData::nextButtonTextureRect);

    mBackButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::backButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mBackButton.setImageRect(ControlBoxData::backButtonTextureRect);

    mStatusButton = Button(assets, AssetsData::Image::statusButtons, ControlBoxData::controlBoxPosition + ControlBoxData::statusButtonRect.getPosition(), ButtonData::ColorSet::set1);
	mStatusButton.setImageRect(ControlBoxData::pausedButtonTextureRect);

    mSpeed = X2;
	mSpeedButton = Button(assets, AssetsData::Image::speedButton, ControlBoxData::controlBoxPosition + ControlBoxData::speedButtonRect.getPosition(), ButtonData::ColorSet::set3);
	mSpeedButton.setImageInside(AssetsData::Image::speed);
	mSpeedButton.setImageInsideRect(ControlBoxData::speedX2TextureRect);

	mIsVideoBarHolding = false;
}

void Visualizer::addNewStep() {
	mDrawFunctions.push_back(std::vector<std::function<void(float, bool)>>());
}

void Visualizer::abortAllSteps() {
	goToEnding();
}

void Visualizer::clearAllSteps() {
	mStatus = CONTINUE;
	mDirection = FORWARD;

	mDrawFunctions.clear();
	mCurrentStep = 0;
	mCurrentFrame = 0;
}

int Visualizer::getNumberOfFramePassed() {
	return VisualizerData::FPS * mCurrentStep + mCurrentFrame;
}

void Visualizer::goToNextStep() {
	if (mStatus == PAUSED) {
		if (mDirection == FORWARD) {
            for (auto draw : mDrawFunctions[mCurrentStep]) {
                draw(1.f, false);
            }

            mCurrentFrame = VisualizerData::FPS;
            mStatus = (mCurrentStep == (int)mDrawFunctions.size() - 1) ? REPLAY : PAUSED;
            mDirection = NONE;
		} else if (mDirection == BACKWARD) {
			mDirection = FORWARD;
		} else if (mDirection == NONE) {
			mDirection = FORWARD;
			if (mCurrentFrame == VisualizerData::FPS && mCurrentStep < (int)mDrawFunctions.size() - 1) {
				mCurrentStep++;
				mCurrentFrame = 0;
			}
		}
	} else if (mStatus == CONTINUE) {
		assert(mDirection == FORWARD);

		if (mDirection == FORWARD) {
			mStatus = PAUSED;
		}
	}
}

void Visualizer::goToPrevStep() {
	if (mStatus == PAUSED || mStatus == REPLAY) {
		if (mDirection == BACKWARD) {
            for (auto draw : mDrawFunctions[mCurrentStep]) {
                draw(0.f, false);
            }

            mCurrentFrame = 0;
            mStatus = PAUSED;
            mDirection = NONE;
		} else if (mDirection == FORWARD) {
			mDirection = BACKWARD;
		} else if (mDirection == NONE) {
			if (mCurrentStep > 0 || mCurrentFrame > 0) {
				mDirection = BACKWARD;
			}

			if (mCurrentFrame == 0 && mCurrentStep > 0) {
				mCurrentStep--;
				mCurrentFrame = VisualizerData::FPS;
			}
		}
	} else if (mStatus == CONTINUE) {
		assert(mDirection == FORWARD);

		if (mDirection == FORWARD) {
			mStatus = PAUSED;
			mDirection = BACKWARD;
		}
	}
}

void Visualizer::goToEnding() {
	while (mCurrentStep < (int)mDrawFunctions.size() - 1 || mCurrentFrame < VisualizerData::FPS) {
		goToNextStep();
	}
}

void Visualizer::goToBeginning() {
	while (mCurrentStep > 0 || mCurrentFrame > 0) {
		goToPrevStep();
	}
}

void Visualizer::goToSpecificFrame(int step, int frame) {
	while (mCurrentStep < step) {
		for (auto draw : mDrawFunctions[mCurrentStep]) {
			draw(1.0f, false);
		}

		mCurrentStep++;
	}

	while (mCurrentStep > step) {
		for (auto draw : mDrawFunctions[mCurrentStep]) {
			draw(0.0f, false);
		}

		mCurrentStep--;
	}

	mCurrentFrame = frame;
}

sf::Texture* Visualizer::getNodeTexture(Shape shape, Type type) {
    if (shape == Shape::circle) {
        if (type == Type::hollow) {
            return mAssets->get(AssetsData::Image::hollowCircle);
        } else if (type == Type::filled) {
            return mAssets->get(AssetsData::Image::filledCircle);
        }
    } else {
        if (type == Type::hollow) {
            return mAssets->get(AssetsData::Image::hollowSquare);
        } else if (type == Type::filled) {
            return mAssets->get(AssetsData::Image::filledSquare);
        }
    }

	assert(false);
    return nullptr;
}

void Visualizer::draw(std::vector<GraphicNode*> nodes, Shape shape, Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&GraphicNode::draw, node, mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawFadeIn(std::vector<GraphicNode*> nodes, Shape shape, Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&GraphicNode::drawFadeIn, node, mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawFadeOut(std::vector<GraphicNode*> nodes, Shape shape, Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&GraphicNode::drawFadeOut, node, mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangePosition(std::vector<GraphicNode*> nodes, std::vector<sf::Vector2f> oldPositions, std::vector<sf::Vector2f> newPositions) {
	assert(nodes.size() == oldPositions.size() && nodes.size() == newPositions.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		if (oldPositions[i] == GraphicNodeData::initialPosition) {
			oldPositions[i] = newPositions[i];
		}

		mDrawFunctions.back().push_back(std::bind(&GraphicNode::drawChangePosition, nodes[i], oldPositions[i], newPositions[i], std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeValue(std::vector<GraphicNode*> nodes, Shape shape, Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor, std::vector<std::string> oldValues, std::vector<std::string> newValues) {
	assert(nodes.size() == oldValues.size() && nodes.size() == newValues.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		mDrawFunctions.back().push_back(std::bind(&GraphicNode::drawChangeValue, nodes[i], mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), oldValues[i], newValues[i], std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeColor(std::vector<GraphicNode*> nodes, Shape shape, Type type, std::vector<AssetsData::Color> oldNodeColor, std::vector<AssetsData::Color> newNodeColor, std::vector<AssetsData::Color> oldValueColor, std::vector<AssetsData::Color> newValueColor) {
	assert(nodes.size() == oldNodeColor.size() && nodes.size() == newNodeColor.size() && nodes.size() == oldValueColor.size() && nodes.size() == newValueColor.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		mDrawFunctions.back().push_back(std::bind(&GraphicNode::drawChangeColor, nodes[i], mWindow, getNodeTexture(shape, type), mAssets->get(oldNodeColor[i]), mAssets->get(newNodeColor[i]), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(oldValueColor[i]), mAssets->get(newValueColor[i]), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeColor(std::vector<GraphicNode*> nodes, Shape shape, Type type, AssetsData::Color oldNodeColor, AssetsData::Color newNodeColor, AssetsData::Color oldValueColor, AssetsData::Color newValueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&GraphicNode::drawChangeColor, node, mWindow, getNodeTexture(shape, type), mAssets->get(oldNodeColor), mAssets->get(newNodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(oldValueColor), mAssets->get(newValueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeValueColor(GraphicNode* node, Shape shape, Type type, Color oldNodeColor, Color newNodeColor, Color oldValueColor, Color newValueColor, std::string oldValues, std::string newValues) {
	mDrawFunctions.back().push_back(std::bind(&GraphicNode::drawChangeValueColor, node, mWindow, getNodeTexture(shape, type), mAssets->get(oldNodeColor), mAssets->get(newNodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(oldValueColor), mAssets->get(newValueColor), oldValues, newValues, std::placeholders::_1, std::placeholders::_2));
}

void Visualizer::drawEdge(std::vector<std::pair<GraphicNode*, GraphicNode*>> pnodes, Color color) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::draw, &edge, mWindow, pnode.first, pnode.second, mAssets->get(AssetsData::stick), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeFadeIn(std::vector<std::pair<GraphicNode*, GraphicNode*>> pnodes, Color color) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawFadeIn, &edge, mWindow, pnode.first, pnode.second, mAssets->get(AssetsData::stick), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeSlideIn(std::vector<std::pair<GraphicNode*, GraphicNode*>> pnodes, Color color) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawSlideIn, &edge, mWindow, pnode.first, pnode.second, mAssets->get(AssetsData::stick), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeSlideOut(std::vector<std::pair<GraphicNode*, GraphicNode*>> pnodes, Color color) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawSlideOut, &edge, mWindow, pnode.first, pnode.second, mAssets->get(AssetsData::stick), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeChangeColor(std::vector<std::pair<GraphicNode*, GraphicNode*>> pnodes, Color oldColor, Color newColor) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawChangeColor, &edge, mWindow, pnode.first, pnode.second, mAssets->get(AssetsData::stick), mAssets->get(oldColor), mAssets->get(newColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeChangeNode(std::vector<std::pair<GraphicNode*, std::pair<GraphicNode*, GraphicNode*>>> pnodes, Color color) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawChangeNode, &edge, mWindow, pnode.first, pnode.second.first, pnode.second.second, mAssets->get(AssetsData::stick), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeSlideOutChangeNode(std::vector<std::pair<GraphicNode*, std::pair<GraphicNode*, GraphicNode*>>> pnodes, Color color) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawSlideOutChangeNode, &edge, mWindow, pnode.first, pnode.second.first, pnode.second.second, mAssets->get(AssetsData::stick), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeFixed(std::vector<std::pair<GraphicNode*, GraphicNode*>> pnodes, Color color) {
	for (auto pnode : pnodes) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawFixed, &edge, mWindow, *pnode.first, *pnode.second, mAssets->get(AssetsData::stick), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawEdgeWeight(std::vector<std::pair<GraphicNode*, GraphicNode*>> pnodes, std::vector<std::string> weights, Color color) {
	assert(pnodes.size() == weights.size());
	for (int i = 0; i < (int)pnodes.size(); i++) {
		GraphicEdge edge;
		mDrawFunctions.back().push_back(std::bind(&GraphicEdge::drawWeight, &edge, mWindow, pnodes[i].first, pnodes[i].second, mAssets->get(AssetsData::stick), mAssets->get(color), mAssets->get(AssetsData::consolasBold), weights[i], std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawLabel(std::vector<GraphicNode*> nodes, std::vector<std::string> names, AssetsData::Color labelColor) {
	assert(nodes.size() == names.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		GraphicLabel label;
		mDrawFunctions.back().push_back(std::bind(&GraphicLabel::draw, &label, mWindow, nodes[i], names[i], mAssets->get(AssetsData::Font::consolasBold), mAssets->get(labelColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawLabelFadeIn(std::vector<GraphicNode*> nodes, std::vector<std::string> names, AssetsData::Color labelColor) {
	assert(nodes.size() == names.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		GraphicLabel label;
		mDrawFunctions.back().push_back(std::bind(&GraphicLabel::drawFadeIn, &label, mWindow, nodes[i], names[i], mAssets->get(AssetsData::Font::consolasBold), mAssets->get(labelColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawLabelFadeOut(std::vector<GraphicNode*> nodes, std::vector<std::string> names, AssetsData::Color labelColor) {
	assert(nodes.size() == names.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		GraphicLabel label;
		mDrawFunctions.back().push_back(std::bind(&GraphicLabel::drawFadeOut, &label, mWindow, nodes[i], names[i], mAssets->get(AssetsData::Font::consolasBold), mAssets->get(labelColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawLabelChangeName(std::vector<GraphicNode*> nodes, std::vector<std::string> oldNames, std::vector<std::string> newNames, Color color) {
	assert(nodes.size() == oldNames.size() && nodes.size() == newNames.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		GraphicLabel label;
		mDrawFunctions.back().push_back(std::bind(&GraphicLabel::drawChangeName, &label, mWindow, nodes[i], oldNames[i], newNames[i], mAssets->get(AssetsData::Font::consolasBold), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawLabelFixed(std::vector<GraphicNode*> nodes, std::vector<std::string> names, Color color) {
	assert(nodes.size() == names.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		GraphicLabel label;
		mDrawFunctions.back().push_back(std::bind(&GraphicLabel::drawFixed, &label, mWindow, *nodes[i], names[i], mAssets->get(AssetsData::Font::consolasBold), mAssets->get(color), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawCode() {
	mDrawFunctions.back().push_back(std::bind(&CodeBox::draw, mCode, mWindow, mAssets->get(AssetsData::Image::codeBox), mAssets->get(AssetsData::Color::boxComponent), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(AssetsData::Color::boxText), std::placeholders::_1, std::placeholders::_2));
}

void Visualizer::drawCodeFadeIn(int focusLine) {
	mDrawFunctions.back().push_back(std::bind(&CodeBox::drawFadeIn, mCode, mWindow, mAssets->get(AssetsData::Image::codeBox), mAssets->get(AssetsData::Color::boxComponent), mAssets->get(AssetsData::Image::codeBar), mAssets->get(AssetsData::Color::box), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(AssetsData::Color::boxText), focusLine, std::placeholders::_1, std::placeholders::_2));
}

void Visualizer::drawCodeFadeOut(int focusLine) {
	mDrawFunctions.back().push_back(std::bind(&CodeBox::drawFadeOut, mCode, mWindow, mAssets->get(AssetsData::Image::codeBox), mAssets->get(AssetsData::Color::boxComponent), mAssets->get(AssetsData::Image::codeBar), mAssets->get(AssetsData::Color::box), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(AssetsData::Color::boxText), focusLine, std::placeholders::_1, std::placeholders::_2));
}

void Visualizer::drawCodeChangeLine(int oldFocusLine, int newFocusLine) {
	mDrawFunctions.back().push_back(std::bind(&CodeBox::drawChangeLine, mCode, mWindow, mAssets->get(AssetsData::Image::codeBox), mAssets->get(AssetsData::Color::boxComponent), mAssets->get(AssetsData::Image::codeBar), mAssets->get(AssetsData::Color::box), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(AssetsData::Color::boxText), oldFocusLine, newFocusLine, std::placeholders::_1, std::placeholders::_2));
}

void Visualizer::updateState() {
    mOption.updateState(mWindow);

	mThemeButton.updateState(mWindow);

    mFrontButton.updateState(mWindow);
    mPrevButton.updateState(mWindow);
    mNextButton.updateState(mWindow);
    mBackButton.updateState(mWindow);
    mStatusButton.updateState(mWindow);

	if (mIsVideoBarHolding) {
		float mousePositionX = sf::Mouse::getPosition(*mWindow).x - ControlBoxData::videoBarRect.getPosition().x - ControlBoxData::controlBoxPosition.x;
		mousePositionX = std::max(mousePositionX, 0.f);
		mousePositionX = std::min(mousePositionX, ControlBoxData::videoBarRect.getSize().x);

		int step = mousePositionX * (int)mDrawFunctions.size() / ControlBoxData::videoBarRect.getSize().x;
		int frame = (mousePositionX - step * ControlBoxData::videoBarRect.getSize().x / (int)mDrawFunctions.size()) / (ControlBoxData::videoBarRect.getSize().x / (int)mDrawFunctions.size()) * VisualizerData::FPS;
		if (step == (int)mDrawFunctions.size()) {
			step--;
			frame = VisualizerData::FPS;
		}

		goToSpecificFrame(step, frame);

		mStatus = (step == (int)mDrawFunctions.size() - 1 && frame == VisualizerData::FPS) ? REPLAY : PAUSED;
		mDirection = NONE;
	}

	if (mStatus == PAUSED) {
		if (mDirection == FORWARD) {
			if (mCurrentFrame < VisualizerData::FPS) {
				mCurrentFrame += std::min((int)mSpeed, VisualizerData::FPS - mCurrentFrame);
			} else {
				mStatus = (mCurrentStep == (int)mDrawFunctions.size() - 1) ? REPLAY : PAUSED;
				mDirection = NONE;
			}
		} else if (mDirection == BACKWARD) {
			if (mCurrentFrame > 0) {
				mCurrentFrame -= std::min((int)mSpeed, mCurrentFrame);
			} else {
				mDirection = NONE;
			}
		}
	} else if (mStatus == CONTINUE) {
		mDirection = FORWARD;

		if (mDirection == FORWARD) {
			if (mCurrentFrame < VisualizerData::FPS) {
				mCurrentFrame += std::min((int)mSpeed, VisualizerData::FPS - mCurrentFrame);
			} else {
				if (mCurrentStep < (int)mDrawFunctions.size() - 1) {
					mCurrentStep++;
					mCurrentFrame = 0;

					mCurrentFrame += std::min((int)mSpeed, VisualizerData::FPS - mCurrentFrame);
				} else {
					mStatus = REPLAY;
					mDirection = NONE;
				}
			}
		}
	} else if (mStatus == REPLAY) {
		assert(mCurrentStep == (int)mDrawFunctions.size() - 1 && mCurrentFrame == VisualizerData::FPS);

		if (mDirection == BACKWARD) {
			mStatus = PAUSED;
			if (mCurrentFrame > 0) {
				mCurrentFrame -= std::min((int)mSpeed, mCurrentFrame);
			} else {
				mDirection = NONE;
			}
		}
	}

    if (mCurrentStep == 0 && mCurrentFrame == 0) {
        mFrontButton.setDisabled(true);
        mPrevButton.setDisabled(true);
    } else {
        mFrontButton.setDisabled(false);
        mPrevButton.setDisabled(false);
    }

    if (mCurrentStep == (int)mDrawFunctions.size() - 1 && mCurrentFrame == VisualizerData::FPS) {
        mNextButton.setDisabled(true);
        mBackButton.setDisabled(true);
    } else {
        mNextButton.setDisabled(false);
        mBackButton.setDisabled(false);
    }

    if (mStatus == PAUSED && mDirection == NONE) {
        mStatusButton.setImageRect(ControlBoxData::continueButtonTextureRect);
    } else if (mStatus == REPLAY) {
        mStatusButton.setImageRect(ControlBoxData::replayButtonTextureRect);
    } else {
        mStatusButton.setImageRect(ControlBoxData::pausedButtonTextureRect);
    }

	mSpeedButton.updateState(mWindow);
}

bool Visualizer::handleEvent(sf::Event event) {
    if (mFrontButton.handleEvent(mWindow, event)) {
        goToBeginning();
    } else if (mPrevButton.handleEvent(mWindow, event)) {
        goToPrevStep();
    } else if (mNextButton.handleEvent(mWindow, event)) {
        goToNextStep();
    } else if (mBackButton.handleEvent(mWindow, event)) {
        goToEnding();
    } else if (mStatusButton.handleEvent(mWindow, event)) {
        if (mStatus == PAUSED) {
            if (mDirection == NONE) {
                mStatus = CONTINUE;
            } else {
                mDirection = NONE;
            }
        } else if (mStatus == CONTINUE) {
            mStatus = PAUSED;
            mDirection = NONE;
        } else if (mStatus == REPLAY) {
            goToBeginning();
            mStatus = CONTINUE;
            mDirection = FORWARD;
        }
    } else if (mThemeButton.handleEvent(mWindow, event)) {
		mAssets->switchTheme();
	}
	
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (sfhelper::isMouseOver(mWindow, ControlBoxData::controlBoxPosition + ControlBoxData::videoBarRect.getPosition(), ControlBoxData::videoBarRect.getSize())) {
				mIsVideoBarHolding = true;
			}
		}
	} else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			mIsVideoBarHolding = false;
		}
	} else if (event.type == sf::Event::KeyPressed) {
		if (!mOption.isAnyInputBoxSelected()) {
			switch (event.key.code) {
			case sf::Keyboard::Home:
				goToBeginning();
				break;

			case sf::Keyboard::Left:
				goToPrevStep();
				break;

			case sf::Keyboard::Right:
				goToNextStep();
				break;

			case sf::Keyboard::End:
				goToEnding();
				break;

			case sf::Keyboard::Space:
				if (mStatus == PAUSED) {
					if (mDirection == NONE) {
						mStatus = CONTINUE;
					} else {
						mDirection = NONE;
					}
				} else if (mStatus == CONTINUE) {
					mStatus = PAUSED;
					mDirection = NONE;
				} else if (mStatus == REPLAY) {
					goToBeginning();
					mStatus = CONTINUE;
					mDirection = FORWARD;
				}

				break;

			default:
				break;
			}
		}
	}

	if (mSpeedButton.handleEvent(mWindow, event)) {
		if (mSpeed == X1) {
			mSpeed = X2;
			mSpeedButton.setImageInsideRect(ControlBoxData::speedX2TextureRect);
		} else if (mSpeed == X2) {
			mSpeed = X4;
			mSpeedButton.setImageInsideRect(ControlBoxData::speedX4TextureRect);
		} else if (mSpeed == X4) {
			mSpeed = X8;
			mSpeedButton.setImageInsideRect(ControlBoxData::speedX8TextureRect);
		} else {
			mSpeed = X1;
			mSpeedButton.setImageInsideRect(ControlBoxData::speedX1TextureRect);
		}
	}

	return mOption.handleEvent(mWindow, event);
}

void Visualizer::draw() {
	sf::Sprite backgroundSprite;
	if (mAssets->getTheme() == AssetsData::Theme::light) {
		backgroundSprite.setTexture(*mAssets->get(AssetsData::Image::lightBackground));
	} else {
		backgroundSprite.setTexture(*mAssets->get(AssetsData::Image::darkBackground));
	}
	backgroundSprite.setPosition(sf::Vector2f(0, 0));
	mWindow->draw(backgroundSprite);

    mWindow->draw(mOption);

    sf::Sprite controlBoxSprite(*mAssets->get(AssetsData::Image::controlBox));
    controlBoxSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    controlBoxSprite.setPosition(ControlBoxData::controlBoxPosition);
    mWindow->draw(controlBoxSprite);

    mWindow->draw(mFrontButton);
    mWindow->draw(mPrevButton);
    mWindow->draw(mNextButton);
    mWindow->draw(mBackButton);
    mWindow->draw(mStatusButton);

	sf::Sprite videoBarSprite(*mAssets->get(AssetsData::Image::videoBar));
	videoBarSprite.setColor(*mAssets->get(AssetsData::Color::box));
	videoBarSprite.setPosition(ControlBoxData::controlBoxPosition + ControlBoxData::videoBarRect.getPosition());
	mWindow->draw(videoBarSprite);

	videoBarSprite.setTextureRect(sf::IntRect(0, 0, std::floor(ControlBoxData::videoBarRect.getSize().x * getNumberOfFramePassed() / (VisualizerData::FPS * (int)mDrawFunctions.size())), ControlBoxData::videoBarRect.getSize().y));
	videoBarSprite.setColor(*mAssets->get(AssetsData::Color::boxFocus));
	mWindow->draw(videoBarSprite);

	sf::RectangleShape seperatorLine(sf::Vector2f(1, 20));
	seperatorLine.setFillColor(*mAssets->get(AssetsData::Color::boxComponent));
	seperatorLine.setOrigin(sfhelper::getCenterPosition(seperatorLine.getGlobalBounds()));
	for (int i = 1; i < (int)mDrawFunctions.size(); i++) {
		seperatorLine.setPosition(ControlBoxData::controlBoxPosition + ControlBoxData::videoBarRect.getPosition() + sf::Vector2f(std::floor(ControlBoxData::videoBarRect.getSize().x * i / (int)mDrawFunctions.size()), ControlBoxData::videoBarRect.getSize().y / 2));
		mWindow->draw(seperatorLine);
	}

	mWindow->draw(mSpeedButton);

	sf::Sprite VisualizeBoxSprite(*mAssets->get(AssetsData::Image::visualizeBox));
	VisualizeBoxSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
	VisualizeBoxSprite.setPosition(VisualizerData::visualizeBoxRect.getPosition());
	mWindow->draw(VisualizeBoxSprite);

	mWindow->draw(mThemeButton);

	for (auto draw : mDrawFunctions[mCurrentStep]) {
		draw(mCurrentFrame / (float)VisualizerData::FPS, false);
	}

	for (auto draw : mDrawFunctions[mCurrentStep]) {
		draw(mCurrentFrame / (float)VisualizerData::FPS, true);
	}
}