# TestMCP2515-Library

Library which provides tests for the public Methods of the MCP2515-Library.

## Setup

Use the [main.cpp](src/main.cpp)-File to setup your test-requirements.

In the defaultSettings you can define your "normal" Modul-Configuration like:

```c++
uint32_t defaultSettings[5] = {
    53,                 // CS-Pin (Mega = 53; Micro = 17)
    4,                  // INT-Pin
    (uint32_t)4E6,      // SPI-Frequency
    (uint32_t)8E6,      // Clock-Frequency
    (uint32_t)500E3,    // Baudrate
};
```

It is important that all settings are matching your hardware-configuration, because during the test the module will be initialized and also deinitialized several times.

It is also important to connect your Int-Pin so Interrupts can also been tested.

Of course the Serial-Interface should also be correct configured.

## Testcases

### General

In each Testcase several Teststeps will be executed.

Including fault attacks.

If during a fault attack an unexpected error occurse it will be printed.

Otherwise if during a fault attack the expected error occurs the teststep passed.

### Testcase-Selection

Because the Testcases need very much memory you have to choose your test cases separately.

For this purpose you can remove the Comment-Characters like:

```c++
Serial.println("=============================================================================");
Serial.println("=============================== Start Testing ===============================");
Serial.println("=============================================================================");
Serial.println();
//TestCaseNumber++;
//Errors = Errors + testInit(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetSpiPins(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetSpiMode(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetDataOrder(TestCaseNumber, MCP2515Module, defaultSettings);
TestCaseNumber++;
Errors = Errors + testSetSpiFrequency(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetClockFrequency(TestCaseNumber, MCP2515Module, defaultSettings);
TestCaseNumber++;
Errors = Errors + testSetConfigurationMode(TestCaseNumber, MCP2515Module, defaultSettings);
TestCaseNumber++;
Errors = Errors + testSetNormalMode(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetListenOnlyMode(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetLoopbackMode(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetSleepMode(TestCaseNumber, MCP2515Module, defaultSettings);
//TestCaseNumber++;
//Errors = Errors + testSetReCheckEnabler(TestCaseNumber, MCP2515Module, defaultSettings);
.
.
.
```

## Result

During the Testcase-Execution the Result (passed or failed) of each Teststep will be printed:

```text
.
.
.
1.1: Test setSleepMode-Function when not initialized
        - passed
1.2: Test setSleepMode-Function when initialized
        - Failed. Setting SleepMode not successfull in initialized Mode with Error 0x20
.
.
.
```

When a Testcase completed it will print the count of Errors that occurs during the Testcase-Execution.

```text
.
.
.
1: Tests executed with 1 error.
.
.
.
```

Also when the whole Execution completed the count of Errors will be printed.
```text
.
.
.
Total count of Errors = 1
```