#include <Arduino.h>
#include <MCP2515.h>

// Create Instances of the CAN-Controller
MCP2515 MCP2515Module;

// Definition of Chip-Select-Pin for the SPI-Communication
// Choose the Pin of the Arduino (or similar) which is connected with the CS-Pin of the MCP2515
uint8_t CS_Pin = 17;  // for Micro
//uint8_t CS_Pin = 53;  // for Mega

// Define possible BaudRate-Array
uint64_t possibleBaudRates[12] = {
    (uint64_t)5E3,      // Not allowed for ClockRate 25MHz and 40MHz
    (uint64_t)10E3,     // Not allowed for ClockRate 40MHz
    (uint64_t)20E3,
    (uint64_t)40E3,
    (uint64_t)50E3,
    (uint64_t)80E3,
    (uint64_t)100E3,
    (uint64_t)125E3,
    (uint64_t)200E3,
    (uint64_t)250E3,
    (uint64_t)500E3,
    (uint64_t)1000E3,   // Not allowed for ClockRate 8MHz
};

bool baudRateRecognized = false;


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
    // When initialization of CAN-Bus failed (check MCP2515Error.h)
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
    // When initialization of CAN-Bus failed (check MCP2515Error.h)
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
  // Set the Oscillator-ClockRate if nessecary (per default 8MHz is set)
  while (!MCP2515Module.setSpiFrequency(5e6))
  {
    // When initialization of CAN-Bus failed (check MCP2515Error.h)
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
  // Start the CAN bus at 500 kbps initially
  while (!MCP2515Module.init((uint64_t)500E3, true)) {
    // When initialization of CAN-Bus failed (check MCP2515Error.h)
    Serial.print("Init-Error: 0x");
    Serial.println(MCP2515Module.getLastMCPError(), HEX);
    Serial.println(MCP2515Module.getLastSPIError(), HEX);

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

  Serial.println("Set Listen-Only-Mode.");
  // Change Operation-Mode to Listen-Only
  while (!MCP2515Module.setListenOnlyMode())
  {
    // When setting Operation-Mode failed (check MCP2515Error.h)
    Serial.print("Operation-Mode-Error: 0x");
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

  // Iterate through each BaudRate till plausible Messages are received
  if (!baudRateRecognized)
  {
    for (size_t i = 0; i < 12; i++)
    {
      if (!MCP2515Module.changeBaudRate(possibleBaudRates[i]))
      {
        // When Baudrate could not been set (check MCP2515Error.h)
        Serial.print("Baudrate ");
        Serial.print((uint16_t)(possibleBaudRates[i]/1000), DEC);
        Serial.print("kB couldn't be set with Error 0x");
        Serial.println(MCP2515Module.getLastMCPError(), HEX);
      } else {
        Serial.print("Baudrate ");
        Serial.print((uint16_t)(possibleBaudRates[i]/1000), DEC);
        Serial.println("kB set.");

        // Check if Message-Error-Flag occurs.
        uint16_t trys=100;  // try this 100 times
        uint16_t Messages_Received = 0;
        uint16_t Errors_occured = 0;
        while (trys > 0)
        {
          // Check for Message-Error-Flags and Receive-Buffer-Full Flags
          if ((MCP2515Module.check4InterruptFlags() & 0x83) > 0x00)
          {
            if ((MCP2515Module.check4InterruptFlags() & 0x80) > 0x00)
            {
              Errors_occured++;
              MCP2515Module.resetInterruptFlag(7);
            }
            if ((MCP2515Module.check4InterruptFlags() & 0x03) > 0x00)
            {
              Messages_Received++;
              MCP2515Module.resetInterruptFlag(0);
              MCP2515Module.resetInterruptFlag(1);
            }
          }
          delay(10);
          trys--;
        }
        Serial.print(Messages_Received, DEC);
        Serial.println(" Messages Received.");

        Serial.print(Errors_occured, DEC);
        Serial.println(" Errors occured.");

        delay(1000);

        if ((Errors_occured > 10) || (Messages_Received < 50))
        {
          Serial.println("Baudrate not plausible. Try with next Baudrate.");
        } else {
          Serial.println("Baudrate plausible.");
          baudRateRecognized = true;
          break;
        }
      }
    }

    // After each iteration, wait for manual Input of the Baudrate to set.
    delay(1000);
    Serial.println("Set Baudrate manually:");
    delay(5000);
    if (Serial.available() > 0) {

      String receivedBaudrate;

      while (Serial.available() > 0){
        receivedBaudrate = Serial.readString();
      }

      uint64_t Baudrate = 0;

      if (receivedBaudrate.startsWith("0005"))
      {
        Serial.println("Use 5kB");
        Baudrate = 5E3;
      } else if (receivedBaudrate.startsWith("0010"))
      {
        Serial.println("Use 10kB");
        Baudrate = 10E3;
      } else if (receivedBaudrate.startsWith("0020"))
      {
        Serial.println("Use 20kB");
        Baudrate = 20E3;
      } else if (receivedBaudrate.startsWith("0040"))
      {
        Serial.println("Use 40kB");
        Baudrate = 40E3;
      } else if (receivedBaudrate.startsWith("0050"))
      {
        Serial.println("Use 50kB");
        Baudrate = 50E3;
      } else if (receivedBaudrate.startsWith("0080"))
      {
        Serial.println("Use 80kB");
        Baudrate = 80E3;
      } else if (receivedBaudrate.startsWith("0100"))
      {
        Serial.println("Use 100kB");
        Baudrate = 100E3;
      } else if (receivedBaudrate.startsWith("0125"))
      {
        Serial.println("Use 125kB");
        Baudrate = 125E3;
      } else if (receivedBaudrate.startsWith("0200"))
      {
        Serial.println("Use 200kB");
        Baudrate = 200E3;
      } else if (receivedBaudrate.startsWith("0250"))
      {
        Serial.println("Use 250kB");
        Baudrate = 250E3;
      } else if (receivedBaudrate.startsWith("0500"))
      {
        Serial.println("Use 500kB");
        Baudrate = 500E3;
      } else if (receivedBaudrate.startsWith("1000"))
      {
        Serial.println("Use 1000kB");
        Baudrate = 1000E3;
      } else {
        Serial.println("No match");
      }

      if (Baudrate != 0)
      {
        if (!MCP2515Module.changeBaudRate(Baudrate))
        {
          // When Baudrate could not been set (check MCP2515Error.h)
          Serial.print("Baudrate ");
          Serial.print((uint16_t)(Baudrate/1000), DEC);
          Serial.print("kB couldn't be set with Error 0x");
          Serial.println(MCP2515Module.getLastMCPError(), HEX);
        } else {
          baudRateRecognized = true;
        }
      }
    }
  } else {
    // Check for Receive-Buffer-Full Flags
    uint8_t Receive = (MCP2515Module.check4InterruptFlags() & 0x03);
    uint32_t ID1 = 0;
    uint8_t DLC1 = 0;
    bool RTR1 = 0;
    bool Frame1 = 0;
    uint8_t Data1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t ID2 = 0;
    uint8_t DLC2 = 0;
    bool RTR2 = 0;
    bool Frame2 = 0;
    uint8_t Data2[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    // Check if Receive-Buffer 0 is full
    if ((Receive & 0x01) == 0x01)
    {
      // Get the Data of Receive-Buffer 0
      if (!MCP2515Module.getAllFromReceiveBuffer(0, ID1, Frame1, RTR1, DLC1, Data1))
      {
        ID1 = 0;
      }
    }

    // Check if Receive-Buffer 0 is full
    if ((Receive & 0x02) == 0x02)
    {
      // Get the Data of Receive-Buffer 0
      if (!MCP2515Module.getAllFromReceiveBuffer(1, ID2, Frame2, RTR2, DLC2, Data2))
      {
        ID2 = 0;
      }
    }

    // If Data is collected from Receive-Buffer 0, print the Data.
    if (ID1 != 0)
    {
      Serial.print("ID: 0x");
      if (!Frame1)
      {
        Serial.print((uint16_t)(ID1 & 0xFFFF), HEX);
      } else {
        Serial.print((uint16_t)(((ID1 >> 8) >> 8) & 0xFFFF), HEX);
        Serial.print((uint16_t)(ID1 & 0xFFFF), HEX);
      }

      Serial.print("\tFrame: ");
      if (!Frame1)
      {
        Serial.print("Standard");
      } else {
        Serial.print("Extended");
      }

      if (!RTR1)
      {
        Serial.print("\tDLC: ");
        Serial.print(DLC1, DEC);

        Serial.print("\tData:");

        for (size_t i = 0; i < DLC1; i++)
        {
          Serial.print(" 0x");
          Serial.print(Data1[i], HEX);
        }
        Serial.println();
      } else {
        Serial.println("\tRemote Transmission Request");
      }
    }

    // If Data is collected from Receive-Buffer 1, print the Data.
    if (ID2 != 0)
    {
      Serial.print("ID: 0x");
      if (!Frame2)
      {
        Serial.print((uint16_t)(ID2 & 0xFFFF), HEX);
      } else {
        Serial.print((uint16_t)(((ID2 >> 8) >> 8) & 0xFFFF), HEX);
        Serial.print((uint16_t)(ID2 & 0xFFFF), HEX);
      }

      Serial.print("\tFrame: ");
      if (!Frame2)
      {
        Serial.print("Standard");
      } else {
        Serial.print("Extended");
      }

      if (!RTR2)
      {
        Serial.print("\tDLC: ");
        Serial.print(DLC2, DEC);

        Serial.print("\tData:");

        for (size_t i = 0; i < DLC2; i++)
        {
          Serial.print(" 0x");
          Serial.print(Data2[i], HEX);
        }
        Serial.println();
      } else {
        Serial.println("\tRemote Transmission Request");
      }
    }
  }
}
