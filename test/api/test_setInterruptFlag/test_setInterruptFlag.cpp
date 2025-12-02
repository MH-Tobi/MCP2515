#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setInterruptFlag_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setInterruptFlag(0), "Setting Interrupt-Flag successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting Interrupt-Flag when not initialized.");
    }
}

void test_setInterruptFlag_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(0), "Setting Interrupt-Flag not successfull.");
}

void test_setInterruptFlag_for_defined_flags()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<8; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(i), "Setting Interrupt-Flag not successfull.");
    }
}

void test_setInterruptFlag_for_undefined_flags()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=8; i<11; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setInterruptFlag(i), "Setting Interrupt-Flag successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for setting undefined Interrupt-Flag.");
        }
    }
}

void test_setInterruptFlag_for_each_flag()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<8; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(i), "Resetting Interrupt-Flag not successfull.");
    }
    delay(100);

    for (size_t i=0; i<8; i++)
    {
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, MCP2515Module.check4InterruptFlags(), "Check for Interrupt Flag not successfull.");
        delay(100);

        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(i), "Setting Interrupt-Flag not successfull.");
        delay(100);

        TEST_ASSERT_EQUAL_HEX8_MESSAGE((0x01 << i), MCP2515Module.check4InterruptFlags(), "Check for Interrupt Flag not successfull.");
        delay(100);

        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(i), "Resetting Interrupt-Flag not successfull.");
        delay(100);

        TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, MCP2515Module.check4InterruptFlags(), "Check for Interrupt Flag not successfull.");
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
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_setInterruptFlag_when_not_initialized);
    RUN_TEST(test_setInterruptFlag_when_initialized);
    RUN_TEST(test_setInterruptFlag_for_defined_flags);
    RUN_TEST(test_setInterruptFlag_for_undefined_flags);
    RUN_TEST(test_setInterruptFlag_for_each_flag);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
