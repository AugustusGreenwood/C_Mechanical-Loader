#include <stdio.h>
#include "BasicStageCommands.h"
#include "AdvancedStageControl.h"
#include "NonStageFunctions.h"

int main() {
    AR_HANDLE handle = openStage();

    calibrateTime(handle, 1000, 1000, 4900, 1, 10000, 0.5, 3, 0.9, 1, 0);

    closeStage(handle);
    return 0;
}