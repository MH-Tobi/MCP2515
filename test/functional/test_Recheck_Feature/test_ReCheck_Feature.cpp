#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_ReCheck_Feature_with_true_value()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setReCheckEnabler(true), "Setting ReCheck-Enabler not successfull.");
    pinMode(defaultSettings[0], INPUT_PULLUP);
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setConfigurationMode(), "Setting Configuration-Mode with wrong CS-Pin Configuration successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET))!=static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting Configuration-Mode with wrong CS-Pin Configuration.");
    }
}

void test_ReCheck_Feature_with_false_value()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setReCheckEnabler(false), "Setting ReCheck-Enabler not successfull.");
    pinMode(defaultSettings[0], INPUT_PULLUP);
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setConfigurationMode(), "Setting Configuration-Mode with wrong CS-Pin Configuration not successfull.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
    initializeModule(MCP2515Module, defaultSettings[0], defaultSettings[2], defaultSettings[3], defaultSettings[4]);
}

void tearDown() {
    // clean stuff up here
    delay(500);
    pinMode(defaultSettings[0], OUTPUT);
    delay(500);
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_ReCheck_Feature_with_true_value);
    RUN_TEST(test_ReCheck_Feature_with_false_value);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
