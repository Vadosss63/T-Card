#include "testinterface.h"
#include <stdio.h>

void TEST_parsingCommandInterface()
{
    printf("TEST_parsingCommandInterface - BEGIN \n");

    // Проверка команды "Запрос суммы на карте"
    uint8_t dataSum[4] = {0xAB, 0x00, 0x01, 0xAA};
    if(SUM_CARD != parsingCommandInterface(dataSum, sizeof (dataSum))){
        printf("Fail SUM_CARD! \n");
    }

    uint8_t dataWrite[6] = {0xAB,	0x02,	0x02,	0x00,	0x02, 0xA9};
    if(WRITE_SUM != parsingCommandInterface(dataWrite, sizeof (dataWrite))){
        printf("Fail WRITE_SUM! \n");
    }

    uint8_t dataInc[6] = {0xAB,	0x02, 0x03, 0x00, 0x02, 0xA8};
    if(INC_SUM != parsingCommandInterface(dataInc, sizeof (dataInc))){
        printf("Fail INC_SUM! \n");
    }

    uint8_t dataDec[6] = {0xAB,	0x02, 0x04, 0x00, 0x02, 0xAF};
    if(DEC_SUM != parsingCommandInterface(dataDec, sizeof (dataDec))){
        printf("Fail DEC_SUM! \n");
    }

    printf("TEST_parsingCommandInterface - END \n");
}

void testInitInsertedCard(){
    InsertedCard insertedCard;
    initInsertedCard(&insertedCard);

    if(insertedCard.HEAD != 0xAB ||
            insertedCard.SIZE != 0x00 ||
            insertedCard.COMMAND != 0x00 ||
            insertedCard.CHECKSUM != 0xAB){
        printf("Fail testInitInsertedCard! \n");
    }
}

void testInitAnswerSumCard(){
    AnswerSumCard answerSumCard;
    initAnswerSumCard(&answerSumCard);

    if(answerSumCard.HEAD != 0xAB ||
            answerSumCard.SIZE != 0x02 ||
            answerSumCard.COMMAND != 0x01){
        printf("Fail testInitAnswerSumCard! \n");
    }
    setAnswerSumCard(&answerSumCard, 5000);

    if(answerSumCard.DATA[0] != 0x13 ||
            answerSumCard.DATA[1] != 0x88 ||
            answerSumCard.CHECKSUM != 0x33){
        printf("Fail setAnswerSumCard! \n");
    }

}

void testInitAnswerOperationCard(){
    AnswerOperationCard answerOperationCard;
    initAnswerOperationCard(&answerOperationCard);

    if(answerOperationCard.HEAD != 0xAB ||
            answerOperationCard.SIZE != 0x01 ||
            answerOperationCard.ERR != 0x00){
        printf("Fail testInitAnswerOperationCard! \n");
    }

    setWriteAnswerOperationCard(&answerOperationCard, ERROR_NO);

    if(answerOperationCard.COMMAND != 0x02 ||
            answerOperationCard.CHECKSUM != 0xA8 ||
            answerOperationCard.ERR != 0x00){
        printf("Fail setWriteAnswerOperationCard! \n");
    }

    setIncAnswerOperationCard(&answerOperationCard, ERROR_NO);

    if(answerOperationCard.COMMAND != 0x03 ||
            answerOperationCard.CHECKSUM != 0xA9 ||
            answerOperationCard.ERR != 0x00){
        printf("Fail setIncAnswerOperationCard! \n");
    }

    setDecAnswerOperationCard(&answerOperationCard, ERROR_NO);

    if(answerOperationCard.COMMAND != 0x04 ||
            answerOperationCard.CHECKSUM != 0xAE ||
            answerOperationCard.ERR != 0x00){
        printf("Fail setDecAnswerOperationCard! \n");
    }



}

void TEST_initCommandInterface()
{
    printf("TEST_initCommandInterface - BEGIN \n");
    testInitInsertedCard();
    testInitAnswerSumCard();
    testInitAnswerOperationCard();
    printf("TEST_initCommandInterface - END \n");
}
