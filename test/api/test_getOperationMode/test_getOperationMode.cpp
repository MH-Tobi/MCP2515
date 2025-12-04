#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_getOperationMode_when_not_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setConfigurationMode(), "Setting Configuration-Mode not successfull.");
    delay(100);

    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_MESSAGE(MCP2515Module.getOperationMode() == MCP2515OperationMode::NORMAL, "Getting Operation-Mode successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for getting Operation-Mode when not initialized.");
    }
}

void test_getOperationMode_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setConfigurationMode(), "Setting Configuration-Mode not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_MESSAGE(MCP2515Module.getOperationMode() == MCP2515OperationMode::CONFIGURATION, "Getting Operation-Mode not successfull.");
}

void test_getOperationMode_when_in_normal_mode()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_MESSAGE(MCP2515Module.getOperationMode() == MCP2515OperationMode::NORMAL, "Getting Operation-Mode not successfull.");
}

void test_getOperationMode_when_in_configuration_mode()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setConfigurationMode(), "Setting Configuration-Mode not successfull.");
    delay(100);

    TEST_ASSERT_MESSAGE(MCP2515Module.getOperationMode() == MCP2515OperationMode::CONFIGURATION, "Getting Operation-Mode not successfull.");
}

void test_getOperationMode_when_in_sleep_mode()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSleepMode(), "Setting Sleep-Mode not successfull.");
    delay(100);

    TEST_ASSERT_MESSAGE(MCP2515Module.getOperationMode() == MCP2515OperationMode::SLEEP, "Getting Operation-Mode not successfull.");
}

void test_getOperationMode_when_in_listen_only_mode()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setListenOnlyMode(), "Setting Listen-Only-Mode not successfull.");
    delay(100);

    TEST_ASSERT_MESSAGE(MCP2515Module.getOperationMode() == MCP2515OperationMode::LISTEN, "Getting Operation-Mode not successfull.");
}

void test_getOperationMode_when_in_loopback_mode()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Setting Loopback-Mode not successfull.");
    delay(100);

    TEST_ASSERT_MESSAGE(MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK, "Getting Operation-Mode not successfull.");
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

    RUN_TEST(test_getOperationMode_when_not_initialized);
    RUN_TEST(test_getOperationMode_when_initialized);
    RUN_TEST(test_getOperationMode_when_in_normal_mode);
    RUN_TEST(test_getOperationMode_when_in_configuration_mode);
    RUN_TEST(test_getOperationMode_when_in_sleep_mode);
    RUN_TEST(test_getOperationMode_when_in_listen_only_mode);
    RUN_TEST(test_getOperationMode_when_in_loopback_mode);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
