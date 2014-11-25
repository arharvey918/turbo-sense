/**
  * conf_reader.h is used to read from the sensor configuration file.
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
#include <ctype.h>
#include <fcntl.h>
#include "constants.h"

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

/** The sensor map for the configuration file */
int sensorMap[10] = { 24, 25, 16, 20, 21, 12, 18, 4, 17, 27 };

/** The filter map for the configuration file */
char *filterMap[5] =
    { "Raw", "Median", "EMA", "All combined", "Standard deviation" };


int startsWith(const char *a, const char *b);

void readConfig(struct Config *conf);