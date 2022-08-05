CC = gcc
CFLAGS = -O2 -g
#CFLAGS =  -g
LDFLAGS = -lm
USER_LDFLAGS = -lm -lcrypto

INST_DIR = ../../grid-challenge-05/grch-trial

DISTAPPS = make-tr-pic make-tr-ans 

APPS = $(DISTAPPS)

all: $(APPS)

install: $(DISTAPPS)
	mkdir -p $(INST_DIR)
	strip $(DISTAPPS)
	cp make-trial.sh $(DISTAPPS) $(INST_DIR)

make-tr-pic: make-tr-pic.o pic.o

make-tr-ans: make-tr-ans.o pic.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $*.o

clean:
	rm -f $(APPS)
	rm -f *.o
	rm -f *~
	rm -f core
	rm -f core.*
	rm -f gmon.out

