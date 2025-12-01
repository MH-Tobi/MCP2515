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


uint8_t Data_Transmit[8] = {1, 2, 3, 4, 0, 0, 0, 0};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_check4Receive_when_not_initialized()
{
    uint8_t Data_Receive[8];
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Receive(defined_Standard_IDs[0], false, 4, Data_Receive), "Check for received Message successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for check for received Message when not initialized.");
    }
}

void test_check4Receive_when_initialized()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    if (!MCP2515Module.check4Receive(defined_Standard_IDs[0], false, 4, Data_Receive))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                        MCP2515Module.getLastMCPError(),
                                        "Error-Code occured for check for received Message when initialized.");
    }
}

void test_check4Receive_with_defined_standard_id()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        if (!MCP2515Module.check4Receive(defined_Standard_IDs[i], false, 4, Data_Receive))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                            MCP2515Module.getLastMCPError(),
                                            "Error-Code occured for check for received Message with defined Standard-ID.");
        }
    }
}

void test_check4Receive_with_undefined_standard_id()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Receive(undefined_Standard_IDs[i], false, 4, Data_Receive), "Check for received Message with undefined Standard-ID successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for check for received Message with undefined Standard-ID.");
        }
    }
}

void test_check4Receive_with_defined_extended_id()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        if (!MCP2515Module.check4Receive(defined_Extended_IDs[i], true, 4, Data_Receive))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                            MCP2515Module.getLastMCPError(),
                                            "Error-Code occured for check for received Message with defined Extended-ID.");
        }
    }
}

void test_check4Receive_with_undefined_extended_id()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Receive(undefined_Extended_IDs[i], true, 4, Data_Receive), "Check for received Message with undefined Extended-ID successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for check for received Message with undefined Extended-ID.");
        }
    }
}

void test_check4Receive_with_defined_dlc()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<8; i++)
    {
        if (!MCP2515Module.check4Receive(defined_Standard_IDs[0], false, i, Data_Receive))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::NO_ERROR),
                                            MCP2515Module.getLastMCPError(),
                                            "Error-Code occured for check for received Message with defined DLC.");
        }
    }
}

void test_check4Receive_with_undefined_dlc()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=9; i<11; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.check4Receive(defined_Standard_IDs[0], false, i, Data_Receive), "Check for received Message with undefined DLC successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for check for received Message with undefined DLC.");
        }
    }
}

void test_check4Receive_for_received_message()
{
    uint8_t Data_Receive[8];
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Loopback Mode is not set.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.fillTransmitBuffer(0, defined_Standard_IDs[1], false, false, 4, Data_Transmit), "Filling the TransmitBuffer 0 not successfull.");
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.sendMessage(0, 0), "Sending Message not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.check4Receive(defined_Standard_IDs[1], false, 4, Data_Receive), "Check for received Message not successfull.");
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

    RUN_TEST(test_check4Receive_when_not_initialized);
    RUN_TEST(test_check4Receive_when_initialized);
    RUN_TEST(test_check4Receive_with_defined_standard_id);
    RUN_TEST(test_check4Receive_with_undefined_standard_id);
    RUN_TEST(test_check4Receive_with_defined_extended_id);
    RUN_TEST(test_check4Receive_with_undefined_extended_id);
    RUN_TEST(test_check4Receive_with_defined_dlc);
    RUN_TEST(test_check4Receive_with_undefined_dlc);
    RUN_TEST(test_check4Receive_for_received_message);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
