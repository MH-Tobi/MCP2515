/*
    CAN Transmit

    This example sends messages (received by the example CAN_Receive) with specific Counter.


    Setup:
    First, you need to correctly connect your MCP2515 module to your Arduino board.
    The connection to the CAN bus must also be correct.
    You also need to check whether you need the terminating resistor for your MCP2515 module.

    For SPI communication to work, you need to adjust the CS pin in this script.
    The CS pin is important because the Arduino uses this pin to signal to the MCP2515 that it wants to communicate with it.

    The other pins used for SPI communication can be found in your Arduino board's datasheet.
    These do not need to be changed in the script, but they must still be correct.

    In addition to the CS pin, the clock rate of the MCP2515 module must be defined.
    This is fixed for each MCP2515 module, as it is (usually) a permanently soldered oscillator.

    You can of course also adjust the baudrate for serial communication to suit your needs.

    Since this script expects the messages from the CAN_Transmit example, all other settings such as CAN baud rate and
    the definition of the messages should not be changed.


    Procedure in setup function:
    At the beginning of the script, the Serial-Communication is established and
    the built-in LED of the Arduino board is configured so that in case of errors,
    not only a serial output is given, but also a blinking output.

    After setting the CS pin, the MCP2515 clock rate and the SPI frequency, the MCP2515 module is initialized.

    Since we only want to monitor the CAN communication, the MCP2515 is put into Listen-Only operating mode.

    During setup, if an error occurs, the corresponding error code is output via serial and
    a specific number of LED flashes are displayed.
    The failed process step is then re-executed. This continues until the process step is successfully completed.


    Procedure in loop function:
    The message data is populated during each loop iteration.

    However, this only happens if a certain time has elapsed (Message_1 every 100 ms, Message_2 every 1000 ms).

    Once the data for a message has been generated, the message is marked as populated,
    prepared for transmission in the next possible loop iteration, and sent.


    Created 05.12.2025
    By MH-Tobi
*/

#include <MCP2515.h>

// Create Instances of the CAN-Controller
MCP2515 MCP2515Module;

// Definition of Chip-Select-Pin for the SPI-Communication
// Choose the Pin of the Arduino (or similar) which is connected with the CS-Pin of the MCP2515
uint8_t CS_Pin = 17;

// Set the Oscillator-ClockRate if nessecary (per default 8MHz is set)
// The following sizes are defined in this MCP2515 library:
// 8MHz (8E6)
// 16MHz (16E6)
// 25MHz (25E6)
// 40MHz (40E6)
uint32_t ClockRate = 8E6;

// Initialize the Counter for the Messages
uint16_t counter_up=0;
uint16_t counter_up_overflow=0;
uint16_t counter_down=4095;
uint16_t counter_down_overflow=0;

// Initialize booleans for Message-Handling
bool Message_1_Filled=false;
bool Message_2_Filled=false;
bool Send_Message_1 = false;
bool Send_Message_2 = false;

// Definition of a CAN-Message
struct CanMessage{
  uint32_t ID;
  bool Extended;
  uint8_t DLC;
  uint8_t DataBuffer[8];
};

// Definition of Message_1
CanMessage Message_1 = {
  0xA74BF55,
  true,
  8,
};

// Definition of Message_2
CanMessage Message_2 = {
  0x1AB,
  false,
  8,
};

void setup() {
  // Initialize Serial for Debug
  // Attention!!! When you started the Serial-Connection once you have to keep it open.
  // Otherwise on each try to print a message it will stuck till the timeout (default 1s) is expired.
  // This disrupts the process.
  Serial.begin(115200);

  // Declaration of the Board-LED for Error display
  pinMode(LED_BUILTIN, OUTPUT);

  delay(5000);

  // Showing startup of the board
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);

  Serial.println("Set CS-Pin.");
  // Set the ChipSelect-Pin for the SPI-Communication
  while (!MCP2515Module.setSpiPins(CS_Pin))
  {
    // When Setting the CS-Pin failed (check MCP2515Error.h)
    Serial.print("Set Pin-Error: 0x");
    Serial.println(MCP2515Module.getLastMCPError(), HEX);

    for (size_t i = 0; i < 2; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    delay(1000);
  }

  Serial.println("Set MCP2515-ClockFrequency.");
  // Set the Oscillator-ClockRate if nessecary (per default 8MHz is set)
  while (!MCP2515Module.setClockFrequency(ClockRate))
  {
    // When Setting the ClockFrequency failed (check MCP2515Error.h)
    Serial.print("Set Clock-Frequency-Error: 0x");
    Serial.println(MCP2515Module.getLastMCPError(), HEX);

    for (size_t i = 0; i < 3; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    delay(1000);
  }

  Serial.println("Change SPI-Frequency.");
  // Set the SPI-Frequency if nessecary (per default 10MHz is set)
  while (!MCP2515Module.setSpiFrequency(5e6))
  {
    // When Setting the SPI-Frequency failed (check MCP2515Error.h)
    Serial.print("Set SPI-Frequency-Error: 0x");
    Serial.println(MCP2515Module.getLastMCPError(), HEX);

    for (size_t i = 0; i < 4; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    delay(1000);
  }

  Serial.println("Initialize MCP2515.");
  // Initialize the MCP2515 with the selected Baudrate (per default 500kB is set)
  while (!MCP2515Module.init((uint64_t)500E3, true))
  {
    // When Initialize the MCP2515 failed (check MCP2515Error.h)
    Serial.print("Init-Error: 0x");
    Serial.println(MCP2515Module.getLastMCPError(), HEX);

    for (size_t i = 0; i < 5; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    delay(1000);
  }

  delay(2000);
}

void loop() {

  // Initialize the Timer
  uint32_t time_ms = millis() % 1000;
  uint32_t time_s = millis()/1000;

  // Each 100ms (when Buffer of Message_1 is not filled)
  if ((millis() % 100) == 0 && Message_1_Filled == false)
  {
    // fill the DataBuffer of the Message_1 with the calculated Data.
    Message_1.DataBuffer[0] = (uint8_t)((((time_s >> 8) >> 8) >> 8) & 0xFF);
    Message_1.DataBuffer[1] = (uint8_t)(((time_s >> 8) >> 8) & 0xFF);
    Message_1.DataBuffer[2] = (uint8_t)((time_s >> 8) & 0xFF);
    Message_1.DataBuffer[3] = (uint8_t)(time_s & 0xFF);

    Message_1.DataBuffer[4] = (uint8_t)((((time_ms >> 8) >> 8) >> 8) & 0xFF);
    Message_1.DataBuffer[5] = (uint8_t)(((time_ms >> 8) >> 8) & 0xFF);
    Message_1.DataBuffer[6] = (uint8_t)((time_ms >> 8) & 0xFF);
    Message_1.DataBuffer[7] = (uint8_t)(time_ms & 0xFF);
    Message_1_Filled = true;

  } else if ((millis() % 100) != 0 && Message_1_Filled == true)
  {
    Message_1_Filled = false;
    Send_Message_1 = true;
  }

  // Each 1000ms (when Buffer of Message_2 is not filled)
  if ((millis() % 1000) == 0 && Message_2_Filled == false)
  {
    // Calculate the Data
    if (counter_up > 4094)
    {
      counter_up = 0;
      counter_up_overflow++;
    } else {
      counter_up++;
    }

    if (counter_down < 1)
    {
      counter_down = 4095;
      counter_down_overflow++;
    } else {
      counter_down--;
    }

    // and fill the DataBuffer of the Message_2 with the calculated Data.
    Message_2.DataBuffer[0] = (uint8_t)((counter_up >> 8) & 0xFF);
    Message_2.DataBuffer[1] = (uint8_t)(counter_up & 0xFF);
    Message_2.DataBuffer[2] = (uint8_t)((counter_up_overflow >> 8) & 0xFF);
    Message_2.DataBuffer[3] = (uint8_t)(counter_up_overflow & 0xFF);
    Message_2.DataBuffer[4] = (uint8_t)((counter_down >> 8) & 0xFF);
    Message_2.DataBuffer[5] = (uint8_t)(counter_down & 0xFF);
    Message_2.DataBuffer[6] = (uint8_t)((counter_down_overflow >> 8) & 0xFF);
    Message_2.DataBuffer[7] = (uint8_t)(counter_down_overflow & 0xFF);

    Message_2_Filled = true;

  } else if ((millis() % 1000) != 0 && Message_2_Filled == true)
  {
    Message_2_Filled = false;
    Send_Message_2 = true;
  }

  // Check if Message_1 is Ready to send
  if (MCP2515Module.check4FreeTransmitBuffer() != 0xFF && Send_Message_1)
  {
    uint8_t Buffer = MCP2515Module.check4FreeTransmitBuffer();
    MCP2515Module.fillTransmitBuffer(Buffer, Message_1.ID, Message_1.Extended, false, Message_1.DLC, Message_1.DataBuffer);
    if (!MCP2515Module.sendMessage(Buffer, 0))
    {
      Serial.print("Message_1 send-Error: 0x");
      Serial.println(MCP2515Module.getLastMCPError(), HEX);
    } else {
      Serial.println("Message_1 send.");
      Send_Message_1 = false;
    }
  }

  // Check if Message_2 is Ready to send
  if (MCP2515Module.check4FreeTransmitBuffer() != 0xFF && Send_Message_2)
  {
    uint8_t Buffer = MCP2515Module.check4FreeTransmitBuffer();
    MCP2515Module.fillTransmitBuffer(Buffer, Message_2.ID, Message_2.Extended, false, Message_2.DLC, Message_2.DataBuffer);
    if (!MCP2515Module.sendMessage(Buffer, 0))
    {
      Serial.print("Message_2 send-Error: 0x");
      Serial.println(MCP2515Module.getLastMCPError(), HEX);
    } else {
      Serial.println("Message_2 send.");
      Send_Message_2 = false;
    }
  }
}
