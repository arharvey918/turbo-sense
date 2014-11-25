/**
  * multisense.c is used to perform concurrent ultrasonic sensor readings.
  * This uses the wiringPi library and is not ideal for real time applications.
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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define TRUE      1
#define SENSORS   10            // The number of sensors
#define READINGS  10            // The number of readings to take for the median
#define SENSITVTY 4             // The nth number from the sorted set of readings
#define TRIG      23            // Trigger pin
#define ECHO      24            // Echo pin for sensor 1
#define ECHO1     25            // Echo pin for sensor 2
#define ECHO2     16            // Echo pin for sensor 3
#define ECHO3     20            // Echo pin for sensor 4
#define ECHO4     21            // Echo pin for sensor 5
#define ECHO5     12            // Echo pin for sensor 6
#define ECHO6     18            // Echo pin for sensor 7
#define ECHO7     4             // Echo pin for sensor 8
#define ECHO8     17            // Echo pin for sensor 9
#define ECHO9     27            // Echo pin for sensor 10

/** Indicates if a pin has maxed out */
unsigned short maxedOut;

/** The system start time */
unsigned long sysStart;

/** The start time in system microseconds */
unsigned long start;

/** The end time in system microseconds */
unsigned long end[SENSORS];

/** Arrays to collect readings for medians */
int A[SENSORS][READINGS];

/**
  * Recursive method to sort a list.
  */
int partitions(int low, int high, int sensor) {
  int p = low, r = high, x = A[sensor][r], i = p - 1;
  for (int j = p; j <= r - 1; j++) {
    if (A[sensor][j] <= x) {
      i = i + 1;
      int tmp = A[sensor][i];
      A[sensor][i] = A[sensor][j];
      A[sensor][j] = tmp;
    }
  }
  int tmp2 = A[sensor][i + 1];
  A[sensor][i + 1] = A[sensor][r];
  A[sensor][r] = tmp2;
  return i + 1;
}

/**
  * Finds the kth smallest number in an unsorted list.
  */
int selection_algorithm(int left, int right, int kth, int sensor) {
  for (;;) {
    int pivotIndex = partitions(left, right, sensor); //Pivot Between Left and Right
    int len = pivotIndex - left + 1;

    if (kth == len) {
      return A[sensor][pivotIndex];
    } else if (kth < len) {
      right = pivotIndex - 1;
    } else {
      kth = kth - len;
      left = pivotIndex + 1;
    }
  }
}

void myInterrupt(void) {
  //printf("Caught interrupt for sensor 0\n");
  end[0] = micros();
}

void myInterrupt1(void) {
  //printf("Caught interrupt for sensor 1\n");
  end[1] = micros();
}

void myInterrupt2(void) {
  //printf("Caught interrupt for sensor 2\n");
  end[2] = micros();
}

void myInterrupt3(void) {
  //printf("Caught interrupt for sensor 3\n");
  end[3] = micros();
}

void myInterrupt4(void) {
  //printf("Caught interrupt for sensor 4\n");
  end[4] = micros();
}

void myInterrupt5(void) {
  //printf("Caught interrupt for sensor 5\n");
  end[5] = micros();
}

void myInterrupt6(void) {
  //printf("Caught interrupt for sensor 6\n");
  end[6] = micros();
}

void myInterrupt7(void) {
  //printf("Caught interrupt for sensor 7\n");
  end[7] = micros();
}

void myInterrupt8(void) {
  //printf("Caught interrupt for sensor 8\n");
  end[8] = micros();
}

void myInterrupt9(void) {
  //printf("Caught interrupt for sensor 9\n");
  end[9] = micros();
}

/**
  * Set up the GPIO pins on the Raspberry Pi.
  */
void setup() {
  // Set up the library to use the Broadcom GPIO numbers
  wiringPiSetupGpio();

  // Set up the GPIO pins for I/O mode
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(ECHO1, INPUT);
  pinMode(ECHO2, INPUT);
  pinMode(ECHO3, INPUT);
  pinMode(ECHO4, INPUT);
  pinMode(ECHO5, INPUT);
  pinMode(ECHO6, INPUT);
  pinMode(ECHO7, INPUT);
  pinMode(ECHO8, INPUT);
  pinMode(ECHO9, INPUT);

  // Set the interrupts
  if (wiringPiISR(ECHO, INT_EDGE_FALLING, &myInterrupt) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO2, INT_EDGE_FALLING, &myInterrupt2) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO3, INT_EDGE_FALLING, &myInterrupt3) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO4, INT_EDGE_FALLING, &myInterrupt4) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO5, INT_EDGE_FALLING, &myInterrupt5) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO6, INT_EDGE_FALLING, &myInterrupt6) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO7, INT_EDGE_FALLING, &myInterrupt7) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO8, INT_EDGE_FALLING, &myInterrupt8) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }

  if (wiringPiISR(ECHO9, INT_EDGE_FALLING, &myInterrupt9) < 0) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
  }
  // TRIG pin must start LOW
  digitalWrite(TRIG, LOW);

  // Small delay to let the pin set
  delayMicroseconds(63);
}

void sendPulse() {
  //Send trig pulse
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  //Wait for echo start
  delayMicroseconds(475);

  //Wait for echo end
  start = micros();
  maxedOut = 0;
  end[0] = 0;
  end[1] = 0;
  end[2] = 0;
  end[3] = 0;
  end[4] = 0;
  end[5] = 0;
  end[6] = 0;
  end[7] = 0;
  end[8] = 0;
  end[9] = 0;

  // Wait for 23000 us
  while ((end[0] == 0 || end[1] == 0 || end[2] == 0 || end[3] == 0
          || end[4] == 0 || end[5] == 0 || end[6] == 0 || end[7] == 0
          || end[8] == 0 || end[9] == 0)
         && (maxedOut = (micros() - start)) < 23000) {
    delayMicroseconds(100);
  }

}

/**
  * Converts microseconds returned from the sensor to centimeters.
  */
int getCM(int sensor) {
  long travelTime = end[sensor] - start;

  //Get distance in cm
  int distance = travelTime / 58;

  return distance;
}

/**
  * The main functionality of the program.
  *
  */
int main(int argc, char *argv[]) {
  // Gets the number of measurements to display from the first parameter
  int numDisplayed = atoi(argv[1]);

  // Sets up the GPIO pins
  setup();

  // Set the system start time
  sysStart = micros();

  // Print the headers for each row
  printf("Time\t\t");
  for (int m = 1; m < SENSORS + 1; m++) {
    printf("%d\t", m);
  }
  printf("\n");

  // Start printing readings
  for (int i = 0; i < numDisplayed; i++) {

    for (int j = 0; j < READINGS; j++) {
      // Minimum 60us delay between readings
      delayMicroseconds(60);
      sendPulse();

      // Print current time
      printf("%lu\t\t", (micros() - sysStart) / 1000);

      // Print CMs
      int cms[SENSORS];
      for (int k = 0; k < SENSORS; k++) {
        cms[k] = getCM(k);
        if (cms[k] < 0) {
          cms[k] = 400;
        }
        A[k][j] = cms[k];

        // print raw data
        printf("%d\t", cms[k]);
      }
      printf("\n");

      // Wait for echos to die out
      delayMicroseconds(23000);
    }

    //int displayCm[SENSORS];

    // Display the results
    //for (int l = 0; l < SENSORS; l++) {
    //displayCm[l] = selection_algorithm(0, READINGS - 1, SENSITVTY, l);
    //printf("%d\t", displayCm[l]);
    //}

    //printf("\n");
  }

  return 0;
}
