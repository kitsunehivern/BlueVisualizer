#ifndef MOTION_H
#define MOTION_H

namespace Motion {

	float linear(float time);
	float Bezier(float time);
	float parametric(float time, int alpha = 2);

} // namespace Motion

#endif