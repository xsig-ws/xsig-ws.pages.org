#######################################################################
# Cell Speed Challenge 2007, ToolKit Version 2007-02-09
#######################################################################
EXE_FILE    = ~/speed/main
IDIR        = /export/home/cell2007/
TIMEOUT_SEC = 15

#######################################################################
SPERE   = -I/usr/local/tsb/tool/spere/include

PPE_OPT = -O3 -Wall -m32
PPE_I   = -I$(IDIR)include/ppe -I$(IDIR)include/common $(SPERE) -I..
PPE_L   = -L$(IDIR)lib -L/usr/local/tsb/tool/spere/lib
PPE_LIB = -lspe_stdio_srv -lspe_rpc_srv -lspere_lib -lspere -lpthread
PPE_L2  = $(IDIR)share/lib/lib.o -lm
PPE_CC  = /usr/local/tsb/tool/bin/ppu-gcc
PPE_ADD = $(PPE_OPT) $(PPE_I) $(PPE_L)

SPE_OPT = -O3 -Wall -Wno-main
SPE_I   = -I$(IDIR)include/spe -I$(IDIR)include/common $(SPERE) -I..
SPE_L   = -L$(IDIR)lib -L/usr/local/tsb/tool/spere/lib
SPE_LIB = -lspe_stdio -lspe_rpc 
SPE_CC  = /usr/local/tsb/tool/bin/spu-gcc
SPE_ADD = $(SPE_OPT) $(SPE_I) $(SPE_L)

#######################################################################
all:
	make main spe1

#######################################################################
main: main_ppe.c define.h spe.h Makefile
	$(PPE_CC) $(PPE_ADD) main_ppe.c -o main $(PPE_LIB) $(PPE_L2)

spe1: main_spe.c spe1.c define.h spe.h Makefile
	$(SPE_CC) $(SPE_ADD) main_spe.c spe1.c -o spe1 $(SPE_LIB) 

#######################################################################
run:
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE)

runall:
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 1
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 2
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 3
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 4
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 5

run1:
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 1
run2:
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 2
run3:
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 3
run4:
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 4
run5:
	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE) 5
#######################################################################
clean:
	rm -f *~ *.o main spe1 spe2 spe3 spe4 spe5 spe6 spe7 

#######################################################################
lib:
	$(PPE_CC) $(PPE_ADD) lib.c -c
	cp lib.o /export/home/cell2007/share/lib/
#######################################################################
FILE1 = Makefile define.h main_ppe.c main_spe.c spe.h spe1.c
FILE2 = spe1_sample.c
release:
	cp  $(FILE1) $(FILE2) $(IDIR)/share/toolkit10/
#######################################################################
text:
	kcat define.h main_ppe.c spe.h main_spe.c > Code.txt
#######################################################################
