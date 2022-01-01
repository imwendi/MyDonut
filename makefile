CC = gcc
CFLAGS = -Wall -pedantic --std=gnu99 
OBJS = 
.PHONY: all clean
.DEFAULT_GOAL := all

all :
	$(CC) $(CFLAGS) -o donut donut_tings.c donut_tings.h -lm

clean :
		rm donut *.o