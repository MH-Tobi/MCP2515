#include <unity.h>
#include "../common_test_settings.h"

// additional Test Settings
uint8_t Data[8] = {1};

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

void test_fillTransmitBuffer_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, true, false, 4, Data), "Filling the TransmitBuffer successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for filling the TransmitBuffer when not initialized.");
    }
}

void test_fillTransmitBuffer_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, true, false, 4, Data), "Filling the TransmitBuffer not successfull.");
}

void test_fillTransmitBuffer_for_buffer_0()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, true, false, 4, Data), "Filling the TransmitBuffer 0 not successfull.");
}

void test_fillTransmitBuffer_for_buffer_1()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(1, 1, true, false, 4, Data), "Filling the TransmitBuffer 1 not successfull.");
}

void test_fillTransmitBuffer_for_buffer_2()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(2, 1, true, false, 4, Data), "Filling the TransmitBuffer 2 not successfull.");
}

void test_fillTransmitBuffer_no_buffer_free()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag((i + 2)), "Resetting \"Transmit-Buffer free\"-Flags not successfull.");
    }

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.fillTransmitBuffer(i, 1, true, false, 4, Data), "Filling the TransmitBuffer successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_TRANSMITBUFFER_NOT_FREE))!=static_cast<uint16_t>(MCP2515Error::MAIN_TRANSMITBUFFER_NOT_FREE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_TRANSMITBUFFER_NOT_FREE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for filling the TransmitBuffer that is not free.");
        }
    }
}

void test_fillTransmitBuffer_for_undefined_buffer_3()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.fillTransmitBuffer(3, 1, true, false, 4, Data), "Filling the TransmitBuffer 3 successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for filling the TransmitBuffer with undefined Buffernumber.");
    }
}

void test_fillTransmitBuffer_with_defined_standard_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(i, defined_Standard_IDs[i], false, false, 4, Data), "Filling the TransmitBuffer with defined Standard-ID not successfull.");
    }
}

void test_fillTransmitBuffer_with_undefined_standard_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.fillTransmitBuffer(i, undefined_Standard_IDs[i], false, false, 4, Data), "Filling the TransmitBuffer with undefined Standard-ID successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for filling the TransmitBuffer with undefined Standard-ID.");
        }
    }
}

void test_fillTransmitBuffer_with_defined_extended_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(i, defined_Extended_IDs[i], true, false, 4, Data), "Filling the TransmitBuffer with defined Extended-ID not successfull.");
    }
}

void test_fillTransmitBuffer_with_undefined_extended_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.fillTransmitBuffer(i, undefined_Extended_IDs[i], true, false, 4, Data), "Filling the TransmitBuffer with undefined Extended-ID successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for filling the TransmitBuffer with undefined Extended-ID.");
        }
    }
}

void test_fillTransmitBuffer_with_rtr_bit_set()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, false, true, 4, Data), "Filling the TransmitBuffer not successfull.");
}

void test_fillTransmitBuffer_with_defined_dlc()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<8; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, true, false, i, Data), "Filling the TransmitBuffer with defined DLC not successfull.");
        delay(500);
        MCP2515Module.setInterruptFlag(2);
    }
}

void test_fillTransmitBuffer_with_undefined_dlc()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=9; i<11; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, true, false, i, Data), "Filling the TransmitBuffer with undefined DLC successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for filling the TransmitBuffer with undefined DLC.");
        }
        delay(500);
        MCP2515Module.setInterruptFlag(2);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
    initializeModule(MCP2515Module, defaultSettings[0], defaultSettings[2], defaultSettings[3], defaultSettings[4]);
    delay(500);

    for (size_t i = 0; i < 3; i++)
    {
        MCP2515Module.setInterruptFlag((i + 2));
    }
}

void tearDown() {
    // clean stuff up here
    for (size_t i=0; i<3; i++)
    {
        MCP2515Module.resetInterruptFlag((i + 2));
    }
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_fillTransmitBuffer_with_defined_standard_id);
    RUN_TEST(test_fillTransmitBuffer_with_undefined_standard_id);
    RUN_TEST(test_fillTransmitBuffer_with_defined_extended_id);
    RUN_TEST(test_fillTransmitBuffer_with_undefined_extended_id);
    RUN_TEST(test_fillTransmitBuffer_with_rtr_bit_set);
    RUN_TEST(test_fillTransmitBuffer_with_defined_dlc);
    RUN_TEST(test_fillTransmitBuffer_with_undefined_dlc);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
