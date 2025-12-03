#include <MCP2515.h>

// Create Instances of the CAN-Controller
MCP2515 MCP2515Module;

uint32_t defaultSettings[5] = {
    53,                 // CS-Pin (Mega = 53; Micro = 17)
    4,                  // INT-Pin
    (uint32_t)4E6,      // SPI-Frequency
    (uint32_t)8E6,      // Clock-Frequency
    (uint32_t)500E3,    // Baudrate
};

bool initializeModule(MCP2515 &Module, uint8_t CS_Pin, uint32_t SPIFrequency, uint32_t ClockFrequency, uint32_t Baudrate)
{
    if (!Module.setSpiFrequency(SPIFrequency)){
        return false;
    }

    delay(100);

    if (!Module.setClockFrequency(ClockFrequency)){
        return false;
    }

    delay(100);

    if (CS_Pin != 0)
    {
        if (!Module.setSpiPins(CS_Pin)){
            return false;
        }
    }else{
        return false;
    }

    delay(100);

    uint8_t trys = 10;
    while (!Module.init(Baudrate) && trys > 0)
    {
        delay(100);
        trys--;
    }

    if (trys == 0)
    {
        return false;
    } else {
        return true;
    }
}
