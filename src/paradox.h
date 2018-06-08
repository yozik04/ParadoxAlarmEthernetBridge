#ifndef H_PARADOX
 #define H_PARADOX

  #include <Arduino.h>
  #include <Stream.h>
  #include "applogger.h"
  extern SyslogLogger logger;

  enum LabelType: byte {
    NO = 0,
    USER,
    ZONE,
    DOOR,
    AREA,
    MODULE,
    FUTURE
  };

  #pragma pack(push, 1)
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
  #pragma pack(pop)

  //Lets check ourselves. ParadoxPacket should be 37 bytes long
  static_assert (sizeof(ParadoxPacket) == 37, "ParadoxPacket size is not correct");

  template<class SerialClass>
  class Paradox {
    SerialClass &serial;

    bool readParadoxSerial(uint8_t *buffer);

    public:
    // Paradox(uint8_t, uint8_t, long);
    Paradox(SerialClass &serial);
    void begin();

    int getPacket(ParadoxPacket &paradoxPacket);
    void flushBuffer();
  };

  template<class SerialClass>
  Paradox<SerialClass>::Paradox(SerialClass &serial): serial(serial) {

  }

  template<class SerialClass>
  void Paradox<SerialClass>::begin() {
    // DEBUG_PRINT(LEVEL_INFO, F("Initializing Paradox serial."));
    serial.begin(PARADOX_SERIAL_SPEED);
    flushBuffer();
  }

  template<class SerialClass>
  int Paradox<SerialClass>::getPacket(ParadoxPacket &paradoxPacket) {
    uint8_t inBuffer[sizeof(ParadoxPacket)];
    if(readParadoxSerial(inBuffer)) {
      DEBUG_PRINT(LOG_INFO, F("Packet received"));
      DEBUG_PRINTDATA(LOG_DEBUG, inBuffer, sizeof(ParadoxPacket));
      if ((inBuffer[0] & 0xF0) == 0xE0) { // Does it look like a valid packet?
        uint8_t checksum = 0;
        for(uint8_t i = 0; i < sizeof(ParadoxPacket) - 1; i++) {
          checksum += inBuffer[i];
        }
        if (checksum == inBuffer[sizeof(ParadoxPacket) - 1]) {
          memcpy(&paradoxPacket, &inBuffer[0], sizeof(ParadoxPacket));
          DEBUG_PRINT(LOG_DEBUG, F("Valid packet received from paradox serial"));
          return 0;
        } else {
          DEBUG_PRINT(LOG_ERR, F("Packet checksum failed"));
          flushBuffer();
          return 3;
        }
      } else {
        DEBUG_PRINT(LOG_ERR, F("Invalid packet received from paradox serial"));
        DEBUG_PRINTDATA(LOG_ERR, inBuffer, sizeof(ParadoxPacket));
        flushBuffer();
        return 2;
      }
    };
    return 1;
  }

  template<class SerialClass>
  bool Paradox<SerialClass>::readParadoxSerial(uint8_t *buffer) {
    if((unsigned int)serial.available() >= sizeof(ParadoxPacket)) {
      byte index=0;
      while(index < sizeof(ParadoxPacket)) // Paradox packet
      {
        char ch = serial.read();
        buffer[index++]=ch;
      }
      return true;
    }
    return false;
  }

  template<class SerialClass>
  void Paradox<SerialClass>::flushBuffer() {
    while (serial.read() >= 0)
    ;
  }

#endif
