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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setFilter_when_not_initialized()
{
    MCP2515Module.deinit();
    delay(500);

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is initialized.");

    TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setFilter(0, 1, false), "Setting Filter successfull.");

    if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))!=static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED))
    {
        TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_NOT_INITIALIZED),
                                        MCP2515Module.getLastMCPError(),
                                        "Wrong Error-Code for setting Filter when not initialized.");
    }
}

void test_setFilter_when_initialized()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setFilter(0, 1, false), "Setting Filter not successfull.");
}

void test_setFilter_with_defined_filter_buffers()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<6; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setFilter(i, 1, false), "Setting Filter not successfull.");
    }
}

void test_setFilter_with_undefined_filter_buffers()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=6; i<9; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setFilter(i, 1, false), "Setting Filter successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for setting undefined FilterBuffer.");
        }
    }
}

void test_setFilter_with_defined_standard_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setFilter(0, defined_Standard_IDs[i], false), "Setting Filter not successfull.");
    }
}

void test_setFilter_with_undefined_standard_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setFilter(0, undefined_Standard_IDs[i], false), "Setting Filter successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for setting Filter with undefined Standard-ID.");
        }
    }
}

void test_setFilter_with_defined_extended_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<3; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setFilter(0, defined_Extended_IDs[i], true), "Setting Filter not successfull.");
    }
}

void test_setFilter_with_undefined_extended_id()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    for (size_t i=0; i<2; i++)
    {
        TEST_ASSERT_FALSE_MESSAGE(MCP2515Module.setFilter(0, undefined_Extended_IDs[i], true), "Setting Filter successfull.");
        if ((MCP2515Module.getLastMCPError() & static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))!=static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE))
        {
            TEST_ASSERT_EQUAL_HEX16_MESSAGE(static_cast<uint16_t>(MCP2515Error::MAIN_VALUE_OUTA_RANGE),
                                            MCP2515Module.getLastMCPError(),
                                            "Wrong Error-Code for setting Filter with undefined Extended-ID.");
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
}

void tearDown() {
    // clean stuff up here
    MCP2515Module.deinit();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_setFilter_when_not_initialized);
    RUN_TEST(test_setFilter_when_initialized);
    RUN_TEST(test_setFilter_with_defined_filter_buffers);
    RUN_TEST(test_setFilter_with_undefined_filter_buffers);
    RUN_TEST(test_setFilter_with_defined_standard_id);
    RUN_TEST(test_setFilter_with_undefined_standard_id);
    RUN_TEST(test_setFilter_with_defined_extended_id);
    RUN_TEST(test_setFilter_with_undefined_extended_id);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
