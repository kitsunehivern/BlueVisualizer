#include "Visualizer.hpp"

Visualizer::Visualizer() {
}

Visualizer::Visualizer(sf::RenderWindow* window, AssetsHolder* assets) {
    mWindow = window;
    mAssets = assets;
    mOption = OptionBox(mAssets);

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

sf::Texture* Visualizer::getNodeTexture(NodeData::Shape shape, NodeData::Type type) {
    if (shape == NodeData::Shape::circle) {
        if (type == NodeData::Type::hollow) {
            return mAssets->get(AssetsData::Image::hollowCircle);
        } else if (type == NodeData::Type::filled) {
            return mAssets->get(AssetsData::Image::filledCircle);
        }
    } else {
        if (type == NodeData::Type::hollow) {
            return mAssets->get(AssetsData::Image::hollowSquare);
        } else if (type == NodeData::Type::filled) {
            return mAssets->get(AssetsData::Image::filledSquare);
        } else if (type == NodeData::Type::marked) {
			return mAssets->get(AssetsData::Image::markedSquare);
		}
    }

	assert(false);
    return nullptr;
}

void Visualizer::draw(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&Node::draw, node, mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawFadeIn(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&Node::drawFadeIn, node, mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawFadeOut(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&Node::drawFadeOut, node, mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangePosition(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor, std::vector<sf::Vector2f> oldPositions, std::vector<sf::Vector2f> newPositions) {
	assert(nodes.size() == oldPositions.size() && nodes.size() == newPositions.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		mDrawFunctions.back().push_back(std::bind(&Node::drawChangePosition, nodes[i], mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), oldPositions[i], newPositions[i], std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeValue(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color nodeColor, AssetsData::Color valueColor, std::vector<std::string> oldValues, std::vector<std::string> newValues) {
	assert(nodes.size() == oldValues.size() && nodes.size() == newValues.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		mDrawFunctions.back().push_back(std::bind(&Node::drawChangeValue, nodes[i], mWindow, getNodeTexture(shape, type), mAssets->get(nodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(valueColor), oldValues[i], newValues[i], std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeColor(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, std::vector<AssetsData::Color> oldNodeColor, std::vector<AssetsData::Color> newNodeColor, std::vector<AssetsData::Color> oldValueColor, std::vector<AssetsData::Color> newValueColor) {
	assert(nodes.size() == oldNodeColor.size() && nodes.size() == newNodeColor.size() && nodes.size() == oldValueColor.size() && nodes.size() == newValueColor.size());
	for (int i = 0; i < (int)nodes.size(); i++) {
		mDrawFunctions.back().push_back(std::bind(&Node::drawChangeColor, nodes[i], mWindow, getNodeTexture(shape, type), mAssets->get(oldNodeColor[i]), mAssets->get(newNodeColor[i]), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(oldValueColor[i]), mAssets->get(newValueColor[i]), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeColor(std::vector<Node*> nodes, NodeData::Shape shape, NodeData::Type type, AssetsData::Color oldNodeColor, AssetsData::Color newNodeColor, AssetsData::Color oldValueColor, AssetsData::Color newValueColor) {
	for (auto node : nodes) {
		mDrawFunctions.back().push_back(std::bind(&Node::drawChangeColor, node, mWindow, getNodeTexture(shape, type), mAssets->get(oldNodeColor), mAssets->get(newNodeColor), mAssets->get(AssetsData::Font::consolasBold), mAssets->get(oldValueColor), mAssets->get(newValueColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::draw(std::vector<Label*> labels, AssetsData::Color labelColor) {
	for (auto label : labels) {
		mDrawFunctions.back().push_back(std::bind(&Label::draw, label, mWindow, mAssets->get(AssetsData::Font::consolasBold), mAssets->get(labelColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawFadeIn(std::vector<Label*> labels, AssetsData::Color labelColor) {
	for (auto label : labels) {
		mDrawFunctions.back().push_back(std::bind(&Label::drawFadeIn, label, mWindow, mAssets->get(AssetsData::Font::consolasBold), mAssets->get(labelColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawFadeOut(std::vector<Label*> labels, AssetsData::Color labelColor) {
	for (auto label : labels) {
		mDrawFunctions.back().push_back(std::bind(&Label::drawFadeOut, label, mWindow, mAssets->get(AssetsData::Font::consolasBold), mAssets->get(labelColor), std::placeholders::_1, std::placeholders::_2));
	}
}

void Visualizer::drawChangeName(std::vector<Label*> labels, AssetsData::Color labelColor, std::vector<std::string> oldNames, std::vector<std::string> newNames) {
	assert(labels.size() == oldNames.size() && labels.size() == newNames.size());
	for (int i = 0; i < (int)labels.size(); i++) {
		mDrawFunctions.back().push_back(std::bind(&Label::drawChangeName, labels[i], mWindow, mAssets->get(AssetsData::Font::consolasBold), mAssets->get(labelColor), oldNames[i], newNames[i], std::placeholders::_1, std::placeholders::_2));
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

		default:
			break;
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

	for (auto draw : mDrawFunctions[mCurrentStep]) {
		draw(mCurrentFrame / (float)VisualizerData::FPS, false);
	}

	for (auto draw : mDrawFunctions[mCurrentStep]) {
		draw(mCurrentFrame / (float)VisualizerData::FPS, true);
	}
}