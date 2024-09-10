/**
 * @file Tests.h
 * @author MH-Tobi
 * @brief blub
 * @version 0.0.1
 * @date 2024-09-03
 *
 * @copyright -
 *
 */

#ifndef TEST_MCP2515_H
#define TEST_MCP2515_H

#include "MCP2515.h"

bool _initialize_Module(MCP2515 &Module, uint8_t CS_Pin, uint32_t SPIFrequency, uint32_t ClockFrequency, uint32_t Baudrate);

uint8_t testInit(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetSpiPins(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetSpiMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetDataOrder(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetSpiFrequency(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetClockFrequency(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetConfigurationMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetNormalMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetListenOnlyMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetLoopbackMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetSleepMode(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetReCheckEnabler(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testChangeBaudRate(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testChangeInterruptSetting(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testEnableFilterMask(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testDisableFilterMask(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testCheck4FreeTransmitBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testFillTransmitBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSendMessage(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testCheck4Rtr(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testCheck4Receive(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testGetIdFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testGetDlcFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testGetFrameFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testGetRtrFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testGetDataFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testGetAllFromReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testReleaseReceiveBuffer(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testCheck4InterruptFlags(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testResetInterruptFlag(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);
uint8_t testSetInterruptFlag(uint8_t Teststep, MCP2515 &Module, uint32_t defaultSettings[]);

#endif