#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	int i, fd;
	struct stat fs;
	void *fm;
	unsigned char *mcvec;
	size_t pgsiz = getpagesize();
	size_t pgidx, pgs, cnt;
	size_t totpgs = 0, totcnt = 0;

	if (argc<2) {
		fprintf(stderr, "Missing file name\n");
		exit(1);
	}
	for (i = 1; i < argc; i++) {
		fd = open(argv[i], O_RDONLY);
		if (!fd) { 
			fprintf(stderr, "Error opening %s\n", argv[1]); 
		} else {
			fstat(fd, &fs);
			fm = mmap((void *) 0, fs.st_size, PROT_NONE, MAP_SHARED, fd, 0);
			pgs = (fs.st_size + pgsiz - 1)/pgsiz;
			mcvec = calloc(1, pgs);
			mincore(fm, fs.st_size, mcvec);
			cnt = 0;  
			for (pgidx = 0; pgidx < pgs; pgidx++) {
				if (mcvec[pgidx]&1) cnt++;
			}
			totpgs += pgs;
			totcnt += cnt;
			printf("%8d %8d %s\n", (unsigned) pgs, (unsigned) cnt, argv[i]);
			free(mcvec);
			munmap(fm, fs.st_size);
			close(fd);
		}
	}
	printf("%8d %8d %s\n", (unsigned) totpgs, (unsigned) totcnt, "Total");
	return 0;
}
