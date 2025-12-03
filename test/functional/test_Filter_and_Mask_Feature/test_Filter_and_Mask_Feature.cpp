#include <unity.h>
#include "../common_test_settings.h"

// additional Test Settings
struct CanMessage{
    uint32_t ID;
    bool Extended;
    bool RTR;
    uint8_t DLC;
    uint8_t Data[8];
};

CanMessage allowedMessages[6] = {
    {0x00001B2, false, false, 4, {0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00}},  // Mask 0, Filter 0; Note first two Databytes
    {0x0360146, true , false, 4, {0x00, 0x00, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00}},  // Mask 0, Filter 1
    {0x000010F, false, false, 4, {0xA0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},  // Mask 1, Filter 2; Note first two Databytes
    {0x22AA509, true , false, 4, {0x11, 0x12, 0x13, 0x14, 0x00, 0x00, 0x00, 0x00}},  // Mask 1, Filter 3
    {0x0000559, false, false, 4, {0x60, 0x90, 0x17, 0x18, 0x00, 0x00, 0x00, 0x00}},  // Mask 1, Filter 4; Note first two Databytes
    {0xE8C4815, true , false, 4, {0x19, 0x1A, 0x1B, 0x1C, 0x00, 0x00, 0x00, 0x00}}   // Mask 1, Filter 5
};

CanMessage blockedMessages[3] = {
    {0x00001B2, false, false, 4, {0xA5, 0x16, 0x07, 0x08, 0x00, 0x00, 0x00, 0x00}},  // Mask 0, Filter 1; Note first two Databytes
    {0x1ABCDE1, true, false, 4, {0x0D, 0x0E, 0x0F, 0x10, 0x00, 0x00, 0x00, 0x00}},
    {0x1ABCDE2, true , false, 4, {0x11, 0x12, 0x13, 0x14, 0x00, 0x00, 0x00, 0x00}}
};

Filter Filters[6]{
    {0x000000B0, false},
    {0x0030074F, true},
    {0x0000050F, false},
    {0x020A0509, true},
    {0x00000549, false},
    {0x1E7C8895, true}
};
uint32_t Masks[2]{
    0x10F0F0F0,
    0x0F0F0F0F
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_Filter_and_Mask_Feature_send_allowed_Message_1_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, allowedMessages[0].ID, allowedMessages[0].Extended, allowedMessages[0].RTR, allowedMessages[0].DLC, allowedMessages[0].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Receive(allowedMessages[0].ID, allowedMessages[0].Extended, allowedMessages[0].DLC, Data_Receive), "Check for received Message not successfull.");

    for (size_t i = 0; i < allowedMessages[0].DLC; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE((Data_Receive[i] == allowedMessages[0].Data[i]), "Data of the received Message doesn't match the sended Data.");
    }
}

void test_Filter_and_Mask_Feature_send_allowed_Message_2_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, allowedMessages[1].ID, allowedMessages[1].Extended, allowedMessages[1].RTR, allowedMessages[1].DLC, allowedMessages[1].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Receive(allowedMessages[1].ID, allowedMessages[1].Extended, allowedMessages[1].DLC, Data_Receive), "Check for received Message not successfull.");

    for (size_t i = 0; i < allowedMessages[1].DLC; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE((Data_Receive[i] == allowedMessages[1].Data[i]), "Data of the received Message doesn't match the sended Data.");
    }
}

void test_Filter_and_Mask_Feature_send_allowed_Message_3_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, allowedMessages[2].ID, allowedMessages[2].Extended, allowedMessages[2].RTR, allowedMessages[2].DLC, allowedMessages[2].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Receive(allowedMessages[2].ID, allowedMessages[2].Extended, allowedMessages[2].DLC, Data_Receive), "Check for received Message not successfull.");

    for (size_t i = 0; i < allowedMessages[2].DLC; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE((Data_Receive[i] == allowedMessages[2].Data[i]), "Data of the received Message doesn't match the sended Data.");
    }
}

void test_Filter_and_Mask_Feature_send_allowed_Message_4_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, allowedMessages[3].ID, allowedMessages[3].Extended, allowedMessages[3].RTR, allowedMessages[3].DLC, allowedMessages[3].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Receive(allowedMessages[3].ID, allowedMessages[3].Extended, allowedMessages[3].DLC, Data_Receive), "Check for received Message not successfull.");

    for (size_t i = 0; i < allowedMessages[3].DLC; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE((Data_Receive[i] == allowedMessages[3].Data[i]), "Data of the received Message doesn't match the sended Data.");
    }
}

void test_Filter_and_Mask_Feature_send_allowed_Message_5_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, allowedMessages[4].ID, allowedMessages[4].Extended, allowedMessages[4].RTR, allowedMessages[4].DLC, allowedMessages[4].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Receive(allowedMessages[4].ID, allowedMessages[4].Extended, allowedMessages[4].DLC, Data_Receive), "Check for received Message not successfull.");

    for (size_t i = 0; i < allowedMessages[4].DLC; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE((Data_Receive[i] == allowedMessages[4].Data[i]), "Data of the received Message doesn't match the sended Data.");
    }
}

void test_Filter_and_Mask_Feature_send_allowed_Message_6_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, allowedMessages[5].ID, allowedMessages[5].Extended, allowedMessages[5].RTR, allowedMessages[5].DLC, allowedMessages[5].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Receive(allowedMessages[5].ID, allowedMessages[5].Extended, allowedMessages[5].DLC, Data_Receive), "Check for received Message not successfull.");

    for (size_t i = 0; i < allowedMessages[5].DLC; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE((Data_Receive[i] == allowedMessages[5].Data[i]), "Data of the received Message doesn't match the sended Data.");
    }
}

void test_Filter_and_Mask_Feature_send_blocked_Message_1_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, blockedMessages[0].ID, blockedMessages[0].Extended, blockedMessages[0].RTR, blockedMessages[0].DLC, blockedMessages[0].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Receive(blockedMessages[0].ID, blockedMessages[0].Extended, blockedMessages[0].DLC, Data_Receive), "Check for received Message successfull.");
}

void test_Filter_and_Mask_Feature_send_blocked_Message_2_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, blockedMessages[1].ID, blockedMessages[1].Extended, blockedMessages[1].RTR, blockedMessages[1].DLC, blockedMessages[1].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Receive(blockedMessages[1].ID, blockedMessages[1].Extended, blockedMessages[1].DLC, Data_Receive), "Check for received Message successfull.");
}

void test_Filter_and_Mask_Feature_send_blocked_Message_3_and_check_for_reception()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE((MCP2515Module.getOperationMode() == MCP2515OperationMode::LOOPBACK), "Loopback-Mode is not set.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, blockedMessages[2].ID, blockedMessages[2].Extended, blockedMessages[2].RTR, blockedMessages[2].DLC, blockedMessages[2].Data), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Receive(blockedMessages[2].ID, blockedMessages[2].Extended, blockedMessages[2].DLC, Data_Receive), "Check for received Message successfull.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
    initializeModule(MCP2515Module, defaultSettings[0], defaultSettings[2], defaultSettings[3], defaultSettings[4]);
    delay(100);

    MCP2515Module.setLoopbackMode();

    for (size_t i = 0; i < 6; i++)
    {
        MCP2515Module.setFilter(i, Filters[i].ID, Filters[i].Extended);
        delay(100);
    }
    delay(100);

    for (size_t i = 0; i < 2; i++)
    {
        MCP2515Module.setMask(i, Masks[i]);
        delay(100);
    }

    delay(100);

    for (size_t i=0; i < 2; i++)
    {
        MCP2515Module.enableFilterMask(i);
        delay(100);
    }

    delay(100);
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    delay(5000);

    RUN_TEST(test_Filter_and_Mask_Feature_send_allowed_Message_1_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_allowed_Message_2_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_allowed_Message_3_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_allowed_Message_4_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_allowed_Message_5_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_allowed_Message_6_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_blocked_Message_1_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_blocked_Message_2_and_check_for_reception);
    RUN_TEST(test_Filter_and_Mask_Feature_send_blocked_Message_3_and_check_for_reception);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
