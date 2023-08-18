#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <string>
#include <stdexcept>
#include <sstream>
#include <functional>

namespace Validator {

std::string isInteger(std::string value, std::string name);
std::string isIntegerInRange(std::string value, std::string name, int min, int max);
std::string isIntegerInRange(std::string value, std::string name, int* min, int* max);
std::string isIntegerInRange(std::string value, std::string name, int* min, int max);
std::string isIntegerInRange(std::string value, std::string name, int min, int* max);
std::string isIntegerInRange(std::string value, std::string name, std::function<int()> min, std::function<int()> max);
std::string isListOfInteger(std::string list, std::string name);
std::string isListOfIntegerInRange(std::string list, std::string name, int min, int max);
std::string isStringWithLowercaseLetters(std::string str, std::string name, int minSize, int maxSize);

}

#endif