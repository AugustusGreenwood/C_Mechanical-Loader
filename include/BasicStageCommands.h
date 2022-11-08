#ifndef BASICSTAGECOMMANDS_H
#define BASICSTAGECOMMANDS_H
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArcusPerformaxDriver.h"

AR_HANDLE openStage();

void closeStage(AR_HANDLE handle);

void interactiveMode(AR_HANDLE handle);

void flushStage(AR_HANDLE handle);

//universal getters
int getAcceleration(AR_HANDLE handle);
int getDeceleration(AR_HANDLE handle);
int getIdleTime(AR_HANDLE handle);
void getMoveMode(AR_HANDLE handle, char *reply_buffer);
int getMotorStatus(AR_HANDLE handle);
void readDriverSettings(AR_HANDLE handle);
int getEncoderPosition(AR_HANDLE handle);
int accelerationProfileIsSin(AR_HANDLE handle);

//universal setters
void setTimeouts(int w_timeout, int r_timeout);
void setAcceleration(AR_HANDLE handle, int new_acceleration);
void setDeceleration(AR_HANDLE handle, int new_deceleration);
void setIdleTime(AR_HANDLE handle, int new_idle_time);
void writeDriverSettings(AR_HANDLE handle);
void setMotorPosition_pulses(AR_HANDLE handle, int new_position);
void setEncoderPosition(AR_HANDLE handle, int new_position);



//pulse getters
int getMotorPosition_pulses(AR_HANDLE handle);
int getHighSpeed_pulses(AR_HANDLE handle);
int getLowSpeed_pulses(AR_HANDLE handle);


//pulse setters
void setHighSpeed_pulses(AR_HANDLE handle, int new_high_speed);
void setLowSpeed_pulses(AR_HANDLE handle, int new_low_speed);


//pulse movers
void moveUseCurrent_pulses(AR_HANDLE handle, char *ABS_INC, int pulses);
void moveDefineAll_pulses(AR_HANDLE handle, 
                            char *ABS_INC, 
                            int pulses, 
                            int high_speed, 
                            int low_speed, 
                            int acceleration, 
                            int deceleration, 
                            int idle_time);



void setAccelerationProfileToSin(AR_HANDLE handle);
void setAccelerationProfileToTrap(AR_HANDLE handle);


#endif
