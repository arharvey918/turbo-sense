#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#define PIPE_NAME "/tmp/myfifo"
#define SENSOR_SEM "/sensorSem"
#define PIPE_IN_SEM "/pipeInSem"

/**
  * Struct for the pipe information
  */
struct PipeInfo {
  int fd;
  char *myfifo;
};

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
