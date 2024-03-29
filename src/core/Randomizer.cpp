#include "Randomizer.hpp"

template<typename T>
T Randomizer::random(T min, T max) {
    return std::uniform_int_distribution<T>(min, max)(Randomizer::rng);
}

std::string Randomizer::integerInRange(int min, int max) {
    return std::to_string(Randomizer::random(min, max));
}

std::string Randomizer::integerInRange(int* min, int* max) {
    return Randomizer::integerInRange(*min, *max);
}

std::string Randomizer::integerInRange(int* min, int max) {
    return Randomizer::integerInRange(*min, max);
}

std::string Randomizer::integerInRange(int min, int* max) {
    return Randomizer::integerInRange(min, *max);
}

std::string Randomizer::integerInRange(std::function<int()> min, std::function<int()> max) {
    return Randomizer::integerInRange(min(), max());
}

std::string Randomizer::listOfIntegerInRange(int size, int min, int max) {
    std::string list = "";
    for (int i = 0; i < size; i++) {
        list += Randomizer::integerInRange(min, max);
        if (i < size - 1) {
            list += ", ";
        }
    }

    return list;
}

std::string Randomizer::listOfIntegerInRange(int size, int* min, int* max) {
    return Randomizer::listOfIntegerInRange(size, *min, *max);
}

std::string Randomizer::stringOfLowercaseLetters(int size) {
    std::string str = "";
    for (int i = 0; i < size; i++) {
        str += Randomizer::random('a', 'z');
    }

    return str;
}

std::string Randomizer::stringOfLowercaseLetters(int minSize, int maxSize) {
    return Randomizer::stringOfLowercaseLetters(Randomizer::random(minSize, maxSize));
}

float Randomizer::randomFloat(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(Randomizer::rng);
}