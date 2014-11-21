/**
  * Input pipe program for six-sided integer output.
  * Created date: 08NOV2014
  *
  * Author: Avy Harvey (c) 2014
  */

#include "npipe.h"
#include <errno.h>

int fd;
short useFile = 0;

/**
  * Opens the named pipe (FIFO) for input.
  */
void open_pipe(char *myfifo) {
  printf("Waiting for pipe creation... ");
  fflush(stdout);
  while (fd == -1) {
    fd = open(myfifo, O_RDONLY);
    if (fd == -1) {
      sleep(1);
    }
  }

  printf("Pipe opened.\n");
}

void close_pipe() {
  close(fd);
}

int main(int argc, char *argv[]) {

  FILE *fdOut = NULL;
  if (argc == 3 && !strcmp(argv[1], "-p")) {
    fdOut = fopen(argv[2], "w");
    if (fdOut > 0) {
      useFile = 1;
    }
  }
  // Open the pipe
  fd = -1;
  char *myfifo = PIPE_NAME;

  // Create the input variables to store information
  int array[6];
  int tmp;
  int len;

  // Open the pipe
  open_pipe(myfifo);

  // Open the semaphore; if the pipe is open, the semaphore also exists
  printf("Opening semaphores\n");
  sem_t *pipeSem = sem_open(PIPE_IN_SEM, 0);
  sem_t *sensorSem = sem_open(SENSOR_SEM, 0);

  if (pipeSem == SEM_FAILED || sensorSem == SEM_FAILED) {
    printf("Error : %s\n", strerror(errno));
    exit(0);
  }
  printf("Opened semaphores\n");

  // Output everything in the pipe in sets of six integers
  while (1) {
    sem_wait(pipeSem);
    len = read(fd, &tmp, sizeof(int));
    if (len > 0) {
      if (tmp == -1) {
        break;
      }
      // We got one int, so look for five more
      array[0] = tmp;

      int len2 = -1;
      for (int i = 1; i < 6; i++) {
        if (len2 == sizeof(int)) {
          len2 = -1;
        }
        while (len2 != sizeof(int)) {
          len2 = read(fd, &array[i], sizeof(int));
        }
      }
      if (useFile == 1) {
        fprintf(fdOut, "%d\t%d\t%d\t%d\t%d\t%d\n", array[0], array[1], array[2],
                array[3], array[4], array[5]);
      }
      printf("%d\t%d\t%d\t%d\t%d\t%d\n", array[0], array[1], array[2],
             array[3], array[4], array[5]);

    }
    sem_post(sensorSem);

  }

  // Close the semaphore
  sem_close(pipeSem);
  sem_close(sensorSem);

  // Close the pipe
  close_pipe();

  // Close the output file if open
  if (useFile > 0) {
    fclose(fdOut);
  }

  return 0;
}
