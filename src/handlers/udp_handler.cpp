#include "udp_handler.h"

UDPHandler::UDPHandler(EthernetUDP &_udp): udp(_udp) {

}

void UDPHandler::handle_packet(ParadoxPacket &packet) {
  int success;
  unsigned long next = millis()+5000;
  do {
    success = udp.beginPacket(IPAddress(UDP_HANDLER_HOST_IP),UDP_HANDLER_PORT);
  } while (!success && (millis() < next)); //Wait maximum for 5 seconds
  if (success) {
    success = udp.write((char *) &packet, sizeof(ParadoxPacket));
    success = udp.endPacket();
  }
  // udp.stop();
}
