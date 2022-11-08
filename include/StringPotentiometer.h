/*
    This contains functions which are actually mainly based upon serial communication,
    with the end goal of getting the data from the arduino. This was NOT easy, and I
    do not have the absolute max confidence that it is without the possibility of 
    dramatic and traumatic errros. I'm testing it to the best of my abilities but as 
    of now, I make not promises. I need some if statements and checks. See TODO.
*/

#ifndef STRINGPOTENTIOMETER_H
#define STRINGPOTENTIOMETER_H

#include <windows.h>
#include <stdio.h>
#include "NonStageFunctions.h"

extern int OUTPUT_POSITION_TO_FILE_IN_THREAD_loop_variable;


// This clears...something. I think this makes sure
// that anything being read is fresh since I first
// query with some sent data then read. Leftover
// bytes would be bad
void purgeSerialComm(HANDLE *com_handle);

// This is some weird windows naming with 
// "DCB". It is where things like parity,
// stopbits, byte size, baud rate are set. 
// If there are data curruption problems, 
// check the baud rate first. DO NOT USE 
// BAUD_####. It does not work
void setSerialDCBParams(HANDLE *com_handle);

// Sets timeouts for the serial communication.
// It's somewhat complicated. Read windows 
// documentation and see comments in 
// StringPotentiometer.c
void setSerialTimeoutParams(HANDLE *com_handle);

// This mainly puts everything together. It 
// gets the handle pointing to the COM port,
// sets the timeouts, and the DCB params. 
// Importantly, it ends by waiting for 2 secs.
// This allows the arduino to successfully 
// reset and helps avoid data corruption
HANDLE * initializeSerialComm(char *com_name);

// Closes the com port to prevent error if
// the port is used by other programs.
void closeSerialComm(HANDLE *com_handle);

// FINALLY. The function that actually gets the
// data from the string potentiometer.
long int getStringPotData(HANDLE *com_handle);

// It seems weird but in order to run in a different thread, it needs to return
// a void pointer and take in a void pointer to parameters, even though
// I don't use any parameters
void * outputPositionToFileInThread(void *params);

#endif