#include <Arduino.h>
#include <TestLibrary.h>    // in lib-Folder

// Create Instances of the CAN-Controller
MCP2515 MCP2515Module;

uint32_t defaultSettings[5] = {
    53,                 // CS-Pin (Mega = 53; Micro = 17)
    4,                  // INT-Pin
    (uint32_t)4E6,      // SPI-Frequency
    (uint32_t)8E6,      // Clock-Frequency
    (uint32_t)500E3,    // Baudrate
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

  delay(5000);

  uint8_t TestCaseNumber = 0;
  uint8_t Errors = 0;
  
  pinMode(defaultSettings[1], INPUT_PULLUP);

  Serial.println("=============================================================================");
  Serial.println("=============================== Start Testing ===============================");
  Serial.println("=============================================================================");
  Serial.println();

  //TestCaseNumber++;
  //Errors = Errors + testInit(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetSpiPins(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetSpiMode(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetDataOrder(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetSpiFrequency(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetClockFrequency(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetConfigurationMode(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetNormalMode(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetListenOnlyMode(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetLoopbackMode(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetSleepMode(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetReCheckEnabler(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testChangeBaudRate(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testChangeInterruptSetting(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testEnableFilterMask(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testDisableFilterMask(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testCheck4FreeTransmitBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testFillTransmitBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSendMessage(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testCheck4Rtr(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testCheck4Receive(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testGetIdFromReceiveBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testGetDlcFromReceiveBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testGetFrameFromReceiveBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testGetRtrFromReceiveBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testGetDataFromReceiveBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testGetAllFromReceiveBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testReleaseReceiveBuffer(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testCheck4InterruptFlags(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testResetInterruptFlag(TestCaseNumber, MCP2515Module, defaultSettings);
  //TestCaseNumber++;
  //Errors = Errors + testSetInterruptFlag(TestCaseNumber, MCP2515Module, defaultSettings);

  Serial.println();
  Serial.println();
  Serial.print("Total count of Errors = ");
  Serial.println(Errors, DEC);
}

void loop() {

}


