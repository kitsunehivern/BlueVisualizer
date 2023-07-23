#ifndef ANIMATION_HPP
#define ANIMATION_HPP

namespace Animation {
    float easeIn(float ratioTime);
    float easeOut(float ratioTime);
    float easeInOut(float ratioTime);
    float Bezier(float ratioTime);
    float linear(float ratioTime);
}

#endif // ANIMATION_HPP