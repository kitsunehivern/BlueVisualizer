#include "Validator.hpp"

std::string Validator::isInteger(std::string value, std::string name) {
    if (!value.empty() && value.front() == '-') {
        value.erase(0, 1);
    }

    if (value.empty()) {
        return name + " is not an integer";
    }

    for (auto digit : value) {
        if (digit < '0' || digit > '9') {
            return name + " is not an integer";
        }
    }

    return "";
}

std::string Validator::isIntegerInRange(std::string value, std::string name, int min, int max) {
    std::string error = isInteger(value, name);
    if (!error.empty()) {
        return error;
    }

    int number;
    try {
        number = std::stoi(value);
    } catch (const std::invalid_argument& e) {
        return name + " is not an integer";
    } catch (const std::out_of_range& e) {
        return name + " is out of range [" + std::to_string(min) + ", " + std::to_string(max) + "]";
    }
    
    if (number < min || number > max) {
        return name + " is out of range [" + std::to_string(min) + ", " + std::to_string(max) + "]";
    }

    return "";
}

std::string Validator::isIntegerInRange(std::string value, std::string name, int* min, int* max) {
    return isIntegerInRange(value, name, *min, *max);
}

std::string Validator::isIntegerInRange(std::string value, std::string name, int* min, int max) {
    return isIntegerInRange(value, name, *min, max);
}

std::string Validator::isIntegerInRange(std::string value, std::string name, int min, int* max) {
    return isIntegerInRange(value, name, min, *max);
}

std::string Validator::isIntegerInRange(std::string value, std::string name, std::function<int()> min, std::function<int()> max) {
    return isIntegerInRange(value, name, min(), max());
}

std::string Validator::isListOfInteger(std::string list, std::string name) {
    if (list.empty()) {
        return name + " is not a list of integer";
    }

    for (auto& character : list) {
        if (!(character == '-' || (character >= '0' && character <= '9'))) {
            character = ' ';
        }
    }

    std::stringstream stream(list);
    std::string number;
    for (int i = 0; stream >> number; i++) {
        std::string error = isInteger(number, name + "[" + std::to_string(i) + "]");
        if (!error.empty()) {
            return error;
        }
    }

    return "";
}

std::string Validator::isListOfIntegerInRange(std::string list, std::string name, int minSize, int maxSize, int minValue, int maxValue) {
    std::string error;

    for (auto& character : list) {
        if (!(character == '-' || (character >= '0' && character <= '9'))) {
            character = ' ';
        }
    }

    std::stringstream stream(list);
    std::string number;
    bool notEmpty = false;
    int counter = 0;
    for (int i = 0; stream >> number; i++) {
        error = isIntegerInRange(number, name + "[" + std::to_string(i) + "]", minValue, maxValue);
        if (!error.empty()) {
            return error;
        }

        notEmpty = true;
        counter++;
    }

    if (!notEmpty) {
        return name + " is not a list of integer";
    }

    if (counter < minSize || counter > maxSize) {
        return "Size of " + name + " is out of range [" + std::to_string(minSize) + ", " + std::to_string(maxSize) + "]";
    }

    return "";
}

std::string Validator::isStringWithLowercaseLetters(std::string str, std::string name, int minLength, int maxLength) {
    for (auto& letter : str) {
        if (letter < 'a' || letter > 'z') {
            return name + " is not a string with lowercase letters";
        }
    }

    if ((int)str.size() < minLength || (int)str.size() > maxLength) {
        return "Length of " + name + " is out of range [" + std::to_string(minLength) + ", " + std::to_string(maxLength) + "]";
    }

    return "";
}

std::string Validator::isListOfStringWithLowercaseLetters(std::string list, std::string name, int minSize, int maxSize, int minLength, int maxLength) {
    std::string error;
    
    for (auto& character : list) {
        if (!(character >= 'a' && character <= 'z')) {
            character = ' ';
        }
    }

    std::stringstream stream(list);
    std::string str;
    bool notEmpty = false;
    int counter = 0;
    for (int i = 0; stream >> str; i++) {
        error = isStringWithLowercaseLetters(str, name + "[" + std::to_string(i) + "]", minLength, maxLength);
        if (!error.empty()) {
            return error;
        }

        notEmpty = true;
        counter++;
    }

    if (!notEmpty) {
        return name + " is not a list of string";
    }

    if (counter < minSize || counter > maxSize) {
        return "Size of " + name + " is out of range [" + std::to_string(minSize) + ", " + std::to_string(maxSize) + "]";
    }

    return "";
}