#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setSpiPins_when_not_initialized()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiPins(defaultSettings[0]), "Setting SPI-Pins not successfull.");
}

void test_setSpiPins_when_initialized()
{
    MCP2515Module.setSpiFrequency(defaultSettings[2]);
    delay(100);
    MCP2515Module.setClockFrequency(defaultSettings[3]);
    delay(100);
    MCP2515Module.setSpiPins(defaultSettings[0]);
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.init(defaultSettings[4]), "Init not successful.");
    delay(100);
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setSpiPins(defaultSettings[0]), "Setting SPI-Pins successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting SPI-Pins when initialized.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_setSpiPins_when_not_initialized);
    RUN_TEST(test_setSpiPins_when_initialized);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
