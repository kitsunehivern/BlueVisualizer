#include "Animation.hpp"

float Animation::easeIn(float ratioTime) {
    return ratioTime * ratioTime;
}

float Animation::easeOut(float ratioTime) {
    return ratioTime * (2 - ratioTime);
}

float Animation::easeInOut(float ratioTime) {
    return ratioTime < 0.5 ? 2 * ratioTime * ratioTime : -1 + (4 - 2 * ratioTime) * ratioTime;
}

float Animation::Bezier(float ratioTime) {
    return 3.f * ratioTime * ratioTime - 2.f * ratioTime * ratioTime * ratioTime;
}

float Animation::linear(float ratioTime) {
    return ratioTime;
}