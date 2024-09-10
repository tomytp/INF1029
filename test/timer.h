#ifndef TIMER_LIB
#define TIMER_LIB

#include <sys/time.h>
float timedifference_msec(struct timeval t0, struct timeval t1);

#endif