#include "mqtt_handler.h"
#include "conf.h"

MQTTHandler::MQTTHandler(EthernetClient &eth): client(eth) {
  client.setServer(IPAddress(MQTT_HANDLER_HOST_IP), MQTT_HANDLER_PORT);
  client.setCallback(MQTTHandler::handle_topic);
}

void MQTTHandler::handle_packet(ParadoxPacket &packet) {
  if (!client.connected()) {
    client.connect("arduinoClient");
  } else {
    client.loop();
  }

  char label[17]; //fixing and trimming char array
  label[16] = 0;
  memcpy(&label, &packet.label, 16);
  String lbl(label);
  lbl.trim();

  char topic[50], payload[50];
  sprintf(topic, "/home_manniliiva19/paradox/%u/%u/%u", packet.event_group, packet.partition, packet.event_1);
  sprintf(payload, "{\"label_type\":%u, \"label\":%s}", packet.label_type, lbl.c_str());

  client.publish(topic, payload);
}

void MQTTHandler::handle_topic(char* topic, uint8_t* payload, unsigned int length) {

}
