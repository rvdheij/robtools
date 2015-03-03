// Generate a simple workload with specified footprint and
// CPU usage.

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <sys/times.h>

#include "poissinv.h"

#define CPUPASS 514

double runone(long *buf, int memsz, int memst, int *cursor, int cpulp, int verb) {
        int i, j;
        double tot = 0;
        struct tms c1, c2;
        clock_t t1, t2;
        int p = *cursor;

        if (cpulp==0) {
                sched_yield();
        } else {
                t1 = times(&c1);
                cpulp = poissinv(cpulp);
                for (i=0; i<cpulp; i++) {
                        for (j=0; j<CPUPASS; j++) {
                                buf[p] += 1;
                                p += memst;
                                if (p >= memsz) p = 0;
                        }
                        tot += j * sizeof(long) / 1024/1024;
                }
                t2 = times(&c2);
                if (verb) {
                        int c = c2.tms_utime + c2.tms_stime -
                                c1.tms_utime - c1.tms_stime;
                        int t = t2 - t1;
                        double e = (c==0) ? 0 : (double) t/c;
                        printf("Run %8d used %8d ticks, elongation %8.4f\n", cpulp, c, e);
                }
                *cursor = p;
              }
              return tot;
}

// Sleep for time (poisson distributed around specified mean in ms) 
void thinksome(int think, int verb) {
        struct timespec t1, t2;
        if (think == 0) sched_yield();
        else {
                think = poissinv(think);
                t1.tv_sec = think / 1000;           		// full seconds
                t1.tv_nsec = (long) (think % 1000) * 1000000L;  // rest in ns
                nanosleep(&t1, &t2);
        }
}

int main (int argc, char **argv) {
        int c;
        int verb = 0;
        int runt = 60;        // Default 60 seconds
        int think = 1000;     // Default think 1000 ms
        int memsz = 16;       // Default 64 KB
        int memst = 1;        // Memory step size
        int cpulp = 1000;     // Default 1000 us of CPU

        int cursor = 0;
        int memrange;
        double mbs = 0;       // Count number of MBs touched

        clock_t t1, t2;
        struct tms tms1, tms2;

        opterr = 0;
        while ((c = getopt(argc, argv, ":d:k:m:s:c:t:v")) != -1) {
                switch (c) {
                case 'd':                         // delay in ms
                        think  = atoi(optarg);
                        break;
                case 'k':                         // memory usage in KB
                        memsz = atoi(optarg);
                        break;
                case 'm':                         // memory usage in MB
                        memsz = atoi(optarg) * 1024;
                        break;
                case 's':                         // Stride size
                        memst = atoi(optarg);
                        break;
                case 'c':                         // CPU consumption
                        cpulp = atoi(optarg);
                        break;
                case 't':                         // Test runtime
                        runt = atoi(optarg);
                        break;
                case 'v':
                        verb = 1;
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

        if (runt<1) {
                printf(stderr, "Invalid runtime %d\n", runt);
                exit(1);
        }
        srand48(time(0) ^ getpid());

        memrange = memsz * 1024 / sizeof(long);
        long *buf = malloc(memrange * sizeof(long));

        t1 = times(&tms1); t2 = t1;
        int ticks = sysconf(_SC_CLK_TCK);
        while ((t2-t1) < runt * ticks) {
                double mb = runone(buf, memrange, memst, &cursor, cpulp, verb);
                mbs += mb;
                thinksome(think, verb);
                t2 = times(&tms2);
        }
        double utime = (double) (tms2.tms_utime - tms1.tms_utime) / ticks;
        double stime = (double) (tms2.tms_stime - tms1.tms_stime) / ticks;

        printf("Mem: %6d KB  User: %6.3f  Syst: %6.3f  BW: %8.1f \n",
                memsz, utime, stime, mbs/utime);

        free(buf);
        return 0;
}
