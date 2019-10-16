#ifndef ZMT_PROTOCOL_H
#define ZMT_PROTOCOL_H

// --> Protocol
typedef enum{
   VER_1_0 = 0b00010000,
   VER_1_1 = 0b00010001
}en_protocol_version_t;

typedef enum{
   WAIT_FOR_SYNCHRO_BYTE = 0,
   WAIT_FOR_SECOND_SYNCHRO_BYTE = 1,
   WAIT_FOR_VERSION = 2,
   WAIT_FOR_SIZE_BYTE = 3,
   WAIT_FOR_SIZE_SECOND_BYTE = 4,
   FILL_BUFFER = 5,
   ONE_DATA_DETECTED = 6
} en_parser_state_t ;

void rcvFrame(uint8_t byte);
static inline uint16_t calculateCRC(uint8_t * buff, uint16_t frameSize);
static inline uint16_t crc_ccitt_update (uint16_t crc, uint8_t data);
#endif // ZMT_PROTOCOL_H
