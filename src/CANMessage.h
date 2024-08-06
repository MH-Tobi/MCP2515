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

#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include <Arduino.h>
#include <MCP2515.h>


#define CANMESSAGE_DIRECTION_RECEIVE	0
#define CANMESSAGE_DIRECTION_TRANSMIT	1

#define CANMESSAGE_FRAME_STANDARD   	0
#define CANMESSAGE_FRAME_EXTENDED   	1

/**
 * @brief Klassengrundgeruest
 *
 */
class CANMessage
{
	private:
		uint32_t _ID;               // Message-ID (11 bit for Standard-Frame; 29 bit for Extended Frame)
        uint8_t _DLC;               // Datallength 0-8
		bool _RTR;                  // Remote Transmission Request
		uint8_t _Frame;             // Standard-Frame = 0; Extended-Frame = 1
        uint8_t _Direction;         // Message-Direction (0 = Receive; 1 = Transmit)
        MCP2515 _Controller;        // MCP2515-Controller Instance
		uint8_t _DataByte[8];       // Buffer for the sending or receiving Data
        int8_t _DataBufferIndex;    // Index of the actual readed Buffer
        bool _BufferFilled[8];      // Shows if a Buffer is filled with Data

	public:

        bool init(uint32_t id, uint8_t dlc, bool rtr, uint8_t frame, uint32_t direction, MCP2515 controller);

		CANMessage();
		~CANMessage();

        // For Transmit-Messages
        bool send();
        bool addDataByte(uint8_t Data, uint8_t BufferNumber = 0);
        void releaseBuffer(uint8_t BufferNumber = 0);
        bool checkForRTR();

        // for Receive-Messages
        bool checkReceive();
        uint8_t getDataByte();
        bool dataAvailable();

};

#endif
