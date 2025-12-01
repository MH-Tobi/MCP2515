#include <unity.h>
#include "../common_test_settings.h"

// additional Test Settings
uint32_t defined_Standard_IDs[3] = {
    (uint32_t)0x001,
    (uint32_t)0x111,
    (uint32_t)0x7FF,
};

uint32_t undefined_Standard_IDs[2] = {
    (uint32_t)0x800,
    (uint32_t)0x801,
};

uint32_t defined_Extended_IDs[3] = {
    (uint32_t)0x00000800,
    (uint32_t)0x11111111,
    (uint32_t)0x1FFFFFFF,
};

uint32_t undefined_Extended_IDs[2] = {
    (uint32_t)0x20000000,
    (uint32_t)0x20000001,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_check4Rtr_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Rtr(defined_Standard_IDs[0], false), "Check for RTR-Message successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for sending Message when not initialized.");
    }
}

void test_check4Rtr_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    if (!MCP2515Module.check4Rtr(defined_Standard_IDs[0], false))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                        MCP2515Module.getLastMCPError(),
                                        "Error-Code occured for check for RTR-Message when initialized.");
    }
}

void test_check4Rtr_with_defined_standard_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        if (!MCP2515Module.check4Rtr(defined_Standard_IDs[i], false))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                            MCP2515Module.getLastMCPError(),
                                            "Error-Code occured for check for RTR-Message with defined Standard-ID.");
        }
    }
}

void test_check4Rtr_with_undefined_standard_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Rtr(undefined_Standard_IDs[i], false), "Check for RTR-Message with undefined Standard-ID successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for check for RTR-Message with undefined Standard-ID.");
        }
    }
}

void test_check4Rtr_with_defined_extended_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        if (!MCP2515Module.check4Rtr(defined_Extended_IDs[i], true))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                            MCP2515Module.getLastMCPError(),
                                            "Error-Code occured for check for RTR-Message with defined Extended-ID.");
        }
    }
}

void test_check4Rtr_with_undefined_extended_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Rtr(undefined_Extended_IDs[i], true), "Check for RTR-Message with undefined Extended-ID successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for check for RTR-Message with undefined Extended-ID.");
        }
    }
}

void test_check4Rtr_for_received_message()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Loopback Mode is not set.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, defined_Standard_IDs[0], false, true, 0, 0), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Rtr(defined_Standard_IDs[0], false), "Check for RTR-Message not successfull.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
    initializeModule(MCP2515Module, defaultSettings[0], defaultSettings[2], defaultSettings[3], defaultSettings[4]);
    delay(500);
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_check4Rtr_when_not_initialized);
    RUN_TEST(test_check4Rtr_when_initialized);
    RUN_TEST(test_check4Rtr_with_defined_standard_id);
    RUN_TEST(test_check4Rtr_with_undefined_standard_id);
    RUN_TEST(test_check4Rtr_with_defined_extended_id);
    RUN_TEST(test_check4Rtr_with_undefined_extended_id);
    RUN_TEST(test_check4Rtr_for_received_message);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
