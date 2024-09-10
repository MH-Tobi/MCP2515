#include "TestLibrary.h"

// Define possible BaudRate-Array
const uint32_t possibleBaudRates[12] = {
    (uint32_t)5E3,      // Not allowed for ClockRate 25MHz and 40MHz
    (uint32_t)10E3,     // Not allowed for ClockRate 40MHz
    (uint32_t)20E3,
    (uint32_t)40E3,
    (uint32_t)50E3,
    (uint32_t)80E3,
    (uint32_t)100E3,
    (uint32_t)125E3,
    (uint32_t)200E3,
    (uint32_t)250E3,
    (uint32_t)500E3,
    (uint32_t)1000E3,   // Not allowed for ClockRate 8MHz
};

// Define possible ClockFrequency-Array
const uint32_t possibleClockFrequency[4] = {
    (uint32_t)8E6,
    (uint32_t)16E6,
    (uint32_t)25E6,
    (uint32_t)40E6,
};

bool _initialize_Module(MCP2515 &Module, uint8_t CS_Pin, uint32_t SPIFrequency, uint32_t ClockFrequency, uint32_t Baudrate){
    // Set SPI-Frequency
    if (!Module.setSpiFrequency(SPIFrequency)){
        Serial.println("\t- Setting SPI-Frequency failed.");
    }

    delay(100);

    if (!Module.setClockFrequency(ClockFrequency)){
        Serial.println("\t- Setting Clock-Frequency failed.");
    }
    
    delay(100);

    if (CS_Pin != 0)
    {
        if (!Module.setSpiPins(CS_Pin)){
            Serial.println("\t- Setting SPI-Pin failed.");
        }
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

uint8_t testInit(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];
    bool InitReturn = false;
    uint8_t Errors = 0;

    Serial.print(Teststep, DEC);
    Serial.println(": Test init-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test init-Function without setted CS-Pin");

    InitReturn = _initialize_Module(Module, 0, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate);

    if (!InitReturn){
        if (Module.getLastMCPError() != ERROR_MCP2515_INIT_CS_PIN_NOT_DEFINED){
            Serial.println("\t- Failed. False Error during Initialising with not setted CS-Pin");
            Serial.print("\t- Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    } else {
        Serial.println("\t- Failed. No Error during Initialising with not setted CS-Pin");
        Module.deinit();
        Errors++;
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test init-Function with false CS-Pin setted");

    InitReturn = _initialize_Module(Module, (CS_Pin + 1), defaultSPIFrequency, defaultClockFrequency, defaultBaudrate);

    if (!InitReturn){
        if ((Module.getLastMCPError() & ERROR_MCP2515_INIT_RESET_FAILED) != ERROR_MCP2515_INIT_RESET_FAILED){
        Serial.println("\t- Failed. False Error during Initialising with false CS-Pin");
        Serial.print("\t- Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        } else {
        Serial.println("\t- passed");
        }
    } else {
        Serial.println("\t- Failed. No Error during Initialising with false CS-Pin");
        Module.deinit();
        Errors++;
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test possible Baudrates in init-Function");

    for (size_t i = 0; i < 12; i++){

        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Set Baudrate ");
        Serial.print(possibleBaudRates[i]/1000, DEC);
        Serial.println("kB");

        InitReturn = _initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, possibleBaudRates[i]);

        if (!InitReturn){
            if ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) != ERROR_MCP2515_CNF_NOT_FOUND){
                Serial.print("\t- Failed with Error 0x");
                Serial.print(Module.getLastMCPError(), HEX);
                Serial.println(".");
                Errors++;
            } else {
                Serial.print("\t- Baudrate for choosen ClockFrequency ");
                Serial.print(defaultClockFrequency/1000000, DEC);
                Serial.println("MHz not defined. No Error!");
            }
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
        Module.deinit();
        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test undefined Baudrates in init-Function");
    uint32_t undefinedBaudrates[4] = {
        (uint32_t)0,
        (uint32_t)6e3,
        (uint32_t)110e3,
        (uint32_t)1100e3,
    };

    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i+1), DEC);
        Serial.print(": Baudrate ");
        Serial.print(undefinedBaudrates[i]/1000, DEC);
        Serial.println("kB");

        InitReturn = _initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, undefinedBaudrates[i]);

        if (!InitReturn){
            if ((Module.getLastMCPError() & ERROR_MCP2515_INIT_CHANGE_BITTIMING) != ERROR_MCP2515_INIT_CHANGE_BITTIMING){
                Serial.print("\t- Failed. False Error during Initialising with undefined Baudrate ");
                Serial.print(undefinedBaudrates[i]/1000, DEC);
                Serial.println("kB");
                Serial.print("\t- Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        } else {
            Serial.print("\t- Failed. No Error during Initialising with undefined Baudrate ");
            Serial.print(undefinedBaudrates[i]/1000, DEC);
            Serial.println("kB");
            Errors++;
        }
        delay(500);
        Module.deinit();
        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
};

uint8_t testSetSpiPins(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setSpiPins-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setSpiPins-Function when not initialized");

    if (!Module.setSpiPins(CS_Pin))
    {
        Serial.println("\t- Failed. Setting SPI-Pin not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }
    
    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setSpiPins-Function when initialized");

    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    if (Module.setSpiPins(CS_Pin))
    {
        Serial.println("\t- Failed. Setting SPI-Pin successfull in initialized Mode");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Module.deinit();
    delay(1000);
    Module.setSpiPins(CS_Pin);
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
};

uint8_t testSetSpiMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
        delay(1000);
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setSpiMode-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setSpiMode-Function when not initialized");

    if (!Module.setSpiMode(SPI_MODE0))
    {
        Serial.println("\t- Failed. Setting SPI-Mode not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setSpiMode-Function when initialized");

    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    if (Module.setSpiMode(SPI_MODE0))
    {
        Serial.println("\t- Failed. Setting SPI-Mode successfull in initialized Mode");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(500);
    Module.deinit();
    delay(500);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setSpiMode-Function with allowed Values");
    Serial.print(Teststep, DEC);
    Serial.println(".3.1: Set SPI-Mode 0");
    if (!Module.setSpiMode(SPI_MODE0))
    {
        Serial.println("\t- Failed. Setting SPI-Mode SPI_MODE0 not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(".3.2: Set SPI-Mode 3");
    if (!Module.setSpiMode(SPI_MODE3))
    {
        Serial.println("\t- Failed. Setting SPI-Mode SPI_MODE3 not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test setSpiMode-Function with not allowed Values");
    Serial.print(Teststep, DEC);
    Serial.println(".4.1: Set SPI-Mode 1");
    if (Module.setSpiMode(SPI_MODE1))
    {
        Serial.println("\t- Failed. Setting SPI-Mode SPI_MODE1 successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(".4.2: Set SPI-Mode 2");
    if (Module.setSpiMode(SPI_MODE2))
    {
        Serial.println("\t- Failed. Setting SPI-Mode SPI_MODE2 successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Module.setSpiMode(SPI_MODE0);
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
};


uint8_t testSetDataOrder(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setDataOrder-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setDataOrder-Function when not initialized");

    if (!Module.setDataOrder(MSBFIRST))
    {
        Serial.println("\t- Failed. Setting DataOrder not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setDataOrder-Function when initialized");

    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    if (Module.setDataOrder(MSBFIRST))
    {
        Serial.println("\t- Failed. Setting DataOrder successfull in initialized Mode");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(500);
    Module.deinit();
    delay(500);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setDataOrder-Function with allowed Values");
    Serial.print(Teststep, DEC);
    Serial.println(".3.1: Set DataOrder MSBFIRST");
    if (!Module.setDataOrder(MSBFIRST))
    {
        Serial.println("\t- Failed. Setting DataOrder MSBFIRST not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test setDataOrder-Function with not allowed Values");
    Serial.print(Teststep, DEC);
    Serial.println(".4.1: Set DataOrder LSBFIRST");
    if (Module.setDataOrder(LSBFIRST))
    {
        Serial.println("\t- Failed. Setting DataOrder LSBFIRST successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Module.setSpiMode(MSBFIRST);
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
};


uint8_t testSetSpiFrequency(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t allowedSPIFrequencys[4] = {
        (uint32_t)1e6,
        (uint32_t)2e6,
        (uint32_t)8e6,
        (uint32_t)10e6,
    };

    uint32_t unallowedSPIFrequencys[2] = {
        (uint32_t)5e3,
        (uint32_t)11e6,
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setSpiFrequency-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setSpiFrequency-Function when not initialized");

    if (!Module.setSpiFrequency(defaultSPIFrequency))
    {
        Serial.println("\t- Failed. Setting SpiFrequency not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setSpiFrequency-Function when initialized");

    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    if (Module.setSpiFrequency(defaultSPIFrequency))
    {
        Serial.println("\t- Failed. Setting SpiFrequency successfull in initialized Mode");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(500);
    Module.deinit();

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setSpiFrequency-Function with allowed Values");

    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Set SpiFrequency ");
        Serial.print(allowedSPIFrequencys[i]/1000000, DEC);
        Serial.println("MHz");
        if (!Module.setSpiFrequency(allowedSPIFrequencys[i]))
        {
            Serial.println("\t- Failed. Setting SpiFrequency not successfull");
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test setSpiFrequency-Function with not allowed Values");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i+1), DEC);
        Serial.print(": Set SpiFrequency ");
        Serial.print(unallowedSPIFrequencys[i]/1000000, DEC);
        Serial.println("MHz");
        if (Module.setSpiFrequency(unallowedSPIFrequencys[i]))
        {
            Serial.println("\t- Failed. Setting SpiFrequency successfull");
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Module.setSpiFrequency(defaultSPIFrequency);
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
};

uint8_t testSetClockFrequency(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){

    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t unallowedClockFrequencys[4] = {
        (uint32_t)5e6,
        (uint32_t)11e6,
        (uint32_t)20e6,
        (uint32_t)45e6,
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setClockFrequency-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setClockFrequency-Function when not initialized");

    if (!Module.setClockFrequency(defaultClockFrequency))
    {
        Serial.println("\t- Failed. Setting ClockFrequency not successfull");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    delay(500);

    if (!Module.setSpiFrequency(defaultSPIFrequency)){
        Serial.println("\t- Setting SPI-Frequency failed.");
    }
    if (!Module.setSpiPins(CS_Pin)){
        Serial.println("\t- Setting SPI-Pin failed.");
    }

    delay(500);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setClockFrequency-Function when initialized");

    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    if (Module.setClockFrequency(defaultClockFrequency))
    {
        Serial.println("\t- Failed. Setting ClockFrequency successfull in initialized Mode");
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(500);
    Module.deinit();

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setClockFrequency-Function with allowed Values");

    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Set ClockFrequency ");
        Serial.print(possibleClockFrequency[i]/1000000, DEC);
        Serial.println("MHz");
        if (!Module.setClockFrequency(possibleClockFrequency[i]))
        {
            Serial.println("\t- Failed. Setting ClockFrequency not successfull");
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test setClockFrequency-Function with not allowed Values");

    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i+1), DEC);
        Serial.print(": Set ClockFrequency ");
        Serial.print(unallowedClockFrequencys[i]/1000000, DEC);
        Serial.println("MHz");
        if (Module.setClockFrequency(unallowedClockFrequencys[i]))
        {
            Serial.println("\t- Failed. Setting ClockFrequency successfull");
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Module.setClockFrequency(defaultClockFrequency);
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
};

uint8_t testSetConfigurationMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setConfigurationMode-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setConfigurationMode-Function when not initialized");

    if (Module.setConfigurationMode()){
        Serial.println("\t- Failed. Setting ConfigurationMode successfull");
        Errors++;
        delay(500);
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Setting ConfigurationMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    delay(500);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setConfigurationMode-Function when initialized");
    
    if (!Module.setConfigurationMode())
    {
        Serial.print("\t- Failed. Setting ConfigurationMode not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
        Module.setNormalMode();
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setConfigurationMode-Function with wrong CS-Pin Configuration");
    
    delay(500);
    pinMode(CS_Pin, INPUT_PULLUP);
    delay(500);

    if (Module.setConfigurationMode())
    {
        Serial.println("\t- Failed. Setting ConfigurationMode successfull with wrong CS-Pin Configuration");
        Errors++;
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_OPERATION_MODE_NOT_SET) != ERROR_MCP2515_OPERATION_MODE_NOT_SET)
        {
            Serial.print("\t- Failed. Setting ConfigurationMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pinMode(CS_Pin, OUTPUT);
    delay(500);
    Module.setNormalMode();
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testSetNormalMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setNormalMode-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setNormalMode-Function when not initialized");

    if (Module.setNormalMode()){
        Serial.println("\t- Failed. Setting NormalMode successfull");
        Errors++;
        delay(500);
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Setting NormalMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }
    while (!Module.setConfigurationMode()){
        delay(100);
    }

    delay(500);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setNormalMode-Function when initialized");
    
    if (!Module.setNormalMode())
    {
        Serial.print("\t- Failed. Setting NormalMode not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setNormalMode-Function with wrong CS-Pin Configuration");

    while (!Module.setConfigurationMode()){
        delay(100);
    }
    
    delay(500);
    pinMode(CS_Pin, INPUT_PULLUP);
    delay(500);

    if (Module.setNormalMode())
    {
        Serial.println("\t- Failed. Setting NormalMode successfull with wrong CS-Pin Configuration");
        Errors++;
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_OPERATION_MODE_NOT_SET) != ERROR_MCP2515_OPERATION_MODE_NOT_SET)
        {
            Serial.print("\t- Failed. Setting NormalMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pinMode(CS_Pin, OUTPUT);
    delay(500);
    Module.setNormalMode();
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testSetListenOnlyMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setListenOnlyMode-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setListenOnlyMode-Function when not initialized");

    if (Module.setListenOnlyMode()){
        Serial.println("\t- Failed. Setting ListenOnlyMode successfull");
        Errors++;
        delay(500);
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Setting ListenOnlyMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    delay(500);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setListenOnlyMode-Function when initialized");
    
    if (!Module.setListenOnlyMode())
    {
        Serial.print("\t- Failed. Setting ListenOnlyMode not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
        Module.setNormalMode();
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setListenOnlyMode-Function with wrong CS-Pin Configuration");
    
    delay(500);
    pinMode(CS_Pin, INPUT_PULLUP);
    delay(500);

    if (Module.setListenOnlyMode())
    {
        Serial.println("\t- Failed. Setting ListenOnlyMode successfull with wrong CS-Pin Configuration");
        Errors++;
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_OPERATION_MODE_NOT_SET) != ERROR_MCP2515_OPERATION_MODE_NOT_SET)
        {
            Serial.print("\t- Failed. Setting ListenOnlyMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pinMode(CS_Pin, OUTPUT);
    delay(500);
    Module.setNormalMode();
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testSetLoopbackMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setLoopbackMode-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setLoopbackMode-Function when not initialized");

    if (Module.setLoopbackMode()){
        Serial.println("\t- Failed. Setting LoopbackMode successfull");
        Errors++;
        delay(500);
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Setting LoopbackMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    delay(500);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setLoopbackMode-Function when initialized");
    
    if (!Module.setLoopbackMode())
    {
        Serial.print("\t- Failed. Setting LoopbackMode not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
        Module.setNormalMode();
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setLoopbackMode-Function with wrong CS-Pin Configuration");
    
    delay(500);
    pinMode(CS_Pin, INPUT_PULLUP);
    delay(500);

    if (Module.setLoopbackMode())
    {
        Serial.println("\t- Failed. Setting LoopbackMode successfull with wrong CS-Pin Configuration");
        Errors++;
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_OPERATION_MODE_NOT_SET) != ERROR_MCP2515_OPERATION_MODE_NOT_SET)
        {
            Serial.print("\t- Failed. Setting LoopbackMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pinMode(CS_Pin, OUTPUT);
    delay(500);
    Module.setNormalMode();
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testSetSleepMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setSleepMode-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setSleepMode-Function when not initialized");

    if (Module.setSleepMode()){
        Serial.println("\t- Failed. Setting SleepMode successfull");
        Errors++;
        delay(500);
        Module.setInterruptFlag(6);
        delay(1000);
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Setting SleepMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    delay(1000);
    Module.resetInterruptFlag(6);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setSleepMode-Function when initialized");
    
    if (!Module.setSleepMode())
    {
        Serial.print("\t- Failed. Setting SleepMode not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
        delay(500);
        Module.setInterruptFlag(6);
        delay(1000);
        Module.setNormalMode();
    }

    delay(1000);
    Module.resetInterruptFlag(6);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setSleepMode-Function with wrong CS-Pin Configuration");
    
    delay(500);
    pinMode(CS_Pin, INPUT_PULLUP);
    delay(500);

    if (Module.setSleepMode())
    {
        Serial.println("\t- Failed. Setting SleepMode successfull with wrong CS-Pin Configuration");
        Errors++;
        delay(500);
        Module.setInterruptFlag(6);
        delay(1000);
        Module.setNormalMode();
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_OPERATION_MODE_NOT_SET) != ERROR_MCP2515_OPERATION_MODE_NOT_SET)
        {
            Serial.print("\t- Failed. Setting SleepMode failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pinMode(CS_Pin, OUTPUT);
    delay(500);
    Module.setNormalMode();
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testSetReCheckEnabler(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setReCheckEnabler-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setReCheckEnabler-Function when not initialized");

    if (Module.setReCheckEnabler(false)){
        Serial.println("\t- Failed. Setting ReCheckEnabler successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Setting ReCheckEnabler failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setReCheckEnabler-Function when initialized");
    
    if (!Module.setReCheckEnabler(true))
    {
        Serial.print("\t- Failed. Setting ReCheckEnabler not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setReCheckEnabler-Function with defined Values");
    Serial.println(".3.1: Set ReCheckEnabler true");

    if (!Module.setReCheckEnabler(true))
    {
        Serial.print("\t- Failed. Setting ReCheckEnabler true failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t- passed");
    }

    delay(500);

    Serial.println(".3.2: Set ReCheckEnabler false");

    if (!Module.setReCheckEnabler(false))
    {
        Serial.print("\t- Failed. Setting ReCheckEnabler false failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    Module.setReCheckEnabler(true);
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}


uint8_t testChangeBaudRate(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];
    uint8_t possibleBaudrateCounter = 0;
    
    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test changeBaudRate-Function (not stable)");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test changeBaudRate-Function when not initialized");

    if (Module.changeBaudRate(possibleBaudRates[possibleBaudrateCounter])){
        if (possibleBaudRates[possibleBaudrateCounter] == MCP2515_DEFAULT_BAUDRATE)
        {
            Serial.print("\t- \"Change\" Baudrate successfull, because the used default Baudrate equals the default Baudrate during Instanziation (");
            Serial.print((MCP2515_DEFAULT_BAUDRATE/1000), DEC);
            Serial.print("kB). Change-Process will be skipped. No Error");

            delay(500);

            possibleBaudrateCounter++;

            Serial.print(Teststep, DEC);
            Serial.println(".1.1: Test with different Baudrate");
            if (Module.changeBaudRate(possibleBaudRates[possibleBaudrateCounter])){
                Serial.println("\t- Failed. Setting changeBaudRate successfull");
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        } else {
            Serial.println("\t- Failed. Setting changeBaudRate successfull");
            Errors++;
        }
    } else {
        Serial.println("\t- passed");
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    possibleBaudrateCounter++;

    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, possibleBaudRates[possibleBaudrateCounter])){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test changeBaudRate-Function when initialized");

    possibleBaudrateCounter++;
    
    if (!Module.changeBaudRate(possibleBaudRates[possibleBaudrateCounter]))
    {
        if ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) == ERROR_MCP2515_CNF_NOT_FOUND){
            while ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) == ERROR_MCP2515_CNF_NOT_FOUND)
            {
                Serial.print("\t- Change BaudRate not successfull because Baudrate ");
                Serial.print(possibleBaudRates[possibleBaudrateCounter]/1000, DEC);
                Serial.print("kB is not defined for the used ClockFrequency ");
                Serial.print(defaultClockFrequency/1000000, DEC);
                Serial.println("MHz. No Error. Repeat test with a different Baudrate.");
                possibleBaudrateCounter++;

                delay(500);

                Module.changeBaudRate(possibleBaudRates[possibleBaudrateCounter]);
            }
                        
            if (Module.getLastMCPError() == 0x00){
                Serial.println("\t- passed");
            } else {
                Serial.print("\t- Failed. Change BaudRate with different Baudrate not successfull in initialized Mode with Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            }
        } else {
            Serial.print("\t- Failed. Change BaudRate not successfull in initialized Mode with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        }
        
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test possible Baudrates in changeBaudRate-Function");

    for (size_t i = 0; i < 12; i++){

        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Set Baudrate ");
        Serial.print(possibleBaudRates[i]/1000, DEC);
        Serial.println("kB");

        if (!Module.changeBaudRate(possibleBaudRates[i])){
            if ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) != ERROR_MCP2515_CNF_NOT_FOUND){
                Serial.print("\t- Failed with Error 0x");
                Serial.print(Module.getLastMCPError(), HEX);
                Serial.println(".");
                Errors++;
            } else {
                Serial.print("\t- Baudrate for choosen ClockFrequency ");
                Serial.print(defaultClockFrequency/1000000, DEC);
                Serial.println("MHz not defined. No Error!");
            }
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test undefined Baudrates in changeBaudRate-Function");
    uint32_t undefinedBaudrates[4] = {
        (uint32_t)0,
        (uint32_t)6e3,
        (uint32_t)110e3,
        (uint32_t)1100e3,
    };

    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i+1), DEC);
        Serial.print(": Baudrate ");
        Serial.print(undefinedBaudrates[i]/1000, DEC);
        Serial.println("kB");

        if (!Module.changeBaudRate(undefinedBaudrates[i])){
            if ((Module.getLastMCPError() & ERROR_MCP2515_BAUDRATE_NOT_VALID) != ERROR_MCP2515_BAUDRATE_NOT_VALID){
                Serial.print("\t- Failed. False Error during changeBaudRate with undefined Baudrate ");
                Serial.print(undefinedBaudrates[i]/1000, DEC);
                Serial.println("kB");
                Serial.print("\t- Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        } else {
            Serial.print("\t- Failed. No Error during changeBaudRate with undefined Baudrate ");
            Serial.print(undefinedBaudrates[i]/1000, DEC);
            Serial.println("kB");
            Errors++;
        }
        
        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test changeBaudRate from OperationMode ListenOnly");

    for (size_t i = 0; i < 12; i++){

        while (!Module.setListenOnlyMode()){
            delay(100);
        }
        delay(500);

        Serial.print(Teststep, DEC);
        Serial.print(".5.");
        Serial.print((i+1), DEC);
        Serial.print(": Set Baudrate ");
        Serial.print(possibleBaudRates[i]/1000, DEC);
        Serial.println("kB");

        if (!Module.changeBaudRate(possibleBaudRates[i])){
            if ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) != ERROR_MCP2515_CNF_NOT_FOUND){
                Serial.print("\t- Failed with Error 0x");
                Serial.print(Module.getLastMCPError(), HEX);
                Serial.println(".");
                Errors++;
            } else {
                Serial.print("\t- Baudrate ");
                Serial.print(possibleBaudRates[i]/1000, DEC);
                Serial.print("kB for choosen ClockFrequency ");
                Serial.print(defaultClockFrequency/1000000, DEC);
                Serial.println("MHz not defined. No Error!");
            }
        } else {
            if (Module.getOperationMode() != MCP2515_OP_LISTEN)
            {
                Serial.println("\t- failed. ChangeBaudrate didn't finish in ListenOnly-Mode.");
                Errors++;
            } else {
                Serial.println("\t- passed");
            }            
        }
        delay(500);
    }

    while (!Module.setNormalMode()){
        delay(100);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".6: Test changeBaudRate from OperationMode Loopback");

    for (size_t i = 0; i < 12; i++){

        while (!Module.setLoopbackMode()){
            delay(100);
        }
        delay(500);

        Serial.print(Teststep, DEC);
        Serial.print(".6.");
        Serial.print((i+1), DEC);
        Serial.print(": Set Baudrate ");
        Serial.print(possibleBaudRates[i]/1000, DEC);
        Serial.println("kB");

        if (!Module.changeBaudRate(possibleBaudRates[i])){
            if ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) != ERROR_MCP2515_CNF_NOT_FOUND){
                Serial.print("\t- Failed with Error 0x");
                Serial.print(Module.getLastMCPError(), HEX);
                Serial.println(".");
                Errors++;
            } else {
                Serial.print("\t- Baudrate ");
                Serial.print(possibleBaudRates[i]/1000, DEC);
                Serial.print("kB for choosen ClockFrequency ");
                Serial.print(defaultClockFrequency/1000000, DEC);
                Serial.println("MHz not defined. No Error!");
            }
        } else {
            if (Module.getOperationMode() != MCP2515_OP_LOOPBACK)
            {
                Serial.println("\t- failed. ChangeBaudrate didn't finish in Loopback-Mode.");
                Errors++;
            } else {
                Serial.println("\t- passed");
            }            
        }
        delay(500);
    }

    while (!Module.setNormalMode()){
        delay(100);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".7: Test changeBaudRate from OperationMode Configuration");

    for (size_t i = 0; i < 12; i++){

        while (!Module.setConfigurationMode()){
            delay(100);
        }
        delay(500);

        Serial.print(Teststep, DEC);
        Serial.print(".7.");
        Serial.print((i+1), DEC);
        Serial.print(": Set Baudrate ");
        Serial.print(possibleBaudRates[i]/1000, DEC);
        Serial.println("kB");

        if (!Module.changeBaudRate(possibleBaudRates[i])){
            if ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) != ERROR_MCP2515_CNF_NOT_FOUND){
                Serial.print("\t- Failed with Error 0x");
                Serial.print(Module.getLastMCPError(), HEX);
                Serial.println(".");
                Errors++;
            } else {
                Serial.print("\t- Baudrate ");
                Serial.print(possibleBaudRates[i]/1000, DEC);
                Serial.print("kB for choosen ClockFrequency ");
                Serial.print(defaultClockFrequency/1000000, DEC);
                Serial.println("MHz not defined. No Error!");
            }
        } else {
            if (Module.getOperationMode() != MCP2515_OP_CONFIGURATION)
            {
                Serial.println("\t- failed. ChangeBaudrate didn't finish in Configuration-Mode.");
                Errors++;
            } else {
                Serial.println("\t- passed");
            }            
        }
        delay(500);
    }

    while (!Module.setNormalMode()){
        delay(100);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".8: Test changeBaudRate from OperationMode Normal");

    for (size_t i = 0; i < 12; i++){

        while (!Module.setNormalMode()){
            delay(100);
        }
        delay(500);

        Serial.print(Teststep, DEC);
        Serial.print(".8.");
        Serial.print((i+1), DEC);
        Serial.print(": Set Baudrate ");
        Serial.print(possibleBaudRates[i]/1000, DEC);
        Serial.println("kB");

        if (!Module.changeBaudRate(possibleBaudRates[i])){
            if ((Module.getLastMCPError() & ERROR_MCP2515_CNF_NOT_FOUND) != ERROR_MCP2515_CNF_NOT_FOUND){
                Serial.print("\t- Failed with Error 0x");
                Serial.print(Module.getLastMCPError(), HEX);
                Serial.println(".");
                Errors++;
            } else {
                Serial.print("\t- Baudrate ");
                Serial.print(possibleBaudRates[i]/1000, DEC);
                Serial.print("kB for choosen ClockFrequency ");
                Serial.print(defaultClockFrequency/1000000, DEC);
                Serial.println("MHz not defined. No Error!");
            }
        } else {
            if (Module.getOperationMode() != MCP2515_OP_NORMAL)
            {
                Serial.println("\t- failed. ChangeBaudrate didn't finish in Normal-Mode.");
                Errors++;
            } else {
                Serial.println("\t- passed");
            }            
        }
        delay(500);
    }

    while (!Module.setNormalMode()){
        delay(100);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".9: Test changeBaudRate from OperationMode Sleep");
    Serial.println("\t- Makes no sense!");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    Module.changeBaudRate(defaultBaudrate);
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
};

uint8_t testChangeInterruptSetting(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test changeInterruptSetting-Function");
    Serial.println();
    Serial.println("Please be sure that the Interrupt-Pin is connected correctly!");

    delay(5000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test changeInterruptSetting-Function when not initialized");

    if (Module.changeInterruptSetting(true, 1)){
        Serial.println("\t- Failed. Change InterruptSetting successfull");
        Module.changeInterruptSetting(false, 1);
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Change InterruptSetting failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }   
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test changeInterruptSetting-Function when initialized");

    if (!Module.changeInterruptSetting(true, 1))
    {
        Serial.print("\t- Failed. Change InterruptSetting not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
        Module.changeInterruptSetting(false, 1);
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test changeInterruptSetting-Function with defined Values");

    for (size_t i = 0; i < 8; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Change Interrupt ");
        Serial.print(i, DEC);
        Serial.println(" to true");

        if (!Module.changeInterruptSetting(true, i))
        {
            Serial.print("\t- Failed. Change InterruptSetting failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;

        } else {
            Serial.println("\t- passed");
            Module.changeInterruptSetting(false, i);
        }

        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test changeInterruptSetting-Function with undefined Values");

    for (size_t i = 8; i < 14; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-8), DEC);
        Serial.print(": Change Interrupt ");
        Serial.print(i, DEC);
        Serial.println(" to true");

        if (Module.changeInterruptSetting(true, i))
        {
            Serial.println("\t- Failed. Change InterruptSetting successfull");
            Errors++;

        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. Change InterruptSetting failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test if Interrupts occurs on INT-Pin");

    for (size_t i = 0; i < 8; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".5.");
        Serial.print((i+1), DEC);
        Serial.print(": Test Interrupt ");
        Serial.println(i, DEC);

        while (!Module.resetInterruptFlag(i))
        {
            delay(100);
        }

        Serial.println("\t- set Interrupt-Enabler true");

        while (!Module.changeInterruptSetting(true, i))
        {
            delay(100);
        }

        delay(500);
        Serial.print("\t\t- corresponding Flag-Value ");
        Serial.println((((Module.check4InterruptFlags() & (0x01 << i)) > 0) ? 1 : 0), DEC);

        Serial.println("\t- set corresponding Flag");
        while (!Module.setInterruptFlag(i))
        {
            delay(100);
        }

        delay(100);

        Serial.println("\t- Check Value of the INT-Pin");

        if (digitalRead(INT_Pin) == 0)
        {
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t\t- no Interrupt occured");
            Errors++;
        }

        while (!Module.resetInterruptFlag(i))
        {
            delay(100);
        }

        while (!Module.changeInterruptSetting(false, i))
        {
            delay(100);
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}


uint8_t testEnableFilterMask(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test enableFilterMask-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test enableFilterMask-Function when not initialized");

    if (Module.enableFilterMask(0)){
        Serial.println("\t- Failed. Enable FilterMask successfull");
        Module.disableFilterMask(0);
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Enable FilterMask failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test enableFilterMask-Function when initialized");
    
    if (!Module.enableFilterMask(0))
    {
        Serial.print("\t- Failed. Enable FilterMask not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
        Module.disableFilterMask(0);
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test enableFilterMask-Function with defined Values");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Enable FilterMask of Buffer ");
        Serial.println(i, DEC);

        if (!Module.enableFilterMask(i))
        {
            Serial.print("\t- Failed. Enable FilterMask failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
            Module.disableFilterMask(i);
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test enableFilterMask-Function with undefined Values");

    for (size_t i = 2; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": Enable FilterMask of Buffer ");
        Serial.println(i, DEC);

        if (Module.enableFilterMask(i))
        {
            Serial.println("\t- Failed. Enable FilterMask successfull");
            Module.disableFilterMask(i);
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. Enable FilterMask failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testDisableFilterMask(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test disableFilterMask-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test disableFilterMask-Function when not initialized");

    if (Module.disableFilterMask(0)){
        Serial.println("\t- Failed. Disnable FilterMask successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Disable FilterMask failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test disableFilterMask-Function when initialized");
    
    if (!Module.disableFilterMask(0))
    {
        Serial.print("\t- Failed. Disable FilterMask not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test disableFilterMask-Function with defined Values");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Disable FilterMask of Buffer ");
        Serial.println(i, DEC);

        if (!Module.disableFilterMask(i))
        {
            Serial.print("\t- Failed. Disable FilterMask failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test disableFilterMask-Function with undefined Values");

    for (size_t i = 2; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": Disable FilterMask of Buffer ");
        Serial.println(i, DEC);

        if (Module.disableFilterMask(i))
        {
            Serial.println("\t- Failed. Disnable FilterMask successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. Disable FilterMask failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}


uint8_t testCheck4FreeTransmitBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test check4FreeTransmitBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test check4FreeTransmitBuffer-Function when not initialized");

    if (Module.check4FreeTransmitBuffer() != 0xFF){
        Serial.println("\t- Failed. Check for a free Transmit-Buffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Check for a free Transmit-Buffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test check4FreeTransmitBuffer-Function when initialized");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);
    
    if (Module.check4FreeTransmitBuffer() == 0xFF)
    {
        Serial.print("\t- Failed. Check for a free Transmit-Buffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test check4FreeTransmitBuffer-Function for each TX-Buffer");

    Serial.println("\t- Reset all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.resetInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 0; i < 3; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Check for a free Transmit-Buffer ");
        Serial.println(i, DEC);

        Serial.print("\t- Set \"Transmit-Buffer free\"-Flag for Buffer ");
        Serial.println(i, DEC);

        Module.setInterruptFlag((i + 2));

        delay(500);

        if (Module.check4FreeTransmitBuffer() != i)
        {
            Serial.print("\t\t- Failed. Check for a free Transmit-Buffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t\t- passed");
        }

        delay(500);

        Module.resetInterruptFlag((i + 2));
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testFillTransmitBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint8_t Data[8] = {
        1,
        1,
        1,
        1
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test fillTransmitBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test fillTransmitBuffer-Function when not initialized");

    if (Module.fillTransmitBuffer(0, 1, true, false, 4, Data)){
        Serial.println("\t- Failed. Fill Transmit-Buffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Fill Transmit-Buffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test fillTransmitBuffer-Function when initialized");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);
    
    if (!Module.fillTransmitBuffer(0, 1, true, false, 4, Data))
    {
        Serial.print("\t- Failed. Fill Transmit-Buffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
        Module.setInterruptFlag(2);
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test fillTransmitBuffer-Function for each TX-Buffer");

    Serial.println("\t- Reset all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.resetInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 0; i < 3; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Fill Transmit-Buffer ");
        Serial.println(i, DEC);

        Serial.print("\t- Set \"Transmit-Buffer free\"-Flag for Buffer ");
        Serial.println(i, DEC);

        Module.setInterruptFlag((i + 2));

        delay(500);

        if (!Module.fillTransmitBuffer(i, 1, true, false, 4, Data))
        {
            Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t\t- passed");
        }

        delay(500);

        Module.resetInterruptFlag((i + 2));
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test fillTransmitBuffer-Function for undefined TX-Buffer");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 3; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-2), DEC);
        Serial.print(": Fill Transmit-Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (Module.fillTransmitBuffer(i, 1, true, false, 4, Data))
        {
            Serial.println("\t\t- Failed. Fill Transmit-Buffer successfull");
            Module.setInterruptFlag((i + 2));
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test fillTransmitBuffer-Function for defined IDs");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 0; i < 3; i++)
    {
        uint32_t ID = 150000000 * (i + 1);
        Serial.print(Teststep, DEC);
        Serial.print(".5.");
        Serial.print((i+1), DEC);
        Serial.print(": Fill Transmit-Buffer with ID ");
        Serial.println(ID, HEX);

        delay(500);

        if (!Module.fillTransmitBuffer(0, ID, true, false, 4, Data))
        {
            Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t\t- passed");
        }

        delay(500);

        Module.setInterruptFlag(2);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".6: Test fillTransmitBuffer-Function for undefined IDs");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 3; i < 5; i++)
    {
        uint32_t ID = 540000000 + (i * 2);
        Serial.print(Teststep, DEC);
        Serial.print(".6.");
        Serial.print((i-2), DEC);
        Serial.print(": Fill Transmit-Buffer with ID ");
        Serial.println(ID, HEX);

        delay(500);

        if (Module.fillTransmitBuffer(0, ID, true, false, 4, Data))
        {
            Serial.println("\t\t- Failed. Fill Transmit-Buffer successfull");
            Module.setInterruptFlag(2);
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".7: Test fillTransmitBuffer-Function for Extended Enabler");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(".7.1: Fill Transmit-Buffer with Extended True");

    if (!Module.fillTransmitBuffer(0, 1, true, false, 4, Data))
    {
        Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t\t- passed");
    }

    delay(500);

    Module.setInterruptFlag(2);
    
    Serial.print(Teststep, DEC);
    Serial.println(".7.2: Fill Transmit-Buffer with Extended False");

    if (!Module.fillTransmitBuffer(0, 1, false, false, 4, Data))
    {
        Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t\t- passed");
    }

    Module.setInterruptFlag(2);

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".8: Test fillTransmitBuffer-Function for RTR Enabler");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(".8.1: Fill Transmit-Buffer with RTR True");

    if (!Module.fillTransmitBuffer(0, 1, true, true, 4, Data))
    {
        Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t\t- passed");
    }

    delay(500);

    Module.setInterruptFlag(2);
    
    Serial.print(Teststep, DEC);
    Serial.println(".8.2: Fill Transmit-Buffer with Extended False");

    if (!Module.fillTransmitBuffer(0, 1, true, false, 4, Data))
    {
        Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t\t- passed");
    }

    Module.setInterruptFlag(2);

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".9: Test fillTransmitBuffer-Function for defined DLCs");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 0; i < 9; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".9.");
        Serial.print((i+1), DEC);
        Serial.print(": Fill Transmit-Buffer with DLC ");
        Serial.println(i, DEC);

        if (!Module.fillTransmitBuffer(0, 1, true, false, i, Data))
        {
            Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t\t- passed");
        }

        delay(500);

        Module.setInterruptFlag(2);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".10: Test fillTransmitBuffer-Function for undefined DLCs");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 9; i < 12; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".10.");
        Serial.print((i-8), DEC);
        Serial.print(": Fill Transmit-Buffer with DLC ");
        Serial.println(i, DEC);

        if (Module.fillTransmitBuffer(0, 1, true, false, i, Data))
        {
            Serial.print("\t\t- Failed. Fill Transmit-Buffer successfull");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t\t- passed");
            }
        }

        delay(500);

        Module.setInterruptFlag(2);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".11: Test fillTransmitBuffer-Function with no free Transmit-Buffer");

    Serial.println("\t- Reset all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.resetInterruptFlag((i + 2));
    }
    
    delay(500);

    if (Module.fillTransmitBuffer(0, 1, true, false, 4, Data))
    {
        Serial.println("\t- Failed. Fill Transmit-Buffer successfull");
        Errors++;
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_TRANSMITBUFFER_NOT_FREE) != ERROR_MCP2515_TRANSMITBUFFER_NOT_FREE)
        {
            Serial.print("\t\t- Failed. Fill Transmit-Buffer failed with wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testSendMessage(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 1;
    bool Extended = false;
    bool RTR = false;
    uint8_t DLC = 2;
    uint8_t Data[2] = {
        1,
        1
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test sendMessage-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test sendMessage-Function when not initialized");

    if (Module.sendMessage(0, 0)){
        Serial.println("\t- Failed. Send Message successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. Send Message failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    Serial.println("- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test sendMessage-Function when initialized");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Data");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
        delay(100);
    }
    
    delay(500);
    
    Serial.println("\t- send Message");
    if (!Module.sendMessage(0, 0))
    {
        Serial.print("\t\t- Failed. Send Message not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test sendMessage-Function with each TX-Buffer");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 0; i < 3; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": Send Message from Transmit-Buffer ");
        Serial.println(i, DEC);

        Serial.print("\t- Fill Transmit-Buffer ");
        Serial.print(i, DEC);
        Serial.println(" with Data");
        while (!Module.fillTransmitBuffer(i, ID, Extended, RTR, DLC, Data)){
            delay(100);
        }

        delay(500);

        if (!Module.sendMessage(i, 0))
        {
            Serial.print("\t\t- Failed. Send Message failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test sendMessage-Function with undefined TX-Buffer");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 3; i < 6; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-2), DEC);
        Serial.print(": Send Message from Transmit-Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (Module.sendMessage(i, 0))
        {
            Serial.println("\t\t- Failed. Send Message successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t\t- Failed. Send Message failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test sendMessage-Function with defined Priority");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".5.");
        Serial.print((i+1), DEC);
        Serial.print(": Send Message with Priority ");
        Serial.println(i, DEC);

        Serial.println("\t- Fill Transmit-Buffer 0 with Data");
        while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
            delay(100);
        }

        delay(500);

        if (!Module.sendMessage(0, i))
        {
            Serial.print("\t\t- Failed. Send Message failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".6: Test sendMessage-Function with undefined Priority");

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    for (size_t i = 5; i < 7; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".6.");
        Serial.print((i-4), DEC);
        Serial.print(": Send Message with Priority ");
        Serial.println(i, DEC);

        delay(500);

        if (Module.sendMessage(i, 0))
        {
            Serial.println("\t\t- Failed. Send Message successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t\t- Failed. Send Message failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t\t- passed");
            }
        }
        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".7: Test sendMessage-Function without CAN-Bus");

    Serial.println("\t- Set OperationMode Normal");
    while (!Module.setNormalMode()){
        delay(100);
    }

    Serial.println("\t- Set all \"Transmit-Buffer free\"-Flags");
    for (size_t i = 0; i < 3; i++)
    {
        Module.setInterruptFlag((i + 2));
    }
    
    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Data");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
        delay(100);
    }

    Serial.println("\t- Send Message");
    if (Module.sendMessage(0, 0))
    {
        Serial.println("\t\t- Failed. Send Message successfull");
        Errors++;
        
    } else {
        if (((Module.getLastMCPError() & 0x0010) != 0x0010) && ((Module.getLastMCPError() & 0x0020) != 0x0020))
        {
            Serial.print("\t\t- Failed. Send Message failed with wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testCheck4Rtr(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 1;
    bool Extended = false;
    bool RTR = true;
    uint8_t DLC = 2;
    uint8_t Data[2] = {
        1,
        2
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test check4Rtr-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test check4Rtr-Function when not initialized");

    Module.check4Rtr(ID, Extended);

    if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT){
        Serial.println("\t- Failed. check4Rtr successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. check4Rtr failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test check4Rtr-Function when initialized");
    
    Module.check4Rtr(ID, Extended);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. check4Rtr not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test check4Rtr-Function with defined IDs");

    for (size_t i = 0; i < 3; i++)
    {
        uint32_t ID_defined = 150000000 * (i + 1);
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": check4Rtr with ID ");
        Serial.println(ID_defined, HEX);

        delay(500);

        Module.check4Rtr(ID_defined, Extended);

        if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
        {
            Serial.print("\t- Failed. check4Rtr failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test check4Rtr-Function for undefined IDs");

    for (size_t i = 3; i < 5; i++)
    {
        uint32_t ID_undefined = 540000000 + (i * 2);
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-2), DEC);
        Serial.print(": check4Rtr with ID ");
        Serial.println(ID_undefined, HEX);

        delay(500);

        Module.check4Rtr(ID_undefined, Extended);

        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT)
        {
            Serial.println("\t- Failed. check4Rtr successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. check4Rtr failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test check4Rtr-Function for Extended Enabler");

    Serial.print(Teststep, DEC);
    Serial.println(".5.1: check4Rtr with Extended True");

    Module.check4Rtr(ID, true);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. check4Rtr failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t- passed");
    }

    delay(500);
    
    Serial.print(Teststep, DEC);
    Serial.println(".5.2: check4Rtr with Extended False");

    Module.check4Rtr(ID, false);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. check4Rtr failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".6: Test check4Rtr-Function for received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with RTR-Message");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Check for received RTR Message");
    if (!Module.check4Rtr(ID, false))
    {
        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT) {
            Serial.println("\t\t- Failed. No RTR Message Received");
            Errors++;
        } else {
            Serial.print("\t\t- Failed. check4Rtr failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        }
    } else {
        Serial.println("\t\t- passed");
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testCheck4Receive(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 1;
    bool Extended = false;
    bool RTR = false;
    uint8_t DLC = 4;
    uint8_t Data_Receive[8] = {0, 0, 0, 0};
    uint8_t Data_Transmit[8] = {1, 2, 3, 4, 0, 0, 0, 0};

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test check4Receive-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test check4Receive-Function when not initialized");

    Module.check4Receive(ID, Extended, DLC, Data_Receive);

    if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT){
        Serial.println("\t- Failed. check4Receive successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. check4Receive failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test check4Receive-Function when initialized");
    
    Module.check4Receive(ID, Extended, DLC, Data_Receive);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. check4Receive not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test check4Receive-Function with defined IDs");

    for (size_t i = 0; i < 3; i++)
    {
        uint32_t ID_defined = 150000000 * (i + 1);
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": check4Receive with ID ");
        Serial.println(ID_defined, HEX);

        delay(500);

        Module.check4Receive(ID_defined, true, DLC, Data_Receive);

        if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
        {
            Serial.print("\t- Failed. check4Receive failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test check4Receive-Function for undefined IDs");

    for (size_t i = 3; i < 5; i++)
    {
        uint32_t ID_undefined = 540000000 + (i * 2);
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-2), DEC);
        Serial.print(": check4Receive with ID ");
        Serial.println(ID_undefined, HEX);

        delay(500);

        Module.check4Receive(ID_undefined, true, DLC, Data_Receive);

        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT)
        {
            Serial.println("\t- Failed. check4Receive successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. check4Receive failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test check4Receive-Function for Extended Enabler");

    Serial.print(Teststep, DEC);
    Serial.println(".5.1: check4Receive with Extended True");

    Module.check4Receive(ID, true, DLC, Data_Receive);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. check4Receive failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t- passed");
    }

    delay(500);
    
    Serial.print(Teststep, DEC);
    Serial.println(".5.2: check4Receive with Extended False");

    Module.check4Receive(ID, false, DLC, Data_Receive);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. check4Receive failed with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
        
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".6: Test check4Receive-Function with defined DLCs");

    for (size_t i = 0; i < 9; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".6.");
        Serial.print((i+1), DEC);
        Serial.print(": check4Receive with DLC ");
        Serial.println(i, DEC);

        delay(500);

        Module.check4Receive(ID, Extended, i, Data_Receive);

        if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
        {
            Serial.print("\t- Failed. check4Receive failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".7: Test check4Receive-Function for undefined DLCs");

    for (size_t i = 9; i < 12; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".7.");
        Serial.print((i-8), DEC);
        Serial.print(": check4Receive with DLC ");
        Serial.println(i, DEC);

        delay(500);

        Module.check4Receive(ID, Extended, i, Data_Receive);

        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT)
        {
            Serial.println("\t- Failed. check4Receive successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. check4Receive failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".8: Test check4Receive-Function for received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);
    
    Serial.println("\t- Fill Transmit-Buffer 0 with RTR-Message");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data_Transmit)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    bool Received_Data_passed = true;

    Serial.println("\t- Check for received Message");
    if (!Module.check4Receive(ID, Extended, DLC, Data_Receive))
    {
        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT) {
            Serial.println("\t\t- Failed. No Message Received");
            Errors++;
        } else {
            Serial.print("\t\t- Failed. check4Receive failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        }
    } else {
        for (size_t i = 0; i < DLC; i++) {
            if ((Data_Receive[i] != Data_Transmit[i])) {
                Received_Data_passed = false;
                Serial.println("\t\t- Failed. Data of the received Message doesn't match the sended Data.");
                Errors++;
                break;
            }
        }

        if (Received_Data_passed)
        {
            Serial.println("\t\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testGetIdFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 435;
    bool Extended = false;
    bool RTR = true;
    uint8_t DLC = 2;
    uint8_t Data[2] = {
        1,
        2
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test getIdFromReceiveBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test getIdFromReceiveBuffer-Function when not initialized");

    Module.getIdFromReceiveBuffer(0);

    if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT){
        Serial.println("\t- Failed. getIdFromReceiveBuffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. getIdFromReceiveBuffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test getIdFromReceiveBuffer-Function when initialized");
    
    Module.getIdFromReceiveBuffer(0);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. getIdFromReceiveBuffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test getIdFromReceiveBuffer-Function from defined Receive-Buffers");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": getIdFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getIdFromReceiveBuffer(i);

        if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
        {
            Serial.print("\t- Failed. getIdFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test getIdFromReceiveBuffer-Function from undefined Receive-Buffers");

    for (size_t i = 2; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": getIdFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getIdFromReceiveBuffer(i);

        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT)
        {
            Serial.println("\t- Failed. getIdFromReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. getIdFromReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test getIdFromReceiveBuffer-Function from received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Message");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Check which Receive-Buffer received Message");
    if ((Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        Serial.println("\t- Message received in Buffer 0");
        Serial.println("\t- Check if received ID matches sended ID");
        if (Module.getIdFromReceiveBuffer(0) == ID){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received ID doesn't match sended ID");
            Errors++;
        }
    } else if ((Module.check4InterruptFlags() & 0x02) == 0x02)
    {
        Serial.println("\t- Message received in Buffer 1");
        Serial.println("\t- Check if received ID matches sended ID");
        if (Module.getIdFromReceiveBuffer(1) == ID){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received ID doesn't match sended ID");
            Errors++;
        }
    } else {
        Serial.println("\t- Failed. No Message received");
        Errors++;
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testGetDlcFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 435;
    bool Extended = false;
    bool RTR = true;
    uint8_t DLC = 2;
    uint8_t Data[2] = {
        1,
        2
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test getDlcFromReceiveBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test getDlcFromReceiveBuffer-Function when not initialized");

    Module.getDlcFromReceiveBuffer(0);

    if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT){
        Serial.println("\t- Failed. getDlcFromReceiveBuffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. getDlcFromReceiveBuffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test getDlcFromReceiveBuffer-Function when initialized");
    
    Module.getDlcFromReceiveBuffer(0);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. getDlcFromReceiveBuffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test getDlcFromReceiveBuffer-Function from defined Receive-Buffers");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": getDlcFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getDlcFromReceiveBuffer(i);

        if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
        {
            Serial.print("\t- Failed. getDlcFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test getDlcFromReceiveBuffer-Function from undefined Receive-Buffers");

    for (size_t i = 2; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": getDlcFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getDlcFromReceiveBuffer(i);

        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT)
        {
            Serial.println("\t- Failed. getDlcFromReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. getDlcFromReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test getDlcFromReceiveBuffer-Function from received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Message");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Check which Receive-Buffer received Message");
    if ((Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        Serial.println("\t- Message received in Buffer 0");
        Serial.println("\t- Check if received DLC matches sended DLC");
        if (Module.getDlcFromReceiveBuffer(0) == DLC){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received DLC doesn't match sended DLC");
            Errors++;
        }
    } else if ((Module.check4InterruptFlags() & 0x02) == 0x02)
    {
        Serial.println("\t- Message received in Buffer 1");
        Serial.println("\t- Check if received DLC matches sended DLC");
        if (Module.getDlcFromReceiveBuffer(1) == DLC){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received DLC doesn't match sended DLC");
            Errors++;
        }
    } else {
        Serial.println("\t- Failed. No Message received");
        Errors++;
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testGetFrameFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 435;
    bool Extended = false;
    bool RTR = true;
    uint8_t DLC = 2;
    uint8_t Data[2] = {
        1,
        2
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test getFrameFromReceiveBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test getFrameFromReceiveBuffer-Function when not initialized");

    Module.getFrameFromReceiveBuffer(0);

    if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT){
        Serial.println("\t- Failed. getFrameFromReceiveBuffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. getFrameFromReceiveBuffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test getFrameFromReceiveBuffer-Function when initialized");
    
    Module.getFrameFromReceiveBuffer(0);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. getFrameFromReceiveBuffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test getFrameFromReceiveBuffer-Function from defined Receive-Buffers");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": getFrameFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getFrameFromReceiveBuffer(i);

        if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
        {
            Serial.print("\t- Failed. getFrameFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test getFrameFromReceiveBuffer-Function from undefined Receive-Buffers");

    for (size_t i = 2; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": getFrameFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getFrameFromReceiveBuffer(i);

        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT)
        {
            Serial.println("\t- Failed. getFrameFromReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. getFrameFromReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test getFrameFromReceiveBuffer-Function from received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Message");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Check which Receive-Buffer received Message");
    if ((Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        Serial.println("\t- Message received in Buffer 0");
        Serial.println("\t- Check if received Frame matches sended Frame");
        if (Module.getFrameFromReceiveBuffer(0) == Extended){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received Frame doesn't match sended Frame");
            Errors++;
        }
    } else if ((Module.check4InterruptFlags() & 0x02) == 0x02)
    {
        Serial.println("\t- Message received in Buffer 1");
        Serial.println("\t- Check if received Frame matches sended Frame");
        if (Module.getFrameFromReceiveBuffer(1) == Extended){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received Frame doesn't match sended Frame");
            Errors++;
        }
    } else {
        Serial.println("\t- Failed. No Message received");
        Errors++;
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testGetRtrFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 435;
    bool Extended = false;
    bool RTR = true;
    uint8_t DLC = 2;
    uint8_t Data[2] = {
        1,
        2
    };

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test getRtrFromReceiveBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test getRtrFromReceiveBuffer-Function when not initialized");

    Module.getRtrFromReceiveBuffer(0);

    if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT){
        Serial.println("\t- Failed. getRtrFromReceiveBuffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. getRtrFromReceiveBuffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test getRtrFromReceiveBuffer-Function when initialized");
    
    Module.getRtrFromReceiveBuffer(0);

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. getRtrFromReceiveBuffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test getRtrFromReceiveBuffer-Function from defined Receive-Buffers");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": getRtrFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getRtrFromReceiveBuffer(i);

        if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
        {
            Serial.print("\t- Failed. getRtrFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test getRtrFromReceiveBuffer-Function from undefined Receive-Buffers");

    for (size_t i = 2; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": getRtrFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        Module.getRtrFromReceiveBuffer(i);

        if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT)
        {
            Serial.println("\t- Failed. getRtrFromReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. getRtrFromReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test getRtrFromReceiveBuffer-Function from received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Message");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Check which Receive-Buffer received Message");
    if ((Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        Serial.println("\t- Message received in Buffer 0");
        Serial.println("\t- Check if received RTR matches sended RTR");
        if (Module.getRtrFromReceiveBuffer(0) == RTR){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received RTR doesn't match sended RTR");
            Errors++;
        }
    } else if ((Module.check4InterruptFlags() & 0x02) == 0x02)
    {
        Serial.println("\t- Message received in Buffer 1");
        Serial.println("\t- Check if received RTR matches sended RTR");
        if (Module.getRtrFromReceiveBuffer(1) == RTR){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received RTR doesn't match sended RTR");
            Errors++;
        }
    } else {
        Serial.println("\t- Failed. No Message received");
        Errors++;
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testGetDataFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID = 1;
    bool Extended = false;
    bool RTR = false;
    uint8_t DLC = 4;
    uint8_t Data_Receive[8] = {0, 0, 0, 0};
    uint8_t Data_Transmit[8] = {1, 2, 3, 4, 0, 0, 0, 0};

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test getDataFromReceiveBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test getDataFromReceiveBuffer-Function when not initialized");

    if (Module.getDataFromReceiveBuffer(0, DLC, Data_Receive)){
        Serial.println("\t- Failed. getDataFromReceiveBuffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. getDataFromReceiveBuffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test getDataFromReceiveBuffer-Function when initialized");

    if (!Module.getDataFromReceiveBuffer(0, DLC, Data_Receive))
    {
        Serial.print("\t- Failed. getDataFromReceiveBuffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test getDataFromReceiveBuffer-Function from defined Receive-Buffers");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": getDataFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (!Module.getDataFromReceiveBuffer(i, DLC, Data_Receive))
        {
            Serial.print("\t- Failed. getDataFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test getDataFromReceiveBuffer-Function from undefined Receive-Buffers");

    for (size_t i = 2; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": getDataFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (Module.getDataFromReceiveBuffer(i, DLC, Data_Receive))
        {
            Serial.println("\t- Failed. getDataFromReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. getDataFromReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test getDataFromReceiveBuffer-Function with defined DLCs");

    for (size_t i = 0; i < 9; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".5.");
        Serial.print((i+1), DEC);
        Serial.print(": getDataFromReceiveBuffer with DLC ");
        Serial.println(i, DEC);

        delay(500);

        if (!Module.getDataFromReceiveBuffer(0, i, Data_Receive))
        {
            Serial.print("\t- Failed. getDataFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".6: Test getDataFromReceiveBuffer-Function with undefined DLCs");

    for (size_t i = 9; i < 12; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".6.");
        Serial.print((i-8), DEC);
        Serial.print(": getDataFromReceiveBuffer with DLC ");
        Serial.println(i, DEC);

        delay(500);

        if (Module.getDataFromReceiveBuffer(0, i, Data_Receive))
        {
            Serial.println("\t- Failed. getDataFromReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. getDataFromReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".7: Test getDataFromReceiveBuffer-Function from received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Message");
    while (!Module.fillTransmitBuffer(0, ID, Extended, RTR, DLC, Data_Transmit)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    bool Received_Data_passed = true;

    Serial.println("\t- Check which Receive-Buffer received Message");
    if ((Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        Serial.println("\t- Message received in Buffer 0");
        Serial.println("\t- Check if received Data matches sended Data");
        if (Module.getDataFromReceiveBuffer(0, DLC, Data_Receive)){
            for (size_t i = 0; i < DLC; i++) {
                if ((Data_Receive[i] != Data_Transmit[i])) {
                    Received_Data_passed = false;
                    Serial.println("\t\t- Failed. Data of the received Message doesn't match the sended Data.");
                    Errors++;
                    break;
                }
            }

            if (Received_Data_passed)
            {
                Serial.println("\t\t- passed");
            }
        } else {
            Serial.print("\t- Failed. getDataFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        }
    } else if ((Module.check4InterruptFlags() & 0x02) == 0x02)
    {
        Serial.println("\t- Message received in Buffer 1");
        Serial.println("\t- Check if received Data matches sended Data");
        if (Module.getDataFromReceiveBuffer(1, DLC, Data_Receive)){
            for (size_t i = 0; i < DLC; i++) {
                if ((Data_Receive[i] != Data_Transmit[i])) {
                    Received_Data_passed = false;
                    Serial.println("\t\t- Failed. Data of the received Message doesn't match the sended Data.");
                    Errors++;
                    break;
                }
            }

            if (Received_Data_passed)
            {
                Serial.println("\t\t- passed");
            }
        } else {
            Serial.print("\t- Failed. getDataFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        }
    } else {
        Serial.println("\t- Failed. No Message received");
        Errors++;
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testGetAllFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];

    uint32_t ID_Transmit = 435;
    uint32_t ID_Receive = 0;
    bool Extended_Transmit = true;
    bool Extended_Receive = false;
    bool RTR_Transmit = false;
    bool RTR_Receive = true;
    uint8_t DLC_Transmit = 4;
    uint8_t DLC_Receive = 0;
    uint8_t Data_Transmit[8] = {1, 2, 3, 4, 0, 0, 0, 0};
    uint8_t Data_Receive[8] = {0, 0, 0, 0};
    

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test getAllFromReceiveBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test getAllFromReceiveBuffer-Function when not initialized");

    if (Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive)){
        Serial.println("\t- Failed. getAllFromReceiveBuffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. getAllFromReceiveBuffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test getAllFromReceiveBuffer-Function when initialized");

    if (!Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive))
    {
        Serial.print("\t- Failed. getAllFromReceiveBuffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test getAllFromReceiveBuffer-Function from defined Receive-Buffers");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": getAllFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (!Module.getAllFromReceiveBuffer(i, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive))
        {
            Serial.print("\t- Failed. getAllFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test getAllFromReceiveBuffer-Function from undefined Receive-Buffers");

    for (size_t i = 2; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": getAllFromReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (Module.getAllFromReceiveBuffer(i, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive))
        {
            Serial.println("\t- Failed. getAllFromReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. getAllFromReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".5: Test getAllFromReceiveBuffer-Function from received Message");

    Serial.println("\t- Set OperationMode Loopback");
    while (!Module.setLoopbackMode()){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Fill Transmit-Buffer 0 with Message");
    while (!Module.fillTransmitBuffer(0, ID_Transmit, Extended_Transmit, RTR_Transmit, DLC_Transmit, Data_Transmit)){
        delay(100);
    }

    delay(500);

    Serial.println("\t- Send Message from Transmit-Buffer 0");
    while (!Module.sendMessage(0, 0)){
        delay(100);
    }

    delay(500);

    bool Received_Data_passed = true;
    bool Received_passed = false;

    Serial.println("\t- Check which Receive-Buffer received Message");
    if ((Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        Serial.println("\t- Message received in Buffer 0");
        if (Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive)){
            Received_passed = true;
        } else {
            Serial.print("\t- Failed. getAllFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        }
    } else if ((Module.check4InterruptFlags() & 0x02) == 0x02)
    {
        Serial.println("\t- Message received in Buffer 0");
        if (Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive)){
            Received_passed = true;
        } else {
            Serial.print("\t- Failed. getAllFromReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        }
    } else {
        Serial.println("\t- Failed. No Message received");
        Errors++;
    }

    if (Received_passed)
    {
        Serial.println("\t- Check if received ID matches sended ID");
        if (ID_Transmit == ID_Receive){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received ID doesn't match sended ID");
            Errors++;
        }
        
        delay(100);

        Serial.println("\t- Check if received Frame matches sended Frame");
        if (Extended_Transmit == Extended_Receive){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received Frame doesn't match sended Frame");
            Errors++;
        }

        delay(100);

        Serial.println("\t- Check if received RTR matches sended RTR");
        if (RTR_Transmit == RTR_Receive){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received RTR doesn't match sended RTR");
            Errors++;
        }

        delay(100);

        Serial.println("\t- Check if received DLC matches sended DLC");
        if (DLC_Transmit == DLC_Receive){
            Serial.println("\t\t- passed");
        } else {
            Serial.println("\t- Failed. Received DLC doesn't match sended DLC");
            Errors++;
        }

        delay(100);

        Serial.println("\t- Check if received Data matches sended Data");
        for (size_t i = 0; i < DLC_Transmit; i++) {
            if ((Data_Receive[i] != Data_Transmit[i])) {
                Received_Data_passed = false;
                Serial.println("\t\t- Failed. Data of the received Message doesn't match the sended Data.");
                Errors++;
                break;
            }
        }

        if (Received_Data_passed)
        {
            Serial.println("\t\t- passed");
        }
    }

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(500);
    while (!Module.setNormalMode()){
        delay(100);
    }
    delay(500);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testReleaseReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];    

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test releaseReceiveBuffer-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test releaseReceiveBuffer-Function when not initialized");

    if (Module.releaseReceiveBuffer(0)){
        Serial.println("\t- Failed. releaseReceiveBuffer successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. releaseReceiveBuffer failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test releaseReceiveBuffer-Function when initialized");

    if (!Module.releaseReceiveBuffer(0))
    {
        Serial.print("\t- Failed. releaseReceiveBuffer not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test releaseReceiveBuffer-Function with defined Receive-Buffers");

    for (size_t i = 0; i < 2; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": releaseReceiveBuffer with Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (!Module.releaseReceiveBuffer(i))
        {
            Serial.print("\t- Failed. releaseReceiveBuffer failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test releaseReceiveBuffer-Function from undefined Receive-Buffers");

    for (size_t i = 2; i < 5; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-1), DEC);
        Serial.print(": releaseReceiveBuffer from Buffer ");
        Serial.println(i, DEC);

        delay(500);

        if (Module.releaseReceiveBuffer(i))
        {
            Serial.println("\t- Failed. releaseReceiveBuffer successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. releaseReceiveBuffer failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), HEX);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testCheck4InterruptFlags(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];    

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test check4InterruptFlags-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test check4InterruptFlags-Function when not initialized");

    Module.check4InterruptFlags();

    if (Module.getLastMCPError() == EMPTY_VALUE_16_BIT){
        Serial.println("\t- Failed. check4InterruptFlags successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. check4InterruptFlags failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test check4InterruptFlags-Function when initialized");

    Module.check4InterruptFlags();

    if (Module.getLastMCPError() != EMPTY_VALUE_16_BIT)
    {
        Serial.print("\t- Failed. check4InterruptFlags not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test check4InterruptFlags-Function with each Flag");

    Serial.println("\t- Reset all Interrupt Flags");
    for (size_t i = 0; i < 8; i++)
    {
        Module.resetInterruptFlag(i);
    }
    
    delay(500);

    for (size_t i = 0; i < 8; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": check4InterruptFlags with Flag ");
        Serial.println(i, DEC);

        delay(500);

        Serial.print("\t- Set Interrupt Flag ");
        Serial.println(i, DEC);

        Module.setInterruptFlag(i);

        delay(500);

        uint8_t Flag = Module.check4InterruptFlags();

        if (((Flag & (0x01 << i)) != (0x01 << i)) || (Module.getLastMCPError() != EMPTY_VALUE_16_BIT))
        {
            Serial.print("\t- Failed. check4InterruptFlags failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            Serial.println("\t- passed");
        }

        delay(500);

        Module.resetInterruptFlag(i);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testResetInterruptFlag(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];    

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test resetInterruptFlag-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test resetInterruptFlag-Function when not initialized");

    if (Module.resetInterruptFlag(0)){
        Serial.println("\t- Failed. resetInterruptFlag successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. resetInterruptFlag failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test resetInterruptFlag-Function when initialized");

    if (!Module.resetInterruptFlag(0))
    {
        Serial.print("\t- Failed. resetInterruptFlag not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test resetInterruptFlag-Function for defined Flag");
    
    Serial.println("\t- Set all Interrupt Flags");
    for (size_t i = 0; i < 8; i++)
    {
        Module.setInterruptFlag(i);
    }
    
    delay(500);

    for (size_t i = 0; i < 8; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": resetInterruptFlag with Flag ");
        Serial.println(i, DEC);


        if (!Module.resetInterruptFlag(i))
        {
            Serial.print("\t- Failed. resetInterruptFlag failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            if ((Module.check4InterruptFlags() & (0x01 << i)) != (0x00 << i))
            {
                Serial.println("\t- Failed. Flag not resetted");
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test resetInterruptFlag-Function for undefined Flag");

    for (size_t i = 8; i < 12; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-7), DEC);
        Serial.print(": resetInterruptFlag with Flag ");
        Serial.println(i, DEC);

        if (Module.resetInterruptFlag(i))
        {
            Serial.println("\t- Failed. resetInterruptFlag successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. resetInterruptFlag failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), DEC);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}

uint8_t testSetInterruptFlag(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]){
    uint8_t CS_Pin = (uint8_t)defaultSettings[0];
    //uint8_t INT_Pin = (uint8_t)defaultSettings[1];
    uint32_t defaultSPIFrequency = (uint32_t)defaultSettings[2];
    uint32_t defaultClockFrequency = (uint32_t)defaultSettings[3];
    uint32_t defaultBaudrate = (uint32_t)defaultSettings[4];    

    uint8_t Errors = 0;

    delay(500);
    if (Module.getIsInitialized())
    {
        Module.deinit();
    }
    delay(500);

    Serial.print(Teststep, DEC);
    Serial.println(": Test setInterruptFlag-Function");

    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".1: Test setInterruptFlag-Function when not initialized");

    if (Module.setInterruptFlag(0)){
        Serial.println("\t- Failed. setInterruptFlag successfull");
        Errors++;    
    } else {
        if ((Module.getLastMCPError() & ERROR_MCP2515_NOT_INITIALIZED) != ERROR_MCP2515_NOT_INITIALIZED)
        {
            Serial.print("\t- Failed. setInterruptFlag failed with the wrong Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
        } else {
            Serial.println("\t- passed");
        }
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (!_initialize_Module(Module, CS_Pin, defaultSPIFrequency, defaultClockFrequency, defaultBaudrate)){
        delay(100);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".2: Test setInterruptFlag-Function when initialized");

    if (!Module.setInterruptFlag(0))
    {
        Serial.print("\t- Failed. setInterruptFlag not successfull in initialized Mode with Error 0x");
        Serial.println(Module.getLastMCPError(), HEX);
        Errors++;
    } else {
        Serial.println("\t- passed");
    }

    delay(1000);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".3: Test setInterruptFlag-Function for defined Flag");
    
    Serial.println("\t- Reset all Interrupt Flags");
    for (size_t i = 0; i < 8; i++)
    {
        Module.resetInterruptFlag(i);
    }
    
    delay(500);

    for (size_t i = 0; i < 8; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".3.");
        Serial.print((i+1), DEC);
        Serial.print(": setInterruptFlag with Flag ");
        Serial.println(i, DEC);


        if (!Module.setInterruptFlag(i))
        {
            Serial.print("\t- Failed. setInterruptFlag failed with Error 0x");
            Serial.println(Module.getLastMCPError(), HEX);
            Errors++;
            
        } else {
            if ((Module.check4InterruptFlags() & (0x01 << i)) != (0x01 << i))
            {
                Serial.println("\t- Failed. Flag not setted");
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.print(Teststep, DEC);
    Serial.println(".4: Test setInterruptFlag-Function for undefined Flag");

    for (size_t i = 8; i < 12; i++)
    {
        Serial.print(Teststep, DEC);
        Serial.print(".4.");
        Serial.print((i-7), DEC);
        Serial.print(": setInterruptFlag with Flag ");
        Serial.println(i, DEC);

        if (Module.setInterruptFlag(i))
        {
            Serial.println("\t- Failed. setInterruptFlag successfull");
            Errors++;
            
        } else {
            if ((Module.getLastMCPError() & ERROR_MCP2515_VALUE_OUTA_RANGE) != ERROR_MCP2515_VALUE_OUTA_RANGE)
            {
                Serial.print("\t- Failed. setInterruptFlag failed with wrong Error 0x");
                Serial.println(Module.getLastMCPError(), DEC);
                Errors++;
            } else {
                Serial.println("\t- passed");
            }
        }

        delay(500);
    }
    
    delay(1000);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delay(1000);
    Serial.print(Teststep, DEC);
    Serial.print(": Tests executed with ");
    Serial.print(Errors, DEC);
    Serial.println(" error.");
    Serial.println();
    delay(1000);

    return Errors;
}