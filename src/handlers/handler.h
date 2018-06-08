#ifndef H_HANDLER
  #define H_HANDLER

  #include "paradox.h"

  class IHandler {
    public:
      virtual void handle_packet(ParadoxPacket &packet) = 0;

  };

#endif
