#include "NonStageFunctions.h"
#include <stdio.h>

int MONITOR_USER_INPUT_IN_THREAD_loop_variable = 1;


double averageArray(double arr[], int length) {
    double sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum = sum + arr[i];
    }
    return sum / length;
}

int isInRange(double low, double value, double high) {
    if (low <= value && value <= high) {
        return 1;
    }
    else {return 0;}
}

double getElapsedTime(struct timespec time_s, struct timespec time_e) {
    double end_time     =   time_e.tv_nsec * 1e-9 + time_e.tv_sec;
    double start_time   =   time_s.tv_nsec * 1e-9 + time_s.tv_sec;
    return end_time - start_time;
}

double getCurrentTime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_nsec * 1e-9 + time.tv_sec;
}

void wait(double time) {
    double start = getCurrentTime();
    while (getCurrentTime() - start < time) {};
}

void monitorUserInputInThread() {
}