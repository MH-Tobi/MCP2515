#include <Arduino.h>
#include <MCP2515.h>

MCP2515 MCP2515Module;


void setup() {
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

  MCP2515Module.setSpiPins(17);

  // start the CAN bus at 500 kbps
  while (!MCP2515Module.init(500E3)) {
    // when initialization of CAN-Bus failed
    Serial.print("init-Error: 0x");
    Serial.println(MCP2515Module.getLastMCPError(), HEX);
    digitalWrite(LED_BUILTIN, HIGH);  // sets the LED_BUILTIN-pin on
    delay(500);                       // waits for 500 millisecond
    digitalWrite(LED_BUILTIN, LOW);   // sets the LED_BUILTIN-pin off
    delay(500);                       // waits for 500 millisecond
  }
}

void loop() {
  uint8_t Value = MCP2515Module.check4FreeTransmitBuffer();

  Serial.print("Value: 0x");
  Serial.println(Value, HEX);

  delay(100);
}