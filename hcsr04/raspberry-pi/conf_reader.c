/**
  * conf_reader.c is used to read from the sensor configuration file.
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

#include "conf_reader.h"

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
