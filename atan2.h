#pragma once

#include "maths.h"

// Self-contained atan2 (LUT-based). Works out of the box with no init call.
// Returns the angle in radians in the range (-PI, PI].
double atan2(double y, double x);
