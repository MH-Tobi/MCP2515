#include <unity.h>
#include "../common_test_settings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_if_interrupt_0_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(0), "Reset Interrupt Flag 0 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 0), "Changing Interrupt Setting 0 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(0), "Setting Interrupt Flag 0 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
}

void test_if_interrupt_1_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(1), "Reset Interrupt Flag 1 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 1), "Changing Interrupt Setting 1 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(1), "Setting Interrupt Flag 1 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
}

void test_if_interrupt_2_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(2), "Reset Interrupt Flag 2 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 2), "Changing Interrupt Setting 2 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(2), "Setting Interrupt Flag 2 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
}

void test_if_interrupt_3_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(3), "Reset Interrupt Flag 3 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 3), "Changing Interrupt Setting 3 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(3), "Setting Interrupt Flag 3 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
}

void test_if_interrupt_4_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(4), "Reset Interrupt Flag 4 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 4), "Changing Interrupt Setting 4 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(4), "Setting Interrupt Flag 4 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
}

void test_if_interrupt_5_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(5), "Reset Interrupt Flag 5 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 5), "Changing Interrupt Setting 5 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(5), "Setting Interrupt Flag 5 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
}

void test_if_interrupt_6_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(6), "Reset Interrupt Flag 6 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 6), "Changing Interrupt Setting 6 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(6), "Setting Interrupt Flag 6 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
}

void test_if_interrupt_7_occurs_on_int_pin()
{
    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.getIsInitialized(), "Module is not initialized.");

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.resetInterruptFlag(7), "Reset Interrupt Flag 7 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.changeInterruptSetting(true, 7), "Changing Interrupt Setting 7 not successfull.");
    delay(100);

    TEST_ASSERT_TRUE_MESSAGE(MCP2515Module.setInterruptFlag(7), "Setting Interrupt Flag 7 not successfull.");
    delay(100);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, digitalRead(defaultSettings[1]), "No Interrupt occured on INT-Pin.");
    delay(100);
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

    delay(5000);

    RUN_TEST(test_if_interrupt_0_occurs_on_int_pin);
    RUN_TEST(test_if_interrupt_1_occurs_on_int_pin);
    RUN_TEST(test_if_interrupt_2_occurs_on_int_pin);
    RUN_TEST(test_if_interrupt_3_occurs_on_int_pin);
    RUN_TEST(test_if_interrupt_4_occurs_on_int_pin);
    RUN_TEST(test_if_interrupt_5_occurs_on_int_pin);
    RUN_TEST(test_if_interrupt_6_occurs_on_int_pin);
    RUN_TEST(test_if_interrupt_7_occurs_on_int_pin);

    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}
