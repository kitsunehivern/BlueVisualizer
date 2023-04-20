#include "Random.h"

std::mt19937_64 mt_rng(std::chrono::steady_clock::now().time_since_epoch().count());

int randInt(int minValue, int maxValue) {
    return std::uniform_int_distribution <int> (minValue, maxValue) (mt_rng);
}