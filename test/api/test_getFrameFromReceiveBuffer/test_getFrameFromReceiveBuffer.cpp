#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_getFrameFromReceiveBuffer_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    MCP2515Module.getFrameFromReceiveBuffer(0);

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for get Frame from ReceiveBuffer when not initialized.");
    }
}

void test_getFrameFromReceiveBuffer_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    MCP2515Module.getFrameFromReceiveBuffer(0);

    TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                    MCP2515Module.getLastMCPError(),
                                    "Error-Code occured for get Frame from ReceiveBuffer when initialized.");
}

void test_getFrameFromReceiveBuffer_from_defined_buffer()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        MCP2515Module.getFrameFromReceiveBuffer(i);
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                        MCP2515Module.getLastMCPError(),
                                        "Error-Code occured for get Frame from defined ReceiveBuffer.");
    }
}

void test_getFrameFromReceiveBuffer_from_undefined_buffer()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=2; i<4; i++)
    {
        MCP2515Module.getFrameFromReceiveBuffer(i);
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for get Frame from undefined ReceivedBuffer.");
        }
    }
}

void test_getFrameFromReceiveBuffer_from_received_standard_message()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Loopback Mode is not set.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, false, false, 4, 0), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    if ((MCP2515Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(0,
                                        MCP2515Module.getFrameFromReceiveBuffer(0),
                                        "Get wrong Standard-Frame from ReceiveBuffer 0.");
    }else{
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(0,
                                        MCP2515Module.getFrameFromReceiveBuffer(1),
                                        "Get wrong Standard-Frame from ReceiveBuffer 1.");
    }
}

void test_getFrameFromReceiveBuffer_from_received_extended_message()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Loopback Mode is not set.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, true, false, 4, 0), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    if ((MCP2515Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(1,
                                        MCP2515Module.getFrameFromReceiveBuffer(0),
                                        "Get wrong Extended-Frame from ReceiveBuffer 0.");
    }else{
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(1,
                                        MCP2515Module.getFrameFromReceiveBuffer(1),
                                        "Get wrong Extended-Frame from ReceiveBuffer 1.");
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

    RUN_TEST(test_getFrameFromReceiveBuffer_when_not_initialized);
    RUN_TEST(test_getFrameFromReceiveBuffer_when_initialized);
    RUN_TEST(test_getFrameFromReceiveBuffer_from_defined_buffer);
    RUN_TEST(test_getFrameFromReceiveBuffer_from_undefined_buffer);
    RUN_TEST(test_getFrameFromReceiveBuffer_from_received_standard_message);
    RUN_TEST(test_getFrameFromReceiveBuffer_from_received_extended_message);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
