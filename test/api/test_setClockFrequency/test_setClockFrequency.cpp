#include <unity.h>
#include "../common_test_settings.h"

// additional Test Settings
const uint32_t possibleClockFrequency[4] = {
    (uint32_t)8E6,
    (uint32_t)16E6,
    (uint32_t)25E6,
    (uint32_t)40E6,
};

uint32_t unallowedClockFrequencys[4] = {
    (uint32_t)5e6,
    (uint32_t)11e6,
    (uint32_t)20e6,
    (uint32_t)45e6,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setClockFrequency_when_not_initialized()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setClockFrequency(defaultSettings[3]), "Setting Clock-Frequency not successfull.");
}

void test_setClockFrequency_when_initialized()
{
    MCP2515Module.setSpiFrequency(defaultSettings[2]);
    delay(100);
    MCP2515Module.setClockFrequency(defaultSettings[3]);
    delay(100);
    MCP2515Module.setSpiPins(defaultSettings[0]);
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.init(defaultSettings[4]), "Init not successful.");
    delay(100);
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setClockFrequency(defaultSettings[3]), "Setting Clock-Frequency successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting Clock-Frequency when initialized.");
    }
}

void test_setClockFrequency_with_allowed_frequency()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    for (size_t i = 0; i < 4; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setClockFrequency(possibleClockFrequency[i]), "Setting allowed Clock-Frequency not successful.");
        delay(500);
    }
}

void test_setClockFrequency_with_unallowed_frequency()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    for (size_t i = 0; i < 4; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setClockFrequency(unallowedClockFrequencys[i]), "Setting unallowed Clock-Frequency successful.");

        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_CLOCKFREQUENCY_NOT_VALID))!=static_cast<uint16_t>(MCP2515Error::MAIN_CLOCKFREQUENCY_NOT_VALID))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_CLOCKFREQUENCY_NOT_VALID),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for setting unallowed Clock-Frequency.");
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

    RUN_TEST(test_setClockFrequency_when_not_initialized);
    RUN_TEST(test_setClockFrequency_when_initialized);
    RUN_TEST(test_setClockFrequency_with_allowed_frequency);
    RUN_TEST(test_setClockFrequency_with_unallowed_frequency);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
