#include <stdio.h>
#include "BasicStageCommands.h"
#include "AdvancedStageControl.h"
#include "NonStageFunctions.h"
#include "StringPotentiometer.h"

struct Return {
    double dwell_time;
    int length;
    int HSPD;
    int Accel;
    int Scurve;
    int cycles;
    char name[64];
};


void setParamsFromFile(AR_HANDLE handle, struct Return *params) {
    char buffer[256];

    FILE *in_file = fopen("input.txt", "r");
    fscanf(in_file, "%s\t=\t%lf", buffer, &params->dwell_time);
    fscanf(in_file, "%s\t=\t%i", buffer,  &params->length);
    fscanf(in_file, "%s\t=\t%i", buffer,  &params->HSPD);
    fscanf(in_file, "%s\t=\t%i", buffer,  &params->Accel);
    fscanf(in_file, "%s\t=\t%i", buffer,  &params->Scurve);
    fscanf(in_file, "%s\t=\t%i", buffer,  &params->cycles);
    fscanf(in_file, "%s\t=\t%s", buffer,  params->name);



    printf("dwell\t=\t%f\n", params->dwell_time);
    printf("lengt\t=\t%i\n", params->length);
    printf("hspd\t=\t%i\n",  params->HSPD);
    printf("accel\t=\t%i\n", params->Accel);
    printf("scurve\t=\t%i\n",params->Scurve);
    printf("cyles\t=\t%i\n", params->cycles);
    printf("name\t=\t%s\n",  params->name);

    fclose(in_file);

    setHighSpeed_pulses(handle, params->HSPD);
    setAcceleration(handle, params->Accel);
    if ( params->Scurve == 1)
    {setAccelerationProfileToSin(handle);}
    else 
    {setAccelerationProfileToTrap(handle);}

    setEncoderPosition(handle, 0);
    setMotorPosition_pulses(handle, 0);
}




int main() {
    struct Return params;


    AR_HANDLE handle = openStage();
    setParamsFromFile(handle, &params);

    char path[256];
    sprintf(path, "./python_data/%s", params.name);
    FILE *file = fopen(path, "w");
    outputStageValues(handle, file);
    fprintf(file, "## Dwell time = %.1f\n", params.dwell_time);
    fprintf(file, "## Length = %i\n", params.length);
    fprintf(file, "## Cycles = %i\n", params.cycles);
    fprintf(file, "\n\n");
    interactiveMode(handle);
    for (int i = 0; i < params.cycles; i++)
    {
        moveOneCycle_POS(handle, params.length, params.dwell_time, file);
    }

    fclose(file);
    closeStage(handle);
    return 0;
}