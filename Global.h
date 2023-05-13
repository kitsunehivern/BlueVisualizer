#ifndef GLOBAL_H
#define GLOBAL_H

#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <vector>
#include <array>
#include <tuple>
#include <string>
#include <math.h>
#include <assert.h>
#include <locale>
#include <codecvt>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

static bool positionInRect(sf::Vector2i position, sf::FloatRect rectangle) {
	return rectangle.left < position.x && position.x < rectangle.left + rectangle.width && rectangle.top < position.y && position.y < rectangle.top + rectangle.height;
}

#endif