#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pigpio.h>
#include <ctype.h>
#include <fcntl.h>

#define TRUE      1
#define MAX_SENSORS   10        // The number of sensors
#define MEDIAN_READINGS  5      // The number of readings to take for the median
#define SENSITVTY 4             // The nth number from the sorted set of readings
#define MAX_RANGE 400           // The maximum sensor range

#define NF_RAW    0             // Noise filter number for raw data
#define NF_MEDIAN 1             // Noise filter number for median
#define NF_EMA    2             // Noise filter number for EMA
#define NF_ALL    3             // Noise filter number for EMA of median
#define NF_STD    4             // Noise filter number for standard deviaiton
