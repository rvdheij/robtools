#
LIBS =
INCLUDE = .

CFLAGS:=-g -O3 -Wall -Werror
# LDFLAGS = -lrt -lcrypto

M:=poisson rmhwload
O:=poissinv.o

all: ${M} 

%.o: %.c
	$(CC) -c $< ${CFLAGS}

%: %.c
	$(CC) -o $@ $< ${CFLAGS} ${O}
