# AL8RelayModuleDriver

This library uses an ATtiny85 as an I2C slave to control a shift register that ultimately manipulates an active-low 8-channel relay module. Many examples of this particular style of relay module exist.

The ATtiny85 I2C slave receives and/or sends data to an Arduino or similar microcontroller master device. The benefit of using this library is that it frees up digital IO pins on the master controller (only uses SDA and SCL, plus the obvious ground and most likely the 5V or 3.3V power pins) to be able to control potentially hundreds of relays. Another benefit is that you can specify the I2C address of the ATtiny85, allowing you to avoid clashing with other I2C enabled devices in the same project.

Wiring connection:
'''
/******************************************************************************
 * ATtiny Pin 1 = (RESET) to 5V (no reset)    ATtiny Pin 8 = (VCC) to 5V      *
 * ATtiny Pin 2 = (D3) to SR_LATCH            ATtiny Pin 7 = (D2) to I2C SCL  *
 * ATtiny Pin 3 = (D4) to SR_DATA             ATtiny Pin 6 = (D1) to SR_CLOCK *
 * ATtiny Pin 4 = (GND) to GND                ATtiny Pin 5 = (D0) to I2C SDA  *
 ******************************************************************************/
'''

Note: This library depends on the rambo/TinyWireS library. It is also very important to include physical pull-up resistors on the SDA and SCL lines at the ATtiny85 (pins 5 and 7).
