CC = gcc
CFLAGS = -Wall -pedantic --std=gnu99 
.PHONY: clean
.DEFAULT_GOAL := donut

donut : donut_tings.o
	$(CC) $(CFLAGS) -o donut donut.c donut_tings.o -lm


clean :
		rm donut *.o

# General compile rule
%.o : %.c 
		$(CC) $(CFLAGS) -o $@ -c $<


