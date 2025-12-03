#include <unity.h>
#include "../common_test_settings.h"

// additional Test Settings
// Define possible BaudRate-Array
const uint32_t possibleBaudRates[12] = {
    (uint32_t)5E3,      // Not allowed for ClockRate 25MHz and 40MHz
    (uint32_t)10E3,     // Not allowed for ClockRate 40MHz
    (uint32_t)20E3,
    (uint32_t)40E3,
    (uint32_t)50E3,
    (uint32_t)80E3,
    (uint32_t)100E3,
    (uint32_t)125E3,
    (uint32_t)200E3,
    (uint32_t)250E3,
    (uint32_t)500E3,
    (uint32_t)1000E3,   // Not allowed for ClockRate 8MHz
};

uint32_t undefinedBaudrates[4] = {
    (uint32_t)0,
    (uint32_t)6e3,
    (uint32_t)110e3,
    (uint32_t)1100e3,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_changeBaudRate_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.changeBaudRate(possibleBaudRates[4]), "Changing BaudRate successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for Changing BaudRate when not initialized.");
    }
}

void test_changeBaudRate_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeBaudRate(possibleBaudRates[5]), "Changing BaudRate not successfull.");
}

void test_changeBaudRate_with_possible_Baudrates()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    for (size_t i = 0; i < 12; i++)
    {
        // If case of fail, there are not all Baudrates allowed for each Clockfrequency.
        if (MCP2515Module.changeBaudRate(possibleBaudRates[i]))
        {
            TEST_ASSERT(true);
        }else{
            if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_CNF_NOT_FOUND))!=static_cast<uint16_t>(MCP2515Error::MAIN_CNF_NOT_FOUND))
            {
                TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_CNF_NOT_FOUND),
                                                MCP2515Module.getLastMCPError(),
                                                "Wrong Error-Code for changeBaudRate with possible but for Clockrate undefined Baudrate.");
            }
        }
        delay(500);
    }
}

void test_changeBaudRate_with_undefined_Baudrates()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    for (size_t i = 0; i < 4; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.changeBaudRate(undefinedBaudrates[i]), "Change Baudrate with undefined BaudRate successfull.");

        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_BAUDRATE_NOT_VALID))!=static_cast<uint16_t>(MCP2515Error::MAIN_BAUDRATE_NOT_VALID))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_BAUDRATE_NOT_VALID),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for init with undefined Baudrate.");
        }
        delay(500);
    }
}

void test_changeBaudRate_in_operation_mode_ListenOnly()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setListenOnlyMode(), "Setting ListenOnly-Mode not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeBaudRate(possibleBaudRates[2]), "Changing BaudRate not successfull.");
    delay(500);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(static_cast<uint8_t>(MCP2515OperationMode::LISTEN),
                                    static_cast<uint8_t>(MCP2515Module.getOperationMode()),
                                    "changeBaudRate does not stay in ListenOnly mode.");
}

void test_changeBaudRate_in_operation_mode_Loopback()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setLoopbackMode(), "Setting Loopback-Mode not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeBaudRate(possibleBaudRates[3]), "Changing BaudRate not successfull.");
    delay(500);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(static_cast<uint8_t>(MCP2515OperationMode::LOOPBACK),
                                    static_cast<uint8_t>(MCP2515Module.getOperationMode()),
                                    "changeBaudRate does not stay in Loopback mode.");
}

void test_changeBaudRate_in_operation_mode_Configuration()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setConfigurationMode(), "Setting Configuration-Mode not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeBaudRate(possibleBaudRates[4]), "Changing BaudRate not successfull.");
    delay(500);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(static_cast<uint8_t>(MCP2515OperationMode::CONFIGURATION),
                                    static_cast<uint8_t>(MCP2515Module.getOperationMode()),
                                    "changeBaudRate does not stay in Configuration mode.");
}

void test_changeBaudRate_in_operation_mode_Normal()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setNormalMode(), "Setting Normal-Mode not successfull.");
    delay(500);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeBaudRate(possibleBaudRates[5]), "Changing BaudRate not successfull.");
    delay(500);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(static_cast<uint8_t>(MCP2515OperationMode::NORMAL),
                                    static_cast<uint8_t>(MCP2515Module.getOperationMode()),
                                    "changeBaudRate does not stay in Normal mode.");
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

    RUN_TEST(test_changeBaudRate_when_not_initialized);
    RUN_TEST(test_changeBaudRate_when_initialized);
    RUN_TEST(test_changeBaudRate_with_possible_Baudrates);
    RUN_TEST(test_changeBaudRate_with_undefined_Baudrates);
    RUN_TEST(test_changeBaudRate_in_operation_mode_ListenOnly);
    RUN_TEST(test_changeBaudRate_in_operation_mode_Loopback);
    RUN_TEST(test_changeBaudRate_in_operation_mode_Configuration);
    RUN_TEST(test_changeBaudRate_in_operation_mode_Normal);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
