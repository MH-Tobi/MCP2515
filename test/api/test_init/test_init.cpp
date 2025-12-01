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

// Define possible ClockFrequency-Array
const uint32_t possibleClockFrequency[4] = {
    (uint32_t)8E6,
    (uint32_t)16E6,
    (uint32_t)25E6,
    (uint32_t)40E6,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_init_without_setted_CS_Pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiPins(0), "Setting SPI-Pin not successful.");

    delay(100);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.init(defaultSettings[4]), "Init successful.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_INIT_CS_PIN_NOT_DEFINED))!=static_cast<uint16_t>(MCP2515Error::MAIN_INIT_CS_PIN_NOT_DEFINED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_INIT_CS_PIN_NOT_DEFINED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for init with unsetted SPI-Pins.");
    }
}

void test_init_with_false_CS_Pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiPins((defaultSettings[0]+1)), "Setting SPI-Pin not successful.");

    delay(100);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.init(defaultSettings[4]), "Init successful.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::SECONDARY_INIT_RESET_FAILED))!=static_cast<uint16_t>(MCP2515Error::SECONDARY_INIT_RESET_FAILED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::SECONDARY_INIT_RESET_FAILED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for init with wrong SPI-Pins.");
    }
}

void test_init_with_possible_Baudrates()
{
    for (size_t i = 0; i < 12; i++)
    {
        // If case of fail, there are not all Baudrates allowed for each Clockfrequency.
        if (MCP2515Module.init(possibleBaudRates[i]))
        {
            TEST_ASSERT(true);
        }else{
            if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_CNF_NOT_FOUND))!=static_cast<uint16_t>(MCP2515Error::MAIN_CNF_NOT_FOUND))
            {
                TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_CNF_NOT_FOUND),
                                                MCP2515Module.getLastMCPError(),
                                                "Wrong Error-Code for init with possible but for Clockrate undefined Baudrate.");
            }
        }
        delay(500);
        MCP2515Module.deinit();
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module still initialized.");
        delay(500);
    }
}

void test_init_with_undefined_Baudrates()
{
    for (size_t i = 0; i < 4; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.init(undefinedBaudrates[i]), "Init successful.");

        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_BAUDRATE_NOT_VALID))!=static_cast<uint16_t>(MCP2515Error::MAIN_BAUDRATE_NOT_VALID))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_BAUDRATE_NOT_VALID),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for init with undefined Baudrate.");
        }

        // deinitialise Module in case init is successfull
        delay(500);
        MCP2515Module.deinit();
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module still initialized.");
        delay(500);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
    MCP2515Module.setSpiFrequency(defaultSettings[2]);

    delay(100);

    MCP2515Module.setClockFrequency(defaultSettings[3]);

    delay(100);

    MCP2515Module.setSpiPins(defaultSettings[0]);
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_init_without_setted_CS_Pin);
    RUN_TEST(test_init_with_false_CS_Pin);
    RUN_TEST(test_init_with_possible_Baudrates);
    RUN_TEST(test_init_with_undefined_Baudrates);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
