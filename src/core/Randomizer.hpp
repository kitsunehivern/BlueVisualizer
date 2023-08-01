#ifndef RANOMIZER_HPP
#define RANOMIZER_HPP

#include <random>
#include <chrono>
#include <string>
#include <functional>

namespace Randomizer {

std::string integerInRange(int min, int max);
std::string integerInRange(int* min, int* max);
std::string integerInRange(int* min, int max);
std::string integerInRange(int min, int* max);
std::string integerInRange(std::function<int()> min, std::function<int()> max);
std::string listOfIntegerInRange(int size, int min, int max);
std::string listOfIntegerInRange(int size, int* min, int* max);

}

#endif // RANOMIZER_HPP