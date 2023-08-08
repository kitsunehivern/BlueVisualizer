#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <tuple>
#include <math.h>
#include <assert.h>
#include <locale>
#include <codecvt>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace sfhelper {

bool isMouseOver(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size);
bool isMouseOver(sf::RenderWindow* window, sf::FloatRect rect);
sf::Vector2f getCenterPosition(sf::Vector2f position, sf::Vector2f size);
sf::Vector2f getCenterPosition(sf::FloatRect rect);
float getDistance(sf::Vector2f position1, sf::Vector2f position2);
sf::IntRect toIntRect(sf::FloatRect rect);
sf::FloatRect toFloatRect(sf::IntRect rect);
char keyCodeToChar(sf::Keyboard::Key keyCode);

}

#endif // GLOBAL_HPP