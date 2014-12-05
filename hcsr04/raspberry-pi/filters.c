/**
  * filters.c defines the noise filters for Turbosense.
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

#include "filters.h"

/**
 * Calculates an exponential moving average given a sensor and the current value.
 */
int ema(int sensor, int current) {
  // Define static variables
  static float alpha = 0.3;
  static float lastema[MAX_SENSORS];
  static short initialized[MAX_SENSORS] = { 0 };
  float ema;

  // If initialized is set to 0, then we must be on the first observation.
  // Set the returned ema value to the current value and set the initialized flag to true.
  if (initialized[sensor] == 0) {
    ema = current;
    initialized[sensor] = 1;
  }
  
  // If the initialized flag has already been set, then there was a prior value.
  // We can now use the recursive formula to calculate the EMA. 
  else {
    ema = (alpha * current + (1 - alpha) * lastema[sensor]);
  }

  // Set the previous value to the current EMA calculation to be used on the next round.
  lastema[sensor] = ema;

  // Return the EMA as an integer (will truncate decimal)
  return (int)ema;
}

/**
  * Used to partition a list using the concept of quick-sort.
  * This method was modified from the following source:
  * http://discuss.codechef.com/questions/1489/find-median-in-an-unsorted-array-without-sorting-it 
  */
int partitions(int low, int high, int *values) {
  int p = low, r = high, x = values[r], i = p - 1;
  for (int j = p; j <= r - 1; j++) {
    if (values[j] <= x) {
      i = i + 1;
      int tmp = values[i];
      values[i] = values[j];
      values[j] = tmp;
    }
  }
  int tmp2 = values[i + 1];
  values[i + 1] = values[r];
  values[r] = tmp2;
  return i + 1;
}

/**
  * Finds the kth smallest number in an unsorted list using the selection algorithm.
  * This method was modified form the following source:
  * http://discuss.codechef.com/questions/1489/find-median-in-an-unsorted-array-without-sorting-it
  */
int selection_algorithm(int left, int right, int kth, int *values) {
  for (;;) {
    // Pivot between left and right
    int pivotIndex = partitions(left, right, values);
    int len = pivotIndex - left + 1;

    if (kth == len) {
      return values[pivotIndex];
    } else if (kth < len) {
      right = pivotIndex - 1;
    } else {
      kth = kth - len;
      left = pivotIndex + 1;
    }
  }
}

/**
  * Calculates the moving median of a set of numbers given the sensor, current value, and list of past values.
  */
int moving_median(int sensor, int current, int *values) {

  // Set up the static variables.  The 'count' array tells us how many past values have been stored.  'dupArray' is used to store the data that gets passed to the selection algorithm.  The selection algorithm has the potential to reorder the list, and we need the list to maintain the same order so we know which values are old and can be discarded from the moving filter.
  static short count[MAX_SENSORS] = { 0 };
  static int dupArray[MEDIAN_READINGS] = { 0 };

  // If there are less readings than the number we need to take a median, return the current value and store it in the array.
  if (count[sensor] < MEDIAN_READINGS) {
    values[count[sensor]] = current;
    count[sensor]++;
    return current;
  }

  // If the array containing the past readings is full, shift everything left (pushing the first observation out) and put the current reading in the end position of the array.  Take the median of this and return it.
  else {
    if (count[sensor] != MEDIAN_READINGS - 1) {
      // Shift everything one position left
      for (int i = 0; i < (MEDIAN_READINGS - 1); i++) {
        values[i] = values[i + 1];
      }
    } else {
      count[sensor] = MEDIAN_READINGS + 1;
    }
    values[MEDIAN_READINGS - 1] = current;

    // Duplicate array to pass so 'values' doesn't get sorted
    for (int i = 0; i < MEDIAN_READINGS; i++) {
      dupArray[i] = values[i];
    }

    // Return the median of the values
    return selection_algorithm(0, MEDIAN_READINGS - 1, 3, dupArray);
  }

  // We should never get to this point.
  return 0;
}

/**
  * The standard deviation filter performs a single-point based elimination.  If the difference between the last and current readings is greater than the specified value and the last reading was not eliminated, the last reading is returned and the elimination flag is set.  Otherwise, the current reading is returned and the elimination flag is unset.  The algorithm will never eliminate two successive readings.
  */
int stdev_elimination(int sensor, int current) {

  // Static variables to hold the last value information
  static int lastValue[MAX_SENSORS] = { 0 };
  static short lastEliminated[MAX_SENSORS] = { 0 };

  // If the last value hasn't been initialized, start now and return the current value
  if (lastValue[sensor] == 0) {
    lastValue[sensor] = current;
    lastEliminated[sensor] = 0;
    return current;
  }
  // Check if |current - last| is greater than the stdev for the last value
  int difference = abs(current - lastValue[sensor]);
  if (difference > stdevElimThreshold[lastValue[sensor]]
      && lastEliminated[sensor] == 0) {
    lastEliminated[sensor] = 1;
    return lastValue[sensor];
  } else {
    lastValue[sensor] = current;
    lastEliminated[sensor] = 0;
    return current;
  }

}

/**
  * Helper function that creates the standard deviation thresholds.
  */
void create_stdev_threshold(int low, int high, int value) {
  for (int i = low; i < high; i++) {
    stdevElimThreshold[i] = value;
  }
}

/**
  * Helper function that creates the standard deviation ranges.
  */
void create_stdev_ranges() {
  stdevElimThreshold = calloc(MAX_RANGE, sizeof(int));

  create_stdev_threshold(0, 50, 8);
  create_stdev_threshold(50, 100, 10);
  create_stdev_threshold(100, 150, 15);
  create_stdev_threshold(150, 200, 18);
  create_stdev_threshold(200, 250, 20);
  create_stdev_threshold(250, 300, 30);
  create_stdev_threshold(300, 400, 50);
}

/**
  * Runs a noise filter on the data based on the selected algorithm.
  *
  */
int filter(int noiseFilter, int sensor, int value) {
  // Raw data
  if (noiseFilter == NF_RAW) {
    return value;
  } 

  // Moving median filter
  else if (noiseFilter == NF_MEDIAN) {
    return moving_median(sensor, value, medianValues[sensor]);
  }

  // Exponential moving average filter 
  else if (noiseFilter == NF_EMA) {
    return ema(sensor, value);
  }

  // EMA of the moving median of the standard deviation filter of the raw data
  else if (noiseFilter == NF_ALL) {
    return ema(sensor,
               moving_median(sensor, stdev_elimination(sensor, value),
                             medianValues[sensor]));
  }
  
  // Standard deviation filter
  else if (noiseFilter == NF_STD) {
    return stdev_elimination(sensor, value);
  }

  // Anything else is invalid; return 0
  return 0;
}
