
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>     // For dlopen()
#include <sys/types.h>
#include <sys/stat.h>

static int active = 0;

static int (*r_close) (int fd);
static int (*r_fclose) (FILE *fp);

void load_symbols(void) {
	if (!active) {
	  void *handle;
	  handle = dlopen("/lib64/libc.so.6", RTLD_LAZY);
	  if (!handle) {
	    fputs(dlerror(), stderr);
	    exit(1);
	  }

	  r_close = dlsym(handle, "close");
	  if (dlerror()) abort();
	  r_fclose = dlsym(handle, "fclose");
	  if (dlerror()) abort();
	  dlclose(handle);

	  active = 1;
	}
}

void flush(int fd) {
	int rc;
	struct stat fs;
	fstat(fd, &fs);
	if (S_ISREG(fs.st_mode)) {
	  rc = posix_fadvise(fd,0,0,POSIX_FADV_DONTNEED);
//	  if ((rc==0) & (fs.st_blocks <= 112)) {
//	    rc = posix_fadvise(fd,0,0,POSIX_FADV_DONTNEED);
//	  }
	  if (rc != 0) fprintf(stderr, "Failed fadvise %d rc=%d stat=%d\n", fd, rc, fs.st_mode);
	}
}

int close(int fd) {
	load_symbols();
	flush(fd);
	return r_close(fd);
}

int fclose(FILE *fp) {
	int fd;
	fd = fileno(fp);	
	load_symbols();
	flush(fd);
	return r_fclose(fp);
}
