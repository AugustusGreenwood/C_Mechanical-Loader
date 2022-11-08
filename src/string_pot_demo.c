#include <stdio.h>
#include <string.h>
#include "NonStageFunctions.h"
#include "StringPotentiometer.h"


int main() {
    HANDLE *handle = initializeSerialComm("\\\\.\\COM3");

    long int data;
    while (1)
    {
        data = getStringPotData(handle);
        printf("%li\n", data);
    }

    closeSerialComm(handle);
    return 0;
}