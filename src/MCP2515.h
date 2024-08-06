/**
 * @file MCP2515.h
 * @author Tobias Pflug (nichtindiesemleben@gibtesnicht.com)
 * @brief Doku erstellt mit Doxygen
 * @version 0.1
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

/**
 * Konstanten fuer SPI-Kommunikation
 */
#define MCP2515_DEFAULT_CLOCK_FREQUENCY 16e6	// max Clock frequency is 25MHz (see doc page 75)
#define MCP2515_MAX_CLOCK_FREQUENCY 25e6	// max Clock frequency is 25MHz (see doc page 75)
#define MCP2515_MAX_SPI_SPEED 10e6	// max SPI-Speed (see doc page 77 or 1)

/**
 * Default-Pins für die SPI-Kommunikation (aus Arduino.h)
 */
#if defined(ARDUINO_ARCH_SAMD) && defined(PIN_SPI_MISO) && defined(PIN_SPI_MOSI) && defined(PIN_SPI_SCK) && (PIN_SPI_MISO == 10) && (PIN_SPI_MOSI == 8) && (PIN_SPI_SCK == 9)
// Arduino MKR board: MKR CAN shield CS is pin 3, INT is pin 7
#define MCP2515_DEFAULT_CS_PIN          3
#define MCP2515_DEFAULT_INT_PIN         7
#else
#define MCP2515_DEFAULT_CS_PIN          10
#define MCP2515_DEFAULT_INT_PIN         2
#endif

/**
 * Possible SPI-Modes are 0,0 and 1,1 (SPI_MODE0 and SPI_MODE3).
 * Defined SPI-Modes:
 * ___________________________________________________________________________________
 * |    Mode     |  Clock Polarity  |  Clock Phase  |  Output Edge  |  Data Capture  |
 * |             |      (CPOL)      |    (CPHA)     |               |                |
 * |_____________|__________________|_______________|_______________|________________|
 * |  SPI_MODE0  |        0         |       0       |    Falling    |     Rising     |
 * |_____________|__________________|_______________|_______________|________________|
 * |  SPI_MODE1  |        0         |       1       |    Rising     |     Falling    |
 * |_____________|__________________|_______________|_______________|________________|
 * |  SPI_MODE2  |        1         |       0       |    Rising     |     Falling    |
 * |_____________|__________________|_______________|_______________|________________|
 * |  SPI_MODE3  |        1         |       1       |    Falling    |     Rising     |
 * |_____________|__________________|_______________|_______________|________________|
 */
#define MCP2515_SPI_MODE SPI_MODE0

/**
 * DataOrder of the SPI-Communication.
 * For the MCP2515 not defined in the docs. Using Settings from other librarys.
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

#define MCP2515_DEFAULT_BAUDRATE	500E3


/**
 * @brief Klassengrundgeruest
 */
class MCP2515
{
	private:
		SPISettings _spiSettings;
		uint8_t _csPin;
		uint8_t _intPin;
		uint8_t _spiMode;
		uint8_t _dataOrder;
		uint64_t _frequency;
		uint64_t _clockFrequency;
		uint8_t _operationMode;
		uint64_t _baudRate;
		bool _isInitialized;

		bool changeBitTiming(uint64_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE, uint64_t targetClockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);
		bool changeInterruptEnabler(bool value = false, uint8_t InterruptBit = 0x00);

	public:
		// Constructors

		MCP2515();
		~MCP2515();

		// only before initialisation

		bool setSpiPins(uint8_t cs = MCP2515_DEFAULT_CS_PIN, uint8_t irq = MCP2515_DEFAULT_INT_PIN);
		bool setSpiMode(uint8_t spiMode = MCP2515_SPI_MODE);
		bool setDataOrder(uint8_t dataOrder = MCP2515_SPI_DATA_ORDER);
		bool setSpiFrequency(uint64_t frequency = MCP2515_MAX_SPI_SPEED);
  		bool setClockFrequency(uint64_t clockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);

		// De-/Initialisation of the MCP2515

		bool init(uint64_t baudRate = MCP2515_DEFAULT_BAUDRATE);
		void deinit();

		// MCP2515-Operationmodes

		bool setConfigurationMode();
		bool setNormalMode();
		bool setSleepMode();
		bool setListenOnlyMode();
		bool setLoopbackMode();

		// SPI-Instructions

		bool resetInstruction();
		bool writeInstruction(uint8_t address, uint8_t value);
		bool bitModifyInstruction(uint8_t address, uint8_t mask, uint8_t value);
		bool loadTxBufferInstruction(uint8_t value, uint8_t a = 0, uint8_t b = 0, uint8_t c = 0);
		bool rtsInstruction(uint8_t tx_buffer_0 = 0, uint8_t tx_buffer_1 = 0, uint8_t tx_buffer_2 = 0);
		uint8_t rxStatusInstruction();
		uint8_t readInstruction(uint8_t address);
		uint8_t readStatusInstruction();
		uint8_t readRxBufferInstruction(uint8_t n = 0, uint8_t m = 0);

		// change Settings

		bool changeBaudRate(uint64_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE);
		bool changeClockFrequency(uint64_t targetClockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);

		bool enableFilterMask(uint8_t buffer = 0);
		bool disableFilterMask(uint8_t buffer = 0);
		//bool setFilterMask();

		bool changeMessageErrorInterrupt(bool value = false);
		bool changeWakeUpInterrupt(bool value = false);
		bool changeErrorInterrupt(bool value = false);
		bool changeTransmitBufferNEmptyInterrupt(bool value = false, uint8_t buffer = 0);
		bool changeReceiveBufferNFullInterrupt(bool value = false, uint8_t buffer = 0);

		// uint8_t checkForReceive();
		// bool sendMessage();
};

#endif
