#ifndef H_LOGGING_ALARM
 #define H_LOGGING_ALARM

 #define DEBUG_BUFFER_SIZE       80

  #define LOG_EMERG 0 /* system is unusable */
  #define LOG_ALERT 1 /* action must be taken immediately */
  #define LOG_CRIT  2 /* critical conditions */
  #define LOG_ERR   3 /* error conditions */
  #define LOG_WARNING 4 /* warning conditions */
  #define LOG_NOTICE  5 /* normal but significant condition */
  #define LOG_INFO  6 /* informational */
  #define LOG_DEBUG 7 /* debug-level messages */

  #include <UIPEthernet.h>
  #include "conf.h"

  class SyslogLogger {
      IPAddress _targetDebugIp;
      EthernetUDP &_udp;
      bool &_ethernetInitialized;

      char debugBuf[DEBUG_BUFFER_SIZE];

    public:
      void print(uint16_t pri, const __FlashStringHelper* pData);
      void print(uint16_t pri, const char* data);
      void print(uint16_t pri, const char* data, unsigned int len, bool hex=false);
      void print(uint16_t pri, long unsigned int data);
      void print(uint16_t pri, const uint8_t *data, unsigned int len);

      SyslogLogger(EthernetUDP &udp, bool &ethernetInitialized);
  };

  #ifdef DEBUG
    #define DEBUG_PRINT(level, message) if (VERBOSE_LEVEL >= level) { logger.print(level, message); }
    #define DEBUG_PRINTDATA(level, data, len) if (VERBOSE_LEVEL >= level) { logger.print(level, data, len); }
  #else
    #define DEBUG_PRINT(level, message)
    #define DEBUG_PRINTDATA(level, data, len)
  #endif
#endif
