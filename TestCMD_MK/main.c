#include <stdio.h>
#include "CMD_data.h"
#include "testinterface.h"

void printData(uint8_t* data, uint8_t size){
    printf("Print data\n");
    printf("Sise data %02d\n", size);
    for (int i = 0; i < size; ++i) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
}

void testGetStatusCard()
{
    printf("Start test GetStatusCard\n");
    StatusCard statusCard = getStatusCard();
    printData(statusCard.b, sizeof (statusCard));
    uint8_t crc = getCRC(statusCard.b, sizeof (statusCard) -   1);
    printf("CRC 0x%02X \n", crc);
    printf("End Test\n");
}

void testGetWriteSum(uint8_t sum)
{
    printf("Start test GetWriteSum\n");
    WriteSumCard writeSumCard = getWriteSum(sum);
    printData(writeSumCard.b, sizeof (writeSumCard));
    uint8_t crc = getCRC(writeSumCard.b, sizeof (writeSumCard) -   1);
    printf("CRC 0x%02X \n", crc);
    printf("End Test\n");
}

void testGetPasswordSum(PasswordCard password)
{
    printf("Start test GetPasswordSum\n");
    PasswordSumCard passwordSum = getPasswordSum(password);
    printData(passwordSum.b, sizeof (passwordSum));
    uint8_t crc = getCRC(passwordSum.b, sizeof (passwordSum) -   1);
    printf("CRC 0x%02X \n", crc);
    printf("End Test\n");
}


int main()
{    
    //    PasswordCard password;
    //    password.b16[0] = 0xFFFF;
    //    password.b16[1] = 0xFFFF;
    //    password.b16[2] = 0xFFFF;
    //    testGetPasswordSum(password);
    //    StatusCard statusCard = getStatusCard();
    //    bit val = isValidData(statusCard.b, sizeof(statusCard));

    TEST_parsingCommandInterface();
    TEST_initCommandInterface();
    return 0;
}
