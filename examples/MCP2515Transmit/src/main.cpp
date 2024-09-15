#include <Arduino.h>
#include <MCP2515.h>

// Create Instances of the CAN-Controller
MCP2515 MCP2515Module;

// Definition of Chip-Select-Pin for the SPI-Communication
// Choose the Pin of the Arduino (or similar) which is connected with the CS-Pin of the MCP2515
uint8_t CS_Pin = 17;  // for Micro
//uint8_t CS_Pin = 53;  // for Mega

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

  // Initialize the Timer
  uint32_t time_ms = millis() % 1000;
  uint32_t time_s = millis()/1000;

  // Each 100ms (when Buffer of Message_1 is not filled)
  if ((millis() % 100) == 0 && Message_1_Filled == false)
  {
    // fill the DataBuffer of the Message_1 with the calculated Data.
    DataBuffer_1[0] = (uint8_t)((((time_s >> 8) >> 8) >> 8) & 0xFF);
    DataBuffer_1[1] = (uint8_t)(((time_s >> 8) >> 8) & 0xFF);
    DataBuffer_1[2] = (uint8_t)((time_s >> 8) & 0xFF);
    DataBuffer_1[3] = (uint8_t)(time_s & 0xFF);

    DataBuffer_1[4] = (uint8_t)((((time_ms >> 8) >> 8) >> 8) & 0xFF);
    DataBuffer_1[5] = (uint8_t)(((time_ms >> 8) >> 8) & 0xFF);
    DataBuffer_1[6] = (uint8_t)((time_ms >> 8) & 0xFF);
    DataBuffer_1[7] = (uint8_t)(time_ms & 0xFF);
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
    DataBuffer_2[0] = (uint8_t)((counter_up >> 8) & 0xFF);
    DataBuffer_2[1] = (uint8_t)(counter_up & 0xFF);
    DataBuffer_2[2] = (uint8_t)((counter_up_overflow >> 8) & 0xFF);
    DataBuffer_2[3] = (uint8_t)(counter_up_overflow & 0xFF);
    DataBuffer_2[4] = (uint8_t)((counter_down >> 8) & 0xFF);
    DataBuffer_2[5] = (uint8_t)(counter_down & 0xFF);
    DataBuffer_2[6] = (uint8_t)((counter_down_overflow >> 8) & 0xFF);
    DataBuffer_2[7] = (uint8_t)(counter_down_overflow & 0xFF);

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
    MCP2515Module.fillTransmitBuffer(Buffer, ID_1, Extended_1, false, DLC_1, DataBuffer_1);
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
    MCP2515Module.fillTransmitBuffer(Buffer, ID_2, Extended_2, false, DLC_2, DataBuffer_2);
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
