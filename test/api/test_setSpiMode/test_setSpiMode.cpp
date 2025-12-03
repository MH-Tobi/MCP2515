#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setSpiMode_when_not_initialized()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiMode(SPI_MODE0), "Setting SPI-Mode not successfull.");
}

void test_setSpiMode_when_initialized()
{
    MCP2515Module.setSpiFrequency(defaultSettings[2]);
    delay(100);
    MCP2515Module.setClockFrequency(defaultSettings[3]);
    delay(100);
    MCP2515Module.setSpiPins(defaultSettings[0]);
    delay(100);
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.init(defaultSettings[4]), "Init not successful.");
    delay(100);
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setSpiMode(SPI_MODE0), "Setting SPI-Mode successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_IS_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting SPI-Mode when initialized.");
    }
}

void test_setSpiMode_with_allowed_SPI_Modes()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiMode(SPI_MODE0), "Setting SPI-Mode SPI_MODE0 not successfull.");
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setSpiMode(SPI_MODE3), "Setting SPI-Mode SPI_MODE3 not successfull.");
}

void test_setSpiMode_with_unallowed_SPI_Modes()
{
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");
    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setSpiMode(SPI_MODE1), "Setting SPI-Mode SPI_MODE1 successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_SPI_MODE_NOT_ALLOWED))!=static_cast<uint16_t>(MCP2515Error::MAIN_SPI_MODE_NOT_ALLOWED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_SPI_MODE_NOT_ALLOWED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting SPI-Mode SPI_MODE1.");
    }

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setSpiMode(SPI_MODE2), "Setting SPI-Mode SPI_MODE2 successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_SPI_MODE_NOT_ALLOWED))!=static_cast<uint16_t>(MCP2515Error::MAIN_SPI_MODE_NOT_ALLOWED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_SPI_MODE_NOT_ALLOWED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting SPI-Mode SPI_MODE2.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp() {
    // set stuff up here
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_setSpiMode_when_not_initialized);
    RUN_TEST(test_setSpiMode_when_initialized);
    RUN_TEST(test_setSpiMode_with_allowed_SPI_Modes);
    RUN_TEST(test_setSpiMode_with_unallowed_SPI_Modes);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
