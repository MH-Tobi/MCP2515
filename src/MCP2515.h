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
//#include <type_traits>
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
enum class MCP2515SpiInstruction: uint8_t {
	RESET			= 0xC0,
	READ			= 0x03,
	READ_RX_BUFFER	= 0x90,
	WRITE			= 0x02,
	LOAD_TX_BUFFER	= 0x40,
	RTS				= 0x80,
	READ_STATUS		= 0xA0,
	RX_STATUS		= 0xB0,
	BIT_MODIFY		= 0x05,
};

/**
 * MCP2515 Operation-Modes
 */
enum class MCP2515OperationMode: uint8_t {
	NORMAL			= 0x00,
	SLEEP			= 0x01,
	LOOPBACK		= 0x02,
	LISTEN			= 0x03,
	CONFIGURATION	= 0x04,
};

/**
 * MCP2515 other Constants
 */

#define MCP2515_DEFAULT_BAUDRATE	500E3

struct Filter
{
	uint32_t ID;
	bool Extended;
};


class MCP2515
{
	private:
		SPISettings m_spiSettings;
		uint8_t m_csPin;
		uint8_t m_spiMode;
		uint8_t m_dataOrder;
		uint32_t m_spiFrequency;
		uint32_t m_clockFrequency;
		MCP2515OperationMode m_operationMode;
		uint32_t m_baudRate;
		bool m_isInitialized;
		uint16_t m_lastMcpError;
		bool m_reCheckEnabled;
		Filter m_filterSettings[6];
		uint32_t m_maskSettings[2];




		// Register Setter/Getter/Modifier

		uint8_t getFilterStandardIdentifierHigh(const uint8_t FilterNumber);
		bool setFilterStandardIdentifierHigh(const uint8_t FilterNumber, const uint8_t StandardId_10_3);

		uint8_t getFilterStandardIdentifierLow(const uint8_t FilterNumber);
		bool setFilterStandardIdentifierLow(const uint8_t FilterNumber, const uint8_t StandardId_2_0, const bool ExtendedIdEn, const uint8_t ExtendedId_17_16);

		uint8_t getFilterExtendedIdentifierHigh(const uint8_t FilterNumber);
		bool setFilterExtendedIdentifierHigh(const uint8_t FilterNumber, const uint8_t ExtendedId_15_8);

		uint8_t getFilterExtendedIdentifierLow(const uint8_t FilterNumber);
		bool setFilterExtendedIdentifierLow(const uint8_t FilterNumber, const uint8_t ExtendedId_7_0);

		uint8_t getMaskStandardIdentifierHigh(const uint8_t MaskNumber);
		bool setMaskStandardIdentifierHigh(const uint8_t MaskNumber, const uint8_t StandardId_10_3);

		uint8_t getMaskStandardIdentifierLow(const uint8_t MaskNumber);
		bool setMaskStandardIdentifierLow(const uint8_t MaskNumber, const uint8_t StandardId_2_0, const uint8_t ExtendedId_17_16);

		uint8_t getMaskExtendedIdentifierHigh(const uint8_t MaskNumber);
		bool setMaskExtendedIdentifierHigh(const uint8_t MaskNumber, const uint8_t ExtendedId_15_8);

		uint8_t getMaskExtendedIdentifierLow(const uint8_t MaskNumber);
		bool setMaskExtendedIdentifierLow(const uint8_t MaskNumber, const uint8_t ExtendedId_7_0);

		uint8_t getRXnBFPinControl();
		bool setRXnBFPinControl(const bool B1BFS, const bool B0BFS, const bool B1BFE, const bool B0BFE, const bool B1BFM, const bool B0BFM);
		bool modifyRXnBFPinControl(const uint8_t Mask, const uint8_t Value);

		uint8_t getTXnRTSPinControl();
		bool setTXnRTSPinControl(const bool B2RTSM, const bool B1RTSM, const bool B0RTSM);
		bool modifyTXnRTSPinControl(const uint8_t Mask, const uint8_t Value);

		uint8_t getCanStatus();

		uint8_t getCanControl();
		bool setCanControl(const uint8_t REQOP, const bool ABAT, const bool OSM, const bool CLKEN, const uint8_t CLKPRE);
		bool modifyCanControl(const uint8_t Mask, const uint8_t Value);

		uint8_t getTransmitErrorCounter();
		uint8_t getReceiveErrorCounter();

		uint8_t getConfigurationRegister3();
		bool setConfigurationRegister3(const bool SOF, const bool WAKFIL, const uint8_t PHSEG2);
		bool modifyConfigurationRegister3(const uint8_t Mask, const uint8_t Value);

		uint8_t getConfigurationRegister2();
		bool setConfigurationRegister2(const bool BTLMODE, const bool SAM, const uint8_t PHSEG1, const uint8_t PRSEG);
		bool modifyConfigurationRegister2(const uint8_t Mask, const uint8_t Value);

		uint8_t getConfigurationRegister1();
		bool setConfigurationRegister1(const uint8_t SJW, const uint8_t BRP);
		bool modifyConfigurationRegister1(const uint8_t Mask, const uint8_t Value);

		uint8_t getCanInterruptEnable();
		bool setCanInterruptEnable(const bool MERRE, const bool WAKIE, const bool ERRIE, const bool TX2IE, const bool TX1IE, const bool TX0IE, const bool RX1IE, const bool RX0IE);
		bool modifyCanInterruptEnable(const uint8_t Mask, const uint8_t Value);

		uint8_t getCanInterruptFlag();
		bool setCanInterruptFlag(const bool MERRF, const bool WAKIF, const bool ERRIF, const bool TX2IF, const bool TX1IF, const bool TX0IF, const bool RX1IF, const bool RX0IF);
		bool modifyCanInterruptFlag(const uint8_t Mask, const uint8_t Value);

		uint8_t getErrorFlag();
		bool setErrorFlag(const bool RX1OVR, const bool RX0OVR);
		bool modifyErrorFlag(const uint8_t Mask, const uint8_t Value);

		uint8_t getTransmitBufferControl(const uint8_t BufferNumber);
		bool setTransmitBufferControl(const uint8_t BufferNumber, const bool TXREQ, const uint8_t TXP);
		bool modifyTransmitBufferControl(const uint8_t BufferNumber, const uint8_t Mask, const uint8_t Value);

		uint8_t getTransmitBufferStandardIdentifierHigh(const uint8_t BufferNumber);
		bool setTransmitBufferStandardIdentifierHigh(const uint8_t BufferNumber, const uint8_t StandardId_10_3);

		uint8_t getTransmitBufferStandardIdentifierLow(const uint8_t BufferNumber);
		bool setTransmitBufferStandardIdentifierLow(const uint8_t BufferNumber, const uint8_t StandardId_2_0, const bool ExtendedIdEn, const uint8_t ExtendedId_17_16);

		uint8_t getTransmitBufferExtendedIdentifierHigh(const uint8_t BufferNumber);
		bool setTransmitBufferExtendedIdentifierHigh(const uint8_t BufferNumber, const uint8_t ExtendedId_15_8);

		uint8_t getTransmitBufferExtendedIdentifierLow(const uint8_t BufferNumber);
		bool setTransmitBufferExtendedIdentifierLow(const uint8_t BufferNumber, const uint8_t ExtendedId_7_0);

		uint8_t getTransmitBufferDataLengthCode(const uint8_t BufferNumber);
		bool setTransmitBufferDataLengthCode(const uint8_t BufferNumber, const bool RTR, const uint8_t DLC);

		uint8_t getTransmitBufferDataByte(const uint8_t BufferNumber, const uint8_t ByteNumber);
		bool setTransmitBufferDataByte(const uint8_t BufferNumber, const uint8_t ByteNumber, const uint8_t Data);

		uint8_t getReceiveBuffer0Control();
		bool setReceiveBuffer0Control(const uint8_t RXM, const bool BUKT);
		bool modifyReceiveBuffer0Control(const uint8_t Mask, const uint8_t Value);

		uint8_t getReceiveBuffer1Control();
		bool setReceiveBuffer1Control(const uint8_t RXM);
		bool modifyReceiveBuffer1Control(const uint8_t Mask, const uint8_t Value);

		uint8_t getReceiveBufferStandardIdentifierHigh(const uint8_t BufferNumber);

		uint8_t getReceiveBufferStandardIdentifierLow(const uint8_t BufferNumber);

		uint8_t getReceiveBufferExtendedIdentifierHigh(const uint8_t BufferNumber);

		uint8_t getReceiveBufferExtendedIdentifierLow(const uint8_t BufferNumber);

		uint8_t getReceiveBufferDataLengthCode(const uint8_t BufferNumber);

		uint8_t getReceiveBufferDataByte(const uint8_t BufferNumber, const uint8_t ByteNumber);

		// SPI-Instructions

		bool resetInstruction();
		bool writeInstruction(const uint8_t Address, const uint8_t Value);
		bool bitModifyInstruction(const uint8_t Address, const uint8_t Mask, const uint8_t Value);
		bool loadTxBufferInstruction(const uint8_t Value, const bool a = false, const bool b = false, const bool c = false);
		bool rtsInstruction(const bool TXBuffer_0 = false, const bool TXBuffer_1 = false, const bool TXBuffer_2 = false);
		uint8_t rxStatusInstruction();
		uint8_t readInstruction(const uint8_t Address);
		uint8_t readStatusInstruction();
		uint8_t readRxBufferInstruction(const bool n = false, const bool m = false);

		// other privat Methods

		bool changeBitTiming(const uint32_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE, const uint32_t targetClockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);

		bool resetOperationMode(const MCP2515OperationMode OperationMode);

	public:
		// Constructors

		MCP2515();
		~MCP2515();

		// for Error-Handling

		uint16_t getLastMCPError();

		// only before initialisation

		bool setSpiPins(const uint8_t cs = 0);
		bool setSpiMode(const uint8_t SpiMode = MCP2515_SPI_MODE);
		bool setDataOrder(const uint8_t DataOrder = MCP2515_SPI_DATA_ORDER);
		bool setSpiFrequency(const uint32_t frequency = MCP2515_MAX_SPI_SPEED);
		bool setClockFrequency(const uint32_t clockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);

		// De-/Initialisation of the MCP2515

		bool init(const uint32_t BaudRate = MCP2515_DEFAULT_BAUDRATE, const bool reCheckEnabled = true);
		void deinit();

		// MCP2515-Operationmodes

		bool setConfigurationMode();
		bool setNormalMode();
		bool setSleepMode();
		bool setListenOnlyMode();
		bool setLoopbackMode();

		// change Settings

		bool setReCheckEnabler(const bool reCheckEnabler);
		bool changeBaudRate(const uint32_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE);
		//bool changeClockFrequency(uint32_t targetClockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);
		bool changeInterruptSetting(const bool value, const uint8_t InterruptBit);

		// Filter/Mask-Handling

		bool enableFilterMask(const uint8_t buffer = 0);
		bool disableFilterMask(const uint8_t buffer = 0);
		bool setFilter(const uint8_t FilterNumber, const uint32_t &ID, const bool Extended);
		bool setMask(const uint8_t MaskNumber, const uint32_t &ID);

		// Message-Handling
		// Transmission

		uint8_t check4FreeTransmitBuffer();
		bool fillTransmitBuffer(const uint8_t BufferNumber, uint32_t ID, const bool Extended, const bool RTR, const uint8_t DLC, const uint8_t Data[8]);
		bool sendMessage(const uint8_t BufferNumber, const uint8_t Priority = 0);

		// Reception

		bool check4Rtr(const uint32_t &ID, const bool &Extended);
		bool check4Receive(const uint32_t &ID, const bool &Extended, const uint8_t &DLC, uint8_t (&DataBuffer)[8]);
		uint32_t getIdFromReceiveBuffer(const uint8_t BufferNumber);
		uint8_t getDlcFromReceiveBuffer(const uint8_t BufferNumber);
		uint8_t getFrameFromReceiveBuffer(const uint8_t BufferNumber);
		uint8_t getRtrFromReceiveBuffer(const uint8_t BufferNumber);
		bool getDataFromReceiveBuffer(const uint8_t BufferNumber, const uint8_t DLC, uint8_t (&DataBuffer)[8]);
		bool getAllFromReceiveBuffer(const uint8_t BufferNumber, uint32_t (&ID), bool (&Frame), bool (&RTR), uint8_t (&DLC), uint8_t (&DataBuffer)[8]);

		bool releaseReceiveBuffer(const uint8_t BufferNumber);

		// InterruptFlag-Handling

		uint8_t check4InterruptFlags();
		bool resetInterruptFlag(const uint8_t Flag);
		bool setInterruptFlag(const uint8_t Flag);

		// get Settings

		uint8_t getCsPin();
		uint8_t getSpiMode();
		uint8_t getDataOrder();
		uint32_t getFrequency();
		uint32_t getClockFrequency();
		MCP2515OperationMode getOperationMode();
		uint32_t getBaudRate();
		bool getIsInitialized();
		bool getReCheckEnabled();
};

#endif
