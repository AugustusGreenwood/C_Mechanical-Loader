#include "AdvancedStageControl.h"

// Don't like the global but I shall use under one condition:
// Only used by one function
// Never have multiple threads running the same function
int ROUTE_STDOUT_TO_FILE = 0;


void waitForMotorIdle_POS(AR_HANDLE handle, double dwell_time, FILE *file) {
    fprintf(file, "%f\t%i\n", getCurrentTime(), getMotorPosition_pulses(handle));
    while (getMotorStatus(handle) != 0) {
        fprintf(file, "%f\t%i\n", getCurrentTime(), getMotorPosition_pulses(handle));
    }; // For the stage 0 == motor idle
    fprintf(file, "%f\t%i\n", getCurrentTime(), getMotorPosition_pulses(handle));
    wait(dwell_time);
    fprintf(file, "%f\t%i\n", getCurrentTime(), getMotorPosition_pulses(handle));

}

void moveOneCycle_POS(AR_HANDLE handle, int pos_distance, double dwell_time, FILE *file) {
    int neg_distance = pos_distance * -1;

    waitForMotorIdle_POS(handle, 0, file);                           // Not sure about these. Should I only wait for idle 
    moveUseCurrent_pulses(handle, "INC", neg_distance); // between moves? Should I make sure the motor is idle
    waitForMotorIdle_POS(handle, dwell_time, file);                           // before the function returns? I don't know. 
    moveUseCurrent_pulses(handle, "INC", pos_distance); //
    waitForMotorIdle_POS(handle, dwell_time, file);                           //
}


void waitForMotorIdle(AR_HANDLE handle, double dwell_time) {
    while (getMotorStatus(handle) != 0) {}; // For the stage 0 == motor idle
}

void moveOneCycle(AR_HANDLE handle, int pos_distance, double dwell_time) {
    int neg_distance = pos_distance * -1;

    waitForMotorIdle(handle, dwell_time);                           // Not sure about these. Should I only wait for idle 
    moveUseCurrent_pulses(handle, "INC", neg_distance); // between moves? Should I make sure the motor is idle
    waitForMotorIdle(handle, dwell_time);                           // before the function returns? I don't know. 
    moveUseCurrent_pulses(handle, "INC", pos_distance); //
    waitForMotorIdle(handle, dwell_time);                           //
}

double getAverageTimeOverCycles(AR_HANDLE handle, int pos_distance, int averaging_cycles, double dwell_time) {
    double time_start, time_end;
    double times[averaging_cycles];
    for (int i = 0; i < averaging_cycles; i++)
    {
        time_start = getCurrentTime();
        moveOneCycle(handle, pos_distance, dwell_time);
        time_end = getCurrentTime();

        times[i] = time_end - time_start;
    }
    return averageArray(times, averaging_cycles);
}

void calibrateTime(AR_HANDLE handle, int pos_distance, int acceleration, int starting_speed, int idle_time, int max_hspd, 
                double desired_freq, int averaging_cycles, double tolerance, double error_scale, double dwell_time) {
    setAcceleration(handle, acceleration);              //
    setHighSpeed_pulses(handle, starting_speed);        // Set user defined values
    setIdleTime(handle, idle_time);                     //

    int lspd, hspd;                                     //       
    double desired_cycle_time, low_tolerance;           //  Initialize needed variables
    double high_tolerance, error, current_cycle_time;   //

    FILE *file = fopen("calibration.txt", "w");         // Open output file

    lspd = getLowSpeed_pulses(handle);
    hspd = getHighSpeed_pulses(handle);     // I'm worried about weird refernce errors so I get the HSPD insead
                                            // of using the starting speed even though they should be the same

    desired_cycle_time = 1 / desired_freq;

    low_tolerance    =   tolerance       * desired_cycle_time; // Gets the range around the desired time which
    high_tolerance   =   (2 - tolerance) * desired_cycle_time; // is good enough to terminate the loop

    do
    {
        // Make sure that high speed didn't run away
        if (isInRange(lspd, hspd, max_hspd))
        {
            setHighSpeed_pulses(handle, hspd);
        }
        // Terminate loop if the high speed limits were tripped
        else 
        {
            fprintf(file, "Range of HSPD tripped\n\n"); 
            fclose(file);
            exit(-1);
        }
        
        current_cycle_time = getAverageTimeOverCycles(handle, pos_distance, averaging_cycles, dwell_time);
        waitForMotorIdle(handle, dwell_time);   // If the code get too far ahead of the motor undefined behavior happens
                                    // This keeps the code from running away

        error = current_cycle_time - desired_cycle_time;

        hspd = hspd + error * error_scale * 1000; 
        // HSPD is on the order of thousands usually, this can have a nice analog to milliseconds multipled by some 
        // factor which is chosen by the user. time is in seconds so convert error to milliseconds
        // error factor MAY help make sure the value doesn't fluctuate around the desired threshold. It can happen if
        // it is too large (>= 1), it will bound back and forth and not converge.
    } while (!isInRange(low_tolerance, current_cycle_time, high_tolerance));

    outputStageValues(handle, file);
    fprintf(file, "## End cycle time = %f\n", current_cycle_time);
    fprintf(file, "## Desired cycle time = %f\n", desired_cycle_time);
    fprintf(file, "## Tolerance = %f\n", tolerance);
    fprintf(file, "## Dwell time = %f\n", dwell_time);
    fclose(file);
}


void outputStageValues(AR_HANDLE handle, FILE *file) {
    int high_speed   =   getHighSpeed_pulses(handle);
    int low_speed    =   getLowSpeed_pulses(handle);
    int acceleration =   getAcceleration(handle);
    int deceleration =   getDeceleration(handle);
    int position     =   getMotorPosition_pulses(handle);
    int idle_time    =   getIdleTime(handle);
    int sine_curve    =   accelerationProfileIsSin(handle);

    fprintf(file, "## Values\n");
    fprintf(file, "## High Speed = %i\n", high_speed);
    fprintf(file, "## Low Speed = %i\n", low_speed);
    fprintf(file, "## Acceleration = %i\n", acceleration);
    fprintf(file, "## Deceleration = %i\n", deceleration);
    fprintf(file, "## Position = %i\n", position);
    fprintf(file, "## Idle Time = %i\n", idle_time);
    if (sine_curve == 1) 
    {
        fprintf(file, "## Accel/Decel profile = Sinusoidal\n");
    }
    else 
    {
        fprintf(file, "## Accel/Decel profile = Trapezoidal\n");
    }
}
