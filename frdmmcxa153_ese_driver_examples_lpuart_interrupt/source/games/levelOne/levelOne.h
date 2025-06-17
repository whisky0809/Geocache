/*
 * levelOne.h
 *
 *  Created on: 15 Jun 2025
 *      Author: henri
 */

#ifndef GAMES_LEVELONE_LEVELONE_H_
#define GAMES_LEVELONE_LEVELONE_H_

#include <stdbool.h>
#define MAX_SCORE 20
#define BASE_TIMEOUT_MS 1500  // Faster reaction time

typedef enum { UP, DOWN, LEFT, RIGHT, NONE } Direction;


bool levelOne(void);
#endif /* GAMES_LEVELONE_LEVELONE_H_ */
