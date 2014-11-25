/**
 * Raspberry Pi code for concurrent ultrasonic sensor readings.
 * Created date: 11NOV2014
 *
 * Author: Avy Harvey (c) 2014
 */

#include <stdlib.h>
#include <stdio.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))

void convertToPrism(int *elements, int *prism);

void printPrism(int *prism);


