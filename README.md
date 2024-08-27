# MCP2515
Library to control the MCP2515-CAN-Module.

Not really tested yet.


## Setup

1. Import Librarys

```c++
#include <Arduino.h>
#include <MCP2515.h>
```

2. Create an Instance:

```c++
MCP2515 MCP2515Module;
```

3. Set the Chip-Select-Pin (actually required)

```c++
MCP2515Module.setSpiPins(CS_Pin);
```

4. Initialize the Module

```c++
MCP2515Module.init(500E3);
```

 - Here with 500kB


## Usage

After initialization you can write CAN-Messages like:

```c++
uint32_t ID = 0x123;
bool Extended_Frame = false;
bool RTR_Message = false;
uint8_t DLC = 4;
uint8_t Data[DLC] = {
    0xAB,
    0x2C,
    0x10,
    0cFF
}

uint8_t BufferNumber = MCP2515Module.check4FreeTransmitBuffer();

MCP2515Module.fillTransmitBuffer(BufferNumber, ID, Extended_Frame, RTR_Message, DLC, Data);

MCP2515Module.sendMessage(BufferNumber);
```


Also you can read Messages like:

```c++
uint32_t ID;
uint8_t DLC;
bool RTR;
bool Frame;
uint8_t Data[8];

// Checks for any received Message in the RX-Buffer
uint8_t Receive = (MCP2515Module.check4InterruptFlags() & 0x03);

if ((Receive & 0x01) == 0x01)   // Message in RX-Buffer 0
{
    if (!MCP2515Module.getAllFromReceiveBuffer(0, ID, Frame1, RTR, DLC, Data))
    {
        ID = 0;
    }
}
if ((Receive & 0x02) == 0x02)   // Message in RX-Buffer 1
{
    if (!MCP2515Module.getAllFromReceiveBuffer(1, ID, Frame, RTR, DLC, Data))
    {
        ID = 0;
    }
}
```



> For easier Message-Handling use the [CANMessage-Library](https://github.com/MH-Tobi/CANMessage).

## Examples
See [examples](examples) folder.

## API
See [API.md](API.md).
