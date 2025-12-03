#include <unity.h>
#include "../common_test_settings.h"

// additional Test Settings
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setSpiFrequency_when_not_initialized()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiFrequency(defaultSettings[2]), "Setting SPI-Frequency not successfull.");
}

void test_setSpiFrequency_when_initialized()
{
    MCP2515Module.setSpiFrequency(defaultSettings[2]);
    delay(100);
    MCP2515Module.setClockFrequency(defaultSettings[3]);
    delay(100);
    MCP2515Module.setSpiPins(defaultSettings[0]);
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.init(defaultSettings[4]), "Init not successful.");
    delay(100);
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setSpiFrequency(defaultSettings[2]), "Setting SPI-Frequency successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting SPI-Frequency when initialized.");
    }
}

void test_setSpiFrequency_with_allowed_frequency()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    for (size_t i = 0; i < 4; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiFrequency(allowedSPIFrequencys[i]), "Setting allowed SPI-Frequency not successful.");
        delay(500);
    }
}

void test_setSpiFrequency_with_unallowed_frequency()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    for (size_t i = 0; i < 2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setSpiFrequency(unallowedSPIFrequencys[i]), "Setting unallowed SPI-Frequency successful.");

        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_SPI_FREQUENCY_NOT_ALLOWED))!=static_cast<uint16_t>(MCP2515Error::MAIN_SPI_FREQUENCY_NOT_ALLOWED))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_SPI_FREQUENCY_NOT_ALLOWED),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for setting unallowed SPI-Frequency.");
        }
        delay(500);
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

    RUN_TEST(test_setSpiFrequency_when_not_initialized);
    RUN_TEST(test_setSpiFrequency_when_initialized);
    RUN_TEST(test_setSpiFrequency_with_allowed_frequency);
    RUN_TEST(test_setSpiFrequency_with_unallowed_frequency);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
