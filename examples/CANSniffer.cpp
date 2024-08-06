#include <Arduino.h>
#include <CAN.h>

bool checkforaMessage();

int CS_Pin = 53;
int Int_Pin = 2;

uint64_t possibleBaudRates[12] = {
    (uint64_t)5E3,
    (uint64_t)10E3,
    (uint64_t)20E3,
    (uint64_t)40E3,
    (uint64_t)50E3,
    (uint64_t)80E3,
    (uint64_t)100E3,
    (uint64_t)125E3,
    (uint64_t)200E3,
    (uint64_t)250E3,
    (uint64_t)500E3,
    (uint64_t)1000E3,
};

uint8_t actual_BaudRateIndex = 0;


MCP2515 MCP2515Module;

void setup() {

  bool BaudRate_Detected = false;
  bool Message_Received = false;

  // // initialize Serial for Debug
  Serial.begin(115200);

  // declaration of the Board-LED for Error display
  pinMode(LED_BUILTIN, OUTPUT);

  // showing startup of the board
  digitalWrite(LED_BUILTIN, HIGH);  // sets the LED_BUILTIN-pin on
  delay(250);                       // waits for 250 millisecond
  digitalWrite(LED_BUILTIN, LOW);   // sets the LED_BUILTIN-pin off
  delay(250);                       // waits for 250 millisecond
  digitalWrite(LED_BUILTIN, HIGH);  // sets the LED_BUILTIN-pin on
  delay(250);                       // waits for 250 millisecond
  digitalWrite(LED_BUILTIN, LOW);   // sets the LED_BUILTIN-pin off
  delay(250);

  MCP2515Module.setSpiPins(CS_Pin, Int_Pin);

  // Initially start the CAN bus at 5 kbps (and enable all Flags)
  while (!MCP2515Module.init(possibleBaudRates[actual_BaudRateIndex])) {
    // when initialization of CAN-Bus failed
    digitalWrite(LED_BUILTIN, HIGH);  // sets the LED_BUILTIN-pin on
    delay(500);                       // waits for 500 millisecond
    digitalWrite(LED_BUILTIN, LOW);   // sets the LED_BUILTIN-pin off
    delay(500);                       // waits for 500 millisecond
  }

  // Change to the ListenOnly-Mode
  MCP2515Module.setListenOnlyMode();

  // Figure out the BaudRate
  while (!BaudRate_Detected)
  {
    Serial.print("Check if Valid Message can be found with the Baudrate ");
    Serial.print(possibleBaudRates[actual_BaudRateIndex],DEC);
    Serial.println(".");
    int receiveCounter = 10;

    // Check if Message is received
    while (!checkforaMessage() && receiveCounter > 0)
    {
      Serial.print(".");
      delay(100);
      receiveCounter--;
    }

    if (receiveCounter == 0)
    {
      actual_BaudRateIndex++;

      if (actual_BaudRateIndex>11)
      {
        actual_BaudRateIndex=0;
      }
      Serial.println("No Message found. Try next Baudrate.");
      while (!MCP2515Module.changeBaudRate(possibleBaudRates[actual_BaudRateIndex]))
      {
        Serial.print("Baudrate can not be changed to ");
        Serial.print(possibleBaudRates[actual_BaudRateIndex],DEC);
        Serial.println(".");
        delay(1000);
      }
      Serial.print("Baudrate changed to ");
      Serial.print(possibleBaudRates[actual_BaudRateIndex],DEC);
      Serial.println(".");
    } else {
      Serial.print("Valid Message could be found with the Baudrate ");
      Serial.print(possibleBaudRates[actual_BaudRateIndex],DEC);
      Serial.println(".");
    }
  }
}

void loop(){

}

bool checkforaMessage()
{
  for (size_t i = 0; i < 2; i++)
  {
    // Check if something is in RXBn of the MCP2515-Controller
    if ((MCP2515Module.rxStatusInstruction() & (0x40 + i * 0x40)) != 0)
    {
      return true;
      //// Get ID
      //uint32_t ID = ((MCP2515Module.readInstruction(REG_RXBnSIDH(i)) << 3) & 0x07f8) |
      //            ((MCP2515Module.readInstruction(REG_RXBnSIDL(i)) >> 5) & 0x07);
      //
      //if ((MCP2515Module.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
      //{
      //    ID = ((ID << 18) & 0x1FFC0000) | ((((MCP2515Module.readInstruction(REG_RXBnSIDL(i)) & RXBnSIDL_BIT_EID) << 8) << 8) & 0x30000) | ((MCP2515Module.readInstruction(REG_RXBnEID8(i)) << 8) & 0xff00) | MCP2515Module.readInstruction(REG_RXBnEID0(i));
      //}
      //
      //// Check if ID in RXB0 is correct
      //if (ID != _ID)
      //{
      //    continue;
      //}
      //
      //// Check if DLC is correct
      //if ((MCP2515Module.readInstruction(REG_RXBnDLC(i)) & RXBnDLC_BIT_DLC) != _DLC)
      //{
      //    continue;
      //}
      //
      //_DataBufferIndex = 0;
      //
      //// Collect Data
      //for (size_t m = 0; m < _DLC; m++)
      //{
      //    _DataByte[m] = MCP2515Module.readInstruction(REG_RXBnD0(i) + m);
      //}
      //
      //// Release Buffer
      //MCP2515Module.bitModifyInstruction(REG_CANINTF, CANINTF_BIT_RXnIF(i), 0x00);
      //
      //return true;
    }
  }
  return false;
}
