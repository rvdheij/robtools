#
LIBS =
INCLUDE = .

CFLAGS:=-g -O3 -Wall -Werror
LDFLAGS = -lrt

all: minc drop nocache.so 

%: %.c
	$(CC) -o $@ $< ${CFLAGS}

%.so: %.c
	${CC} -o $@ $< ${CFLAGS} -shared -fPIC -ldl -Wall -Werror

