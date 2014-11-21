/**
 * Raspberry Pi code for concurrent ultrasonic sensor readings.
 * Created date: 11NOV2014
 *
 * Author: Avy Harvey (c) 2014
 */

#include <stdlib.h>
#include <stdio.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))

/**
  * Takes in a pointer to a 10-element array, and turns it into a 6-number array.
  * Output array format: Front Back Right Left Up Down
  */
void convertToPrism(int *elements, int *prism) {
  // Use sensor 1 and 2 for front
  prism[0] = min(elements[0], elements[1]);

  // Use sensor 5 and 6 for back
  prism[1] = min(elements[4], elements[5]);

  // Use sensor 3 and 4 for right
  prism[2] = min(elements[2], elements[3]);

  // Use sensor 7 and 8 for left
  prism[3] = min(elements[6], elements[7]);

  // Use sensor 9 for up
  prism[4] = elements[8];

  // Use sensor 10 for down
  prism[5] = elements[9];
}

void printPrism(int *prism) {
  printf("%d\t%d\t%d\t%d\t%d\t%d\n", prism[0], prism[1], prism[2], prism[3],
         prism[4], prism[5]);
}
