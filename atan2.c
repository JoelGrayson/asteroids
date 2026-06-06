#include "atan2.h"

// Self-contained atan2 backed by a lazily-built arctangent lookup table.
// Returns the angle in radians in the range (-PI, PI], matching libc's atan2.
// No initialization call is required: the LUT is built on first use.

#define ATAN_LUT_SIZE 1024

static double atan_lut[ATAN_LUT_SIZE + 1];
static int atan_lut_ready = 0;

// Minimax polynomial approximation of atan(t) for t in [0, 1].
// Accurate to ~1e-5 radians; used only to seed the LUT once.
static double atan_poly(double t) {
    double t2 = t * t;
    return t * (0.9998660
        + t2 * (-0.3302995
        + t2 * (0.1801410
        + t2 * (-0.0851330
        + t2 * 0.0208351))));
}

static void atan_lut_init(void) {
    for (int i = 0; i <= ATAN_LUT_SIZE; i++) {
        double t = (double)i / (double)ATAN_LUT_SIZE; // 0 .. 1
        atan_lut[i] = atan_poly(t);
    }
    atan_lut_ready = 1;
}

// atan(t) for t in [0, 1] via the LUT with linear interpolation.
static double atan_unit(double t) {
    if (!atan_lut_ready) {
        atan_lut_init();
    }

    double pos = t * ATAN_LUT_SIZE;
    int idx = (int)pos;
    if (idx >= ATAN_LUT_SIZE) {
        return atan_lut[ATAN_LUT_SIZE];
    }

    double frac = pos - (double)idx;
    return atan_lut[idx] * (1.0 - frac) + atan_lut[idx + 1] * frac;
}

double atan2(double y, double x) {
    // On the vertical axis the ratio blows up, so handle it directly.
    if (x == 0.0) {
        if (y > 0.0) return PI / 2.0;
        if (y < 0.0) return -PI / 2.0;
        return 0.0; // undefined; match libc which returns 0
    }

    double ay = y < 0.0 ? -y : y;
    double ax = x < 0.0 ? -x : x;

    // Reduce to the first octant so the ratio fed to the LUT stays in [0, 1].
    double a;
    if (ay <= ax) {
        a = atan_unit(ay / ax);
    } else {
        a = PI / 2.0 - atan_unit(ax / ay);
    }

    // Reflect into the correct quadrant.
    if (x > 0.0) {
        return (y >= 0.0) ? a : -a;
    } else {
        return (y >= 0.0) ? (PI - a) : (a - PI);
    }
}
