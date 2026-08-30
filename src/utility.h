#pragma once

#include <math.h>

#define V(x,y) (CLITERAL(Vector2){x, y})
#define R(x,y,w,h) (CLITERAL(Rectangle){x, y, w, h})
#define R_SQ(x,y,w) (CLITERAL(Rectangle){x, y, w, w})

#define CLAMPf(a,b,x) ((x < a) ? a : (x > b) ? b : x)
#define CLAMPfN(x) (CLAMPf(0, 1, x))
#define LERPf(a,b,t) ((a * (1 - t)) + (b * t))
#define LERPfCLAMPED(a,b,t) ( LERPf(a,b,CLAMPfN(0,1,t)) )

#define ABS(a) (a < 0 ? (-(a)) : (a))
#define MIN(a,b) (a < b ? (a) : (b))
#define MAX(a,b) (a < b ? (b) : (a))

#define EASE_OUT_EXP(t) (t > 1 ? 1 : 1 - powf(2, -10 * t))
#define EASE_OUT_EXP_ALT(t, s) (t > 1 ? 1 : 1 - powf(2, -20 * ((t - s) / s)))
#define CHIT_EASE(t) (t < 0.8 ? 0.3f * t : EASE_OUT_EXP_ALT(t, 0.8f))

#define EASE_OUT_BACK(t) (1 + (2.70158 * powf(t - 1, 3)) + (1.70158 * powf(t - 1, 2)))
