#include "crc_xor.h"

uint8_t getCRC(uint8_t* d, uint8_t size)
{
    uint8_t crc = 0;
    uint8_t i;
    for (i = 0; i < size; i++){
        crc = crc ^ d[i];
    }
    return crc;
}
