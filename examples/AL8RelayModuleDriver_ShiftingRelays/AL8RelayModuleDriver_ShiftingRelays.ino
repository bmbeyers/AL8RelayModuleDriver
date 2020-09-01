/* ATtiny85 as an I2C Slave for controlling shift register for active low 8 relay module.
 * ATtiny85 I2C slave receiving and sending data to an Arduino master.
 * Gets data from master, and sends it back, then updates the shift register.
 * 
 * SETUP:
 * 
 * ATtiny Pin 1 = (RESET) to 5V (no reset)        ATtiny Pin 8 = (VCC) to 5V
 * ATtiny Pin 2 = (D3) to SR_LATCH                ATtiny Pin 7 = (D2) to I2c SCK
 * ATtiny Pin 3 = (D4) to SR_DATA                 ATtiny Pin 6 = (D1) to SR_CLOCK
 * ATtiny Pin 4 = (GND) to GND                    ATtiny Pin 5 = (D0) to I2C SDA
 * 
 * 
 * NOTE! - It's very important to use pullups on the SDA & SCL lines!
 * Current Rx & Tx buffers set at 32 bytes - see usiTwiSlave.h
 * Credit and thanks to Don Blake for his usiTwiSlave code. 
 * More on TinyWireS usage - see TinyWireS.h
 */


//#include "TinyWireS.h"                  // wrapper class for I2C slave routines
#include <AL8RelayModule_ATtiny85Driver.h>// custom class for active low relay module

AL8RelayModule_ATtiny85Driver relay_driver(0x14);

void setup(){
  relay_driver.begin();
}


void loop(){
  for (byte count = 1; count <= 8; count++) {
    switch (count) {
      case 1:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_8);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_1);
      break;
      case 2:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_1);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_2);
      break;
      case 3:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_2);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_3);
      break;
      case 4:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_3);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_4);
      break;
      case 5:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_4);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_5);
      break;
      case 6:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_5);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_6);
      break;
      case 7:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_6);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_7);
      break;
      case 8:
        relay_driver.setRelayFromCommand(TURN_OFF_CMD, RELAY_7);
        relay_driver.setRelayFromCommand(TURN_ON_CMD, RELAY_8);
      break;
    }
    delay(1000);
  }
}
