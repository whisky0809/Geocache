/*
 * color_classifier.h
 *
 *  Created on: Jun 4, 2025
 *      Author: phvs2
 */

#ifndef COLOR_CLASSIFIER_H
#define COLOR_CLASSIFIER_H


#include <stdint.h>
#include <stdbool.h>
#include "tcs34725_polling.h"
#include "color_math.h"



/*****************************************************************/


#if !defined(__has_builtin)
  #define __has_builtin(x) 0     /* for pre-C23 compilers */
#endif






#define MAX_BUCKETS 6

typedef struct {
    float r, g, b;
} RGB;

typedef struct {
    float h, s, l;
} HSL;

typedef struct {
    char name[16];
    float hCenter, sCenter, lCenter;
    float hTol, sTol, lTol;
} ColorBucket;

extern ColorBucket Buckets[MAX_BUCKETS];
extern size_t bucketCount;

RGB normalizeRGB(uint16_t r, uint16_t g, uint16_t b, uint16_t c);
HSL rgbToHsl(RGB in);
HSL readHslSample(void);
void calibrateNewBucket(void);
void printBuckets(void);
void editBucketTolerances(void);
int classifyColor(const HSL *hsl);
int getColor(void);


#endif // COLOR_CLASSIFIER_H
