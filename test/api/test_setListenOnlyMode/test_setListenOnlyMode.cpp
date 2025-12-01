#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setListenOnlyMode_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setListenOnlyMode(), "Setting ListenOnly-Mode successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting ListenOnly-Mode when not initialized.");
    }
}

void test_setListenOnlyMode_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setListenOnlyMode(), "Setting ListenOnly-Mode not successfull.");
}

void test_setListenOnlyMode_with_wrong_CS_Pin_configuration()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    pinMode(defaultSettings[0], INPUT_PULLUP);
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setListenOnlyMode(), "Setting ListenOnly-Mode with wrong CS-Pin Configuration successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET))!=static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting ListenOnly-Mode when initialized with wrong CS-Pin Configuration.");
    }

    delay(500);
    pinMode(defaultSettings[0], OUTPUT);
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
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_setListenOnlyMode_when_not_initialized);
    RUN_TEST(test_setListenOnlyMode_when_initialized);
    RUN_TEST(test_setListenOnlyMode_with_wrong_CS_Pin_configuration);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
