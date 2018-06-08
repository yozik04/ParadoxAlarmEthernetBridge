# Paradox Alarm Event Bridge

This project is a source code for Arduino Nano with ENC28J60 Ethernet Module. It receives Paradox Alarm 37 byte telegrams via Serial Port, verifies them and passes to a handler.

Code is verified to work with Paradox EVO192 alarm board. It might also work with other boards as well.

You need to enable Live Event transmission on your Paradox Alarm. Setting on EVO192: [3035] - bit 7.

Baud rate of the Serial port can also be switched between 38400 and 57600: [3035] - bit 8, make sure it matches with *PARADOX_SERIAL_SPEED* environment variable.

# Motivation

As Paradox's own IP150 (Ver 4.x +) module is real sh*t and you cannot do anything useful for your smart house with that, I had to create something that does job better.

Maybe some day someone will find a way how to downgrade IP150 firmware to below 4.x, then it will be possible to work with IP150 through a service port.

## Paradox telegram

```cpp
  struct ParadoxPacket { //37 byte
       byte armstatus;       //00
       byte requested_event_nr; //01 0xFF: Event just occurred (serial port only)
       uint16_t event_nr;    //02-03
       byte century;         //04
       byte year;            //05
       byte month;           //06
       byte day;             //07
       byte hour;            //08
       byte minute;          //09
       byte event_group;     //10
       byte partition;       //11
       byte event_1;         //12
       byte event_2;         //13
       char serial_nr[4];    //14-17
       LabelType label_type; //18
       char label[16];       //19-34
       byte not_used;        //35
       byte checksum;        //36
  };
```

# Telegram handlers

There are multiple handlers available:

- **UDP handler:** Transmits paradox packet to a UDP HOST:PORT. See [server.py](/examples/server.py) for a receiving part.
- **MQTT handler:** Not yet fully implemented. Feel free to help here.
- **Pushover support:** Not implemented. Maybe some day.

# Board
The bridge board is ment to be powered directly from the Alarm board via LM2596 Step-Down module. It will take power, ground and Tx/Rx from there.

## Components

| Name                           | Price  | URL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        |
| ------------------------------ | ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Arduino Nano clone             | € 2,15 | [link](https://www.aliexpress.com/item/5PCS-Nano-3-0-Controller-Compatible-For-arduino-Nano-CH340-USB-Driver-Nano-V3-0-ATmega328/32729710918.html?spm=a2g0v.search0104.3.2.6f48e184KYpVFO&ws_ab_test=searchweb0_0,searchweb201602_3_10152_10151_10065_10344_10068_5722815_10342_10343_10340_5722915_10341_5711416_5722615_10696_10084_10083_10618_10307_10820_10821_10303_10302_5722715_10059_100031_10103_5711515_10624_10623_10622_5722515_10621_10620_10125-10621,searchweb201603_55,ppcSwitch_5&algo_expid=f9cb027d-64b3-4c0a-8893-3f8fa2ac2307-0&algo_pvid=f9cb027d-64b3-4c0a-8893-3f8fa2ac2307&transAbTest=ae803_1&priceBeautifyAB=0) |
| ENC28J60 Ethernet module       | € 3,01 | [link](https://www.aliexpress.com/item/1Pcs-ENC28J60-Ethernet-Shield-V1-0-For-Arduino-Nano-V3-Ethernet-Shield-RJ45-HR911105A-Webserver-Module/32659785447.html?spm=a2g0s.9042311.0.0.274233edaUtnmE)                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| RF 433Mhz Receiver (optional)  | € 1,71 | [link](https://www.aliexpress.com/item/1set-RF-module-433-Mhz-superheterodyne-receiver-and-transmitter-Support-ASK-OOK-small-size-low-power/32571703475.html?spm=a2g0s.9042311.0.0.274233edaUtnmE)                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| DC-DC Step-Down LM2596 12V->5V | € 0,55 | [link](https://www.aliexpress.com/item/1pcs-Ultra-small-LM2596-power-supply-module-DC-DC-BUCK-3A-adjustable-buck-module-regulator-ultra/32640207906.html?spm=a2g0s.9042311.0.0.274233edArNNv7)                                                                                                                                                                                                                                                                                                                                                                                                                                              |
| Connectors, Resistors, Wires | € 1,00 ||

## Paradox serial connector:

```
┌───────┐
│ Rx   ┌╵
│ Tx   │
│ GND  │
│ AUX+ └╷
└───────┘
```

You will need [this](https://www.ebay.com/itm/40-Sets-2-3-4-5P-2-54mm-Pitch-Terminal-Housing-Pin-Header-Connector-Adaptor-Kit/263446969764?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649) connector to plug the bridge to the alarm board's serial port.

# Additional features

## Water leak detector

Requires **RF 433Mhz Receiver**

I also use Wireless water leak detectors to trigger an alarm when a leak is detected. These are wireless sensors that transmit its unique ID via 433Mhz signal. Receiver on the board receives the signal for Arduino to decode and act accordingly.

Wireless leak detectors can be bought on [eBay](https://www.ebay.com/itm/433MHz-100m-Wireless-Water-Leakage-Sensor-Leak-Detector-For-Home-Security-Alarm/172842996909?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2060353.m2749.l2649) for $2.79 each. This seller sends them already with batteries.

**WARNING!:** You must use **Hardware Serial** on Arduino board. RF Module uses interrupt to register RF transmissions. This influences Software Serial. With Software serial you will get corrupted payloads from the Alarm.

# Building

For building the project you will need [Platformio](http://docs.platformio.org/en/latest/installation.html).

Copy `platformio.ini.dist` to `platformio.ini` and configure for your requirements.

To build: `$ platformio run`

To deploy: `$ platformio run --target upload`

**WARNING:** If you use Hardware Serial you will need to disconnect the board from the alarm to be able to upload the sketch to Arduino.

Also please see Platformio's [Quick start guide](http://docs.platformio.org/en/latest/quickstart.html)

## Configuring

In `platformio.ini`

```ini
build_flags =
; Software or Hardware serial. You can use Software serial to debug.
   -D USE_SOFTWARE_SERIAL=0
; Enable UDP Handler
   -D UDP_HANDLER=1
   -D UDP_HANDLER_HOST_IP=192,168,1,2
;   -D MQTT_HANDLER=1
;   -D MQTT_HANDLER_HOST_IP=192,168,1,2
; Enable RF 433Mhz Button modules (Leak detectors). Cannot be used with Software serial
;   -D RC_ALARM=1
; Arduino's IP Address
   -D IP_ADDRESS=192,168,1,100
   -D GATEWAY=192,168,1,1
; If you disable debug nothing will be sent to syslog
   -D DEBUG=1
   -D SYSLOG_IP=192,168,1,2
; VERBOSE_LEVEL=5 is good to get minimal messaging to syslog about reboots, invalid payloads on serial and Wireless Receiver keys
   -D VERBOSE_LEVEL=5
;5 - Notice, 6 - Info, 7 - Debug
;   -D VERBOSE_LEVEL=7 for debug
```

### Water leak detector ids:

Edit in `/src/rc_alarm.cpp`

```cpp
RCOutputPin rc_out_pins[] = {
  //{pin, state, last_trigger_time}
  {4, false, 0}
};

RValueToPin rc_sensors[] = {
  //{value, pin}
  {17492837, &rc_out_pins[0]},
  {13738234, &rc_out_pins[0]}
  //&rc_out_pins[0] is reference to RCOutputPin object
};
```

# LICENSE

[GNU General Public License v3.0](/LICENSE.md)

# TODO

- [ ] Finish MQTT support
- [ ] Pushover support for Alarm events
- [ ] Support write to Alarms's serial to Arm/Disarm
- [ ] Use preprocessor to fill rc_sensors array in rc_alarm.cpp
- [x] Support 433Mhz buttons (For water leak detectors)