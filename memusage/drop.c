#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
        int fd;
        int i;
        if (argc<2) exit(1);
        for (i = 1; i<argc; i++) {
		fd = open(argv[i], O_RDONLY);
	        if (fd>=0) {
			posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED);
			close(fd);
	        }
        }
        return errno;
}

