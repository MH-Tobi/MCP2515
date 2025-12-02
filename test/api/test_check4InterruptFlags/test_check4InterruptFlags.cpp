#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_check4InterruptFlags_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, MCP2515Module.check4InterruptFlags(), "Check for Interrupt Flags successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for check for Interrupt Flags when not initialized.");
    }
}

void test_check4InterruptFlags_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, MCP2515Module.check4InterruptFlags(), "Check for Interrupt Flags not successfull.");

    if (MCP2515Module.getLastMCPError()!=static_cast<uint16_t>(MCP2515Error::NO_ERROR))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                        MCP2515Module.getLastMCPError(),
                                        "Error occured for check for Interrupt Flags when not initialized.");
    }
}

void test_check4InterruptFlags_with_each_flag()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i = 0; i < 8; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(i), "Resetting Interrupt-Flag not successfull.");
    }
    delay(100);

    for (size_t i = 0; i < 8; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(i), "Setting Interrupt-Flag not successfull.");
        delay(100);

        TEST_ASSERT_EQUAL_HEX8_MESSAGE((0x01 << i), MCP2515Module.check4InterruptFlags(), "Check for Interrupt Flag not successfull.");

        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(i), "Resetting Interrupt-Flag not successfull.");
        delay(100);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
    initializeModule(MCP2515Module, defaultSettings[0], defaultSettings[2], defaultSettings[3], defaultSettings[4]);
    delay(500);

    for (size_t i=0; i<8; i++)
    {
        MCP2515Module.setInterruptFlag(i);
    }
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_check4InterruptFlags_when_not_initialized);
    RUN_TEST(test_check4InterruptFlags_when_initialized);
    RUN_TEST(test_check4InterruptFlags_with_each_flag);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
