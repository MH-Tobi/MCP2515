#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setNormalMode_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setNormalMode(), "Setting Normal-Mode successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting Normal-Mode when not initialized.");
    }
}

void test_setNormalMode_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setNormalMode(), "Setting Normal-Mode not successfull.");
}

void test_setNormalMode_with_wrong_CS_Pin_configuration()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setConfigurationMode(), "Setting Configuration-Mode not successfull.");

    pinMode(defaultSettings[0], INPUT_PULLUP);
    delay(500);

    if (!MCP2515Module.setNormalMode())
    {
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET))!=static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::SECONDARY_OPERATION_MODE_NOT_SET),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for setting Normal-Mode when initialized with wrong CS-Pin Configuration.");
        }
    }else{
        // In this test case, it cannot be guaranteed that the "setNormalMode" function will detect
        // that the normal mode could not be set, as only zeros are expected during the normal mode recheck.
        // Since the test uses an incorrectly set CS pin, it is possible that only zeros will be "received".
        // The test was therefore designed so that the error code is only checked in the event of a successful failure of the function.
        TEST_ASSERT(true);
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

    RUN_TEST(test_setNormalMode_when_not_initialized);
    RUN_TEST(test_setNormalMode_when_initialized);
    RUN_TEST(test_setNormalMode_with_wrong_CS_Pin_configuration);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
