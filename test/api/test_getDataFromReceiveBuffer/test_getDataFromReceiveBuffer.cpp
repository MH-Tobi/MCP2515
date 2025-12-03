#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_getDataFromReceiveBuffer_when_not_initialized()
{
    uint8_t Data_Receive[8];
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(0, 4, Data_Receive), "Get Data from ReceiveBuffer successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for get Data from ReceiveBuffer when not initialized.");
    }
}

void test_getDataFromReceiveBuffer_when_initialized()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(0, 4, Data_Receive), "Get Data from ReceiveBuffer not successfull.");
}

void test_getDataFromReceiveBuffer_from_defined_buffer()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(i, 4, Data_Receive), "Get Data from defined ReceiveBuffer not successfull.");
    }
}

void test_getDataFromReceiveBuffer_from_undefined_buffer()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=2; i<4; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(i, 4, Data_Receive), "Get Data from undefined ReceiveBuffer successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for get Data from undefined ReceivedBuffer.");
        }
    }
}

void test_getDataFromReceiveBuffer_with_defined_dlc()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<9; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(0, i, Data_Receive), "Get Data with defined DLC not successfull.");
    }
}

void test_getDataFromReceiveBuffer_with_undefined_dlc()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=9; i<11; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(0, i, Data_Receive), "Get Data with undefined DLC successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for get Data with undefined DLC.");
        }
    }
}

void test_getDataFromReceiveBuffer_from_received_message()
{
    uint8_t Data_Receive[8];
    uint8_t Data_Transmit[8] = {1, 2, 3, 4, 0, 0, 0, 0};
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Loopback Mode is not set.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, false, false, 4, Data_Transmit), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    if ((MCP2515Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(0, 4, Data_Receive), "Get Data from ReceiveBuffer 0 not successfull.");
    }else{
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getDataFromReceiveBuffer(1, 4, Data_Receive), "Get Data from ReceiveBuffer 1 not successfull.");
    }

    for (size_t i = 0; i < 4; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE((Data_Receive[i] == Data_Transmit[i]), "Data of the received Message doesn't match the sended Data.");
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

    RUN_TEST(test_getDataFromReceiveBuffer_when_not_initialized);
    RUN_TEST(test_getDataFromReceiveBuffer_when_initialized);
    RUN_TEST(test_getDataFromReceiveBuffer_from_defined_buffer);
    RUN_TEST(test_getDataFromReceiveBuffer_from_undefined_buffer);
    RUN_TEST(test_getDataFromReceiveBuffer_with_defined_dlc);
    RUN_TEST(test_getDataFromReceiveBuffer_with_undefined_dlc);
    RUN_TEST(test_getDataFromReceiveBuffer_from_received_message);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
