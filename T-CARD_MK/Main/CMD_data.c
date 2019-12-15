#include "CMD_data.h"


///==================Parsing================================///

uint8_t getCRC(uint8_t* d, uint8_t size)
{
    uint8_t crc = 0;
    uint8_t i;
    for (i = 0; i < size; i++){
        crc = crc ^ d[i];
    }
    return crc;
}

bit isValidData(uint8_t* d, uint8_t size)
{
    // Прверка стартового байта
    if(d[0] != CMD_STX)
        return 0;
    // Проверки стоп байта
    if(d[size - 2] != CMD_ETX)
        return 0;
    // Проверка размерности
//    if(size != (d[SIZE_DATA] + 5))
//        return 0;
    // Проверка CRC
    if(getCRC(d, size - 1) != d[size - 1])
        return 0;

    return 1;
}

bit isValidAnswer(uint8_t d)
{
    return d == CMD_OK;
}


NumderCard getNumberCard(uint8_t* d)
{
    NumderCard nc;
    nc.b[0] = d[12];
    nc.b[1] = d[13];
    nc.b[2] = d[14];
    nc.b[3] = d[15];
    return nc;
}


uint16_t convertToUint16(uint8_t* d, uint8_t startByte)
{	
    convert.b[0] = d[startByte];
    convert.b[1] = d[startByte + 1];
    return convert.Uint16;
}

uint16_t getSumCard(uint8_t* d)
{
    return convertToUint16(d, 8);
}

bit checkStatusCard(uint8_t* d, uint8_t size)
{
    if(size < 5)
        return 0;

    return d[3] == CMD_P;
}


ActivateCard getActivate()
{
    static ActivateCard xdata ac;
    ac.b[0] = CMD_STX;
    ac.b[1] = 0x00;
    ac.b[2] = 0x05;
    ac.b[3] = CMD_C;
    ac.b[4] = 0x60;
    ac.b[5] = 0x30;
    ac.b[6] = 0x41;
    ac.b[7] = 0x42;
    ac.b[8] = CMD_ETX;
    ac.b[9] = 0xE4;
    return ac;
    // {STX, 0x00, 0x05, C, 0x60, 0x30, 0x41, 0x42, ETX, 0xE4};
}

LockCardRider getLock(void)
{
    static LockCardRider xdata lockCR;
    lockCR.b[0] = CMD_STX;
    lockCR.b[1] = 0x00;
    lockCR.b[2] = 0x03;
    lockCR.b[3] = CMD_C;
    lockCR.b[4] = 0xB0;
    lockCR.b[5] = 0x30;
    lockCR.b[6] = CMD_ETX;
    lockCR.b[7] = 0x31;
    return lockCR;
    //{STX, 0x00, 0x03, C, 0xB0, 0x30, ETX, 0x31};
}

UnlockCardRider getUnlock(void)
{
    static UnlockCardRider xdata unlockCR;
    unlockCR.b[0] = CMD_STX;
    unlockCR.b[1] = 0x00;
    unlockCR.b[2] = 0x03;
    unlockCR.b[3] = CMD_C;
    unlockCR.b[4] = 0xB0;
    unlockCR.b[5] = 0x31;
    unlockCR.b[6] = CMD_ETX;
    unlockCR.b[7] = 0x30;
    return unlockCR;
    //{STX, 0x00, 0x03, C, 0xB0, 0x31, ETX, 0x30};
}

SumCard getSum()
{
    static SumCard xdata sumCard;
    sumCard.b[0] = CMD_STX;
    sumCard.b[1] = 0x00;
    sumCard.b[2] = 0x08;
    sumCard.b[3] = CMD_C;
    sumCard.b[4] = 0x60;
    sumCard.b[5] = 0x33;
    sumCard.b[6] = 0x00;
    sumCard.b[7] = 0xB0;
    sumCard.b[8] = 0x02;
    sumCard.b[9] = 0x00;
    sumCard.b[10] = 0x01;
    sumCard.b[11] = CMD_ETX;
    sumCard.b[12] = 0x5A;
    return sumCard;
    //{STX, 0x00, 0x08, C, 0x60, 0x33, 0x00, 0xB0, 0x02, 0x00, 0x01, ETX, 0x5A};
}

StatusCard getStatusCard(void)
{
    static StatusCard xdata statusCard;
    statusCard.b[0] = CMD_STX;
    statusCard.b[1] = 0x00;
    statusCard.b[2] = 0x03;
    statusCard.b[3] = 0x43;
    statusCard.b[4] = 0x50;
    statusCard.b[5] = 0x31;
    statusCard.b[6] = CMD_ETX;
    statusCard.b[7] = 0xD0;
    return statusCard;
    //{STX, 0x00, 0x03, 0x43, 0x50, 0x31, ETX, 0xD0};
}


uint8_t xdata countZero;
WriteSumCard getWriteSum(uint16_t sum)
{
    static WriteSumCard xdata cmdWrite;
    cmdWrite.b[0] = CMD_STX;
    cmdWrite.b[1] = 0x00;
    cmdWrite.b[2] = 0x18;
    cmdWrite.b[3] = 0x43;
    cmdWrite.b[4] = 0x60;
    cmdWrite.b[5] = 0x33;
    cmdWrite.b[6] = 0x00;
    cmdWrite.b[7] = 0xD1;
    cmdWrite.b[8] = 0x02;
    cmdWrite.b[9] = 0x00;
    cmdWrite.b[10] = 0x01;

    convert.Uint16 = sum;
    cmdWrite.b[11] = convert.b[0];
    cmdWrite.b[12] = convert.b[1];

    countZero = 14;
    while(countZero){	// заполняем нулями
        cmdWrite.b[countZero + 12] = 0x00;
        countZero--;
    }

    cmdWrite.b[27] = CMD_ETX;
    cmdWrite.b[28] = getCRC(cmdWrite.b, 28);
    return cmdWrite;

    // {0xF2, 0x00, 0x18, 0x43, 0x60, 0x33, 0x00, 0xD1, 0x02, 0x00, 0x01};
    // sum{hb, lb}
    // {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 14 байт
    // {ETX , crc}
}


PasswordSumCard getPasswordSum(PasswordCard password)
{
    static PasswordSumCard xdata cmdPassword;
    cmdPassword.b[0] = CMD_STX;
    cmdPassword.b[1] = 0x00;
    cmdPassword.b[2] = 0x0E;
    cmdPassword.b[3] = CMD_C;
    cmdPassword.b[4] = 0x60;
    cmdPassword.b[5] = 0x33;
    cmdPassword.b[6] = 0x00;
    cmdPassword.b[7] = 0x20;
    cmdPassword.b[8] = 0x00;
    cmdPassword.b[9] = 0x02;
    cmdPassword.b[10] = 0x06;

    cmdPassword.b[11] = password.b[0];
    cmdPassword.b[12] = password.b[1];
    cmdPassword.b[13] = password.b[2];
    cmdPassword.b[14] = password.b[3];
    cmdPassword.b[15] = password.b[4];
    cmdPassword.b[16] = password.b[5];

    cmdPassword.b[17] = CMD_ETX;
    cmdPassword.b[18] = getCRC(cmdPassword.b, 18);
    return cmdPassword;
    // {STX, 0x00, 0x0E, C, 0x60, 0x33, 0x00, 0x20, 0x00, 0x02, 0x06 }
    // password {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    // {ETX, crc}

}
