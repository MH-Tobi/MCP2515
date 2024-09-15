#include <Arduino.h>
#include <MCP2515.h>

// Create Instances of the CAN-Controller
MCP2515 MCP2515Module;

// Definition of Chip-Select-Pin for the SPI-Communication
// Choose the Pin of the Arduino (or similar) which is connected with the CS-Pin of the MCP2515
//uint8_t CS_Pin = 17;  // for Micro
uint8_t CS_Pin = 53;  // for Mega

// Definition of Values they are received
uint32_t time_ms;
uint32_t time_s;
uint16_t counter_up;
uint16_t counter_up_overflow;
uint16_t counter_down;
uint16_t counter_down_overflow;


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
  while (!MCP2515Module.setClockFrequency((uint32_t)8E6))
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
  // Definition of Message_1
  uint32_t ID_1 = 0xA74BF55;
  bool Extended_1 = true;
  uint8_t DLC_1 = 8;
  uint8_t DataBuffer_1[8];

  // Definition of Message_2
  uint32_t ID_2 = 0x1AB;
  bool Extended_2 = false;
  uint8_t DLC_2 = 8;
  uint8_t DataBuffer_2[8];

  // Check if Message_1 was received
  if (MCP2515Module.check4Receive(ID_1, Extended_1, DLC_1, DataBuffer_1)){
    time_s = 0x00000000;    // Stored in the first 4 Bytes of the Message_1-DataBuffer
    time_ms = 0x00000000;   // Stored in the last 4 Bytes of the Message_1-DataBuffer

    // Get the Values from the first 4 Bytes of the Message_1-DataBuffer
    for (size_t i = 0; i < 4; i++)
    {
      time_s = time_s << 8 | DataBuffer_1[i];
    }

    // Get the Values from the last 4 Bytes of the Message_1-DataBuffer
    for (size_t i = 4; i < 8; i++)
    {
      time_ms = time_ms << 8 | DataBuffer_1[i];
    }

    // Print the Values.
    Serial.print("Time [s]\t");
    Serial.print(time_s, DEC);
    Serial.print("\tTime [ms]\t");
    Serial.println(time_ms, DEC);
  }

  // Check if Message_2 was received
  if (MCP2515Module.check4Receive(ID_2, Extended_2, DLC_2, DataBuffer_2)){
    counter_up = 0x0000;              // Stored in the first 2 Bytes of the Message_2-DataBuffer
    counter_up_overflow = 0x0000;     // Stored in the second 2 Bytes of the Message_2-DataBuffer
    counter_down = 0x0000;            // Stored in the third 2 Bytes of the Message_2-DataBuffer
    counter_down_overflow = 0x0000;   // Stored in the last 2 Bytes of the Message_2-DataBuffer

    // Get the Values from the first 2 Bytes of the Message_2-DataBuffer
    counter_up = counter_up << 8 | DataBuffer_2[0];
    counter_up = counter_up << 8 | DataBuffer_2[1];

    // Get the Values from the second 2 Bytes of the Message_2-DataBuffer
    counter_up_overflow = counter_up_overflow << 8 | DataBuffer_2[2];
    counter_up_overflow = counter_up_overflow << 8 | DataBuffer_2[3];

    // Get the Values from the third 2 Bytes of the Message_2-DataBuffer
    counter_down = counter_down << 8 | DataBuffer_2[4];
    counter_down = counter_down << 8 | DataBuffer_2[5];

    // Get the Values from the last 2 Bytes of the Message_2-DataBuffer
    counter_down_overflow = counter_down_overflow << 8 | DataBuffer_2[6];
    counter_down_overflow = counter_down_overflow << 8 | DataBuffer_2[7];

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
