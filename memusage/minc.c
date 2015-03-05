#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <glob.h>

void onefile(char *s, unsigned long *totpgs, unsigned long *totcnt) {
	struct stat fs;
	void *fm;
	unsigned char *mcvec;
	size_t pgsiz = getpagesize();
	size_t pgidx;
	unsigned pgs, cnt;
	int fd;

	fd = open(s, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "%s opening %s\n", strerror(errno), s); 
		return;
	}
	fstat(fd, &fs);
	fm = mmap((void *) 0, fs.st_size, PROT_NONE, MAP_SHARED, fd, 0);
	pgs = (fs.st_size + pgsiz - 1)/pgsiz;
	mcvec = calloc(1, pgs);
	mincore(fm, fs.st_size, mcvec);
	cnt = 0;  
	for (pgidx = 0; pgidx < pgs; pgidx++) {
		if (mcvec[pgidx]&1) cnt++;
	}
	*totpgs += pgs;
	*totcnt += cnt;
	printf("%8d %8d %s\n", pgs, cnt, s);
	free(mcvec);
	munmap(fm, fs.st_size);
	close(fd);
}

int main(int argc, char *argv[]) {
	int i;
	unsigned long totpgs = 0, totcnt = 0;
	if (argc<2) {
		fprintf(stderr, "Missing file name\n");
		exit(1);
	}
	for (i = 1; i < argc; i++) {
		onefile(argv[i], &totpgs, &totcnt);
	}
	printf("%8d %8d %s\n", (unsigned) totpgs, (unsigned) totcnt, "Total");
	return 0;
}
