#include "applogger.h"

#define LOG_PRIMASK 0x07
#define LOG_PRI(p)  ((p) & LOG_PRIMASK)
#define LOG_MAKEPRI(fac, pri) (((fac) << 3) | (pri))

SyslogLogger::SyslogLogger(EthernetUDP &udp, bool &ethernetInitialized):
_udp(udp), _ethernetInitialized(ethernetInitialized), _targetDebugIp(SYSLOG_IP)
{

}

void SyslogLogger::print(uint16_t pri, const char* data) {
  print(pri, data, strlen(data));
}

void SyslogLogger::print(uint16_t pri, const __FlashStringHelper* pData ) {
  // assert m_instance;
  int cursor = 0;
  char *ptr = ( char * ) pData;

  while( ( debugBuf[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' ) ++cursor;
  print(pri, debugBuf, strlen(debugBuf) );
}

void SyslogLogger::print(uint16_t pri, const char * data, unsigned int len, bool hex=false) {
  if (_ethernetInitialized)
  {
    int success;
    unsigned long next = millis()+5000;
    do {
      success = _udp.beginPacket(_targetDebugIp, SYSLOG_PORT);
    } while (!success && (millis() < next)); //Wait maximum for 5 seconds
    if (success) {
      _udp.write("<");
      char hex_pri[3];
      sprintf(hex_pri, "%d", LOG_MAKEPRI(1, pri)); //Facility 1 is user space messages
      _udp.write(hex_pri);
      _udp.write(">paradox paradox: ");

      if(hex) {
        char hex_buffer[3];
        
        for (unsigned int i = 0; i < len; i++) {
          sprintf(hex_buffer, "%02X", data[i]);
          #ifdef USE_SOFTWARE_SERIAL
            Serial.print(hex_buffer);
          #endif
          _udp.write(hex_buffer, 2);
        }
      } else {
        #ifdef USE_SOFTWARE_SERIAL
          Serial.print(data);
        #endif

        _udp.write(data, len);
      }

      success = _udp.endPacket();
    }
  }
}

void SyslogLogger::print(uint16_t pri, const uint8_t * data, unsigned int len) {
  // assert m_instance;
  print(pri, (const char *) data, len, true);
}

void SyslogLogger::print(uint16_t pri, long unsigned int data) {
  sprintf(debugBuf, "%lu", data);
  print(pri, debugBuf, strlen(debugBuf));
}
