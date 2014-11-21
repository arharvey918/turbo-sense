all:
  gcc -Wall -std=c99 multisense_rt.c -o multisense_rt -O3 -lpigpio -lrt -lpthread
  gcc -Wall -std=c99 pipe_in.c -o pipe_in -lpthread -D_XOPEN_SOURCE
wiringPiCode:
	gcc -Wall -std=c99 -lwiringPi -O3 multisense.c -o multisense
clean:
	rm -f multisense multisense_rt reader pipe_in *~
rt:
  gcc -Wall -std=c99 multisense_rt.c -o multisense_rt -O3 -lpigpio -lrt -lpthread
pipe:
	gcc -Wall -std=c99 pipe_in.c -o pipe_in -lpthread -D_XOPEN_SOURCE
