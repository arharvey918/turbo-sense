/** 
  * constants.h defines the constants used for Turbosense.
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

#define PIPE_NAME "/tmp/myfifo"
#define SENSOR_SEM "/sensorSem"
#define PIPE_IN_SEM "/pipeInSem"

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
