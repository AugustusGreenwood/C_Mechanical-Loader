#ifndef NONSTAGEFUNCTIONS_H
#define NONSTAGEFUNCTIONS_H

#include <time.h>

extern int MONITOR_USER_INPUT_IN_THREAD_loop_variable;


double averageArray(double arr[], int length);
int isInRange(double low, double value, double high);
double getElapsedTime(struct timespec time_s, struct timespec time_e);
double getCurrentTime();
void wait();
void monitorUserInputInThread();


#endif 