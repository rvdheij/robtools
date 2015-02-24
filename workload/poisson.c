/*
	Generate one or more pseudo random numbers according to a Poisson
	distribution with specified lambda value.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <sys/times.h>
#include <sched.h>

#include "poissinv.h"

int main(int argc, char **argv)
{
  int count = 1;			// number of samples
  int lambda = -1;			// mean of distribution
  int sflag = 0;
  int c;

  opterr = 0;
  while ((c = getopt(argc, argv, ":n:m:s")) != -1) {
    switch (c) {
      case 'n':				// Number of iterations
        count = atoi(optarg);
        break;
      case 'm':				// Mean value of distribution
        lambda = atoi(optarg);
        break;
      case 's':				// Only statistics
	sflag = 1;
        break;
      case ':':
	fprintf(stderr, "Missing value for option -%c\n", optopt);
	break;
      default:
        if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
    }
  }

  int res, i;
  int maxnum = lambda * 3;
  long *stat;
  if (sflag) {
    stat = malloc(maxnum * sizeof(long));
    for (i=0; i<maxnum; i++) stat[i] = 0;
  }
  srand48(time(0));

  while (count-- > 0) {  
    res = poissinv(lambda);
    if (res>=maxnum) res = maxnum-1;
    if (sflag) stat[res]++;
    else printf("%d ", res);
  }

  if (sflag) {
    printf(" Value    Count\n");
    for (i=0; i<maxnum; i++) {
      if (stat[i]) printf("%6d %8ld\n", i, stat[i]);
    }
    free(stat);
  }
  else printf("\n");
  return 0;
}

