#include <UIPEthernet.h>

#include "conf.h"

#include "paradox.h"

#ifdef MQTT_HANDLER
  #include "handlers/mqtt_handler.h"
#endif
// #include "handlers/syslog_handler.h"
#ifdef UDP_HANDLER
  #include "handlers/udp_handler.h"
#endif

#ifdef RC_ALARM
  #include "rc_alarm.h"
#endif

#if USE_SOFTWARE_SERIAL
  #include <SoftwareSerial.h>
  SoftwareSerial paradox_serial(PARADOX_SERIAL_RX_PIN, PARADOX_SERIAL_TX_PIN);
  Paradox<SoftwareSerial> paradox(paradox_serial);
#else
  #include "HardwareSerial.h"
  Paradox<HardwareSerial> paradox(Serial);
#endif



bool ethernetInitialized = false;
EthernetUDP udp;

#include "applogger.h"
SyslogLogger logger(udp, ethernetInitialized);

/// NETWORK

void initializeAlarmEthernet() {
  static const byte PROGMEM mac[] = {MAC_ADDRESS};
  #ifdef DHCP
    Ethernet.begin(mac);
  #else
    static const IPAddress PROGMEM ip(IP_ADDRESS);
    static const IPAddress PROGMEM gateway(GATEWAY);
    static const IPAddress PROGMEM subnet(NETMASK);
    Ethernet.begin(mac, ip, gateway, subnet);
  #endif

  ethernetInitialized = true;
}

#ifdef UDP_HANDLER
  UDPHandler udp_handler(udp);
#endif
#ifdef MQTT_HANDLER
  EthernetClient eth;
  MQTTHandler mqtt_handler(eth);
#endif

void setup() {
  #if USE_SOFTWARE_SERIAL
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  #endif

  initializeAlarmEthernet();
  DEBUG_PRINT(LOG_NOTICE, F("Booting...\n"));

  paradox.begin();

  #ifdef RC_ALARM
    DEBUG_PRINT(LOG_NOTICE, F("Initializing RCAlarm...\n"));
    rc_alarm_setup();
  #endif

  DEBUG_PRINT(LOG_NOTICE, F("Boot finished\n"));
}

void loop() {
  #ifdef RC_ALARM
    rc_alarm_loop();
  #endif

  ParadoxPacket packet;
  int result = paradox.getPacket(packet);
  if(result == 0){
    DEBUG_PRINT(LOG_DEBUG, F("packet received\n"));
    #ifdef DEBUG
      DEBUG_PRINT(LOG_DEBUG, F("packet.armstatus: "));
      DEBUG_PRINT(LOG_DEBUG, packet.armstatus);
      DEBUG_PRINT(LOG_DEBUG, F("packet.event_group: "));
      DEBUG_PRINT(LOG_DEBUG, packet.event_group);
      DEBUG_PRINT(LOG_DEBUG, F("packet.event_1: "));
      DEBUG_PRINT(LOG_DEBUG, packet.event_1);
      DEBUG_PRINT(LOG_DEBUG, F("packet.partition: "));
      DEBUG_PRINT(LOG_DEBUG, packet.partition);
      DEBUG_PRINT(LOG_DEBUG, F("packet.label_type: "));
      DEBUG_PRINT(LOG_DEBUG, packet.label_type);

      #if (VERBOSE_LEVEL >= LOG_DEBUG)
        char label[17]; //fixing and trimming char array
        label[16] = 0;
        memcpy(&label, &packet.label, 16);
        String lbl(label);
        lbl.trim();
        DEBUG_PRINT(LOG_DEBUG, F("packet.label: "));
        DEBUG_PRINT(LOG_DEBUG, lbl.c_str());
      #endif
    #endif

    #ifdef UDP_HANDLER
      udp_handler.handle_packet(packet);
    #endif
    #ifdef MQTT_HANDLER
      mqtt_handler.handle_packet(packet);
    #endif
  }
}
