/*
 * color_math.h
 *
 *  Created on: Jun 10, 2025
 *      Author: phvs2
 */

#ifndef COLOR_MATH_H
#define COLOR_MATH_H

/* ────────────── basic min / max ────────────── */
static inline float fmaxf_local(float a, float b) { return (a > b) ? a : b; }
static inline float fminf_local(float a, float b) { return (a < b) ? a : b; }

/* Short aliases */
#define fmaxf  fmaxf_local
#define fminf  fminf_local

/* ────────────── clamp helper (0 … 1) ────────────── */
static inline float clamp01(float x)
{
    return (x < 0.f) ? 0.f : (x > 1.f ? 1.f : x);
}

/* ────────────────── minimal fmodf ──────────────────*/
static inline float fmodf_local(float x,float y)
{
    return x - (float)((int)(x / y)) * y;   /* works for positive y */
}
#define fmodf  fmodf_local

/* ────────────── simple fabsf ────────────── */
static inline float fabsf_local(float x) { return (x < 0.f) ? -x : x; }
#define fabsf  fabsf_local

#endif /* COLOR_MATH_H */
