#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_check4FreeTransmitBuffer_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, MCP2515Module.check4FreeTransmitBuffer(), "Check for a free TransmitBuffer successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for check for a free TransmitBuffer when not initialized.");
    }
}

void test_check4FreeTransmitBuffer_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i = 0; i < 3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag((i + 2)), "Set \"Transmit-Buffer free\"-Flags not successfull.");
    }

    TEST_ASSERT_HEX8_WITHIN_MESSAGE(0x02, 0x00, MCP2515Module.check4FreeTransmitBuffer(), "Check for a free TransmitBuffer not successfull.");
}

void test_check4FreeTransmitBuffer_for_buffer_0()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag((i+2)), "Reset \"Transmit-Buffer free\"-Flags not successfull.");
    }

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(2), "Set \"Transmit-Buffer free\"-Flags 0 not successfull.");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, MCP2515Module.check4FreeTransmitBuffer(), "Transmit-Buffer 0 is not free.");
}

void test_check4FreeTransmitBuffer_for_buffer_1()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag((i+2)), "Reset \"Transmit-Buffer free\"-Flags not successfull.");
    }

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(3), "Set \"Transmit-Buffer free\"-Flags 1 not successfull.");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, MCP2515Module.check4FreeTransmitBuffer(), "Transmit-Buffer 1 is not free.");
}

void test_check4FreeTransmitBuffer_for_buffer_2()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag((i+2)), "Reset \"Transmit-Buffer free\"-Flags not successfull.");
    }

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(4), "Set \"Transmit-Buffer free\"-Flags 2 not successfull.");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x02, MCP2515Module.check4FreeTransmitBuffer(), "Transmit-Buffer 2 is not free.");
}

void test_check4FreeTransmitBuffer_no_buffer_free()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag((i + 2)), "Resetting \"Transmit-Buffer free\"-Flags not successfull.");
    }

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, MCP2515Module.check4FreeTransmitBuffer(), "Transmit-Buffer is free.");
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

    RUN_TEST(test_check4FreeTransmitBuffer_when_not_initialized);
    RUN_TEST(test_check4FreeTransmitBuffer_when_initialized);
    RUN_TEST(test_check4FreeTransmitBuffer_for_buffer_0);
    RUN_TEST(test_check4FreeTransmitBuffer_for_buffer_1);
    RUN_TEST(test_check4FreeTransmitBuffer_for_buffer_2);
    RUN_TEST(test_check4FreeTransmitBuffer_no_buffer_free);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
