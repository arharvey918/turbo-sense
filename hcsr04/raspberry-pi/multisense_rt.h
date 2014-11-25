/**
  * multisense_rt.h is used to perform concurrent ultrasonic sensor readings.
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

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pigpio.h>
#include <ctype.h>
#include <fcntl.h>

#include "constants.h"
#include "filters.h"
#include "pipe.h"
#include "prism.h"

/** Indicates if a pin has maxed out */
unsigned short maxedOut;

/** The system start time */
uint32_t sysStart;

/** The start time in system microseconds */
uint32_t start[MAX_SENSORS];

/** The end time in system microseconds */
uint32_t end[MAX_SENSORS];

int sensorMap[10] = { 24, 25, 16, 20, 21, 12, 18, 4, 17, 27 };
char *filterMap[5] =
    { "Raw", "Median", "EMA", "All combined", "Standard deviation" };

int startsWith(const char *a, const char *b);

void readConfig(struct Config *conf);

void interrupt_routine(int gpio, int level, uint32_t tick);

void setup();

void sendPulse();

int getCM(int sensor);

int main(int argc, char *argv[]);