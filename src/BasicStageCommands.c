#include "BasicStageCommands.h"

AR_HANDLE openStage() {
    AR_HANDLE handle;
    fnPerformaxComSetTimeouts(1000, 1000);
    fnPerformaxComOpen(0, &handle);
    fnPerformaxComFlush(handle);
    return handle;
}

void closeStage(AR_HANDLE handle) {
    fnPerformaxComClose(handle);
}


void interactiveMode(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64];
    char end[64] = "EXIT";

    printf("Entering interactive mode\n");
    while (strcmp(command_buffer, end) != 0)
    {
        scanf("%s", command_buffer);
        fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer);
        printf("%s\n", reply_buffer);
    }
}

void flushStage(AR_HANDLE handle) {
    if (!fnPerformaxComFlush(handle))
    {
        printf("Cannot flush communication with stage\n");
    }
}

//universal getters
int getAcceleration(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "ACC";
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get acceleration\n");
    }
    return atoi(reply_buffer);
}
int getDeceleration(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "DEC";
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get current position\n");
    }
    return atoi(reply_buffer);
}
int getIdleTime(AR_HANDLE handle) {
    // For the idle time to be valid, have to read driver params from device
    readDriverSettings(handle);

    // Now the value will be current
    char reply_buffer[64];
    char command_buffer[64] = "DRVIT";
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get current position\n");
    }
    return atoi(reply_buffer);
}
void getMoveMode(AR_HANDLE handle, char *move_mode) {
    char command_buffer[64] = "MM";
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, move_mode))
    {
        printf("Couldn't get move mode\n");
    }
}
int getMotorStatus(AR_HANDLE handle) {
    char motor_status[64];
    char command_buffer[64] = "MST";
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, motor_status))
    {
        printf("Couldn't get motor status\n");
    }
    return atoi(motor_status);
}
void readDriverSettings(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "RR";
    // reading driver params turns the motor off
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't read driver settings\n");
    }

    //turn motor back on, give some errors if it fails
    strcpy(command_buffer, "EO=1");
    fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer);

    strcpy(command_buffer, "EO");
    fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer);

    if (strcmp(reply_buffer, "1") != 0)
    {
        printf("WARNING: Couldn't turn motor back on after reading driver settings.\n");
        printf("Exiting to prevent further errors\n");
        exit(-1);
    }
}
int getEncoderPosition(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "EX";
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get current encoder position\n");
    }
    return atoi(reply_buffer);
}
int accelerationProfileIsSin(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "SCV";
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get acceleration profile\n");
    }
    
    return atoi(reply_buffer);
}

// non pulses versus mm specific setters
void setTimeouts(int w_timeout, int r_timeout) {
    if (!fnPerformaxComSetTimeouts(w_timeout, r_timeout))
    {
        printf("Cannot set timeouts\n");
    }
}
void setAcceleration(AR_HANDLE handle, int new_acceleration) {
    char reply_buffer[64];
    char acceleration[64];
    sprintf(acceleration, "ACC=%i", new_acceleration);

    if (!fnPerformaxComSendRecv(handle, acceleration, 64, 64, reply_buffer))
    {
        printf("Couldn't set high speed\n");
    }
}
void setDeceleration(AR_HANDLE handle, int new_deceleration) {
    char reply_buffer[64];
    char deceleration[64];
    sprintf(deceleration, "DEC=%i", new_deceleration);

    if (!fnPerformaxComSendRecv(handle, deceleration, 64, 64, reply_buffer))
    {
        printf("Couldn't set high speed\n");
    }
}
void setIdleTime(AR_HANDLE handle, int new_idle_time) {
    char reply_buffer[64];
    char idle_time[64];
    sprintf(idle_time, "DRVIT=%i", new_idle_time);

    if (!fnPerformaxComSendRecv(handle, idle_time, 64, 64, reply_buffer))
    {
        printf("Couldn't set high speed\n");
    }

    // Settings like idle time are driver settings and need a speacial
    // Command that writes them to the driver
    writeDriverSettings(handle);
}
void writeDriverSettings(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "RW";


    // Writing driver params turns the motor off
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't read driver settings\n");
    }

    //turn motor back on, give some errors if it fails
    strcpy(command_buffer, "EO=1");
    fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer);

    strcpy(command_buffer, "EO");
    fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer);

    if (strcmp(reply_buffer, "1") != 0)
    {
        printf("WARNING: Couldn't turn motor back on after reading driver settings.\n");
        printf("Exiting to prevent further errors\n");
        exit(1);
    }
}


//pulse getters
int getMotorPosition_pulses(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "PX";
    
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get current motor position\n");
    }
    return atoi(reply_buffer);
}
int getHighSpeed_pulses(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "HSPD";
    
    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get high speed\n");
    }
    return atoi(reply_buffer);
}
int getLowSpeed_pulses(AR_HANDLE handle) {
    char reply_buffer[64];
    char command_buffer[64] = "LSPD";

    if (!fnPerformaxComSendRecv(handle, command_buffer, 64, 64, reply_buffer))
    {
        printf("Couldn't get low speed\n");
    }
    return atoi(reply_buffer);

}


//pulse setters
void setHighSpeed_pulses(AR_HANDLE handle, int new_high_speed) {
    char reply_buffer[64];
    char high_speed[64];
    sprintf(high_speed, "HSPD=%i", new_high_speed);

    if (!fnPerformaxComSendRecv(handle, high_speed, 64, 64, reply_buffer))
    {
        printf("Couldn't set high speed\n");
    }
}
void setLowSpeed_pulses(AR_HANDLE handle, int new_low_speed) {
    char reply_buffer[64];
    char low_speed[64];
    sprintf(low_speed, "LSPD=%i", new_low_speed);

    if (!fnPerformaxComSendRecv(handle, low_speed, 64, 64, reply_buffer))
    {
        printf("Couldn't set low speed\n");
    }
}


//pulse movers
void moveUseCurrent_pulses(AR_HANDLE handle, char *ABS_INC, int pulses) {
    char reply_buffer[64];
    char movement_command[64];
    sprintf(movement_command, "X%i", pulses);

    fnPerformaxComSendRecv(handle, ABS_INC, 64, 64, reply_buffer);
    fnPerformaxComSendRecv(handle, movement_command, 64, 64, reply_buffer);
}
void moveDefineAll_pulses(AR_HANDLE handle, char *ABS_INC, int pulses, int high_speed, int low_speed, int acceleration, int deceleration, int idle_time) {
    setHighSpeed_pulses(handle, high_speed);
    setLowSpeed_pulses(handle, low_speed);
    setAcceleration(handle, acceleration);
    setDeceleration(handle, deceleration);
    setIdleTime(handle, idle_time);

    moveUseCurrent_pulses(handle, ABS_INC, pulses);
}


void setMotorPosition_pulses(AR_HANDLE handle, int new_position) {
    char reply_buffer[64];
    char position[64];
    sprintf(position, "PX=%i", new_position);
    if (!fnPerformaxComSendRecv(handle, position, 64, 64, reply_buffer))
    {
        printf("Couldn't set motor position\n");
    }
}
void setEncoderPosition(AR_HANDLE handle, int new_position) {
    char reply_buffer[64];
    char position[64];
    sprintf(position, "EX=%i", new_position);
    if (!fnPerformaxComSendRecv(handle, position, 64, 64, reply_buffer))
    {
        printf("Couldn't set encoder position\n");
    }
}

void setAccelerationProfileToSin(AR_HANDLE handle) {
    char reply_buffer[64];
    char command[64] = "SCV=1";
    if (!fnPerformaxComSendRecv(handle, command, 64, 64, reply_buffer))
    {
        printf("Couldn't set acceleration profile\n");
    }
}

void setAccelerationProfileToTrap(AR_HANDLE handle) {
    char reply_buffer[64];
    char command[64] = "SCV=0";
    if (!fnPerformaxComSendRecv(handle, command, 64, 64, reply_buffer))
    {
        printf("Couldn't set acceleration profile\n");
    }
}

/*
    Some things to note:
        In these functions, you will see the function "fnPerformaxComSendRecv" everywhere.
            This is the main function that talks to the controller, it is how commands are
            sent. The parameters and what they mean, in order, are:
                AR_HANDLE pHandle : Basically a reference to the USB device which is currently
                    in use
                char *write_buffer : The buffer which actually contains the command as a string
                    the commands can be found in the manual, but they are set to this buffer
                    and passed to the device
                int bytesToWrite : The number of bytes that the machine will interpret 
                    (I think). I think basically the amount of characters the device will read
                    The manual explicitley states this should be 64, which is why all write and 
                    read buffers are initialized to 64 bytes and this is always set to 64.
                int bytesToRead : See above. Just that but for the output of the device.
                    Always set to 64.
                char *read_buffer : The buffer which holds the reply from the device. Basically on
                    all "get*" commands this holds the data which is wanting to be got. On commands
                    where the device is just been given a command with no meaningful reply, an "OK"
                    is given if successful and a "?[unrecognised command]" if it didn't understand.
                    
        Some important retypeing had to happen with most functions. Both to take int inputs and 
            convert to "char *" buffers and take "char *" return buffers and conver to int/int.
            Here is some exoplanation of the non-trivially name functions:
                int atoi(char *) -> takes in a string and returns an int
                void sprintf() -> in general takes some input and formats it into a buffer. It is
                    really great because I can use it to basically convert a number to string and 
                    combine with another string in one line. I create the buffer, define the 
                    formating to be "[command]=[number]" and then have the third parameter be the
                    number to be formatted into the string. Pretty great right?

        Very important to realize that the machine only recognizes movement and speed commands as 
            pulses and pulses/sec, respectively. So, functions which have *_pulses give this command
            in pulses. Commands in *_mm take in a mm and through a scale convert to pulses. I'm 
            debating whether to make the scale between pulses and mm a global or parameter. We will see. 

        Also important is that acceleration/deceleration are in ms and not in pulses/sec/sec. It defines
            the amount of time the motor takes to accelerate before it reaches high speed.
*/