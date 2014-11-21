#include "global.h"

/** The array to hold values for the median filter */
int **medianValues;

/** The array to hold threshold values for the standard deviation filter */
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

/**
 * Calculates an exponential moving average
 */
int ema(int sensor, int current) {
  static float alpha = 0.3;
  static float lastema[MAX_SENSORS];
  static short initialized[MAX_SENSORS] = { 0 };
  float ema;

  // If initialized is 0, then we must be on the first observation.
  if (initialized[sensor] == 0) {
    ema = current;
    initialized[sensor] = 1;
  } else {
    ema = (alpha * current + (1 - alpha) * lastema[sensor]);
  }
  lastema[sensor] = ema;

  return (int)ema;
}

/**
  * Recursive method to sort a list.
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
  * Finds the kth smallest number in an unsorted list.
  */
int selection_algorithm(int left, int right, int kth, int *values) {
  for (;;) {
    int pivotIndex = partitions(left, right, values); //Pivot Between Left and Right
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
  * Moving median filter.
  */
int moving_median(int sensor, int current, int *values) {

  static short count[MAX_SENSORS] = { 0 };
  static int dupArray[MEDIAN_READINGS] = { 0 };

  if (count[sensor] < MEDIAN_READINGS) {
    values[count[sensor]] = current;
    count[sensor]++;
    return current;
  } else {
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

    return selection_algorithm(0, MEDIAN_READINGS - 1, 3, dupArray);
  }

  return 0;
}

/**
  * The standard deviation filter.
  */
int stdev_elimination(int sensor, int current) {
  // Holds the last value information
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
  * Creates the standard deviation thresholds.
  */
void create_stdev_threshold(int *threshold, int low, int high, int value) {
  for (int i = low; i < high; i++) {
    threshold[i] = value;
  }
}

/**
  * Create the standard deviation ranges.
  */
void create_stdev_ranges(int *threshold) {
  create_stdev_threshold(threshold, 0, 50, 8);
  create_stdev_threshold(threshold, 50, 100, 10);
  create_stdev_threshold(threshold, 100, 150, 15);
  create_stdev_threshold(threshold, 150, 200, 18);
  create_stdev_threshold(threshold, 200, 250, 20);
  create_stdev_threshold(threshold, 250, 300, 30);
  create_stdev_threshold(threshold, 300, 400, 50);

  stdevElimThreshold = threshold;
}

/**
  * Filters the data based on the selected algorithm.
  *
  */
int filter(struct Config *conf, int sensor, int value) {
  if (conf->noiseFilter == NF_RAW) {
    return value;
  } else if (conf->noiseFilter == NF_MEDIAN) {
    return moving_median(sensor, value, medianValues[sensor]);
  } else if (conf->noiseFilter == NF_EMA) {
    return ema(sensor, value);
  } else if (conf->noiseFilter == NF_ALL) {
    return ema(sensor,
               moving_median(sensor, stdev_elimination(sensor, value),
                             medianValues[sensor]));
  } else if (conf->noiseFilter == NF_STD) {
    return stdev_elimination(sensor, value);
  }
  // Anything else is invalid; return 0
  return 0;
}
