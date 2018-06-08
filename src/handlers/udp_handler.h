#ifndef H_PARADOX_HANDLER_UDP
  #define H_PARADOX_HANDLER_UDP

  #include <UIPEthernet.h>
  #include "handler.h"

  class UDPHandler: public IHandler {
    EthernetUDP udp;

    public:
    UDPHandler(EthernetUDP &_udp);
    void handle_packet(ParadoxPacket &packet);
  };
#endif
