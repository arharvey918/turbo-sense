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
