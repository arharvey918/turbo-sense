/**
  * prism.c is used to create a six-sided virtual prism for Turbosense.
  *
  * Copyright 2014 by Avriel Harvey <arharvey@ncsu.edu>
  *
  * This file is part of Turbosense.
  *
  * Turbosense is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * Turbosense is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with Turbosense.  If not, see <http://www.gnu.org/licenses/>.
  *
  */

#include "prism.h"

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