#include <unity.h>
#include "../common_test_settings.h"

// additional Test Settings
uint32_t ID_Receive;
bool Extended_Receive;
bool RTR_Receive;
uint8_t DLC_Receive;
uint8_t Data_Receive[8];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_getAllFromReceiveBuffer_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive),
                             "Get all from ReceiveBuffer successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for get all from ReceiveBuffer when not initialized.");
    }
}

void test_getAllFromReceiveBuffer_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive),
                            "Get all from ReceiveBuffer not successfull.");
}

void test_getAllFromReceiveBuffer_from_defined_buffer()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getAllFromReceiveBuffer(i, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive),
                                "Get all from defined ReceiveBuffer not successfull.");
    }
}

void test_getAllFromReceiveBuffer_from_undefined_buffer()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=2; i<4; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getAllFromReceiveBuffer(i, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive),
                                 "Get all from undefined ReceiveBuffer successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for get all from undefined ReceivedBuffer.");
        }
    }
}

void test_getAllFromReceiveBuffer_from_received_message()
{
    uint8_t Data_Transmit[8] = {1, 2, 3, 4, 0, 0, 0, 0};
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Loopback Mode is not set.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, 1, true, false, 4, Data_Transmit), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    if ((MCP2515Module.check4InterruptFlags() & 0x01) == 0x01)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive),
                                "Get all from ReceiveBuffer 0 not successfull.");
    }else{
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getAllFromReceiveBuffer(0, ID_Receive, Extended_Receive, RTR_Receive, DLC_Receive, Data_Receive),
                                "Get all from ReceiveBuffer 0 not successfull.");
    }

    TEST_ASSERT_TRUE_MESSAGE((ID_Receive == 1), "ID of the received Message doesn't match the sended ID.");
    TEST_ASSERT_TRUE_MESSAGE((Extended_Receive == true), "Frame of the received Message doesn't match the sended Frame.");
    TEST_ASSERT_TRUE_MESSAGE((RTR_Receive == false), "RTR of the received Message doesn't match the sended RTR.");
    TEST_ASSERT_TRUE_MESSAGE((DLC_Receive == 4), "DLC of the received Message doesn't match the sended DLC.");

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

    RUN_TEST(test_getAllFromReceiveBuffer_when_not_initialized);
    RUN_TEST(test_getAllFromReceiveBuffer_when_initialized);
    RUN_TEST(test_getAllFromReceiveBuffer_from_defined_buffer);
    RUN_TEST(test_getAllFromReceiveBuffer_from_undefined_buffer);
    RUN_TEST(test_getAllFromReceiveBuffer_from_received_message);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
