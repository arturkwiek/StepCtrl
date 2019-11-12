#ifndef FRAMECOMPOSER_H
#define FRAMECOMPOSER_H

#include "stdint.h"


class FrameComposer
{

public:

#define MESSAGE_SIZE 10
#define FRAME_BORDER 5

#define FRAME_SIZE MESSAGE_SIZE + FRAME_BORDER

#define lo8(x) x & 0x00FF
#define hi8(x) (x & 0xFF00) >> 8

    typedef enum{
       VER_1_0 = 0b00010000,
       VER_1_1 = 0b00010001
    } en_protocol_version_t;

#define PROTOCOL VER_1_0

    typedef enum{
       WAIT_FOR_SYNCHRO_BYTE = 0,
       WAIT_FOR_SECOND_SYNCHRO_BYTE = 1,
       WAIT_FOR_VERSION = 2,
       WAIT_FOR_SIZE_BYTE = 3,
       WAIT_FOR_SIZE_SECOND_BYTE = 4,
       FILL_BUFFER = 5,
       ONE_DATA_DETECTED = 6
    } en_parser_state_t ;


    typedef struct {
        uint8_t B1;
        uint8_t B2;
        uint8_t B3;
        uint8_t B4;
        uint8_t B5;
        uint8_t B6;
        uint8_t B7;
        uint8_t B8;
        uint8_t B9;
        uint8_t B10;
    } st_frame_data_t;

    typedef union {
        st_frame_data_t data;
        uint8_t msg[MESSAGE_SIZE];
    }un_frame_t;

public:
    FrameComposer();

    un_frame_t message;
    uint8_t frame[FRAME_SIZE];

    en_protocol_version_t protocol_version;
    en_parser_state_t parser_state;

    void codeFrame(uint8_t *frame_to_code, uint32_t code_size, uint8_t *frame_to_send, uint32_t *send_size);
    void reciveFrame(uint8_t byte);
    static uint16_t calculateCRC(uint8_t *buff, uint16_t frameSize);
private:

    static uint16_t crc_ccitt_update(uint16_t crc, uint8_t data);
    uint8_t parse_index; //*!< index bufora odbiorczego
    uint8_t parse_frame[FRAME_SIZE]; //*!< Bufor odbiorczy
    uint16_t rcv_frame_data_size = 0; //*!< Rozmiar ramki (ilość) bajtów danych
    uint16_t rcv_frame_full_size = 0;
    uint16_t rcv_crc = 0; //*!< Suma kontrolna zawarta w ramce odbiorczej

};

#endif // FRAMECOMPOSER_H
