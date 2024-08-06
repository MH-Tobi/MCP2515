#include "CANMessage.h"

/**
 * @brief Klassenkonstruktor
 *
 */
CANMessage::CANMessage() :
    _ID(0),
    _DLC(0),
    _RTR(false),
    _Frame(0),
    _Direction(0),
    _Controller(),
    _DataBufferIndex(-1)
{
}

/**
 * @brief Dekonstruktor
 *
 */
CANMessage::~CANMessage()
{
}

/**
 * @brief Initialisation of a Message.
 *
 * @param id Message ID (11-Bit for Standard-Frames, 29-Bit for Extended-Frames)
 * @param dlc Data-Length-Code (0 - 8 bytes)
 * @param rtr Remote-Transmission-Request (only allowed when Message-Direction is a transmit)
 * @param frame CAN-Frame (0 = Standard-Frame; 1 = Extended-Frame)
 * @param direction Message-Direction (0 = Recieve; 1 = Transmit)
 * @param controller Object of a MCP2515 Instance
 *
 * @return
 * True when initialisation is successfull, False when not.
 */
bool CANMessage::init(uint32_t id, uint8_t dlc, bool rtr, uint8_t frame, uint32_t direction, MCP2515 controller){

    // Check if Frame is plausible
    if (frame != CANMESSAGE_FRAME_STANDARD && frame != CANMESSAGE_FRAME_EXTENDED)
    {
        return false;
    }

    // Check if Direction is plausible
    if (direction != CANMESSAGE_DIRECTION_RECEIVE && direction != CANMESSAGE_DIRECTION_TRANSMIT)
    {
        return false;
    }

    // Check if ID is in a plausible range
    if (id <= 0x1FFFFFFF)
    {
        // Check if ID and Frame is extended (only in case the Frame is defined as Standard)
        if (frame != CANMESSAGE_FRAME_EXTENDED && id > 0x7FF)
        {
            return false;
        }
    } else {
        return false;
    }

    // Check if rtr is enabled only for the message-direction Transmit
    if (rtr && direction != CANMESSAGE_DIRECTION_TRANSMIT)
    {
        return false;
    }

    // Check if DLC is plausible
    if (dlc > 8)
    {
        return false;
    }

    _ID = id;
    _DLC = dlc;
    _RTR = rtr;
    _Frame = frame;
    _Direction = direction;
    _Controller = controller;
    return true;
}

/**
 * @brief Add Data to the defined DataBuffer.
 * Fails when Buffer is already filled (release a buffer with releaseBuffer()-Method)
 *
 * @param Data Data to be filled in the Buffer
 * @param BufferNumber Number of the Buffer (0 - 7)
 *
 * @return
 * True when Buffer is filled with the data, False when data is not filled in the Buffer.
 */
bool CANMessage::addDataByte(uint8_t Data, uint8_t BufferNumber){
    // Check if Message is a sending Message.
    if (_Direction != CANMESSAGE_DIRECTION_TRANSMIT)
    {
        return false;
    }

    // Check if BufferNumber is plausible
    if (BufferNumber>7)
    {
        return false;
    }

    // Check if BufferNumber is in the DLC-Range
    if (BufferNumber >= _DLC)
    {
        return false;
    }

    // Check if choosen Buffer is already filled
    if (_BufferFilled[BufferNumber] == true)
    {
        return false;
    }

    // Fill Buffer with Data
    _DataByte[BufferNumber] = Data;
    _BufferFilled[BufferNumber] = true;

    return true;
}

/**
 * @brief Initias the transmitting-Request.
 * Fails when Message is not complete (all relevant Buffer are filled with data).
 * Method waits till a Transmit-Buffer is free (Endless-Loop possible).
 *
 * @todo eliminate endless-loop
 *
 * @return
 * True when Data was received, False when no Data was received.
 */
bool CANMessage::send(){

    int8_t Buffer = -1;
    int8_t counter = 100;

    // Check if Message is a sending Message.
    if (_Direction != CANMESSAGE_DIRECTION_TRANSMIT)
    {
        return false;
    }

    // Check if all Buffers are filled with data (when not RTR).
    if (!_RTR)
    {
        for (size_t i = 0; i < _DLC; i++)
        {
            if (_BufferFilled[i] != true)
            {
                return false;
            }
        }
    }

    // Check for free Transmit-Buffer
    while ((_Controller.readStatusInstruction() & 0x54) != 0 && counter !=0)
    {
        delayMicroseconds(10);
        counter--;
    }

    if ((_Controller.readStatusInstruction() & 0x54) != 0){
        return false;
    }

    // Get the free Buffer
    for (size_t i = 0; i < 3; i++)
    {
        if ((_Controller.readStatusInstruction() & (0x04 << (2 * i))) == 0 )
        {
            Buffer = i;
            break;
        }
    }

    // Check if free Buffer was found
    if (Buffer == -1)
    {
        return false;
    }

    // Fill ID-Register
    if (_Frame == CANMESSAGE_FRAME_EXTENDED) {
        // In case of an extended Frame
        _Controller.writeInstruction(REG_TXBnSIDH(Buffer), _ID >> 21);
        _Controller.writeInstruction(REG_TXBnSIDL(Buffer), (((_ID >> 18) & 0x07) << 5) | 0x08 | ((_ID >> 16) & 0x03));
        _Controller.writeInstruction(REG_TXBnEID8(Buffer), (_ID >> 8) & 0xff);
        _Controller.writeInstruction(REG_TXBnEID0(Buffer), _ID & 0xff);
    } else {
        // In case of a standard Frame
        _Controller.writeInstruction(REG_TXBnSIDH(Buffer), _ID >> 3);
        _Controller.writeInstruction(REG_TXBnSIDL(Buffer), _ID << 5);
        _Controller.writeInstruction(REG_TXBnEID8(Buffer), 0x00);
        _Controller.writeInstruction(REG_TXBnEID0(Buffer), 0x00);
    }

    // Check if Message is a Remote Frame
    if (_RTR) {
        // In this case no Data will be send
        _Controller.writeInstruction(REG_TXBnDLC(Buffer), TXBnDLC_BIT_RTR | _DLC);
    } else {
        // Set the DataLengthCode
        _Controller.writeInstruction(REG_TXBnDLC(Buffer), _DLC);

        // Fill DataBuffer
        for (int i = 0; i < _DLC; i++) {
            _Controller.writeInstruction(REG_TXBnD0(Buffer) + i, _DataByte[i]);
        }
    }

    // Set the Message Transmit Request bit
    _Controller.writeInstruction(REG_TXBnCTRL(Buffer), TXBnCTRL_BIT_TXREQ);

    bool aborted = false;

    // While the Message Transmit Request bit is set
    while (_Controller.readInstruction(REG_TXBnCTRL(Buffer)) & TXBnCTRL_BIT_TXREQ) {
        // Check if an error occures during the transmission
        if (_Controller.readInstruction(REG_TXBnCTRL(Buffer)) & TXBnCTRL_BIT_TXERR) {
            // set aborted to true
            aborted = true;

            // In case of an error during the transmission,
            // set the "Abort All Pending Transmissions"-bit
            _Controller.bitModifyInstruction(REG_CANCTRL, CANCTRL_BIT_ABAT, CANCTRL_BIT_ABAT);

            // In case of "Abort All Pending Transmissions" the while-loop
            // ended automaticaly when from the MCP2515 the Transmit Request bit is resetted.
        }

        yield();
    }

    if (aborted) {
        // when the abortion is successfull reset the "Abort All Pending Transmissions"-bit
        _Controller.bitModifyInstruction(REG_CANCTRL, CANCTRL_BIT_ABAT, 0x00);
    }

    // Reset the Transmit Buffer Empty Interrupt Flag bit
    _Controller.bitModifyInstruction(REG_CANINTF, CANINTF_BIT_TXnIF(Buffer), 0x00);

    // Check if the transmission was successfull (no "Message Aborted Flag bit", "Message Lost Arbitration bit" and "Transmission Error Detected bit")
    if ((_Controller.readInstruction(REG_TXBnCTRL(Buffer)) & (TXBnCTRL_BIT_ABTF | TXBnCTRL_BIT_MLOA | TXBnCTRL_BIT_TXERR)) == 0x00)
    {
        // clear the local Buffer and
        for (size_t i = 0; i < 8; i++)
        {
            _BufferFilled[i] = false;
        }

        // return true
        return true;
    } else {

        // Return false and keep all Data in the local Buffer
        return false;
    }
}

/**
 * @brief Release the defined Buffer.
 *
 * @param BufferNumber Value 0 - 7
 *
 */
void CANMessage::releaseBuffer(uint8_t BufferNumber)
{
    // Check if Message is a sending Message.
    if (_Direction == CANMESSAGE_DIRECTION_TRANSMIT && BufferNumber < 8)
    {
        _BufferFilled[BufferNumber] = false;
    }
}

/**
 * @brief Check if a RemoteTransmissionRequest for the Message was received.
 *
 * @return
 * True if a RemoteTransmissionRequest for the Message was received, False when not.
 *
 */
bool CANMessage::checkForRTR()
{
    // Check if Message is a sending Message.
    if (_Direction != CANMESSAGE_DIRECTION_TRANSMIT)
    {
        return false;
    }

    for (size_t i = 0; i < 2; i++)
    {
        // Check if something is in RXBn of the MCP2515-Controller
        if ((_Controller.rxStatusInstruction() & (0x40 + i * 0x40)) != 0)
        {
            // Check if RTR-Bit is Set
            if ((_Controller.readInstruction(REG_RXBnCTRL(i)) & RXBnCTRL_BIT_RXRTR) != RXBnCTRL_BIT_RXRTR)
            {
                continue;
            }

            // Check if Frame is Extended
            if ((_Controller.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
            {
                // Frame is Extended
                // Check if CANMessage-Object is Extended Frame
                if (_Frame != CANMESSAGE_FRAME_EXTENDED)
                {
                    continue;
                }
            }else{
                // Frame is Standard
                // Check if CANMessage-Object is Extended Frame
                if (_Frame != CANMESSAGE_FRAME_STANDARD)
                {
                    continue;
                }
            }

            // Get ID
            uint32_t ID = ((_Controller.readInstruction(REG_RXBnSIDH(i)) << 3) & 0x07f8) |
                        ((_Controller.readInstruction(REG_RXBnSIDL(i)) >> 5) & 0x07);

            if ((_Controller.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
            {
                ID = ((ID << 18) & 0x1FFC0000) | ((((_Controller.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_EID) << 8) << 8) & 0x30000) | ((_Controller.readInstruction(REG_RXBnEID8(i)) << 8) & 0xff00) | _Controller.readInstruction(REG_RXBnEID0(i));
            }

            // Check if ID in RXB0 is correct
            if (ID != _ID)
            {
                continue;
            }

            // Release Receiving-Buffer
            _Controller.bitModifyInstruction(REG_CANINTF, CANINTF_BIT_RXnIF(i), 0x00);

            return true;
        }
    }
    return false;
}


/**
 * @brief Check if in the CAN-Module RX-Buffer is this Message.
 * If this Message is received the Data is send to the local Buffer, so the Buffer of the CAN-Module can be released for the next incomming messages.
 *
 * @return
 * True when Data was received, False when no Data was received.
 */
bool CANMessage::checkReceive()
{
    // Check if Message is a receiving Message.
    if (_Direction != CANMESSAGE_DIRECTION_RECEIVE)
    {
        return false;
    }

    // Check if Message is already checked
    if (_DataBufferIndex != -1)
    {
        return false;
    }

    for (size_t i = 0; i < 2; i++)
    {
        // Check if something is in RXBn of the MCP2515-Controller
        if ((_Controller.rxStatusInstruction() & (0x40 + i * 0x40)) != 0)
        {
            // Check if Frame is Extended
            if ((_Controller.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
            {
                // Frame is Extended
                // Check if CANMessage-Object is Extended Frame
                if (_Frame != CANMESSAGE_FRAME_EXTENDED)
                {
                    continue;
                }
            }else{
                // Frame is Standard
                // Check if CANMessage-Object is Extended Frame
                if (_Frame != CANMESSAGE_FRAME_STANDARD)
                {
                    continue;
                }
            }

            // Get ID
            uint32_t ID = ((_Controller.readInstruction(REG_RXBnSIDH(i)) << 3) & 0x07f8) |
                        ((_Controller.readInstruction(REG_RXBnSIDL(i)) >> 5) & 0x07);

            if ((_Controller.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
            {
                ID = ((ID << 18) & 0x1FFC0000) | ((((_Controller.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_EID) << 8) << 8) & 0x30000) | ((_Controller.readInstruction(REG_RXBnEID8(i)) << 8) & 0xff00) | _Controller.readInstruction(REG_RXBnEID0(i));
            }

            // Check if ID in RXB0 is correct
            if (ID != _ID)
            {
                continue;
            }

            // Check if DLC is correct
            if ((_Controller.readInstruction(REG_RXBnDLC(i)) & RXBnDLC_BIT_DLC) != _DLC)
            {
                continue;
            }

            _DataBufferIndex = 0;

            // Collect Data
            for (size_t m = 0; m < _DLC; m++)
            {
                _DataByte[m] = _Controller.readInstruction(REG_RXBnD0(i) + m);
            }

            // Release Buffer
            _Controller.bitModifyInstruction(REG_CANINTF, CANINTF_BIT_RXnIF(i), 0x00);

            return true;
        }
    }
    return false;
}

/**
 * @brief Get the highest available DataByte from the Buffer.
 * Check if Data is available with the dataAvailable()-Method.
 *
 * @return
 * The highest available DataByte.
 */
uint8_t CANMessage::getDataByte()
{
    // Check if Message is a receiving Message.
    if (_Direction != CANMESSAGE_DIRECTION_RECEIVE)
    {
        return 0;
    }

    // Check if Message is checked
    if (_DataBufferIndex == -1)
    {
        return 0;
    }

    uint8_t Data = _DataByte[_DataBufferIndex];

    if (_DataBufferIndex == (_DLC - 1))
    {
        _DataBufferIndex = -1;
    } else {
        _DataBufferIndex++;
    }

    return Data;
}

/**
 * @brief Checks if Data is available in the Buffer.
 *
 * @return
 * True when Data is available, False when no Data is available.
 */
bool CANMessage::dataAvailable()
{
    // Check if Message is a receiving Message.
    if (_Direction != CANMESSAGE_DIRECTION_RECEIVE)
    {
        return false;
    }

    if (_DataBufferIndex == -1)
    {
        return false;
    }
    return true;
}
