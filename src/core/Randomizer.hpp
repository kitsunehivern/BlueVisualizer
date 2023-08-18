#ifndef RANOMIZER_HPP
#define RANOMIZER_HPP

#include <random>
#include <chrono>
#include <string>
#include <functional>

namespace Randomizer {

static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

template<typename T> T random(T min, T max);
std::string integerInRange(int min, int max);
std::string integerInRange(int* min, int* max);
std::string integerInRange(int* min, int max);
std::string integerInRange(int min, int* max);
std::string integerInRange(std::function<int()> min, std::function<int()> max);
std::string listOfIntegerInRange(int size, int min, int max);
std::string listOfIntegerInRange(int size, int* min, int* max);
std::string stringOfLowercaseLetters(int size);
std::string stringOfLowercaseLetters(int minSize, int maxSize);

}

#endif // RANOMIZER_HPP