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
  * Determines if a string starts with another string.
  *
  */
int startsWith(const char *a, const char *b) {
  if (strncmp(a, b, strlen(b)) == 0)
    return 1;
  return 0;
}

/**
  * Reads values from the sensor configuration file.
  *
  */
void readConfig(struct Config *conf) {
  char *line = NULL;
  size_t len = 0;
  size_t read;
  FILE *fp;
  fp = fopen("sensor.cfg", "r");

  while ((read = getline(&line, &len, fp)) != -1) {

    if (!(line[0] == '#' || line[0] == '\n')) {
      if (startsWith(line, "active_sensors")) {
        int len = strlen(line) - 15;
        char dest[len];
        strncpy(dest, &line[15], len);
        int i;
        int *v;
        v = malloc(MAX_SENSORS * sizeof(int));

        sscanf(dest, "%d %d %d %d %d %d %d %d %d %d", &v[0], &v[1], &v[2],
               &v[3], &v[4], &v[5], &v[6], &v[7], &v[8], &v[9]);
        for (i = 0; i < MAX_SENSORS; i++) {
          if (v[i] > 0 && v[i] < (MAX_SENSORS + 1)) {
            conf->active[v[i] - 1] = 1;
          }
        }

        free(v);
      } else if (startsWith(line, "readings")) {
        int len = strlen(line) - 9;
        char dest[len];
        strncpy(dest, &line[9], len);
        int readings = atoi(dest);
        conf->readings = readings;
      } else if (startsWith(line, "noise_filter")) {
        int len = strlen(line) - 13;
        char dest[len];
        strncpy(dest, &line[13], len);
        int noiseFilter = atoi(dest);
        conf->noiseFilter = noiseFilter;
      } else if (startsWith(line, "delay")) {
        int len = strlen(line) - 6;
        char dest[len];
        strncpy(dest, &line[6], len);
        int delay = atoi(dest);
        conf->delay = delay;
      } else if (startsWith(line, "pipe")) {
        int len = strlen(line) - 5;
        char dest[len];
        strncpy(dest, &line[5], len);
        int pipe = atoi(dest);
        conf->pipe = pipe;
      }
    }
  }

  fclose(fp);

  if (line)
    free(line);
}

/**
  * Called when the GPIO interrupt is triggered.
  *
  */
void interrupt_routine(int gpio, int level, uint32_t tick) {
  if (level == 0) {             /* falling edge */
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
  } else {                      /* rising edge */
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
  }
  // Delay for setup
  gpioDelay(100000);
}

/**
  * Sends the trigger pulse to the sensor and does a blocking wait for results.
  *
  */
void sendPulse() {
  // Send trigger pulse
  gpioTrigger(TRIG, 10, 1);

  //Wait for echo end
  uint32_t thisStart = gpioTick();
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
         && (maxedOut = (gpioTick() - thisStart)) < 23000) {
    gpioDelay(200);
  }

}

/**
  * Converts microseconds returned from the sensor to centimeters.
  */
int getCM(int sensor) {
  if (end[sensor] == 0 || end[sensor] < start[sensor]) {
    return MAX_RANGE;
  }

  uint32_t travelTime = end[sensor] - start[sensor];

  //Get distance in cm
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
  int *stdevElimThreshold = NULL;
  if (conf->noiseFilter == NF_STD || conf->noiseFilter == NF_ALL) {
    stdevElimThreshold = malloc(MAX_RANGE * sizeof(int));

    // Create standard deviation thresholds
    create_stdev_ranges(stdevElimThreshold);
  }
  // Set up the prism for output
  int *prism = (int *)malloc(6 * sizeof(int));

  // Create the pipe information object
  struct PipeInfo *pipeInfo = malloc(sizeof *pipeInfo);
  pipeInfo->myfifo = strdup2(PIPE_NAME);

  // Set up the semaphores
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

  // Get the jitters out
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

    // Print CMs
    for (int k = 0; k < MAX_SENSORS; k++) {
      if (conf->active[k]) {
        cms[k] = getCM(k);
        if (cms[k] < 0 || cms[k] > MAX_RANGE) {
          cms[k] = MAX_RANGE;
        }
        // Print the measurement
        int filtered = filter(conf, k, cms[k]);
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

  int terminatingPipe[6] = { -1, -1, -1, -1, -1, -1 };
  pipe_output(pipeInfo, terminatingPipe);
  sem_post(pipeSem);
  gpioDelay(10000);

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
  if (conf->noiseFilter == NF_STD || conf->noiseFilter == NF_ALL) {
    free(stdevElimThreshold);
  }
  // Reset the GPIO peripherals
  gpioTerminate();

  return 0;
}