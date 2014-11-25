#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include "constants.h"

/**
  * Struct for the pipe information
  */
struct PipeInfo {
  int fd;
  char *myfifo;
};

char *strdup2(const char *str);

void initialize_pipe(struct PipeInfo *pipeInfo);

void pipe_output(struct PipeInfo *pipeInfo, int *array);

void destroy_pipe(struct PipeInfo *pipeInfo);
