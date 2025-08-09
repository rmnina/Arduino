// Using my Arduino board as an IR decoder, thanks to a VS1838B receiver.
// A LED in the circuit can also be turned on and off with the activation of designated remote buttons.
// This project has been made thanks to a tutorial from the following reference:
// F. Glausinger & C. Doutriaux, *Arduino - S'exercer au prototypage électronique*, 2e édition, Éditions ENI, ISBN 978-2-409-04486-1

#include <IRremote.h>

#define PIN_SIG 7 // Arduino pin on which our sensor is read
#define PIN_LED 8 // Arduino pin on which our led is set

void setup() {
  Serial.begin(115200); // The Arduino's serial port sends bytes at a specified speed. Here, it is initialized to 115200 baud.
  pinMode(PIN_SIG, INPUT); // We initialize the input, in this case pin 7 as previously defined in PIN_SIG.
  pinMode(PIN_LED, OUTPUT); // We initialize the output, in this case pin 8 as previously defined in PIN_LED.
  IrReceiver.begin(PIN_SIG, ENABLE_LED_FEEDBACK); // Starts the IR receiver/decoding manager. Indicates reception on PIN_SIG, and flashes the built-in LED when the signal is received (ENABLE_LED_FEEDBACK).
  Serial.println(F("Decoding following protocols:")); // The string is set as a parameter of a function F to save memory (FLASH storage instead of RAM).
  printActiveIRProtocols(&Serial); // These last two lines display all protocols enabled on the PC's serial monitor.
}

void loop() {
  // Test if a message has been received and decoded.
  if (IrReceiver.decode())
  {
    // Display message informations.
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
  
    // If protocol is unknown, display raw data instead.
    if (IrReceiver.decodedIRData.protocol == UNKNOWN)
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    
    // Turn on and off the LED with the remote control
    if (IrReceiver.decodedIRData.command == 0x11)
      digitalWrite(PIN_LED, HIGH); // Sending a 1 to the led -> turns it on.
    if (IrReceiver.decodedIRData.command == 0x8)
      digitalWrite(PIN_LED, LOW); // Sending a 0 to the led -> turns it off.

    // relaunch listening process
    IrReceiver.resume();
  }
}

/* INFOS IN THE SERIAL MONITOR :
Protocol (NEC, Sony, RC5...) - Device address (0x4, 0x0...) - Command (hexadecimal number identifying the button pressed).
Examples:
Protocol=NEC Address=0x4 Command=0x13 Raw-Data=0xEC13FB04 32 bits LSB first Gap=482250us Duration=67550us
Protocol=NEC Address=0x0 Command=0xA Raw-Data=0xF50AFF00 32 bits LSB first Gap=1065900us Duration=66850us

IrSendUsage -> gives the instruction that'll need to be used in order to reproduce the command from an Arduino circuit.
Example:
Send with: IrSender.sendNEC(0x4, 0x13, <numberOfRepeats>);
Send with: IrSender.sendNEC(0x0, 0xA, <numberOfRepeats>);
*/
