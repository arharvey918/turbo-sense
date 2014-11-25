$(VERBOSE).SILENT:
PROGRAMNAME = multisense_rt
CC = gcc
CC += -c
CFLAGS = -Wall -std=c99 -O3
LD = gcc
LDFLAGS = -lm -lpthread -lrt -lpigpio -O3
OBJFILES = $(patsubst %.c,%.o,$(wildcard *.c))

all: $(PROGRAMNAME)

clean:
	@echo "Cleaning object files"
	@echo "    rm -f     *.o"
	rm -f *.o
	@echo "Cleaning backups"
	@echo "    rm -f     *~"
	rm -f *~
	@echo "Removing program file"
	@echo "    rm -f     "$(PROGRAMNAME)
	rm -f $(PROGRAMNAME)

%.o: %.c
	#@echo "(C)ompiling "$@
	@echo " [CC] "$<
	$(CC) $(CFLAGS) $<

$(PROGRAMNAME): $(OBJFILES)
	#@echo "Get ready...."
	#@echo "Linking "$@
	@echo " [LD] "$(PROGRAMNAME)
	$(LD) -o $(PROGRAMNAME) $(OBJFILES) $(LDFLAGS)
	#@echo "Cry if it worked! Scream swear and cry if it did not..."

strip: $(PROGRAMNAME)
	@echo "Stripping "$(PROGRAMNAME)
	echo -n "Size of "$(PROGRAMNAME)" before stripping is "
	ls -sh $(PROGRAMNAME) | cut -d' ' -f1
	@echo "    Stripping     "$(PROGRAMNAME)
	strip $(PROGRAMNAME)
	echo -n "Size of "$(PROGRAMNAME)" after stripping is "
	ls -sh $(PROGRAMNAME) | cut -d' ' -f1