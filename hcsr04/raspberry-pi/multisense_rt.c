/**
  * multisense_rt.c is used to perform concurrent ultrasonic sensor readings.
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

#include "multisense_rt.h"

/**
  * Called when the GPIO interrupt is triggered.
  * gpio is the Broadcom number of the GPIO pin.
  * level is the level that the pin changed to.
  * tick is the time tick that the event occurred at.
  */
void interrupt_routine(int gpio, int level, uint32_t tick) {
  // Falling edge
  if (level == 0) {
    if (gpio == ECHO) {
      end[0] = tick;
    } else if (gpio == ECHO1) {
      end[1] = tick;
    } else if (gpio == ECHO2) {
      end[2] = tick;
    } else if (gpio == ECHO3) {
      end[3] = tick;
    } else if (gpio == ECHO4) {
      end[4] = tick;
    } else if (gpio == ECHO5) {
      end[5] = tick;
    } else if (gpio == ECHO6) {
      end[6] = tick;
    } else if (gpio == ECHO7) {
      end[7] = tick;
    } else if (gpio == ECHO8) {
      end[8] = tick;
    } else if (gpio == ECHO9) {
      end[9] = tick;
    }
  } 

  // Rising edge
  else {
    if (gpio == ECHO) {
      start[0] = tick;
    } else if (gpio == ECHO1) {
      start[1] = tick;
    } else if (gpio == ECHO2) {
      start[2] = tick;
    } else if (gpio == ECHO3) {
      start[3] = tick;
    } else if (gpio == ECHO4) {
      start[4] = tick;
    } else if (gpio == ECHO5) {
      start[5] = tick;
    } else if (gpio == ECHO6) {
      start[6] = tick;
    } else if (gpio == ECHO7) {
      start[7] = tick;
    } else if (gpio == ECHO8) {
      start[8] = tick;
    } else if (gpio == ECHO9) {
      start[9] = tick;
    }
  }
}

/**
  * Set up the GPIO pins on the Raspberry Pi.
  */
void setup() {
  // Set up the library to use the Broadcom GPIO numbers
  if (gpioInitialise() < 0) {
    printf("Library initialization failed.");
    exit(1);
  }

  // Set up the GPIO pins for I/O mode
  gpioSetMode(TRIG, PI_OUTPUT); // Set trigger as output
  gpioSetMode(ECHO, PI_INPUT);  // Set echo pins as input   
  gpioSetMode(ECHO1, PI_INPUT);
  gpioSetMode(ECHO2, PI_INPUT);
  gpioSetMode(ECHO3, PI_INPUT);
  gpioSetMode(ECHO4, PI_INPUT);
  gpioSetMode(ECHO5, PI_INPUT);
  gpioSetMode(ECHO6, PI_INPUT);
  gpioSetMode(ECHO7, PI_INPUT);
  gpioSetMode(ECHO8, PI_INPUT);
  gpioSetMode(ECHO9, PI_INPUT);

  // Set the interrupts
  gpioSetAlertFunc(ECHO, interrupt_routine);
  gpioSetAlertFunc(ECHO1, interrupt_routine);
  gpioSetAlertFunc(ECHO2, interrupt_routine);
  gpioSetAlertFunc(ECHO3, interrupt_routine);
  gpioSetAlertFunc(ECHO4, interrupt_routine);
  gpioSetAlertFunc(ECHO5, interrupt_routine);
  gpioSetAlertFunc(ECHO6, interrupt_routine);
  gpioSetAlertFunc(ECHO7, interrupt_routine);
  gpioSetAlertFunc(ECHO8, interrupt_routine);
  gpioSetAlertFunc(ECHO9, interrupt_routine);

  // TRIG pin must start LOW
  if (gpioWrite(TRIG, 0) == 1) {
    printf("Could not set TRIGGER pin to low!\n");
    exit(1);
  }

  // Let all the sensors and GPIO pins settle
  gpioDelay(100000);
}

/**
  * Sends the trigger pulse to the sensor and does a blocking wait for results.
  *
  */
void sendPulse() {
  // Send trigger pulse
  gpioTrigger(TRIG, 10, 1);

  // Record the time we sent the pulse for timeout purposes
  uint32_t thisStart = gpioTick();

  // Record the moment past the start that the sensor times out (IF it times out)
  maxedOut = 0;

  // Set the end time for each sensor to 0
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

  // Wait for 23000 us.  If we don't get a response back from any one of the sensors within that time, continue.
  while ((end[0] == 0 || end[1] == 0 || end[2] == 0 || end[3] == 0
          || end[4] == 0 || end[5] == 0 || end[6] == 0 || end[7] == 0
          || end[8] == 0 || end[9] == 0)
         && (maxedOut = (gpioTick() - thisStart)) < 23000) {
    gpioDelay(200);
  }

}

/**
  * Converts microseconds returned from the sensor to centimeters.
  */
int getCM(int sensor) {
  // If the end time for the sensor is 0 or the end time is less than the start time, assume the sensor timed out and return the maximum distance.
  if (end[sensor] == 0 || end[sensor] < start[sensor]) {
    return MAX_RANGE;
  }

  // Calculate the time elapsed between the start and end.
  uint32_t travelTime = end[sensor] - start[sensor];

  // Convert the distance to centimeters
  int distance = travelTime / 58;

  return distance;
}

/**
  * The main functionality of the program.
  *
  */
int main(int argc, char *argv[]) {
  // Read from the configuration file
  struct Config *conf = malloc(sizeof *conf);
  conf->active = calloc(MAX_SENSORS, sizeof(int));
  readConfig(conf);

  // Active sensors
  for (int i = 0; i < MAX_SENSORS; i++) {
    if (conf->active[i]) {
      printf("Sensor %d is active on pin %d\n", i + 1, sensorMap[i]);
    }
  }

  // Other information
  printf("Number of readings to take: %d\n", conf->readings);
  printf("Noise filter method: %s\n", filterMap[conf->noiseFilter]);
  printf("Delay between readings: %d\n", conf->delay);
  printf("Output to six-integer pipe: %d\n", conf->pipe);

  // User input only
  printf("Press <ENTER> to continue");
  getchar();

  // Set up the median filter 2D array
  if (conf->noiseFilter == NF_MEDIAN || conf->noiseFilter == NF_ALL) {
    medianValues = malloc(MAX_SENSORS * sizeof(int *));
    for (int i = 0; i < MAX_SENSORS; i++)
      medianValues[i] = malloc(MEDIAN_READINGS * sizeof(int));
  }
  // Set up the standard deviation filter
  if (conf->noiseFilter == NF_STD || conf->noiseFilter == NF_ALL) {
    // Create standard deviation thresholds
    create_stdev_ranges();
  }
  // Set up the prism for output
  int *prism = (int *)malloc(6 * sizeof(int));

  // Create the pipe information object
  struct PipeInfo *pipeInfo = malloc(sizeof *pipeInfo);
  pipeInfo->myfifo = strdup2(PIPE_NAME);

  // Set up the semaphores for the pipe
  sem_unlink(SENSOR_SEM);
  sem_unlink(PIPE_IN_SEM);
  sem_t *sensorSem = sem_open(SENSOR_SEM, O_CREAT | O_EXCL, 0666, 1);
  sem_t *pipeSem = sem_open(PIPE_IN_SEM, O_CREAT | O_EXCL, 0666, 0);

  // Open the output pipe
  if (conf->pipe > 0) {
    initialize_pipe(pipeInfo);
  }
  // Sets up the GPIO pins
  setup();

  // Get the jitters out (not sure if this actually helps the results or not)
  sendPulse();

  // Set the system start time
  sysStart = gpioTick();

  // Print the headers for each row
  printf("Time\t");
  for (int m = 0; m < MAX_SENSORS; m++) {
    if (conf->active[m]) {
      printf("S%d\t", m + 1);
    }
  }
  printf("\n");

  // Set up the results array
  int *cms = malloc(MAX_SENSORS * sizeof(int));
  int *elements = malloc(MAX_SENSORS * sizeof(int));

  // Start printing readings
  for (int i = 0; i < conf->readings; i++) {
    if (conf->pipe > 0) {
      sem_wait(sensorSem);
    }
    // Send the pulse
    sendPulse();

    // Print current time
    printf("%zu\t", (gpioTick() - sysStart) / 1000);

    // Calculate and print the distances
    for (int k = 0; k < MAX_SENSORS; k++) {
      if (conf->active[k]) {
        cms[k] = getCM(k);
        if (cms[k] < 0 || cms[k] > MAX_RANGE) {
          cms[k] = MAX_RANGE;
        }
        // Print the measurement
        int filtered = filter(conf->noiseFilter, k, cms[k]);
        elements[k] = filtered;
        printf("%d\t", filtered);
      }
    }

    printf("\n");

    // Convert the values to a prism and output
    if (conf->pipe > 0) {
      convertToPrism(elements, prism);
      pipe_output(pipeInfo, prism);
      sem_post(pipeSem);
    }
    // Wait for echos to die out
    gpioDelay(conf->delay);

  }

  // Tell the reading end of the pipe we're done
  if (conf->pipe > 0) {
    int terminatingPipe[6] = { -1, -1, -1, -1, -1, -1 };
    pipe_output(pipeInfo, terminatingPipe);
    sem_post(pipeSem);
    gpioDelay(10000);
  }

  // Close and unlink the semaphores
  sem_close(pipeSem);
  sem_unlink(SENSOR_SEM);
  sem_close(sensorSem);
  sem_unlink(PIPE_IN_SEM);

  // Free the parameter list
  free(conf);
  free(cms);
  free(elements);

  // Free the memory we used
  free(pipeInfo->myfifo);
  free(pipeInfo);
  free(prism);

  if (conf->noiseFilter == NF_MEDIAN || conf->noiseFilter == NF_ALL) {
    free(medianValues);
  }

  // Reset the GPIO peripherals
  gpioTerminate();

  return 0;
}
