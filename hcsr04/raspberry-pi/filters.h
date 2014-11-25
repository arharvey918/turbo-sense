#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pigpio.h>
#include <ctype.h>
#include <fcntl.h>

#define MAX_SENSORS   10        // The number of sensors
#define MEDIAN_READINGS  5      // The number of readings to take for the median
#define SENSITVTY 4             // The nth number from the sorted set of readings
#define MAX_RANGE 400           // The maximum sensor range

#define NF_RAW    0             // Noise filter number for raw data
#define NF_MEDIAN 1             // Noise filter number for median
#define NF_EMA    2             // Noise filter number for EMA
#define NF_ALL    3             // Noise filter number for EMA of median
#define NF_STD    4             // Noise filter number for standard deviaiton


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
