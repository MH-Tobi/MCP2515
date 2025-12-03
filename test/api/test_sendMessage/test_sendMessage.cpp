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

void test_sendMessage_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Send Message successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for sending Message when not initialized.");
    }
}

void test_sendMessage_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Send Message not successfull.");
}

void test_sendMessage_from_buffer_0()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Send Message from TransmitBuffer 0 not successfull.");
}

void test_sendMessage_from_buffer_1()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(1, 0), "Send Message from TransmitBuffer 1 not successfull.");
}

void test_sendMessage_from_buffer_2()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(2, 0), "Send Message from TransmitBuffer 2 not successfull.");
}

void test_sendMessage_from_undefined_buffer_3()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.sendMessage(3, 0), "Send Message from TransmitBuffer 3 successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for sending Message from TransmitBuffer with undefined Buffernumber.");
    }
}

void test_sendMessage_with_defined_priority()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(i, i+1), "Send Message with defined priority not successfull.");
    }
}

void test_sendMessage_with_undefined_priority()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.sendMessage(i, i+4), "Send Message with undefined priority successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for sending Message with undefined priority.");
        }
    }
}

void test_sendMessage_without_can_bus()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setNormalMode(), "Normal Mode is not set.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.sendMessage(i, 0), "Send Message without CAN-Bus successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::SECONDARY_MESSAGE_SENDING_ABORTED))!=static_cast<uint16_t>(MCP2515Error::SECONDARY_MESSAGE_SENDING_ABORTED))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::SECONDARY_MESSAGE_SENDING_ABORTED),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for sending Message without CAN-Bus.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
    initializeModule(MCP2515Module, defaultSettings[0], defaultSettings[2], defaultSettings[3], defaultSettings[4]);
    delay(500);

    MCP2515Module.setLoopbackMode();

    for (size_t i = 0; i < 3; i++)
    {
        MCP2515Module.setInterruptFlag((i + 2));

        MCP2515Module.fillTransmitBuffer(i, defined_Standard_IDs[i], false, false, 4, Data);
    }
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_sendMessage_when_not_initialized);
    RUN_TEST(test_sendMessage_when_initialized);
    RUN_TEST(test_sendMessage_from_buffer_0);
    RUN_TEST(test_sendMessage_from_buffer_1);
    RUN_TEST(test_sendMessage_from_buffer_2);
    RUN_TEST(test_sendMessage_from_undefined_buffer_3);
    RUN_TEST(test_sendMessage_with_defined_priority);
    RUN_TEST(test_sendMessage_with_undefined_priority);
    RUN_TEST(test_sendMessage_without_can_bus);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
