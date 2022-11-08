#include "BasicStageCommands.h"
#include "ArcusPerformaxDriver.h"
#include "AdvancedStageControl.h"
#include "NonStageFunctions.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>

double PI = 3.1415926536;


struct Input {
    AR_HANDLE handle;
    double time_zero;
    double freq;
    int amp;
    FILE *file;
};


double THIS_getNewSpeed(AR_HANDLE handle, double freq, int amp, double time_zero, FILE *file) {
    double time = getCurrentTime() - time_zero;
    double speed =  fabs(2 * PI * freq * amp * cos(2 * PI * freq * time + PI/2));
    fprintf(file, "%f\t%f\t%i\n", getCurrentTime(), speed, getMotorPosition_pulses(handle));
    return speed;
}   

void THIS_onFlySpeedChange(AR_HANDLE handle, double new_speed) {
    char reply_buffer[64];    
    char command_buffer[64];
    sprintf(command_buffer, "SSPD%f", new_speed);
    fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer);
    //printf("%s\t%s\n", command_buffer, reply_buffer);
}


void THIS_cycle(AR_HANDLE handle, double freq, int amp, double time_zero, FILE *file) {
    moveUseCurrent_pulses(handle, "INC", 2*amp);
    while (getMotorStatus(handle) != 0) 
    {
        fprintf(file, "%i\n", getMotorPosition_pulses(handle));
        THIS_onFlySpeedChange(handle, THIS_getNewSpeed(handle, freq, amp, time_zero, file));
    }
}

void * THIS_setNewSpeedInThread(void *params) {
    struct Input *input = params;
    while (getMotorStatus(input->handle) != 0) {
        double speed = THIS_getNewSpeed(input->handle, input->freq, input->amp, input->time_zero, input->file);
        THIS_onFlySpeedChange(input->handle, speed);
    }
    return NULL;
}

int main() {
    struct Input input;
    pthread_t thread_id;
    input.handle = openStage();
    input.file = fopen(".\\input_output\\test.txt", "w");

    interactiveMode(input.handle);

    input.freq = 1;
    input.amp = 500;
    input.time_zero = getCurrentTime();
    printf("%f\n", input.time_zero);
    setHighSpeed_pulses(input.handle, THIS_getNewSpeed(input.handle, input.freq, input.amp, input.time_zero, input.file));
    moveUseCurrent_pulses(input.handle, "INC", -2*input.amp);


    fclose(input.file);
    closeStage(input.handle);
    return 0;
}