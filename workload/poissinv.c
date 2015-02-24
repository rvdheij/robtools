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

// The poissinv function uses an iterative process to produce the
// poisson distribution, which fails when the lambda value is too
// high and e**(-lambda) becomes too small to work with.
// For a large average, we scale down to 256 and use a linear 
// random function to interpolate.

int poissinv(int lambda) {
  int i;
  double p = 1;
  double scale = 1;

  if (lambda>255) {
    scale = (double) lambda/256;
    lambda = 256;
  }
//          	                            Compute e**(-n)
  for (i=1; i <= lambda; i++) p = p / 2.71828182845904523536l;

  double f = 0;
  double r = drand48();
  for (i=0; (r>f) && i++<=(lambda*5); ) {
    p = p * ((double) lambda / (double) i);
    f = f + p;
  }
  return (i + drand48()) * scale;
}

