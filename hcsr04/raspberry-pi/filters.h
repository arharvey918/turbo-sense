/**
  * filters.h defines the noise filters for Turbosense.
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

int **medianValues;
int *stdevElimThreshold;

/**
  * Struct to hold all of the configuration options.
  *
  */
struct Config {
  int active[MAX_SENSORS];
  int readings;
  int noiseFilter;
  int delay;
  int pipe;
};

int ema(int sensor, int current);

int partitions(int low, int high, int *values);

int selection_algorithm(int left, int right, int kth, int *values);

int moving_median(int sensor, int current, int *values);

int stdev_elimination(int sensor, int current);

void create_stdev_threshold(int *threshold, int low, int high, int value);

void create_stdev_ranges(int *threshold);

int filter(struct Config *conf, int sensor, int value);
