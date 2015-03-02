#
CC=gcc
LIBS =
INCLUDE = .

CFLAGS = -g -O3 -Wall -Werror
# LDFLAGS = -lrt -lcrypto

all: poisson

poisson: poisson.o poissinv.o
	$(CC) -o poisson poisson.o poissinv.o
