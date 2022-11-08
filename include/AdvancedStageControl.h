#ifndef ADVANEDSTAGECONTROL_H
#define ADVANEDSTAGECONTROL_H

/*
    These needs a better name. I don't know what to call it but these are functions
    which are not just one level of code, like BasicStageCommands.h. These normally
    Take mulitple of those functions to cooperate in one function. Something like
    calibration, wait for motor idel. I have a problem with the difference being so
    arbitraty. It might be better just to have stage functions in a biffer file.
*/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "ArcusPerformaxDriver.h"
#include "BasicStageCommands.h"
#include "NonStageFunctions.h"


// Functions which output position to file
// These will become obsolete and instead replaced with multithreading
void waitForMotorIdle_POS(AR_HANDLE handle, double dwell_time, FILE *file);
void moveOneCycle_POS(AR_HANDLE handle, int pos_distance, double dwell_time, FILE *file);




//Functions which do not output to file
void waitForMotorIdle(AR_HANDLE handle, double dwell_time);
void moveOneCycle(AR_HANDLE handle, int pos_distance, double dwell_time);
double getAverageTimeOverCycles(AR_HANDLE handle, int pos_distance, int averaging_cycles, double dwell_time);
void calibrateTime(AR_HANDLE handle,
                int pos_distance,
                int acceleration,
                int starting_speed,
                int idle_time,
                int max_hspd,
                double desired_freq,
                int averaging_cycles,
                double tolerance,
                double error_scale,
                double dwell_time);

void outputStageValues(AR_HANDLE handle, FILE *file);

#endif