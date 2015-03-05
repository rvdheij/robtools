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

void onefile(const char *s, const int verb, unsigned long *totpgs, unsigned long *totcnt) {
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
	if (verb) {
		printf("%8d %8d %s\n", pgs, cnt, s);
	}
	free(mcvec);
	munmap(fm, fs.st_size);
	close(fd);
}

void usage() {
	printf("'minc - Count cached pages for specified files\n");
	printf("  -v  Verbose, number of blocks and cached blocks\n");
	printf("  -h  Help\n");
}

int main(int argc, char *argv[]) {
	int i;
	unsigned long totpgs = 0, totcnt = 0;

	int c;
	int verb = 0;

	opterr = 0;
	while ((c = getopt(argc, argv, "hv")) != -1) {
		switch (c) {
		case 'h':
			usage();
			return 0;
		case 'v': 
			verb = 1;
			break;
		default:	
			fprintf(stderr, "Unknown option: %c.\n", optopt);
			break;
		}
	}
	if (verb) printf("  #pages  #cached\n");
	
	for (i = optind; i < argc; i++) {
		onefile(argv[i], verb, &totpgs, &totcnt);
	}
	if (verb) {
		printf("%8d %8d %s\n", (unsigned) totpgs, (unsigned) totcnt, "[Total]");
	} else {
		printf("Total %8d blocks used, %8d cached\n", (unsigned) totpgs, (unsigned) totcnt);
	}
	return 0;
}
