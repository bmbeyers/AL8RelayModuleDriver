/******************************************************************************
 *
 * AL8RelayModule Attiny Driver Library
 * Written by Brandon M Beyers
 *
 * This library is designed to extend the AL8RelayModule library to a self-
 * controlled unit, driven by an Attiny85 microcontroller.  This setup allows
 * for I2C communication protocol, which further enhances the capability of the
 * system by utilizing even less I/O pins on the master microcontroller device.
 *
 ******************************************************************************/

#ifndef AL8RelayModule_ATtiny85Driver_h
#define AL8RelayModule_ATtiny85Driver_h

#include "Arduino.h"
#include <AL8RelayModule.h>
#if defined ( __AVR_ATtiny85__ )
  #include <TinyWireS.h>
#else
  #include <Wire.h>
#endif

#define ALL_ON_CMD      B11111111  // 0xFF -- command to turn all relays on
#define ALL_OFF_CMD     B00000000  // 0x00 -- command to turn all relays off
#define SET_RELAY_CMD   B11000011  // 0xC3 -- command to set a specific relay config
#define TURN_ON_CMD     B11001111  // 0xCF -- command to turn on a specific relay
#define TURN_OFF_CMD    B11000000  // 0xC0 -- command to turn off a specific relay
#define TOGGLE_CMD      B11001010  // 0xCA -- command to toggle a specific relay
#define TOGGLE_ALL_CMD  B10101010  // 0xAA -- command to toggle all relays

#define SR_DATA_PIN     4          // ATtiny Pin 3
#define SR_CLOCK_PIN    1          // ATtiny Pin 6
#define SR_LATCH_PIN    3          // ATtiny Pin 2

#if defined( __AVR_ATtiny85__ )
class AL8RelayModule_ATtiny85Driver {
  public:
    AL8RelayModule_ATtiny85Driver(byte address);
    AL8RelayModule_ATtiny85Driver(byte address, byte relay);

    void begin();

    // // Method to call in the loop() section -- updates relays if told via I2C
    // void updateFromI2C();

    static void setRelayFromCommand(byte command, byte relay_code);

    // Returns current value for _relays
    byte getRelayValue();

  private:
    static byte _address;
    static AL8RelayModule _relay_module;

    // Returns true if relay received matches a single relay defined above
    static bool singleRelayReceived(byte relay_code);

    static void receiveEvent(uint8_t how_many);
    static void requestEvent();
};

#else
class ATtiny85Driver_Master {
  public:
    ATtiny85Driver_Master(byte address);
    ATtiny85Driver_Master(byte address, byte relay);

    void begin();

    bool turnAllOn();
    bool turnAllOff();
    bool toggleAll();
    bool turnOn(byte relay);
    bool turnOff(byte relay);
    bool toggle(byte relay);
    bool setRelay(byte relay);
    bool toggleFromSerial(char serial_input);

    byte getRelayValue();
    byte getMasterValue();

    bool isOn(byte relay);
    bool isOff(byte relay);

  private:
    byte _address;
    uint8_t _relays;
    uint8_t _device_relays;

    bool issueCommandToDriver(byte command_1, byte command_2);
    uint8_t getDeviceRelayValue();
    bool singleRelayReceived(byte relay);
};

#endif
#endif
