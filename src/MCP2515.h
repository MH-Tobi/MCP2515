/**
 * @file MCP2515.h
 * @author MH-Tobi
 * @brief blub
 * @version 0.0.1
 * @date 2024-07-20
 *
 * @copyright -
 *
 */

#ifndef MCP2515_H
#define MCP2515_H

#include <Arduino.h>
#include <SPI.h>
#include "MCP2515Register.h"
#include "MCP2515Error.h"

/**
 * @brief Constants for SPI-Communication
 */

#define MCP2515_DEFAULT_CLOCK_FREQUENCY 8e6		// max Clock frequency is 40MHz (see doc page 75)
#define MCP2515_MAX_CLOCK_FREQUENCY 40e6		// max Clock frequency is 40MHz (see doc page 75)
#define MCP2515_MAX_SPI_SPEED 10e6				// max SPI-Speed (see doc page 77 or 1)

/**
 * @brief Possible SPI-Modes are 0,0 and 1,1 (SPI_MODE0 and SPI_MODE3).
 *
 * Defined SPI-Modes:
 *
 * |____Mode_____|_Clock Polarity (CPOL)_|_Clock Phase (CPHA)_|_Output Edge_|_Data Capture_|
 *
 * |__SPI_MODE0__|___________0___________|_________0__________|___Falling___|____Rising____|
 *
 * |__SPI_MODE1__|___________0___________|_________1__________|___Rising____|___Falling____|
 *
 * |__SPI_MODE2__|___________1___________|_________0__________|___Rising____|___Falling____|
 *
 * |__SPI_MODE3__|___________1___________|_________1__________|___Falling___|____Rising____|
 */
#define MCP2515_SPI_MODE SPI_MODE0

/**
 * @brief DataOrder of the SPI-Communication.
 *
 * For the MCP2515 not defined in the docs. Using Settings from other librarys.
 *
 * Possible Values MSBFIRST or LSBFIRST.
 */
#define MCP2515_SPI_DATA_ORDER MSBFIRST

/**
 * SPI-Instruction-Formats
 */

#define MCP2515_SPI_INSTRUCTION_RESET				0xC0
#define MCP2515_SPI_INSTRUCTION_READ				0x03
#define MCP2515_SPI_INSTRUCTION_READ_RX_BUFFER		0x90
#define MCP2515_SPI_INSTRUCTION_WRITE				0x02
#define MCP2515_SPI_INSTRUCTION_LOAD_TX_BUFFER		0x40
#define MCP2515_SPI_INSTRUCTION_RTS					0x80
#define MCP2515_SPI_INSTRUCTION_READ_STATUS			0xA0
#define MCP2515_SPI_INSTRUCTION_RX_STATUS			0xB0
#define MCP2515_SPI_INSTRUCTION_BIT_MODIFY			0x05

/**
 * MCP2515 Operation-Modes
 */

#define MCP2515_OP_NORMAL			0x00
#define MCP2515_OP_SLEEP			0x01
#define MCP2515_OP_LOOPBACK			0x02
#define MCP2515_OP_LISTEN			0x03
#define MCP2515_OP_CONFIGURATION	0x04


/**
 * MCP2515 other Constants
 */

#define MCP2515_DEFAULT_BAUDRATE	500E3


class MCP2515
{
	private:
		SPISettings _spiSettings;
		uint8_t _csPin;
		uint8_t _spiMode;
		uint8_t _dataOrder;
		uint32_t _spiFrequency;
		uint32_t _clockFrequency;
		uint8_t _operationMode;
		uint32_t _baudRate;
		bool _isInitialized;
		uint16_t _lastMcpError;
		bool _reCheckEnabled;




		// Register Setter/Getter/Modifier

		uint8_t getFilterStandardIdentifierHigh(uint8_t FilterNumber);
		bool setFilterStandardIdentifierHigh(uint8_t FilterNumber, uint8_t StandardId_10_3);

		uint8_t getFilterStandardIdentifierLow(uint8_t FilterNumber);
		bool setFilterStandardIdentifierLow(uint8_t FilterNumber, uint8_t StandardId_2_0, bool ExtendedIdEn, uint8_t ExtendedId_17_16);

		uint8_t getFilterExtendedIdentifierHigh(uint8_t FilterNumber);
		bool setFilterExtendedIdentifierHigh(uint8_t FilterNumber, uint8_t ExtendedId_15_8);

		uint8_t getFilterExtendedIdentifierLow(uint8_t FilterNumber);
		bool setFilterExtendedIdentifierLow(uint8_t FilterNumber, uint8_t ExtendedId_7_0);

		uint8_t getMaskStandardIdentifierHigh(uint8_t MaskNumber);
		bool setMaskStandardIdentifierHigh(uint8_t MaskNumber, uint8_t StandardId_10_3);

		uint8_t getMaskStandardIdentifierLow(uint8_t MaskNumber);
		bool setMaskStandardIdentifierLow(uint8_t MaskNumber, uint8_t StandardId_2_0, uint8_t ExtendedId_17_16);

		uint8_t getMaskExtendedIdentifierHigh(uint8_t MaskNumber);
		bool setMaskExtendedIdentifierHigh(uint8_t MaskNumber, uint8_t ExtendedId_15_8);

		uint8_t getMaskExtendedIdentifierLow(uint8_t MaskNumber);
		bool setMaskExtendedIdentifierLow(uint8_t MaskNumber, uint8_t ExtendedId_7_0);

		uint8_t getRXnBFPinControl();
		bool setRXnBFPinControl(bool B1BFS, bool B0BFS, bool B1BFE, bool B0BFE, bool B1BFM, bool B0BFM);
		bool modifyRXnBFPinControl(uint8_t Mask, uint8_t Value);

		uint8_t getTXnRTSPinControl();
		bool setTXnRTSPinControl(bool B2RTSM, bool B1RTSM, bool B0RTSM);
		bool modifyTXnRTSPinControl(uint8_t Mask, uint8_t Value);

		uint8_t getCanStatus();

		uint8_t getCanControl();
		bool setCanControl(uint8_t REQOP, bool ABAT, bool OSM, bool CLKEN, uint8_t CLKPRE);
		bool modifyCanControl(uint8_t Mask, uint8_t Value);

		uint8_t getTransmitErrorCounter();
		uint8_t getReceiveErrorCounter();

		uint8_t getConfigurationRegister3();
		bool setConfigurationRegister3(bool SOF, bool WAKFIL, uint8_t PHSEG2);
		bool modifyConfigurationRegister3(uint8_t Mask, uint8_t Value);

		uint8_t getConfigurationRegister2();
		bool setConfigurationRegister2(bool BTLMODE, bool SAM, uint8_t PHSEG1, uint8_t PRSEG);
		bool modifyConfigurationRegister2(uint8_t Mask, uint8_t Value);

		uint8_t getConfigurationRegister1();
		bool setConfigurationRegister1(uint8_t SJW, uint8_t BRP);
		bool modifyConfigurationRegister1(uint8_t Mask, uint8_t Value);

		uint8_t getCanInterruptEnable();
		bool setCanInterruptEnable(bool MERRE, bool WAKIE, bool ERRIE, bool TX2IE, bool TX1IE, bool TX0IE, bool RX1IE, bool RX0IE);
		bool modifyCanInterruptEnable(uint8_t Mask, uint8_t Value);

		uint8_t getCanInterruptFlag();
		bool setCanInterruptFlag(bool MERRF, bool WAKIF, bool ERRIF, bool TX2IF, bool TX1IF, bool TX0IF, bool RX1IF, bool RX0IF);
		bool modifyCanInterruptFlag(uint8_t Mask, uint8_t Value);

		uint8_t getErrorFlag();
		bool setErrorFlag(bool RX1OVR, bool RX0OVR);
		bool modifyErrorFlag(uint8_t Mask, uint8_t Value);

		uint8_t getTransmitBufferControl(uint8_t BufferNumber);
		bool setTransmitBufferControl(uint8_t BufferNumber, bool TXREQ, uint8_t TXP);
		bool modifyTransmitBufferControl(uint8_t BufferNumber, uint8_t Mask, uint8_t Value);

		uint8_t getTransmitBufferStandardIdentifierHigh(uint8_t BufferNumber);
		bool setTransmitBufferStandardIdentifierHigh(uint8_t BufferNumber, uint8_t StandardId_10_3);

		uint8_t getTransmitBufferStandardIdentifierLow(uint8_t BufferNumber);
		bool setTransmitBufferStandardIdentifierLow(uint8_t BufferNumber, uint8_t StandardId_2_0, bool ExtendedIdEn, uint8_t ExtendedId_17_16);

		uint8_t getTransmitBufferExtendedIdentifierHigh(uint8_t BufferNumber);
		bool setTransmitBufferExtendedIdentifierHigh(uint8_t BufferNumber, uint8_t ExtendedId_15_8);

		uint8_t getTransmitBufferExtendedIdentifierLow(uint8_t BufferNumber);
		bool setTransmitBufferExtendedIdentifierLow(uint8_t BufferNumber, uint8_t ExtendedId_7_0);

		uint8_t getTransmitBufferDataLengthCode(uint8_t BufferNumber);
		bool setTransmitBufferDataLengthCode(uint8_t BufferNumber, bool RTR, uint8_t DLC);

		uint8_t getTransmitBufferDataByte(uint8_t BufferNumber, uint8_t ByteNumber);
		bool setTransmitBufferDataByte(uint8_t BufferNumber, uint8_t ByteNumber, uint8_t Data);

		uint8_t getReceiveBuffer0Control();
		bool setReceiveBuffer0Control(uint8_t RXM, bool BUKT);
		bool modifyReceiveBuffer0Control(uint8_t Mask, uint8_t Value);

		uint8_t getReceiveBuffer1Control();
		bool setReceiveBuffer1Control(uint8_t RXM);
		bool modifyReceiveBuffer1Control(uint8_t Mask, uint8_t Value);

		uint8_t getReceiveBufferStandardIdentifierHigh(uint8_t BufferNumber);

		uint8_t getReceiveBufferStandardIdentifierLow(uint8_t BufferNumber);

		uint8_t getReceiveBufferExtendedIdentifierHigh(uint8_t BufferNumber);

		uint8_t getReceiveBufferExtendedIdentifierLow(uint8_t BufferNumber);

		uint8_t getReceiveBufferDataLengthCode(uint8_t BufferNumber);

		uint8_t getReceiveBufferDataByte(uint8_t BufferNumber, uint8_t ByteNumber);

		// SPI-Instructions

		bool resetInstruction();
		bool writeInstruction(uint8_t Address, uint8_t Value);
		bool bitModifyInstruction(uint8_t Address, uint8_t Mask, uint8_t Value);
		bool loadTxBufferInstruction(uint8_t Value, bool a = false, bool b = false, bool c = false);
		bool rtsInstruction(bool TXBuffer_0 = false, bool TXBuffer_1 = false, bool TXBuffer_2 = false);
		uint8_t rxStatusInstruction();
		uint8_t readInstruction(uint8_t Address);
		uint8_t readStatusInstruction();
		uint8_t readRxBufferInstruction(bool n = false, bool m = false);

		// other privat Methods

		bool changeBitTiming(uint32_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE, uint32_t targetClockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);


	public:
		// Constructors

		MCP2515();
		~MCP2515();

		// for Error-Handling

		uint16_t getLastMCPError();

		// only before initialisation

		bool setSpiPins(uint8_t cs = 0);
		bool setSpiMode(uint8_t SpiMode = MCP2515_SPI_MODE);
		bool setDataOrder(uint8_t DataOrder = MCP2515_SPI_DATA_ORDER);
		bool setSpiFrequency(uint32_t frequency = MCP2515_MAX_SPI_SPEED);
  		bool setClockFrequency(uint32_t clockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);

		// De-/Initialisation of the MCP2515

		bool init(uint32_t BaudRate = MCP2515_DEFAULT_BAUDRATE, bool reCheckEnabled = true);
		void deinit();

		// MCP2515-Operationmodes

		bool setConfigurationMode();
		bool setNormalMode();
		bool setSleepMode();
		bool setListenOnlyMode();
		bool setLoopbackMode();

		// change Settings

		bool setReCheckEnabler(bool reCheckEnabler);
		bool changeBaudRate(uint32_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE);
		//bool changeClockFrequency(uint32_t targetClockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);
		bool changeInterruptSetting(bool value, uint8_t InterruptBit);

		bool enableFilterMask(uint8_t buffer = 0);
		bool disableFilterMask(uint8_t buffer = 0);

		// Message-Handling
		// Transmission

		uint8_t check4FreeTransmitBuffer();
		bool fillTransmitBuffer(uint8_t BufferNumber, uint32_t ID, bool Extended, bool RTR, uint8_t DLC, uint8_t Data[8]);
		bool sendMessage(uint8_t BufferNumber, uint8_t Priority = 0);

		// Reception

		bool check4Rtr(uint32_t ID, bool Extended);
		bool check4Receive(uint32_t ID, bool Extended, uint8_t DLC, uint8_t (&DataBuffer)[8]);
		uint32_t getIdFromReceiveBuffer(uint8_t BufferNumber);
		uint8_t getDlcFromReceiveBuffer(uint8_t BufferNumber);
		uint8_t getFrameFromReceiveBuffer(uint8_t BufferNumber);
		uint8_t getRtrFromReceiveBuffer(uint8_t BufferNumber);
		bool getDataFromReceiveBuffer(uint8_t BufferNumber, uint8_t DLC, uint8_t (&DataBuffer)[8]);
		bool getAllFromReceiveBuffer(uint8_t BufferNumber, uint32_t (&ID), bool (&Frame), bool (&RTR), uint8_t (&DLC), uint8_t (&DataBuffer)[8]);

		bool releaseReceiveBuffer(uint8_t BufferNumber);

		// InterruptFlag-Handling

		uint8_t check4InterruptFlags();
		bool resetInterruptFlag(uint8_t Flag);
		bool setInterruptFlag(uint8_t Flag);

		// get Settings

		uint8_t getCsPin();
		uint8_t getSpiMode();
		uint8_t getDataOrder();
		uint32_t getFrequency();
		uint32_t getClockFrequency();
		uint8_t getOperationMode();
		uint32_t getBaudRate();
		bool getIsInitialized();
		bool getReCheckEnabled();
};

#endif
