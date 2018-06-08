#ifndef H_PARADOX_HANDLER_MQTT
  #define H_PARADOX_HANDLER_MQTT

  #include <PubSubClient.h>
  #include <UIPEthernet.h>
  #include "handler.h"

  class MQTTHandler: public IHandler {
    PubSubClient client;

    public:
      MQTTHandler(EthernetClient &eth);
      void handle_packet(ParadoxPacket &packet);
      static void handle_topic(char* topic, uint8_t* payload, unsigned int length);
  };
#endif
