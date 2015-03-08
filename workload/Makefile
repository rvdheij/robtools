#
LIBS =
INCLUDE = .

CFLAGS:=-g -O3 -Wall -Werror
LDFLAGS = -lrt -lcrypto

M:=poisson rmhwload rmhrsa
O:=poissinv.o

all: ${M} 

%.o: %.c
	$(CC) -c $< ${CFLAGS} ${LDFLAGS}

%: %.c
	$(CC) -o $@ $< ${CFLAGS} ${O} ${LDFLAGS}
