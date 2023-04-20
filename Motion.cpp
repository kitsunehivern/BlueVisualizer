#include "Motion.h"

#include <assert.h>

float Motion::linear(float time) {
	assert(0 <= time && time <= 1);
	return time;
}

float Motion::Bezier(float time) {
	assert(0 <= time && time <= 1);
	return time * time * (3.0f - 2.0f * time);
}

float Motion::parametric(float time, int alpha) {
	assert(0 <= time && time <= 1);
	float left = 1.0f, right = 1.0f;
	while (alpha--) {
		left *= time;
		right *= (1.0f - time);
	}

	return left / (left + right);
}