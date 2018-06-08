#ifndef H_CONF
  #define H_CONF

  #ifndef VERBOSE_LEVEL
    #define VERBOSE_LEVEL 4
  #endif
  #ifndef SYSLOG_IP
    #define SYSLOG_IP         192,168,1,2
  #endif
  #ifndef SYSLOG_PORT
    #define SYSLOG_PORT       514
  #endif

  #ifndef USE_SOFTWARE_SERIAL
    #define USE_SOFTWARE_SERIAL 1 //For Debug mode use 1, for prod use 0
  #endif

  //Only used for Software Serial
  #if USE_SOFTWARE_SERIAL
    #define PARADOX_SERIAL_RX_PIN 8 // D8 on Nano
    #define PARADOX_SERIAL_TX_PIN 9 // D9 on Nano
  #endif


  #ifndef MAC_ADDRESS
    #define MAC_ADDRESS 0x59,0x76,0x88,0x5f,0xfb,0xd2
  #endif
  #ifndef DHCP
    #ifndef IP_ADDRESS
      #define IP_ADDRESS 192,168,1,200
    #endif
    #ifndef GATEWAY
      #define GATEWAY 192,168,1,1
    #endif
    #ifndef NETMASK
      #define NETMASK 255,255,255,0
    #endif
  #endif

  #ifndef PARADOX_SERIAL_SPEED
    #define PARADOX_SERIAL_SPEED 57600
  #endif
  #ifndef UDP_HANDLER_HOST_IP
    #define UDP_HANDLER_HOST_IP 192,168,1,2
  #endif
  #ifndef UDP_HANDLER_PORT
    #define UDP_HANDLER_PORT 5000
  #endif
  #ifndef MQTT_HANDLER_HOST_IP
    #define MQTT_HANDLER_HOST_IP 192,168,1,2
  #endif
  #ifndef MQTT_HANDLER_PORT
    #define MQTT_HANDLER_PORT 1883
  #endif
  #ifndef MQTT_HANDLER_CLIENT_ID
    #define MQTT_HANDLER_CLIENT_ID "paradox"
  #endif
  #ifndef MQTT_HANDLER_USER
    #define MQTT_HANDLER_USER ""
  #endif
  #ifndef MQTT_HANDLER_PASS
    #define MQTT_HANDLER_PASS ""
  #endif
  #ifndef MQTT_HANDLER_TOPIC_PREFIX
    #define MQTT_HANDLER_TOPIC_PREFIX "/paradox"
  #endif
#endif
