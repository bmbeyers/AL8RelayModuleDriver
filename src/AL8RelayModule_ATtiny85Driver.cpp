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

#include "Arduino.h"
#include <AL8RelayModule_ATtiny85Driver.h>
#include <AL8RelayModule.h>
#if defined( __AVR_ATtiny85__ )
  #include <TinyWireS.h>
#else
  #include <Wire.h>
#endif

#if defined( __AVR_ATtiny85__ )

byte AL8RelayModule_ATtiny85Driver::_address = 0x14;
AL8RelayModule AL8RelayModule_ATtiny85Driver::_relay = AL8RelayModule();

// Intialize with all relays de-energized
AL8RelayModule_ATtiny85Driver::AL8RelayModule_ATtiny85Driver(byte address) {
  _address = address;
  _relay_module.turnAllOff();
}

// Initialize with a specific configuration
AL8RelayModule_ATtiny85Driver::AL8RelayModule_ATtiny85Driver(byte address, byte relay) {
  _address = address;
  _relay_module.setRelay(relay);
}

// Method to call in the setup() section
void AL8RelayModule_ATtiny85Driver::begin() {
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  TinyWireS.begin(_address);
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);
  _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
}

// // Method to call in the loop() section -- updates relays if told via I2C
// void AL8RelayModule_ATtiny85Driver::updateFromI2C() {
//
// }

void AL8RelayModule_ATtiny85Driver::setRelayFromCommand(byte command, byte relay_code) {
  switch ( command ) {
    case ALL_ON_CMD:
      if ( relay_code == ALL_ON ) {
        _relay_module.turnAllOn();
        _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
      }
      break;
    case ALL_OFF_CMD:
      if ( relay_code == ALL_OFF ) {
        _relay_module.turnAllOff();
        _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
      }
      break;
    case SET_RELAY_CMD:
      _relay_module.setRelay( relay_code );
      _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
      break;
    case TURN_ON_CMD:
      if ( singleRelayReceived(relay_code) ) {
        _relay_module.turnOn(relay_code);
        _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
      }
      break;
    case TURN_OFF_CMD:
      if ( singleRelayReceived(relay_code) ) {
        _relay_module.turnOff(relay_code);
        _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
      }
      break;
    case TOGGLE_CMD:
      if ( singleRelayReceived(relay_code) ) {
        _relay_module.toggle(relay_code);
        _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
      }
      break;
    case TOGGLE_ALL_CMD:
      _relay_module.toggleAll();
      _relay_module.sendToRelay(SR_DATA_PIN, SR_CLOCK_PIN, SR_LATCH_PIN);
      break;
  }
}

byte AL8RelayModule_ATtiny85Driver::getRelayValue() {
  return _relay_module.getRelayValue();
}

bool AL8RelayModule_ATtiny85Driver::singleRelayReceived(byte relay_code) {
  switch (relay_code) {
    case RELAY_1:
    case RELAY_2:
    case RELAY_3:
    case RELAY_4:
    case RELAY_5:
    case RELAY_6:
    case RELAY_7:
    case RELAY_8:
      return true;
      break;
    default:
      return false;
      break;
  }
}

void AL8RelayModule_ATtiny85Driver::receiveEvent(uint8_t how_many) {
  while ( TinyWireS.available() ) {
    byte command = TinyWireS.receive(); // get the command from master
    byte relay_code = TinyWireS.receive();  // get the relay code next
    AL8RelayModule_ATtiny85Driver::setRelayFromCommand(command, relay_code);
  }
}

void AL8RelayModule_ATtiny85Driver::requestEvent() {
  TinyWireS.send(_relay_module.getRelayValue());
}

#else  /******************** Master Controller ********************/

ATtiny85Driver_Master::ATtiny85Driver_Master(byte address) {
  _address = address;
  _relays = ALL_OFF;
}

ATtiny85Driver_Master::ATtiny85Driver_Master(byte address, byte relay) {
  _address = address;
  _relays = relay;
}

void ATtiny85Driver_Master::begin() {
  Wire.begin();
  Wire.setClock(100000);
  Wire.setClockStretchLimit(40000);
  setRelay(_relays);
}

bool ATtiny85Driver_Master::turnAllOn() {
  _relays = ALL_ON;
  return issueCommandToDriver(ALL_ON_CMD, ALL_ON);
}

bool ATtiny85Driver_Master::turnAllOff() {
  _relays = ALL_OFF;
  return issueCommandToDriver(ALL_OFF_CMD, ALL_OFF);
}

bool ATtiny85Driver_Master::toggleAll() {
  _relays ^= ALL_OFF;
  return issueCommandToDriver(TOGGLE_ALL_CMD, ALL_OFF);
}

bool ATtiny85Driver_Master::turnOn(byte relay) {
  if ( singleRelayReceived(relay) ) {
    _relays &= ~relay;
    return issueCommandToDriver(TURN_ON_CMD, relay);
  } else {
    return false;
  }
}

bool ATtiny85Driver_Master::turnOff(byte relay) {
  if ( singleRelayReceived(relay) ) {
    _relays |= relay;
    return issueCommandToDriver(TURN_OFF_CMD, relay);
  } else {
    return false;
  }
}

bool ATtiny85Driver_Master::toggle(byte relay) {
  if ( singleRelayReceived(relay) ) {
    _relays ^= relay;
    return issueCommandToDriver(TOGGLE_CMD, relay);
  } else {
    return false;
  }
}

bool ATtiny85Driver_Master::setRelay(byte relay) {
  _relays = relay;
  return issueCommandToDriver(SET_RELAY_CMD, relay);
}

bool ATtiny85Driver_Master::toggleFromSerial(char serial_input) {
  switch(serial_input) {
    case '0':
    case 'F':
    case 'f':
      return turnAllOff();
      break;
    case '1':  // ASCII character 1
      return toggle(RELAY_1);
      break;
    case '2':
      return toggle(RELAY_2);
      break;
    case '3':
      return toggle(RELAY_3);
      break;
    case '4':
      return toggle(RELAY_4);
      break;
    case '5':
      return toggle(RELAY_5);
      break;
    case '6':
      return toggle(RELAY_6);
      break;
    case '7':
      return toggle(RELAY_7);
      break;
    case '8':
      return toggle(RELAY_8);
      break;
    case '9':
    case 'N':
    case 'n':
      return turnAllOn();
      break;
    case 'T':
    case 't':
      return toggleAll();
      break;
  }
}

uint8_t ATtiny85Driver_Master::getDeviceRelayValue() {
  byte byte_received;
  Wire.requestFrom(_address, byte(1) );
  while ( Wire.available() ) {
    byte_received = Wire.read();
  }

  return byte_received;
}

byte ATtiny85Driver_Master::getRelayValue() {
  return _device_relays;
}

byte ATtiny85Driver_Master::getMasterValue() {
  return _relays;
}

bool ATtiny85Driver_Master::isOn(byte relay) {
  return getRelayValue() & relay;
}

bool ATtiny85Driver_Master::isOff(byte relay) {
  return ~getRelayValue() & relay;
}

bool ATtiny85Driver_Master::issueCommandToDriver(byte command, byte relay_code) {
  Wire.beginTransmission(_address);
  Wire.write(command);
  Wire.write(relay_code);
  Wire.endTransmission();
  // delay(100);
  _device_relays = getDeviceRelayValue();

  return ( _relays == _device_relays );
}

bool ATtiny85Driver_Master::singleRelayReceived(byte relay) {
  switch (relay) {
    case RELAY_1:
    case RELAY_2:
    case RELAY_3:
    case RELAY_4:
    case RELAY_5:
    case RELAY_6:
    case RELAY_7:
    case RELAY_8:
      return true;
      break;
    default:
      return false;
      break;
  }
}

#endif
