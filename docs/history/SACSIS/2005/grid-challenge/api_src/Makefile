.SUFFIXES: $(SUFFIXES) .cpp
OBJS = grichalle_api_imple.o
OBJS_P = grichalle_api_private.o

TARGET = libgrch.a
TARGET_P = libgrch_private.a
#CXX = autodep g++
#CC = autodep gcc
CXX = g++
CC = gcc
CFLAGS = -O2
LIBS = -lcrypto

all: $(TARGET)
$(TARGET): $(OBJS)
	ar r $(TARGET) $(OBJS)
	ranlib $(TARGET)
$(TARGET_P): $(OBJS_P)
	ar r $(TARGET_P) $(OBJS_P)
	ranlib $(TARGET_P)

grch_test: $(TARGET) main_c.o
	$(CXX) -o grch_test main_c.o -L. -lgrch $(LIBS)
key_check: $(TARGET_P) key_check.o
	$(CXX) -o key_check key_check.o -L. -lgrch_private $(LIBS)
key_check.o: key_check.cpp
	$(CXX) -c key_check.cpp -g
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET_P) key_check.o
.cpp.o:
	$(CXX) -c $(CFLAGS) $< 
-include .depend
