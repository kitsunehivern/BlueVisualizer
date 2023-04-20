#ifndef GLOBAL_H
#define GLOBAL_H

// Global libary
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <vector>
#include <tuple>
#include <string>
#include <math.h>
#include <assert.h>
#include <locale>
#include <codecvt>

// Global color
static sf::Color BLANK_COLOR                   = sf::Color(  0,   0,   0,   0);

static sf::Color NORMAL_NODE_CIRCLE_COLOR      = sf::Color(  0,   0,   0, 255);
static sf::Color HIGHLIGHT_NODE_CIRCLE_COLOR_1 = sf::Color(255, 140,  40, 255); // orange
static sf::Color HIGHLIGHT_NODE_CIRCLE_COLOR_2 = sf::Color( 45, 185, 210, 255); // blue
static sf::Color NORMAL_NODE_TEXT_COLOR        = sf::Color(  0,   0,   0, 255);
static sf::Color HIGHLIGHT_NODE_TEXT_COLOR_1   = sf::Color(255, 255, 255, 255); // white
static sf::Color HIGHLIGHT_NODE_TEXT_COLOR_2   = sf::Color(255, 140,  40, 255); // orange
static sf::Color INSERTED_NODE_CIRCLE_COLOR    = sf::Color( 80, 190, 105, 255);
static sf::Color ERASED_NODE_CIRCLE_COLOR      = sf::Color(215,  80,  60, 255);

static sf::Color LABEL_COLOR                   = sf::Color(255,   0,   0, 255);

static sf::Color NORMAL_EDGE_COLOR             = sf::Color(  0,   0,   0, 255);
static sf::Color HIGHLIGHT_EDGE_COLOR          = sf::Color(255, 138,  39, 255);
static sf::Color INSERTED_EDGE_COLOR           = sf::Color( 80, 190, 105, 255);
static sf::Color ERASED_EDGE_COLOR             = sf::Color(215,  80,  60, 255);

static sf::Color CODE_TEXT_COLOR               = sf::Color(  0,   0,   0, 255);
static sf::Color CODE_BAR_COLOR                = sf::Color(120, 220, 255, 255);

static sf::Color BOX_COLOR_1                   = sf::Color(170, 225, 245, 255);
static sf::Color BOX_COLOR_2                   = sf::Color(240, 250, 250, 255);
static sf::Color BOX_COLOR_3                   = sf::Color(120, 220, 255, 255);
static sf::Color BOX_COLOR_4                   = sf::Color( 45,  70, 100, 255);
static sf::Color BOX_TEXT_COLOR                = sf::Color(  0,   0,   0, 255);
static sf::Color BOX_EMPTY_TEXT_COLOR		   = sf::Color(125, 125, 125, 255);
static sf::Color SEPERATOR_LINE_COLOR_1        = sf::Color(145, 200, 220, 255);
static sf::Color SEPERATOR_LINE_COLOR_2        = sf::Color(220, 235, 235, 255);
static sf::Color SEPERATOR_LINE_COLOR_3        = sf::Color(250, 250, 250, 255);
static sf::Color INSERTION_POINT_COLOR         = sf::Color(  0,   0,   0, 255);

// Global function
static bool positionInRect(sf::Vector2i position, sf::FloatRect rectangle) {
	return rectangle.left < position.x && position.x < rectangle.left + rectangle.width && rectangle.top < position.y && position.y < rectangle.top + rectangle.height;
}

#endif