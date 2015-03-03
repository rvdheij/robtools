#
CC=gcc
LIBS =
INCLUDE = .

CFLAGS = -g -O3 -Wall -Werror
# LDFLAGS = -lrt -lcrypto

all: minc drop 

drop: drop.o
	$(CC) -o drop drop.c

minc: minc.o
	$(CC) -o minc minc.c
