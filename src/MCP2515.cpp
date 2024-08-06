#include "MCP2515.h"

/**
 * @brief Klassenkonstruktor
 */
MCP2515::MCP2515() :
  _spiSettings(MCP2515_MAX_SPI_SPEED, MCP2515_SPI_DATA_ORDER, MCP2515_SPI_MODE),
  _csPin(MCP2515_DEFAULT_CS_PIN),
  _intPin(MCP2515_DEFAULT_INT_PIN),
  _clockFrequency(MCP2515_DEFAULT_CLOCK_FREQUENCY),
  _operationMode(0xFF),
  _baudRate(MCP2515_DEFAULT_BAUDRATE),
  _isInitialized(false)
{
}

/**
 * @brief Dekonstruktor
 */
MCP2515::~MCP2515()
{
}

/**
 * @brief Führt die RESET-Instruction aus.
 * @note Doc Seite 65; Kapitel 12.2
 *
 * Setzt die internen Register auf die Default-Werte zurück und setzt den Configuration-Mode.
 */
bool MCP2515::resetInstruction()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_RESET);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  delayMicroseconds(10);

  // Check if Configuration-Mode is set
  if ((readInstruction(REG_CANSTAT) & CANSTAT_BIT_OPMOD) != 0x80) {
    return false;
  }

  _operationMode = MCP2515_OP_CONFIGURATION;
  return true;
}

/**
 * @brief Gibt den Register-Wert der übergebenen Register-Adresse durch READ-Instruction zurück.
 * @note Doc Seite 65; Kapitel 12.3
 *
 * @param address Register-Adresse
 * @return Register-Wert
 */
uint8_t MCP2515::readInstruction(uint8_t address)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return 0;
  }

  uint8_t value;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_READ);
  SPI.transfer(address);
  value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return value;
}

/**
 * @brief Gibt den Status-Wert der oft benutzten Status-Bits für den Nachrichtenempfang und -übertragung durch die READ STATUS-Instruction zurück.
 * @note Doc Seite 66; Kapitel 12.8
 * @returns
 * Bit 0 - RX0IF (CANINTF[0])   - Receive Buffer 0 Full Interrupt Flag bit
 *
 * Bit 1 - RX1IF (CANINTF[1])   - Receive Buffer 1 Full Interrupt Flag bit
 *
 * Bit 2 - TXREQ (TXB0CNTRL[3]) - Message Transmit Request bit Buffer 0
 *
 * Bit 3 - TX0IF (CANINTF[2])   - Transmit Buffer 0 Empty Interrupt Flag bit
 *
 * Bit 4 - TXREQ (TXB1CNTRL[3]) - Message Transmit Request bit Buffer 1
 *
 * Bit 5 - TX1IF (CANINTF[3])   - Transmit Buffer 1 Empty Interrupt Flag bit
 *
 * Bit 6 - TXREQ (TXB2CNTRL[3]) - Message Transmit Request bit Buffer 2
 *
 * Bit 7 - TX2IF (CANINTF[4])   - Transmit Buffer 2 Empty Interrupt Flag bit
 */
uint8_t MCP2515::readStatusInstruction()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return 0;
  }

  uint8_t value;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_READ_STATUS);
  value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return value;
}

/**
 * @brief Gibt den RX-Buffer-Wert des übergebenen Adress-Pionters durch die READ RX BUFFER-Instruction zurück.
 *
 * n - m - Address Points to - Address - Flag
 *
 * 0 - 0 - Receive Buffer 0, Start at RXB0SIDH - 0x61 - RX0IF
 *
 * 0 - 1 - Receive Buffer 0, Start at RXB0D0 - 0x66 - RX0IF
 *
 * 1 - 0 - Receive Buffer 1, Start at RXB1SIDH - 0x71 - RX1IF
 *
 * 1 - 1 - Receive Buffer 1, Start at RXB1D0 - 0x76 - RX1IF
 *
 * Zusätzliche wird der entsprechende Receive Flag zurückgesetzt.
 * @note Doc Seite 65; Kapitel 12.4
 *
 * @param n RX-Buffer Adress-Pointer (0 oder 1)
 * @param m RX-Buffer Adress-Pointer (0 oder 1)
 * @return Buffer-Wert
 */
uint8_t MCP2515::readRxBufferInstruction(uint8_t n, uint8_t m)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return 0;
  }

  uint8_t value;

  if (n>1 or m>1)
  {
    return 0;
  } else {
    uint8_t Instruction = MCP2515_SPI_INSTRUCTION_READ_RX_BUFFER | n << 2 | m << 1;

    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    SPI.transfer(Instruction);
    value = SPI.transfer(0x00);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();

    return value;
  }
}

/**
 * @brief Schreibt den übergebenen Wert in die übergebene Register-Adresse durch die WRITE-Instruction.
 * @note Doc Seite 65; Kapitel 12.5
 *
 * @param address Register-Adresse
 * @param value Register-Wert
 *
 * @return true on success, false when not
 */
bool MCP2515::writeInstruction(uint8_t address, uint8_t value)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_WRITE);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();
  delayMicroseconds(10);
  return true;
}

/**
 * @brief Ändert Bits der übergebenen Register-Adresse entsprechend der übergebenen Maske durch die BIT MODIFY-Instruction.
 *
 * Erlaubte Register-Adressen sind:
 *
 * BFPCTRL, TXRTSCTRL, CANCTRL, CNF3, CNF2, CNF1, CANINTE, CANINTF, EFLG, TXB0CTRL, TXB1CTRL, TXB2CTRL, RXB0CTRL, RXB1CTRL
 * @note Doc Seite 66; Kapitel 12.10
 *
 * @param address Register-Adresse
 * @param mask Maske
 * @param value Wert
 */
bool MCP2515::bitModifyInstruction(uint8_t address, uint8_t mask, uint8_t value)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  // Check if adress is one of the allowed Register-Adresses
  if (address==REG_BFPCTRL or address==REG_TXRTSCTRL or address==REG_CANCTRL or address==REG_CNF3 or
      address==REG_CNF2 or address==REG_CNF1 or address==REG_CANINTE or address==REG_CANINTF or
      address==REG_EFLG or address==REG_TXBnCTRL(0) or address==REG_TXBnCTRL(1) or address==REG_TXBnCTRL(2) or
      address==REG_RXBnCTRL(0) or address==REG_RXBnCTRL(1))
  {
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    SPI.transfer(MCP2515_SPI_INSTRUCTION_BIT_MODIFY);
    SPI.transfer(address);
    SPI.transfer(mask);
    SPI.transfer(value);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();

    return true;
  }
  return false;
}

/**
 * @brief Beschreibt den Buffer des übergebenen Adress-Pionters mit dem übergebenen Wert durch die LOAD TX BUFFER-Instruction.
 *
 * a - b - c - Address Points to - Address
 *
 * 0 - 0 - 0 - TX Buffer 0, Start at TXB0SIDH - 0x31
 *
 * 0 - 0 - 1 - TX Buffer 0, Start at TXB0D0 - 0x36
 *
 * 0 - 1 - 0 - TX Buffer 1, Start at TXB1SIDH - 0x41
 *
 * 0 - 1 - 1 - TX Buffer 1, Start at TXB1D0 - 0x46
 *
 * 1 - 0 - 0 - TX Buffer 2, Start at TXB2SIDH - 0x51
 *
 * 1 - 0 - 1 - TX Buffer 2, Start at TXB2D0 - 0x56
 *
 * @note Doc Seite 65; Kapitel 12.6
 *
 * @param a TX-Buffer Adress-Pointer (0 oder 1)
 * @param b TX-Buffer Adress-Pointer (0 oder 1)
 * @param c TX-Buffer Adress-Pointer (0 oder 1)
 * @param value Wert
 * @return false bei einem Fehler, true bei erfolgreicher Durchführung
 */
bool MCP2515::loadTxBufferInstruction(uint8_t value, uint8_t a, uint8_t b, uint8_t c)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  if (a>1 or b>1 or c>1)
  {
    return false;
  } else {
    uint8_t Instruction = MCP2515_SPI_INSTRUCTION_LOAD_TX_BUFFER | a << 2 | b << 1 | c;

    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    SPI.transfer(Instruction);
    SPI.transfer(value);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();

    return true;
  }
}

/**
 * @brief Initiiert für die gewählten TX-Buffer die Sende-Anforderung durch die RTS-Instruction.
 * @note Doc Seite 65; Kapitel 12.7
 *
 * @param tx_buffer_0 initiiere Trandmission des TX-Buffers 0 (0 oder 1)
 * @param tx_buffer_1 initiiere Trandmission des TX-Buffers 1 (0 oder 1)
 * @param tx_buffer_2 initiiere Trandmission des TX-Buffers 2 (0 oder 1)
 * @return false bei einem Fehler, true bei erfolgreicher Durchführung
 */
bool MCP2515::rtsInstruction(uint8_t tx_buffer_0, uint8_t tx_buffer_1, uint8_t tx_buffer_2)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  if (tx_buffer_0 > 1 or tx_buffer_1 > 1 or tx_buffer_2 > 1)
  {
    return false;
  } else if (tx_buffer_0 != 0 or tx_buffer_1 != 0 or tx_buffer_2 != 0)
  {
    uint8_t Instruction = MCP2515_SPI_INSTRUCTION_RTS | tx_buffer_2 << 2 | tx_buffer_1 << 1 | tx_buffer_0;
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    SPI.transfer(Instruction);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
  }
  return true;
}

/**
 * @brief Gibt zurück, welcher Filter auf die Nachricht/Nachrichtentyp zutrifft, durch die RX STATUS-Instruction.
 * @note Doc Seite 66; Kapitel 12.9
 *
 * @return
 *
 *  7 | 6 | Received Message  || 4 | 3 | Msg Type Received     || 2 | 1 | 0 | Filter Match
 *
 *  0 | 0 |  No RX message    || 0 | 0 | Standard data frame   || 0 | 0 | 0 | RXF0
 *
 *  0 | 1 | Message in RXB0   || 0 | 1 | Standard remote frame || 0 | 0 | 1 | RXF1
 *
 *  1 | 0 | Message in RXB1   || 1 | 0 | Extended data frame   || 0 | 1 | 0 | RXF2
 *
 *  1 | 1 | Messages in both buffers^ || 1 | 1 | Extended remote frame || 0 | 1 | 1 | RXF3
 *
 *
 *  RXnIF (CANINTF) bits are  ||  The extended ID bit is       || 1 | 0 | 0 | RXF4
 *
 *  mapped to bits 7 and 6.   ||  mapped to bit 4.             || 1 | 0 | 1 | RXF5
 *
 *                            ||   The RTR bit is mapped to    || 1 | 1 | 0 | RXF0 (rollover to RXB1)
 *
 *                            ||   bit 3.                      || 1 | 1 | 1 | RXF1 (rollover to RXB1)
 *
 * ^Buffer 0 has higher priority; therefore, RXB0 status is reflected in bits[4:0].
 */
uint8_t MCP2515::rxStatusInstruction()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return 0;
  }

  uint8_t value;
  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_RX_STATUS);
  value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return value;
}

/**
 * @brief Setzen des CS (ChipSelect) und des INT (Interrupt) Pins.
 *
 * Dieser Methode muss vor der init()-Methode ausgeführt werden.
 *
 * @param cs
 * @param irq
 */
bool MCP2515::setSpiPins(uint8_t cs, uint8_t irq)
{
  if (_isInitialized)
  {
    return false;
  }
  _csPin = cs;
  _intPin = irq;

  return true;
}

/**
 * @brief Setzen des SPI-Mode.
 *
 * Dieser Methode muss vor der init()-Methode ausgeführt werden.
 *
 * Mögliche Werte für den MCP2515 sind SPI_MODE0 and SPI_MODE3.
 *
 * @param spiMode
 * @return true bei Erfolg, false bei nicht erlaubtem Wert
 */
bool MCP2515::setSpiMode(uint8_t spiMode)
{
  if (_isInitialized)
  {
    return false;
  }
  // check if spiMode is allowed
  if (spiMode == SPI_MODE0 or spiMode == SPI_MODE3)
  {
    // check if spiMode is different to _spiMode
    if (spiMode != _spiMode)
    {
      _spiMode = spiMode;
      _spiSettings = SPISettings(_frequency, _dataOrder, spiMode);
    }
    return true;
  } else {
    // MCP2515 only allows SPI_MODE0 or SPI_MODE3
    return false;
  }
}

/**
 * @brief Setzen der DataOrder.
 *
 * Dieser Methode muss vor der init()-Methode ausgeführt werden.
 *
 * Für den MCP2515 ist in den docs nichts zu finden. Daher wird nur die Einstellung aus bereits existierenden Librarys erlaubt (MSBFIRST).
 *
 * (Mögliche Werte sind MSBFIRST or LSBFIRST.)
 *
 * @param dataOrder
 * @return true bei Erfolg, false bei nicht erlaubtem Wert
 */
bool MCP2515::setDataOrder(uint8_t dataOrder)
{
  if (_isInitialized)
  {
    return false;
  }

  // check if dataOrder is allowed
  if (dataOrder == MSBFIRST)
  {
    // check if dataOrder is different to _dataOrder
    if (dataOrder != _dataOrder)
    {
      _dataOrder = dataOrder;
      _spiSettings = SPISettings(_frequency, dataOrder, _spiMode);
    }
    return true;
  } else {
    // only MSBFIRST is allowed
    return false;
  }
}

/**
 * @brief Setzen der SPI-Clock-Frequenz.
 *
 * Dieser Methode muss vor der init()-Methode ausgeführt werden.
 *
 * Wert muss kleiner sein, als der vom MCP2515 erlaubte (max. 10e6).
 *
 * @param frequency
 * @return true bei Erfolg, false bei nicht erlaubtem Wert
 */
bool MCP2515::setSpiFrequency(uint64_t frequency)
{
  if (_isInitialized)
  {
    return false;
  }

  // check if frequency is allowed
  if (frequency <= MCP2515_MAX_SPI_SPEED)
  {
    // check if frequency is different to _frequency
    if (frequency != _frequency)
    {
      _frequency = frequency;
      _spiSettings = SPISettings(_frequency, _dataOrder, _spiMode);
    }
    return true;
  } else {
    // max. MCP2515_MAX_SPI_SPEED is allowed
    return false;
  }
}

/**
 * @brief Setzen der Clock-Frequenz.
 *
 * Dieser Methode muss vor der init()-Methode ausgeführt werden.
 *
 * Wert muss kleiner sein, als der vom MCP2515 erlaubte (max. 25e6).
 *
 * @param clockFrequency
 * @return true bei Erfolg, false bei nicht erlaubtem Wert
 */
bool MCP2515::setClockFrequency(uint64_t clockFrequency)
{
  if (_isInitialized)
  {
    return false;
  }

  // check if clockFrequency is allowed
  if (clockFrequency <= MCP2515_MAX_CLOCK_FREQUENCY)
  {
    // check if clockFrequency is different to _clockFrequency
    if (clockFrequency != _clockFrequency)
    {
      _clockFrequency = clockFrequency;
    }
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Setzen das MCP2515 in den Configuration-Mode.
 * @note Doc Seite 59; Kapitel 10.1
 *
 * @return true bei Erfolg, false bei nicht erfolg
 */
bool MCP2515::setConfigurationMode()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  uint8_t counter = 100; // Warte bis zu einer Sekunde
  bitModifyInstruction(REG_CANCTRL, CANCTRL_BIT_REQOP, 0x80);
  while ((readInstruction(REG_CANSTAT) & CANSTAT_BIT_OPMOD) != 0x80 and counter != 0)
  {
    counter--;
    delay(10);
  }

  if (counter == 0)
  {
    return false;
  }

  _operationMode = MCP2515_OP_CONFIGURATION;
  return true;
}

/**
 * @brief Setzen das MCP2515 in den Normal-Mode.
 * @note Doc Seite 60; Kapitel 10.5
 *
 * @return true bei Erfolg, false bei nicht erfolg
 */
bool MCP2515::setNormalMode()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  uint8_t counter = 100; // Warte bis zu einer Sekunde
  bitModifyInstruction(REG_CANCTRL, CANCTRL_BIT_REQOP, 0x00);
  while ((readInstruction(REG_CANSTAT) & CANSTAT_BIT_OPMOD) != 0x00 and counter != 0)
  {
    counter--;
    delay(10);
  }

  if (counter == 0)
  {
    return false;
  }

  _operationMode = MCP2515_OP_NORMAL;
  return true;
}

/**
 * @brief Setzen das MCP2515 in den Sleep-Mode.
 * @note Doc Seite 59; Kapitel 10.2
 *
 * @return true bei Erfolg, false bei nicht erfolg
 */
bool MCP2515::setSleepMode()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  uint8_t counter = 100; // Warte bis zu einer Sekunde
  bitModifyInstruction(REG_CANCTRL, CANCTRL_BIT_REQOP, 0x20);
  while ((readInstruction(REG_CANSTAT) & CANSTAT_BIT_OPMOD) != 0x20 and counter != 0)
  {
    counter--;
    delay(10);
  }

  if (counter == 0)
  {
    return false;
  }

  _operationMode = MCP2515_OP_SLEEP;
  return true;
}

/**
 * @brief Setzen das MCP2515 in den Listen-Only-Mode.
 * @note Doc Seite 59; Kapitel 10.3
 *
 * @return true bei Erfolg, false bei nicht erfolg
 */
bool MCP2515::setListenOnlyMode()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  uint8_t counter = 100; // Warte bis zu einer Sekunde
  bitModifyInstruction(REG_CANCTRL, CANCTRL_BIT_REQOP, 0x60);
  while ((readInstruction(REG_CANSTAT) & CANSTAT_BIT_OPMOD) != 0x60 and counter != 0)
  {
    counter--;
    delay(10);
  }

  if (counter == 0)
  {
    return false;
  }

  _operationMode = MCP2515_OP_SLEEP;
  return true;
}

/**
 * @brief Setzen das MCP2515 in den Loopback-Mode.
 * @note Doc Seite 60; Kapitel 10.4
 *
 * @return true bei Erfolg, false bei nicht erfolg
 */
bool MCP2515::setLoopbackMode()
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  uint8_t counter = 100; // Warte bis zu einer Sekunde
  bitModifyInstruction(REG_CANCTRL, CANCTRL_BIT_REQOP, 0x40);
  while ((readInstruction(REG_CANSTAT) & CANSTAT_BIT_OPMOD) != 0x40 and counter != 0)
  {
    counter--;
    delay(10);
  }

  if (counter == 0)
  {
    return false;
  }

  _operationMode = MCP2515_OP_LOOPBACK;
  return true;
}


/**
 * @brief Initialisierung des MCP2515.
 *
 * Setzt das MCP2515 zurück und befüllt anschließend die Konfigurations-Register,
 *
 * die nur im Configuration-Mode befüllt werden können, mit den übergebenen Werten.
 *
 * CNF1, CNF2, CNF3 registers (Bit-Timing-Register)
 *
 * TXRTSCTRL register
 *
 * Filter registers
 *
 * Mask registers
 *
 * @param baudRate Baudrate der CAN-Kommunikation
 * @return true bei Erfolg, false bei nicht Erfolg
 */
bool MCP2515::init(uint64_t baudRate)
{
  pinMode(_csPin, OUTPUT);
  _isInitialized = true;

  // start SPI
  SPI.begin();

  // Reset the MCP2515
  if (!resetInstruction()) {
    _isInitialized = false;
    return false;
  }

  // Change the Bit-Timing
  if (!changeBitTiming(baudRate, _clockFrequency)) {
    _isInitialized = false;
    return false;
  }

  // setzt die TXnRTS-Pin als Digital Input
  // Ansonsten könnte man die Pins verwenden um eine Message-Transmission zu initiieren (siehe doc S. 16, Kapitel 3.5)
  // Pins sind auf dem Modul nicht verbunden.
  bitModifyInstruction(REG_TXRTSCTRL, TXRTSCTRL_BIT_BnRTSM(0) | TXRTSCTRL_BIT_BnRTSM(1) | TXRTSCTRL_BIT_BnRTSM(2), 0x00);

  // Check if TXnRTS-Pins are successfully set
  if ((readInstruction(REG_TXRTSCTRL) & ((TXRTSCTRL_BIT_BnRTSM(0) | TXRTSCTRL_BIT_BnRTSM(1) | TXRTSCTRL_BIT_BnRTSM(2)))) != 0x00)
  {
    _isInitialized = false;
    return false;
  }

  // setzt die RXnBF-Pins auf disabled
  // Ansonsten könnte man die Pins unter anderem dazu verwenden, um einen vollen Receive-Buffer zu erkennen (siehe doc S. 24, Kapitel 4.4)
  // Pins sind auf dem Modul nicht verbunden.
  bitModifyInstruction(REG_BFPCTRL, BFPCTRL_BIT_BnBFE(0) | BFPCTRL_BIT_BnBFE(1), 0x00);

  // Check if RXnBF-Pins are successfully set
  if ((readInstruction(REG_BFPCTRL) & (BFPCTRL_BIT_BnBFE(0) | BFPCTRL_BIT_BnBFE(1))) != 0x00)
  {
    _isInitialized = false;
    return false;
  }

  // Set Interrupt-Enabler for the Receive-Buffer
  if (!changeReceiveBufferNFullInterrupt(true, 0) or !changeReceiveBufferNFullInterrupt(true, 1))
  {
    _isInitialized = false;
    return false;
  }

  // Ausschalten der Mask und Filter-Kriterien, sodass jede Nachricht empfangen wird.
  if (!disableFilterMask(0) or !disableFilterMask(1))
  {
    _isInitialized = false;
    return false;
  }

  // Wechsel in den Normal-Mode
  if (setNormalMode() != true) {
    _isInitialized = false;
    return false;
  }

  return true;
}

/**
 * @brief Deinitialisierung des MCP2515.
 */
void MCP2515::deinit()
{
  _isInitialized = false;
  SPI.end();
}


/**
 * @brief Changing the Bit-Timing according to the Baudrate and ClockFrequency.
 *
 * @param targetBaudrate possible Baudrates= (5E3, 10E3, 20E3, 40E3, 50E3, 80E3, 100E3, 125E3, 200E3, 250E3, 500E3, 1000E3)
 * @param targetClockFrequency possible ClockFrequencys= (8E6, 16E6)
 *
 * @return true when success, false on any error (it is possible that the settings changed anyway)
 */
bool MCP2515::changeBitTiming(uint64_t targetBaudRate, uint64_t targetClockFrequency)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  uint8_t OperationMode = _operationMode;

  // Check if Baudrate is plausible
  if (targetBaudRate != 5E3 && targetBaudRate != 10E3 && targetBaudRate != 20E3 &&
      targetBaudRate != 40E3 && targetBaudRate != 50E3 && targetBaudRate != 80E3 &&
      targetBaudRate != 100E3 && targetBaudRate != 125E3 && targetBaudRate != 200E3 &&
      targetBaudRate != 250E3 && targetBaudRate != 500E3 && targetBaudRate != 1000E3)
  {
    return false;
  }

  // Check if ClockFrequency is plausible
  if (targetClockFrequency != 8E6 && targetClockFrequency != 16E6)
  {
    return false;
  }

  // Bit-Timing Konfigurations-Register setzen
  // Recht kompliziert. Daher aus anderer Bibliothek übernommen.
  const struct {
    uint64_t clockFrequency;
    uint64_t baudRate;
    uint8_t cnf[3];
  } CNF_MAPPER[] = {
    {  (uint64_t)8E6, (uint64_t)1000E3, { 0x00, 0x80, 0x00 } },
    {  (uint64_t)8E6,  (uint64_t)500E3, { 0x00, 0x90, 0x02 } },
    {  (uint64_t)8E6,  (uint64_t)250E3, { 0x00, 0xb1, 0x05 } },
    {  (uint64_t)8E6,  (uint64_t)200E3, { 0x00, 0xb4, 0x06 } },
    {  (uint64_t)8E6,  (uint64_t)125E3, { 0x01, 0xb1, 0x05 } },
    {  (uint64_t)8E6,  (uint64_t)100E3, { 0x01, 0xb4, 0x06 } },
    {  (uint64_t)8E6,   (uint64_t)80E3, { 0x01, 0xbf, 0x07 } },
    {  (uint64_t)8E6,   (uint64_t)50E3, { 0x03, 0xb4, 0x06 } },
    {  (uint64_t)8E6,   (uint64_t)40E3, { 0x03, 0xbf, 0x07 } },
    {  (uint64_t)8E6,   (uint64_t)20E3, { 0x07, 0xbf, 0x07 } },
    {  (uint64_t)8E6,   (uint64_t)10E3, { 0x0f, 0xbf, 0x07 } },
    {  (uint64_t)8E6,    (uint64_t)5E3, { 0x1f, 0xbf, 0x07 } },

    { (uint64_t)16E6, (uint64_t)1000E3, { 0x00, 0xd0, 0x82 } },
    { (uint64_t)16E6,  (uint64_t)500E3, { 0x00, 0xf0, 0x86 } },
    { (uint64_t)16E6,  (uint64_t)250E3, { 0x41, 0xf1, 0x85 } },
    { (uint64_t)16E6,  (uint64_t)200E3, { 0x01, 0xfa, 0x87 } },
    { (uint64_t)16E6,  (uint64_t)125E3, { 0x03, 0xf0, 0x86 } },
    { (uint64_t)16E6,  (uint64_t)100E3, { 0x03, 0xfa, 0x87 } },
    { (uint64_t)16E6,   (uint64_t)80E3, { 0x03, 0xff, 0x87 } },
    { (uint64_t)16E6,   (uint64_t)50E3, { 0x07, 0xfa, 0x87 } },
    { (uint64_t)16E6,   (uint64_t)40E3, { 0x07, 0xff, 0x87 } },
    { (uint64_t)16E6,   (uint64_t)20E3, { 0x0f, 0xff, 0x87 } },
    { (uint64_t)16E6,   (uint64_t)10E3, { 0x1f, 0xff, 0x87 } },
    { (uint64_t)16E6,    (uint64_t)5E3, { 0x3f, 0xff, 0x87 } },
  };

  const uint8_t* cnf = NULL;

  for (unsigned int i = 0; i < (sizeof(CNF_MAPPER) / sizeof(CNF_MAPPER[0])); i++) {
    if (CNF_MAPPER[i].clockFrequency == targetClockFrequency && CNF_MAPPER[i].baudRate == targetBaudRate) {
      cnf = CNF_MAPPER[i].cnf;
      break;
    }
  }

  if (cnf == NULL) {
    return false;
  }

  if (OperationMode != MCP2515_OP_CONFIGURATION)
  {
    if (!setConfigurationMode())
    {
      return false;
    }
  }

  writeInstruction(REG_CNF1, cnf[0]);
  writeInstruction(REG_CNF2, cnf[1]);
  writeInstruction(REG_CNF3, cnf[2]);

  delayMicroseconds(10);

  // Check if Bit-Timing-Register are successfully set
  if (readInstruction(REG_CNF1) != cnf[0] or readInstruction(REG_CNF2) != cnf[1] or readInstruction(REG_CNF3) != cnf[2])
  {
    return false;
  }

  // Change to previous OperationMode
  if (OperationMode != _operationMode)
  {
    switch (OperationMode)
    {
    case MCP2515_OP_LISTEN:
      if (!setListenOnlyMode())
      {
        return false;
      }
      break;
    case MCP2515_OP_LOOPBACK:
      if (!setLoopbackMode())
      {
        return false;
      }
      break;
    case MCP2515_OP_NORMAL:
      if (!setNormalMode())
      {
        return false;
      }
      break;
    case MCP2515_OP_SLEEP:
      if (!setSleepMode())
      {
        return false;
      }
      break;
    case MCP2515_OP_CONFIGURATION:
      if (!setConfigurationMode())
      {
        return false;
      }
      break;
    default:
      return false;
      break;
    }
  }

  return true;
}

/**
 * @brief Changing the Baudrate.
 *
 * @param targetBaudrate possible Baudrates= (5E3, 10E3, 20E3, 40E3, 50E3, 80E3, 100E3, 125E3, 200E3, 250E3, 500E3, 1000E3)
 *
 * @return true when success, false on any error (it is possible that the settings changed anyway)
 */
bool MCP2515::changeBaudRate(uint64_t targetBaudRate)
{
  if (!_isInitialized)
  {
    return false;
  }

  if (targetBaudRate != _baudRate)
  {
    if (!changeBitTiming(targetBaudRate, _clockFrequency))
    {
      return false;
    }
  }
  return true;
}

/**
 * @brief Changing the ClockFrequency.
 *
 * @param targetClockFrequency possible ClockFrequencys= (8E6, 16E6)
 *
 * @return true when success, false on any error (it is possible that the settings changed anyway)
 */
bool MCP2515::changeClockFrequency(uint64_t targetClockFrequency)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  if (targetClockFrequency != _clockFrequency)
  {
    if (!changeBitTiming(_baudRate, targetClockFrequency))
    {
      return false;
    }
  }
  return true;
}

/**
 * @brief Enables the Filter/Mask for the given Receive-Buffer.
 *
 * @param buffer Receive-Buffer-Number (0 - 1)
 *
 * @return true when success, false on any error
 */
bool MCP2515::enableFilterMask(uint8_t buffer)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  // Check if Buffer-Number is plausible
  if (buffer > 1)
  {
    return false;
  }

  // Enables the Filter/Mask
  bitModifyInstruction(REG_RXBnCTRL(buffer), RXBnCTRL_BIT_RXM, 0x00);

  // Check if Mask and Filter-Criterias are successfully set
  if ((readInstruction(REG_RXBnCTRL(buffer)) & RXBnCTRL_BIT_RXM) != 0x00)
  {
    return false;
  }

  return true;
}

/**
 * @brief Disables the Filter/Mask for the given Receive-Buffer.
 *
 * @param buffer Receive-Buffer-Number (0 - 1)
 *
 * @return true when success, false on any error
 */
bool MCP2515::disableFilterMask(uint8_t buffer)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  // Check if Buffer-Number is plausible
  if (buffer > 1)
  {
    return false;
  }

  // Enables the Filter/Mask
  bitModifyInstruction(REG_RXBnCTRL(buffer), RXBnCTRL_BIT_RXM, RXBnCTRL_BIT_RXM);

  // Check if Mask and Filter-Criterias are successfully set
  if ((readInstruction(REG_RXBnCTRL(buffer)) & RXBnCTRL_BIT_RXM) != RXBnCTRL_BIT_RXM)
  {
    return false;
  }

  return true;
}


bool MCP2515::changeInterruptEnabler(bool value, uint8_t InterruptBit)
{
  // Check if MCP2515-Module is initialised
  if (!_isInitialized)
  {
    return false;
  }

  uint8_t EnablerValue = 0x00;
  if (value)
  {
    EnablerValue = InterruptBit;
  }

  // setzt die Interrupt-Enabler auf Übergabewert (siehe doc S. 53, Register CANINTE)
  bitModifyInstruction(REG_CANINTE, InterruptBit, EnablerValue);

  // Check if Interrupt-Enabler are successfully set
  if ((readInstruction(REG_CANINTE) & InterruptBit) != EnablerValue)
  {
    return false;
  }

  return true;
}

bool MCP2515::changeMessageErrorInterrupt(bool value)
{
  return changeInterruptEnabler(value, CANINTE_BIT_MERRE);
}

bool MCP2515::changeWakeUpInterrupt(bool value)
{
  return changeInterruptEnabler(value, CANINTE_BIT_WAKIE);
}

bool MCP2515::changeErrorInterrupt(bool value)
{
  return changeInterruptEnabler(value, CANINTE_BIT_ERRIE);
}

bool MCP2515::changeTransmitBufferNEmptyInterrupt(bool value, uint8_t buffer)
{
  // Check if Buffernumber is plausible
  if (buffer>2)
  {
    return false;
  }

  return changeInterruptEnabler(value, CANINTE_BIT_TXnIE(buffer));
}

bool MCP2515::changeReceiveBufferNFullInterrupt(bool value, uint8_t buffer)
{
  // Check if Buffernumber is plausible
  if (buffer>1)
  {
    return false;
  }

  return changeInterruptEnabler(value, CANINTE_BIT_RXnIE(buffer));
}
