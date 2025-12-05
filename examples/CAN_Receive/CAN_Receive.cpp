/*
    CAN Receive

    This example checks whether certain messages (sent via the example CAN_Transmit) are being received.


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
    During each loop iteration, the system checks whether a message has been received.

    If a message is received, the data is extracted, prepared for output, and then displayed.


    Created 05.12.2025
    By MH-Tobi
*/

#include <MCP2515.h>

// Create Instances of the CAN-Controller
MCP2515 MCP2515Module;

// Definition of Chip-Select-Pin for the SPI-Communication
// Choose the Pin of the Arduino (or similar) which is connected with the CS-Pin of the MCP2515
uint8_t CS_Pin = 53;

// Set the Oscillator-ClockRate if nessecary (per default 8MHz is set)
// The following sizes are defined in this MCP2515 library:
// 8MHz (8E6)
// 16MHz (16E6)
// 25MHz (25E6)
// 40MHz (40E6)
uint32_t ClockRate = 8E6;

// Definition of Values they are received
uint32_t time_ms;
uint32_t time_s;
uint16_t counter_up;
uint16_t counter_up_overflow;
uint16_t counter_down;
uint16_t counter_down_overflow;

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

  // Check if Message_1 was received
  if (MCP2515Module.check4Receive(Message_1.ID, Message_1.Extended, Message_1.DLC, Message_1.DataBuffer)){
    time_s = 0x00000000;    // Stored in the first 4 Bytes of the Message_1-DataBuffer
    time_ms = 0x00000000;   // Stored in the last 4 Bytes of the Message_1-DataBuffer

    // Get the Values from the first 4 Bytes of the Message_1-DataBuffer
    for (size_t i = 0; i < 4; i++)
    {
      time_s = time_s << 8 | Message_1.DataBuffer[i];
    }

    // Get the Values from the last 4 Bytes of the Message_1-DataBuffer
    for (size_t i = 4; i < 8; i++)
    {
      time_ms = time_ms << 8 | Message_1.DataBuffer[i];
    }

    // Print the Values.
    Serial.print("Time [s]\t");
    Serial.print(time_s, DEC);
    Serial.print("\tTime [ms]\t");
    Serial.println(time_ms, DEC);
  }

  // Check if Message_2 was received
  if (MCP2515Module.check4Receive(Message_2.ID, Message_2.Extended, Message_2.DLC, Message_2.DataBuffer)){
    counter_up = 0x0000;              // Stored in the first 2 Bytes of the Message_2-DataBuffer
    counter_up_overflow = 0x0000;     // Stored in the second 2 Bytes of the Message_2-DataBuffer
    counter_down = 0x0000;            // Stored in the third 2 Bytes of the Message_2-DataBuffer
    counter_down_overflow = 0x0000;   // Stored in the last 2 Bytes of the Message_2-DataBuffer

    // Get the Values from the first 2 Bytes of the Message_2-DataBuffer
    counter_up = counter_up << 8 | Message_2.DataBuffer[0];
    counter_up = counter_up << 8 | Message_2.DataBuffer[1];

    // Get the Values from the second 2 Bytes of the Message_2-DataBuffer
    counter_up_overflow = counter_up_overflow << 8 | Message_2.DataBuffer[2];
    counter_up_overflow = counter_up_overflow << 8 | Message_2.DataBuffer[3];

    // Get the Values from the third 2 Bytes of the Message_2-DataBuffer
    counter_down = counter_down << 8 | Message_2.DataBuffer[4];
    counter_down = counter_down << 8 | Message_2.DataBuffer[5];

    // Get the Values from the last 2 Bytes of the Message_2-DataBuffer
    counter_down_overflow = counter_down_overflow << 8 | Message_2.DataBuffer[6];
    counter_down_overflow = counter_down_overflow << 8 | Message_2.DataBuffer[7];

    // Print the Values.
    Serial.print("Counter Up\t");
    Serial.print(counter_up, DEC);
    Serial.print("\tCounter Up Overflow\t");
    Serial.print(counter_up_overflow, DEC);
    Serial.print("\tCounter Down\t");
    Serial.print(counter_down, DEC);
    Serial.print("\tCounter Down Overflow\t");
    Serial.println(counter_down_overflow, DEC);
  }

  delay(10);
}
