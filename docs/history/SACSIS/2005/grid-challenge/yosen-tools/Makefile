CC = gcc
CFLAGS = -O2 -g
#CFLAGS =  -g
LDFLAGS = -lm

APPS = make-pic1

all: $(APPS)

install: $(DISTAPPS)
	mkdir -p $(INST_DIR)
	strip $(DISTAPPS)
	cp make-trial.sh $(DISTAPPS) $(INST_DIR)

make-pic1: make-pic1.o pic.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $*.o

clean:
	rm -f $(APPS)
	rm -f *.o
	rm -f *~
	rm -f core
	rm -f core.*
	rm -f gmon.out

