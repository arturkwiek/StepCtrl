#include <QDebug>
#include "zmt_protocol.h"

static uint8_t parse_index; //*!< index bufora odbiorczego
static uint8_t parse_frame[256]; //*!< Bufor odbiorczy
uint16_t rcv_frame_data_size = 0; //*!< Rozmiar ramki (ilość) bajtów danych
uint16_t rcv_frame_full_size = 0;
uint16_t rcv_crc = 0; //*!< Suma kontrolna zawarta w ramce odbiorczej

en_protocol_version_t protocol_version = VER_1_1;
en_parser_state_t parser_state = WAIT_FOR_SYNCHRO_BYTE;

#define lo8(x) x & 0x00FF
#define hi8(x) (x & 0xFF00) >> 8

void codeFrame(uint8_t* frame_to_code,uint32_t code_size, uint8_t* frame_to_send, uint32_t* send_size ) {
    /** Formatuje dane w ramce nadawczej komunikatu.*/

    if(protocol_version == VER_1_1) {
        *send_size = 0;

        //ADD HEADERS
        frame_to_send[(*send_size)++] = '1';
        frame_to_send[(*send_size)++] = '1';

        //ADD SIZE TO FRAME
        uint16_t size = code_size;
        uint8_t size_lo;
        uint8_t size_hi;

        size_lo = (uint8_t)(size & 0x00FF);
        size_hi = (uint8_t)((size >> 8) & 0x00FF);

        frame_to_send[(*send_size)++] = size_lo;
        if(size_lo == '1')
            frame_to_send[(*send_size)++] = '0';

        frame_to_send[(*send_size)++] = size_hi;
        if(size_hi == '1')
            frame_to_send[(*send_size)++] = '0';

        //ADD DATA
        for (int i = 0; i < code_size; i++) {
            frame_to_send[(*send_size)++] = frame_to_code[i];
            if (frame_to_code[i] == '1') {
                frame_to_send[(*send_size)++] = '0';
            }
        }

        uint16_t send_data_crc;
        uint8_t send_data_crc_hi;
        uint8_t send_data_crc_lo;
        //qDebug() << "SFRAME: ";
        send_data_crc = calculateCRC(frame_to_code, size);
        send_data_crc_lo = send_data_crc & 0xff;
        send_data_crc_hi = (send_data_crc >> 8) & 0xff;
        //cDebug() << "SCRC16: " << send_data_crc << _endl;

        frame_to_send[(*send_size)++] = send_data_crc_lo;//*((uint8_t *) &send_data_crc);

        if(frame_to_send[(*send_size)-1] == '1'){
            frame_to_send[(*send_size)++] = '0';
        }
        frame_to_send[(*send_size)++] = send_data_crc_hi;//*(((uint8_t *) &send_data_crc) + 1);

        if(frame_to_send[(*send_size)-1] == '1'){
            frame_to_send[(*send_size)++] = '0';
        }

    } else if(protocol_version == VER_1_0) {

        *send_size = 0;

        //ADD HEADERS
        frame_to_send[(*send_size)++] = '1';
        frame_to_send[(*send_size)++] = '1';

        //ADD SIZE TO FRAME
        uint16_t size = code_size;
        uint8_t size_lo;

        size_lo = (uint8_t)(size & 0x00FF);

        frame_to_send[(*send_size)++] = size_lo;
        if(size_lo == '1')
            frame_to_send[(*send_size)++] = '0';

        //ADD DATA
        for (int i = 0; i < code_size; i++) {
            frame_to_send[(*send_size)++] = frame_to_code[i];
            if (frame_to_code[i] == '1') {
                frame_to_send[(*send_size)++] = '0';
            }
        }

        uint16_t send_data_crc;
        send_data_crc = calculateCRC(frame_to_code,code_size);

        frame_to_send[(*send_size)++] = *((uint8_t *) &send_data_crc);

        if(frame_to_send[(*send_size)-1] == '1'){
            frame_to_send[(*send_size)++] = '0';
        }

        frame_to_send[(*send_size)++] = *(((uint8_t *) &send_data_crc) + 1);

        if(frame_to_send[(*send_size)-1] == '1'){
            frame_to_send[(*send_size)++] = '0';
        }

    }
}
//-------------------------------------------- CBR standard frame code -->

void rcvFrame(uint8_t byte) {

    int i;
    i = 0;

    switch(parser_state) {
    case WAIT_FOR_SYNCHRO_BYTE:
        if(byte == '1') {
            parser_state = WAIT_FOR_SECOND_SYNCHRO_BYTE;
            parse_index = 0;
            parse_frame[parse_index++] = '1';
        }
        break;

    case WAIT_FOR_SECOND_SYNCHRO_BYTE:
        if(byte == '1') {
            parser_state = WAIT_FOR_SIZE_BYTE;
            parse_frame[parse_index++] = '1';
        } else {
            parser_state = WAIT_FOR_SYNCHRO_BYTE;
        }
        break;

    case WAIT_FOR_SIZE_BYTE:
        rcv_frame_data_size = byte;
        rcv_frame_full_size = rcv_frame_data_size + 5;
        parse_frame[parse_index++] = rcv_frame_data_size;
        parser_state = FILL_BUFFER;
        break;

    case FILL_BUFFER:
        if(byte == '1') {
            parser_state = ONE_DATA_DETECTED;
        } else {
            parse_frame[parse_index++] = byte;
            if(parse_index == rcv_frame_full_size) {
                parser_state = WAIT_FOR_SYNCHRO_BYTE;
                uint8_t* p_byte;
                uint16_t rcv_crc;
                p_byte = (uint8_t*)&rcv_crc;
                *p_byte = parse_frame[parse_index-2];
                *(p_byte + 1) = parse_frame[parse_index-1];
                uint16_t calculated_crc;
                calculated_crc = calculateCRC(parse_frame,rcv_frame_data_size);
                if(calculated_crc != rcv_crc) {
                    //err
                } else {
                    //instance->MyProtocol_onFrameArrive( &(instance->pbuffer[DATA_OFFSET_VER_1_0]), instance->receivedFrameDataSize);
                }
                parse_index = 0;
            }
        }
        break;

    case ONE_DATA_DETECTED:
        if(byte == '0') {
            parse_frame[parse_index++] = '1';
            parser_state = FILL_BUFFER;
            if(parse_index == rcv_frame_full_size) {
                parser_state = WAIT_FOR_SYNCHRO_BYTE;
                uint8_t* p_bajt;
                uint16_t rcv_crc;
                p_bajt= (uint8_t*) & rcv_crc;
                *p_bajt = parse_frame[parse_index - 2];
                *(p_bajt +1) = parse_frame[parse_index - 1];
                uint16_t calculated_rcv_crc;
                calculated_rcv_crc = calculateCRC(parse_frame, rcv_frame_data_size);
                if(calculated_rcv_crc != rcv_crc) {
                    //crc err
                } else {
                    // instance->MyProtocol_onFrameArrive(&(instance->pbuffer[DATA_OFFSET_VER_1_0]), instance->receivedFrameDataSize);
                }
                parse_index = 0;
            }
        } else {
            parser_state = WAIT_FOR_SYNCHRO_BYTE;
            parse_index = 0;
        }
        break;

    default:
        break;
    }
}


static inline uint16_t crc_ccitt_update (uint16_t crc, uint8_t data){
    /** AVR Libc Reference Manual
    <util/crc16.h>: CRC Computations
    Optimized CRC-CCITT calculation.

    Polynomial: x^16 + x^12 + x^5 + 1 (0x8408) Initial value: 0xffff.
    This is the CRC used by PPP and IrDA.
     */
    data ^= lo8 (crc);
    data ^= data << 4;

    return ((((uint16_t)data << 8) | hi8 (crc)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
}

static inline uint16_t calculateCRC(uint8_t * buff, uint16_t frameSize) {
    uint16_t tmp_crc = 0xFFFF;
    for(uint16_t k = 0; k < frameSize; k++){
        tmp_crc = crc_ccitt_update(tmp_crc, buff[k]);
    }
    return tmp_crc;
}
