/**
  * pipe.c is used to handle pipes for Turbosense.
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

#include "pipe.h"
#include "constants.h"

/**
  * Duplicate of the strdup function available in some libraries.
  */
char *strdup2(const char *str) {
  int n = strlen(str) + 1;
  char *dup = malloc(n);
  if (dup) {
    strcpy(dup, str);
  }
  return dup;
}

/**
  * Initializes the named pipe (FIFO) for output.
  */
void initialize_pipe(struct PipeInfo *pipeInfo) {
  /* create the FIFO (named pipe) */
  mkfifo(pipeInfo->myfifo, 0644);

  pipeInfo->fd = open(pipeInfo->myfifo, O_WRONLY);
}

/**
  * Sends the array of six integers to a named pipe (FIFO)
  * for the control module to use as sensor input.
  *
  * The data are sent in multiples of six integers, where
  * integers are the the following order: front, back,
  * left, right, up, down.
  */
void pipe_output(struct PipeInfo *pipeInfo, int *array) {
  for (int i = 0; i < 6; i++) {
    write(pipeInfo->fd, &array[i], sizeof(int));
  }
}

/**
  * Destroys the output pipe.
  */
void destroy_pipe(struct PipeInfo *pipeInfo) {
  close(pipeInfo->fd);

  // Remove the FIFO
  unlink(pipeInfo->myfifo);
}
