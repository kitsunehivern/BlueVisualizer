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
#include <string>
#include <map>
#include <tuple>
#include <math.h>
#include <assert.h>
#include <locale>
#include <codecvt>
#include <any>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace sfhelper {

bool isMouseOver(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size);

}

#endif // GLOBAL_HPP