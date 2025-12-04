# API

## Constructor

```c++
MCP2515 MCP2515Module;
```

## Settings (only before initialisation)

### Set the CS (ChipSelect) Pin that is used for the SPI-Communication

![Teststatus setSpiPins](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setSpiPins(const uint8_t cs = 0);
```
- `cs` - ChipSelect-Pin
- Returns on success `true`, on any failure `false`
- This Method is required before initialization (Initialization will fail without setting)


### Set the SPI-Mode

![Teststatus setSpiMode](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setSpiMode(const uint8_t SpiMode = MCP2515_SPI_MODE);
```
- `SpiMode` - Allowed Values for the MCP2515 are SPI_MODE0 and SPI_MODE3.
    - Default is `SPI_MODE0`
- Returns on success `true`, on any failure `false`


### Set the DataOrder

![Teststatus setDataOrder](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setDataOrder(const uint8_t DataOrder = MCP2515_SPI_DATA_ORDER);
```
- `DataOrder` - Allowed Value for the MCP2515 is `MSBFIRST`.
- Returns on success `true`, on any failure `false`


### Set the SPI-Clock-Frequency

![Teststatus setSpiFrequency](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setSpiFrequency(const uint32_t frequency = MCP2515_MAX_SPI_SPEED);
```
- `frequency` - max. allowed Value for the MCP2515 is 10e6 Hz.
    - Default is 10e6
- Returns on success `true`, on any failure `false`


### Set the MCP2515 Clock-Frequency

![Teststatus setClockFrequency](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setClockFrequency(const uint32_t clockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);
```
- `clockFrequency` - max. allowed Value for the MCP2515 is 40e6 Hz.
    - Default is 8e6
- Returns on success `true`, on any failure `false`



## De-/Initialisation of the MCP2515

### Initialize the MCP2515

![Teststatus init](https://img.shields.io/badge/Teststatus-PASSED-green)

- Reset the MCP2515
- Set the BitTiming for the given BaudRate
- Setting the PinControl for RX-Pins and TX-Pins
- Enable Receive-Interrupts
- Disable Mask and Filter-Settings
- Set the OperationMode Normal

```c++
bool MCP2515Module.init(const uint32_t BaudRate = MCP2515_DEFAULT_BAUDRATE, const bool reCheckEnabled = true);
```
- `BaudRate` - Baudrate of the CAN-Communication
    - Default is `500e3`
- `reCheckEnabled` - true if after Set-Processes a recheck of setted Values has to be execute
    - Default is `true`
- Returns on success `true`, on any failure `false`


### Deinitialize the MCP2515

![Teststatus deinit](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

- Set the MCP2515 in uninitialized mode
- Ends the SPI-Bus

```c++
void MCP2515Module.deinit();
```



## Error-Handling

See also [Error.md](Error.md).

### Get the last MCP-Error

![Teststatus getLastMCPError](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
uint16_t MCP2515Module.getLastMCPError();
```
- Returns the last MCP-Error.
- The last MCP-Error will always been reset at the beginning of a Method.
- When no Error occured it will show `0x0000`



## MCP2515-Operationmodes

### Set Configuration-Mode

![Teststatus setConfigurationMode](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setConfigurationMode();
```
- Returns on success `true`, on any failure `false`


### Set Normal-Mode

![Teststatus setNormalMode](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setNormalMode();
```
- Returns on success `true`, on any failure `false`


### Set Sleep-Mode

![Teststatus setSleepMode](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setSleepMode();
```
- Returns on success `true`, on any failure `false`


### Set Listen-Only-Mode

![Teststatus setListenOnlyMode](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setListenOnlyMode();
```
- Returns on success `true`, on any failure `false`


### Set Loopback-Mode

![Teststatus setLoopbackMode](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setLoopbackMode();
```
- Returns on success `true`, on any failure `false`



## change Settings

### Change Recheck-Behavior

![Teststatus setReCheckEnabler](https://img.shields.io/badge/Teststatus-PASSED-green)

- When it's enabled, after Setting some bit or register it will check if the bit or register is set successfully.

```c++
bool MCP2515Module.setReCheckEnabler(const bool reCheckEnabler);
```
- `reCheckEnabler` - bool
- Returns on success `true`, on any failure `false`


### Changing the Baudrate

![Teststatus changeBaudRate](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.changeBaudRate(const uint32_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE);
```
- `targetBaudRate` - possible Baudrates = (5E3, 10E3, 20E3, 40E3, 50E3, 80E3, 100E3, 125E3, 200E3, 250E3, 500E3, 1000E3) => depending on ClockFrequency
- Returns on success `true`, on any failure `false`


### Enable/Disable an Interrupt

![Teststatus changeInterruptSetting](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.changeInterruptSetting(const bool value, const uint8_t InterruptBit);
```
- `value` - True = Enable Interrupt, False = Disable Interrupt
- `InterruptBit` - Interrupt to set
    - 7 = MERRE - Message Error Interrupt
    - 6 = WAKIE - Wake-up Interrupt
    - 5 = ERRIE - Error Interrupt (multiple sources in EFLG register)
    - 4 = TX2IE - Transmit Buffer 2 Empty Interrupt
    - 3 = TX1IE - Transmit Buffer 1 Empty Interrupt
    - 2 = TX0IE - Transmit Buffer 0 Empty Interrupt
    - 1 = RX1IE - Receive Buffer 1 Full Interrupt
    - 0 = RX0IE - Receive Buffer 0 Full Interrupt
- Returns on success `true`, on any failure `false`

## Filter/Mask Settings

### Enable the Filter/Mask

![Teststatus enableFilterMask](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.enableFilterMask(const uint8_t buffer = 0);
```
- `buffer` - Receive-Buffer-Number  (0 - 1)
- Returns on success `true`, on any failure `false`


### Disable the Filter/Mask

![Teststatus disableFilterMask](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.disableFilterMask(const uint8_t buffer = 0);
```
- `buffer` - Receive-Buffer-Number  (0 - 1)
- Returns on success `true`, on any failure `false`

### Set Filter

![Teststatus setFilter](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setFilter(const uint8_t FilterNumber, const uint32_t &ID, const bool Extended);
```
- `FilterNumber` - Number of the Filter (0-5)
- `ID` - ID to set
- `Extended` - `true` if Filter is applied only to extended frames and `false` if Filter is applied only to standard frames
- Returns on success `true`, on any failure `false`

### Set Mask

![Teststatus setMask](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.setMask(const uint8_t MaskNumber, const uint32_t &ID);
```
- `MaskNumber` - Number of the Mask (0-1)
- `ID` - ID to set
- Returns on success `true`, on any failure `false`

## Message-Handling

### Transmission

#### Check for free Transmit-Buffer

![Teststatus check4FreeTransmitBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
uint8_t MCP2515Module.check4FreeTransmitBuffer();
```
- Returns the Number of the free Transmit-Buffer (0 - 2)
    - if no Transmit-Buffer is free it will return `0xFF` 


#### Fill a Transmit-Buffer with Data

![Teststatus fillTransmitBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.fillTransmitBuffer(const uint8_t BufferNumber, const uint32_t ID, const bool Extended, const bool RTR, const uint8_t DLC, const uint8_t Data[8]);
```
- `BufferNumber` - Number of the Transmit-Buffer (0 - 2)
- `ID` - Message-ID
- `Extended` - `true` if it is an Extended Frame
- `RTR` - `true` if it is a Remote Transmit request Message
- `DLC` - Data Length Code of the Message
- `Data` - 8-Byte Data-Array (in total)
- Returns on success `true`, on any failure `false`


#### Send Message

![Teststatus sendMessage](https://img.shields.io/badge/Teststatus-PASSED-green)

- Initiates a transmission for the given TX-Buffer

```c++
bool MCP2515Module.sendMessage(const uint8_t BufferNumber, const uint8_t Priority = 0);
```
- `BufferNumber` - Number of the Transmit-Buffer (0 - 2)
- `Priority` - Message-Priority 0 (low) - 3 (high)
- Returns on success `true`, on any failure `false`


### Reception

#### Check for a RTR-Message

![Teststatus check4Rtr](https://img.shields.io/badge/Teststatus-PASSED-green)

- Check for the given Message-ID a RTR-Frame was received

```c++
bool MCP2515Module.check4Rtr(const uint32_t &ID, const bool &Extended);
```
- `ID` - Message-ID
- `Extended` - true if it is a Extended Frame
- Returns `true` if a RemoteTransmissionRequest for the Message was received, `false` when not (or on Error)


#### Check for a received Message

![Teststatus check4Receive](https://img.shields.io/badge/Teststatus-PASSED-green)

- Check for the given Message-ID if a Message was received.
- If a Message is received it would fill the given Databuffer.

```c++
bool MCP2515Module.check4Receive(const uint32_t &ID, const bool &Extended, const uint8_t &DLC, uint8_t (&DataBuffer)[8]);
```
- `ID` - Message-ID
- `Extended` - `true` if it is a Extended Frame
- `DLC` - Data-Length-Code of the Message
- `DataBuffer` - Reference to the DataBuffer of the Message
- Returns `true` if a Message was received, `false` when not (or on Error)


#### Get Message-ID from Receive-Buffer

![Teststatus getIdFromReceiveBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
bool MCP2515Module.getIdFromReceiveBuffer(const uint8_t BufferNumber, uint32_t &ID);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- `ID` - Reference where to store the ID of the Message
- Returns `true` if on success, on any failure `false`


#### Get DLC from Receive-Buffer

![Teststatus getDlcFromReceiveBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
uint8_t MCP2515Module.getDlcFromReceiveBuffer(const uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns the Message-DLC that is stored in the given Receive-Buffer
    - on Error it returns 0


#### Get Frame from Receive-Buffer

![Teststatus getFrameFromReceiveBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
uint8_t MCP2515Module.getFrameFromReceiveBuffer(const uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns the Frame of the Message that is stored in the given Receive-Buffer
    - 0 = Standard Frame (return also on error)
    - 1 = Extended Frame


#### Get RTR from Receive-Buffer

![Teststatus getRtrFromReceiveBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
uint8_t MCP2515Module.getRtrFromReceiveBuffer(const uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns if the Message that is stored in the given Receive-Buffer is a Remote-Transmission-Request
    - 0 = Message is not a Remote-Transmission-Request (return also on error)
    - 1 = Message is a Remote-Transmission-Request


#### Get Data from Receive-Buffer

![Teststatus getDataFromReceiveBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

- It would fill the given Databuffer with the Databytes from the given Receive-Buffer

```c++
bool MCP2515Module.getDataFromReceiveBuffer(const uint8_t BufferNumber, const uint8_t DLC, uint8_t (&DataBuffer)[8]);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- `DLC` - Expected Data Length Code
- `DataBuffer` - Reference to the DataBuffer
- Returns on success `true`, on any failure `false`


#### Get all Data from Receive-Buffer

![Teststatus getAllFromReceiveBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

- It would fill the given References with the Data from the given Receive-Buffer

```c++
bool MCP2515Module.getAllFromReceiveBuffer(const uint8_t BufferNumber, uint32_t (&ID), bool (&Frame), bool (&RTR), uint8_t (&DLC),uint8_t (&DataBuffer)[8]);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- `ID` - Reference to the Message-ID Variable
- `Frame` - Reference to the Frame Variable
- `RTR` - Reference to the RTR Variable
- `DLC` - Reference to the DLC Variable
- `DataBuffer` - Reference to the DataBuffer
- Returns on success `true`, on any failure `false`


#### Release Receive-Buffer

![Teststatus releaseReceiveBuffer](https://img.shields.io/badge/Teststatus-PASSED-green)

- It would release the given Receive-Buffer for the next Message

```c++
bool MCP2515Module.releaseReceiveBuffer(const uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns on success `true`, on any failure `false`



## Interrupt/Flag-Handling

### Check all Interrupt-Flags

![Teststatus check4InterruptFlags](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
uint8_t MCP2515Module.check4InterruptFlags();
```
- Returns the Flags they are set in the MCP2515
    - Bit 7 -> MERRF - Message Error Interrupt Flag bit
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending
    - Bit 6 -> WAKIF - Wake-up Interrupt Flag bit
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending
    - Bit 5 -> ERRIF - Error Interrupt Flag bit (multiple sources in EFLG register)
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending
    - Bit 4 -> TX2IF - Transmit Buffer 2 Empty Interrupt Flag bit
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending
    - Bit 3 -> TX1IF - Transmit Buffer 1 Empty Interrupt Flag bit
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending
    - Bit 2 -> TX0IF - Transmit Buffer 0 Empty Interrupt Flag bit
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending
    - Bit 1 -> RX1IF - Receive Buffer 1 Full Interrupt Flag bit
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending
    - Bit 0 -> RX0IF - Receive Buffer 0 Full Interrupt Flag bit
        - 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
        - 0 = No interrupt is pending


### Reset Interrupt-Flags

![Teststatus resetInterruptFlag](https://img.shields.io/badge/Teststatus-PASSED-green)

- Resets the given Interrupt-Flag (only)

```c++
bool MCP2515Module.resetInterruptFlag(const uint8_t Flag);
```
- `Flag` - Interrupt-Flag to reset
    - 7 = MERRF - Message Error Interrupt Flag
    - 6 = WAKIF - Wake-up Interrupt Flag
    - 5 = ERRIF - Error Interrupt Flag
    - 4 = TX2IF - Transmit Buffer 2 Empty Interrupt Flag
    - 3 = TX1IF - Transmit Buffer 1 Empty Interrupt Flag
    - 2 = TX0IF - Transmit Buffer 0 Empty Interrupt Flag
    - 1 = RX1IF - Receive Buffer 1 Full Interrupt Flag
    - 0 = RX0IF - Receive Buffer 0 Full Interrupt Flag
- Returns on success `true`, on any failure `false`

### Set Interrupt-Flags

![Teststatus setInterruptFlag](https://img.shields.io/badge/Teststatus-PASSED-green)

- Sets the given Interrupt-Flag (only)

```c++
bool MCP2515Module.setInterruptFlag(const uint8_t Flag);
```
- `Flag` - Interrupt-Flag to set
    - 7 = MERRF - Message Error Interrupt Flag
    - 6 = WAKIF - Wake-up Interrupt Flag
    - 5 = ERRIF - Error Interrupt Flag
    - 4 = TX2IF - Transmit Buffer 2 Empty Interrupt Flag
    - 3 = TX1IF - Transmit Buffer 1 Empty Interrupt Flag
    - 2 = TX0IF - Transmit Buffer 0 Empty Interrupt Flag
    - 1 = RX1IF - Receive Buffer 1 Full Interrupt Flag
    - 0 = RX0IF - Receive Buffer 0 Full Interrupt Flag
- Returns on success `true`, on any failure `false`

## Get Settings

### CS-Pin

![Teststatus getCsPin](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
uint8_t MCP2515Module.getCsPin();
```
- Returns the choosen CS-Pin

### SPI-Mode

![Teststatus getSpiMode](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
uint8_t MCP2515Module.getSpiMode();
```
- Returns the SPI-Mode
    - 0 = SPI-Mode 0
    - 4 = SPI-Mode 1
    - 8 = SPI-Mode 2
    - 12 = SPI-Mode 3

### Data-Order

![Teststatus getDataOrder](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
uint8_t MCP2515Module.getDataOrder();
```
- Returns the DataOrder of the SPI-Communication
    - 0 = LSB-First
    - 1 = MSB-First
- Should always be MSB-First

### SPI-Frequency

![Teststatus getFrequency](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
uint32_t MCP2515Module.getFrequency();
```
- Returns the SPI-Frequency

### MCP2515-ClockFrequency

![Teststatus getClockFrequency](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
uint32_t MCP2515Module.getClockFrequency();
```
- Returns the ClockFrequency of the MCP2515

### Operation Mode

![Teststatus getOperationMode](https://img.shields.io/badge/Teststatus-PASSED-green)

```c++
MCP2515OperationMode MCP2515Module.getOperationMode();
```
- Returns the actual Operation Mode of the MCP2515
    - Normal (uint8_t 0)
    - Sleep (uint8_t 1)
    - Loopback (uint8_t 2)
    - Listen-Only (uint8_t 3)
    - Configuration (uint8_t 4)

### Baudrate

![Teststatus getBaudRate](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
uint32_t MCP2515Module.getBaudRate();
```
- Returns the Baudrate of the MCP2515

### Initialisation-Status

![Teststatus getIsInitialized](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
bool MCP2515Module.getIsInitialized();
```
- Returns the initialisation-Status of the MCP2515
    - `true` if MCP2515 is initialized
    - `false` if MCP2515 is not initialized

### ReCheck-Status

![Teststatus getReCheckEnabled](https://img.shields.io/badge/Teststatus-NOT_TESTED-gray)

```c++
bool MCP2515Module.getReCheckEnabled();
```
- Returns the ReCheck-Status
    - `true` if ReCheck is enabled
    - `false` if ReCheck is disabled
