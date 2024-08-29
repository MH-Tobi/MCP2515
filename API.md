# API

## Constructor

```c++
MCP2515 MCP2515Module;
```

## Settings (only before initialisation)

### Set the CS (ChipSelect) Pin that is used for the SPI-Communication

```c++
MCP2515Module.setSpiPins(uint8_t cs = 0);
```
- `cs` - ChipSelect-Pin
- Returns on success `true`, on any failure `false`
- This Method is required before initialization (Initialization will fail without setting)


### Set the SPI-Mode

```c++
MCP2515Module.setSpiMode(uint8_t SpiMode = MCP2515_SPI_MODE);
```
- `SpiMode` - Allowed Values for the MCP2515 are SPI_MODE0 and SPI_MODE3.
    - Default is `SPI_MODE0`
- Returns on success `true`, on any failure `false`


### Set the DataOrder

```c++
MCP2515Module.setDataOrder(uint8_t DataOrder = MCP2515_SPI_DATA_ORDER);
```
- `DataOrder` - DataOrder Allowed Value for the MCP2515 is `MSBFIRST`.
- Returns on success `true`, on any failure `false`


### Set the SPI-Clock-Frequency

```c++
MCP2515Module.setSpiFrequency(uint64_t frequency = MCP2515_MAX_SPI_SPEED);
```
- `frequency` - max. allowed Value for the MCP2515 is 10e6 Hz.
    - Default is 10e6
- Returns on success `true`, on any failure `false`


### Set the MCP2515 Clock-Frequency

```c++
MCP2515Module.setClockFrequency(uint64_t clockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);
```
- `clockFrequency` - max. allowed Value for the MCP2515 is 40e6 Hz.
    - Default is 8e6
- Returns on success `true`, on any failure `false`



## De-/Initialisation of the MCP2515

### Initialize the MCP2515

- Reset the MCP2515
- Set the BitTiming for the given BaudRate
- Setting the PinControl for RX-Pins and TX-Pins
- Enable Receive-Interrupts
- Disable Mask and Filter-Settings
- Set the OperationMode Normal

```c++
MCP2515Module.init(uint64_t BaudRate = MCP2515_DEFAULT_BAUDRATE, bool reCheckEnabled = true);
```
- `BaudRate` - Baudrate of the CAN-Communication
    - Default is `500e3`
- `reCheckEnabled` - true if after Set-Processes a recheck of setted Values has to be execute
    - Default is `true`
- Returns on success `true`, on any failure `false`


### Deinitialize the MCP2515

```c++
MCP2515Module.deinit();
```



## Error-Handling

See also [Error.md](Error.md).

### Get the last MCP-Error

```c++
MCP2515Module.getLastMCPError();
```
- Returns the last MCP-Error.
- The last MCP-Error will always been reset at the beginning of a Method.
- When no Error occured it will show `0x0000`


### Get the last SPI-Error

```c++
MCP2515Module.getLastSPIError();
```
- Returns the last SPI-Error.
- The last SPI-Error will always been reset at the beginning of an SPI-Instruction.
- When no Error occured it will show `0x0000`



## MCP2515-Operationmodes

### Set Configuration-Mode

```c++
MCP2515Module.setConfigurationMode();
```
- Returns on success `true`, on any failure `false`


### Set Normal-Mode

```c++
MCP2515Module.setNormalMode();
```
- Returns on success `true`, on any failure `false`


### Set Sleep-Mode

```c++
MCP2515Module.setSleepMode();
```
- Returns on success `true`, on any failure `false`


### Set Listen-Only-Mode

```c++
MCP2515Module.setListenOnlyMode();
```
- Returns on success `true`, on any failure `false`


### Set Loopback-Mode

```c++
MCP2515Module.setLoopbackMode();
```
- Returns on success `true`, on any failure `false`



## change Settings

### Change Recheck-Behavior

- When it's enabled, after Setting some bit it will check if the bit is set successfully.

```c++
MCP2515Module.setReCheckEnabler(bool reCheckEnabler);
```
- `reCheckEnabler` - bool
- Returns on success `true`, on any failure `false`


### Changing the Baudrate

```c++
MCP2515Module.changeBaudRate(uint64_t targetBaudRate = MCP2515_DEFAULT_BAUDRATE);
```
- `targetBaudRate` - possible Baudrates = (5E3, 10E3, 20E3, 40E3, 50E3, 80E3, 100E3, 125E3, 200E3, 250E3, 500E3, 1000E3) => depending on ClockFrequency
- Returns on success `true`, on any failure `false`


### Changing the ClockFrequency

```c++
MCP2515Module.changeClockFrequency(uint64_t targetClockFrequency = MCP2515_DEFAULT_CLOCK_FREQUENCY);
```
- `targetClockFrequency` - possible ClockFrequencys= (8E6, 16E6, 25E6, 40E6)
- Returns on success `true`, on any failure `false`


### Enable/Disable an Interrupt

```c++
MCP2515Module.changeInterruptSetting(bool value, uint8_t InterruptBit);
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


### Enable the Filter/Mask

```c++
MCP2515Module.enableFilterMask(uint8_t buffer = 0);
```
- `buffer` - Receive-Buffer-Number  (0 - 1)
- Returns on success `true`, on any failure `false`


### Disable the Filter/Mask

```c++
MCP2515Module.disableFilterMask(uint8_t buffer = 0);
```
- `buffer` - Receive-Buffer-Number  (0 - 1)
- Returns on success `true`, on any failure `false`


## Message-Handling

### Transmission

#### Check for free Transmit-Buffer

```c++
MCP2515Module.check4FreeTransmitBuffer();
```
- Returns the Number of the free Transmit-Buffer (0 - 2)
    - if no Transmit-Buffer is free it will return `0xFF` 


#### Fill a Transmit-Buffer with Data

```c++
MCP2515Module.fillTransmitBuffer(uint8_t BufferNumber, uint32_t ID, bool Extended, bool RTR, uint8_t DLC, uint8_t Data[8]);
```
- `BufferNumber` - Number of the Transmit-Buffer (0 - 2)
- `ID` - Message-ID
- `Extended` - true if it is a Extended Frame
- `RTR` - true if it is a Remote Transmit request Message
- `DLC` - Data Length Code of the Message
- `Data` - 8-Byte Data-Array (in total)
- Returns on success `true`, on any failure `false`


#### Send Message

- Initiates a transmission for the given TX-Buffer

```c++
MCP2515Module.sendMessage(uint8_t BufferNumber, uint8_t Priority = 0);
```
- `BufferNumber` - Number of the Transmit-Buffer (0 - 2)
- `Priority` - Message-Priority 0 - 3
- Returns on success `true`, on any failure `false`


### Reception

#### Check for a RTR-Message

- Check for the given Message-ID a RTR-Frame was received

```c++
MCP2515Module.check4Rtr(uint32_t ID, bool Extended);
```
- `ID` - Message-ID
- `Extended` - true if it is a Extended Frame
- Returns `true` if a RemoteTransmissionRequest for the Message was received, `false` when not


#### Check for a received Message

- Check for the given Message-ID if a Message was received.
- If a Message is received it would fill the given Databuffer.

```c++
MCP2515Module.check4Receive(uint32_t ID, bool Extended, uint8_t DLC, uint8_t (&DataBuffer)[8]);
```
- `ID` - Message-ID
- `Extended` - true if it is a Extended Frame
- `DLC` - Data-Length-Code of the Message
- `DataBuffer` - Address-Pointer to the DataBuffer of the Message
- Returns `true` if a Message was received, `false` when not


#### Get Message-ID from Receive-Buffer

```c++
MCP2515Module.getIdFromReceiveBuffer(uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns the Message-ID that is stored in the given Receive-Buffer
    - on Error it returns 0


#### Get DLC from Receive-Buffer

```c++
MCP2515Module.getDlcFromReceiveBuffer(uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns the Message-DLC that is stored in the given Receive-Buffer
    - on Error it returns 0


#### Get Frame from Receive-Buffer

```c++
MCP2515Module.getFrameFromReceiveBuffer(uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns the Frame of the Message that is stored in the given Receive-Buffer
    - 0 = Standard Frame (return also on error)
    - 1 = Extended Frame


#### Get RTR from Receive-Buffer

```c++
MCP2515Module.getRtrFromReceiveBuffer(uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns if the Message that is stored in the given Receive-Buffer is a Remote-Transmission-Request
    - 0 = Message is not a Remote-Transmission-Request (return also on error)
    - 1 = Message is a Remote-Transmission-Request


#### Get Data from Receive-Buffer

- It would fill the given Databuffer with the Databytes from the given Receive-Buffer

```c++
MCP2515Module.getDataFromReceiveBuffer(uint8_t BufferNumber, uint8_t DLC, uint8_t (&DataBuffer)[8]);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- `DLC` - Expected Data Length Code
- `DataBuffer` - Address-Pointer to the DataBuffer
- Returns on success `true`, on any failure `false`


#### Get all Data from Receive-Buffer

- It would fill the given Address-Pointer with the Data from the given Receive-Buffer

```c++
MCP2515Module.getAllFromReceiveBuffer(uint8_t BufferNumber, uint32_t (&ID), bool (&Frame), bool (&RTR), uint8_t (&DLC),uint8_t (&DataBuffer)[8]);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- `ID` - Address-Pointer to the Message-ID Variable
- `Frame` - Address-Pointer to the Frame Variable
- `RTR` - Address-Pointer to the RTR Variable
- `DLC` - Address-Pointer to the DLC Variable
- `DataBuffer` - Address-Pointer to the DataBuffer
- Returns on success `true`, on any failure `false`


#### Release Receive-Buffer

- It would release the given Receive-Buffer for the next Message

```c++
MCP2515Module.releaseReceiveBuffer(uint8_t BufferNumber);
```
- `BufferNumber` - Number of the Receive-Buffer (0 - 1)
- Returns on success `true`, on any failure `false`



## Interrupt/Flag-Handling

### Check all Interrupt-Flags

```c++
MCP2515Module.check4InterruptFlags();
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

- Resets the given Interrupt-Flag

```c++
MCP2515Module.resetInterruptFlag(uint8_t Flag);
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
