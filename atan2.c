#include "atan2.h"

// Copied from https://github.com/insidegui/flux/blob/master/libc/math/atan2.c

double atan2(double y, double x) {
	if (y == 0.0) {
		if (x >= 0.0) {
			return 0.0;
		}
		else {
			return PI;
		}
	}
	else if (y > 0.0) {
		if (x == 0.0) {
			return 2 * PI;
		}
		else if (x > 0.0) {
			return atan(y / x);
		}
		else {
			return PI - atan(y / x);
		}
	}
	else {
		if (x == 0.0) {
			return PI + 2 * PI;
		}
		else if (x > 0.0) {
			return 2 * PI - atan(y / x);
		}
		else {
			return PI + atan(y / x);
		}
	}
}

