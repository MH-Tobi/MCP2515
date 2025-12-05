/*
    CAN Sniffer

    This example checks which baud rate is used in the connected CAN bus.


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


    Procedure in Setup:
    At the beginning of the script, the Serial-Communication is established and
    the built-in LED of the Arduino board is configured so that in case of errors,
    not only a serial output is given, but also a blinking output.

    After setting the CS pin, the MCP2515 clock rate and the SPI frequency, the MCP2515 module is initialized.

    Since we only want to monitor the CAN communication, the MCP2515 is put into Listen-Only operating mode.

    During setup, if an error occurs, the corresponding error code is output via serial and
    a specific number of LED flashes are displayed.
    The failed process step is then re-executed. This continues until the process step is successfully completed.


    Procedure in Loop:
    Within the loop function, all possible (predefined) baud rates are iterated.
    At each baud rate, the system checks 100 times whether specific interrupt flags are set in the MCP2515
    (Message Error Flags and Receive Buffer Full Flags).

    If a flag is detected, a corresponding counter is incremented and the flag is reset.

    A baud rate is considered implausible if more than 10 errors or
    fewer than 50 messages are detected in the 100 attempts.

    In this case, the process continues with the next possible baud rate.

    If a specific baud rate outputs plausible messages, then that baud rate will be used for the subsequent test steps.

    In the following steps, the data of each received message is output via Serial.


    Created 05.12.2025
    By MH-Tobi
*/

#include <Arduino.h>
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
  while (!MCP2515Module.init((uint64_t)500E3, true)) {
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

  Serial.println("Set Listen-Only-Mode.");
  // Change Operation-Mode to Listen-Only
  while (!MCP2515Module.setListenOnlyMode())
  {
    // When setting Operation-Mode failed (check MCP2515Error.h)
    Serial.print("Operation-Mode-Error: 0x");
    Serial.println(MCP2515Module.getLastMCPError(), HEX);

    for (size_t i = 0; i < 6; i++)
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
