#include <RCSwitch.h>
#include "applogger.h"

extern SyslogLogger logger;

struct RCOutputPin {
  int pin;
  bool state;
  unsigned long last_trigger_time;
};

RCOutputPin rc_out_pins[] = {
  //{pin, state, last_trigger_time}
  {4, false, 0}
};

struct RValueToPin {
  unsigned long value;
  RCOutputPin *pin;
};

RValueToPin rc_sensors[] = {
  //{value, pin}
  {17492837, &rc_out_pins[0]},
  {13738234, &rc_out_pins[0]}
};
enum {
  numRCPins = sizeof(rc_out_pins) / sizeof(rc_out_pins[0]),
  numRCSensors = sizeof(rc_sensors) / sizeof(rc_sensors[0])
};

RCSwitch mySwitch = RCSwitch();

void rc_alarm_setup() {
  mySwitch.enableReceive(1);  // Receiver on interrupt 0 => that is pin #2

  for(unsigned int i = 0; i < numRCPins; i++) {
    pinMode(rc_out_pins[i].pin, OUTPUT);
  }
}

void rc_alarm_loop() {
  if (mySwitch.available()) {
    DEBUG_PRINT(LOG_NOTICE, F("433Mhz key received "));
    unsigned long value = mySwitch.getReceivedValue();
    DEBUG_PRINT(LOG_NOTICE, value);

    for(unsigned int i = 0; i < numRCSensors; i++) {
      if (rc_sensors[i].value == value) {
        RCOutputPin *pin = rc_sensors[i].pin;
        pin->last_trigger_time = millis();
        if (pin->state != HIGH) {
          digitalWrite(pin->pin, HIGH);
          pin->state = HIGH;
          DEBUG_PRINT(LOG_DEBUG, F("Set HIGH pin: "));
          DEBUG_PRINT(LOG_DEBUG, pin->pin);
        }
      }
    }

    // DEBUG_PRINT(LEVEL_INFO, F("Water alarm received "));
    // DEBUG_PRINT(LEVEL_INFO, mySwitch.getReceivedValue());
    // DEBUG_PRINT(LEVEL_INFO, F(" / "));
    // DEBUG_PRINT(LEVEL_INFO, mySwitch.getReceivedBitlength());
    // DEBUG_PRINT(LEVEL_INFO, F("bit "));
    // DEBUG_PRINT(LEVEL_INFO, F("Protocol: "));
    // DEBUG_PRINT(LEVEL_INFO, mySwitch.getReceivedProtocol());

    mySwitch.resetAvailable();
  }

  unsigned long time = millis();
  for(unsigned int i = 0; i < numRCPins; i++) {
    RCOutputPin *pin = &rc_out_pins[i];
    if(time - pin->last_trigger_time > 1000) { //if more than a second passed. Change pin back to LOW
      if (pin->state != LOW) {
        digitalWrite(pin->pin, LOW);
        pin->state = LOW;
        DEBUG_PRINT(LOG_DEBUG, F("Set LOW pin: "));
        DEBUG_PRINT(LOG_DEBUG, pin->pin);
      }

    } else if(time < pin->last_trigger_time) {
      pin->last_trigger_time = time;
    }
  }
}
