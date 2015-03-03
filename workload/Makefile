#
CC=gcc
LIBS =
INCLUDE = .

CFLAGS = -g -O3 -Wall -Werror
# LDFLAGS = -lrt -lcrypto

all: poisson rmhwload

poisson: poisson.o poissinv.o
	$(CC) -o poisson poisson.o poissinv.o

rmhwload: rmhwload.o poissinv.o
	$(CC) -o rmhwload rmhwload.o poissinv.o
