#include "cmd_interface.h"

// Карта вставлена
void initInsertedCard(InsertedCard* insertedCard){
    insertedCard->HEAD = HEAD_CONST;
    insertedCard->SIZE = 0x00;
    insertedCard->COMMAND = INSERTED_CARD;
    insertedCard->CHECKSUM = 0xAB;
    // 0xAB	0x00	0x00	CHECKSUM
}

// Ответ в интерфейс на запрос суммы
void initAnswerSumCard(AnswerSumCard* answerSumCard){
    answerSumCard->HEAD = HEAD_CONST;
    answerSumCard->SIZE = 0x02;
    answerSumCard->COMMAND = SUM_CARD;
    answerSumCard->DATA[0] = 0x00;
    answerSumCard->DATA[1] = 0x00;
    answerSumCard->CHECKSUM = 0x00;
    // 0xAB	0x02 0x01	SUMM_H	SUMM_L	CHECKSUM
}

// Установка суммы
void setAnswerSumCard(AnswerSumCard* answerSumCard, uint16_t sum){
    ToUint16 sum16;
    sum16.Uint16 = sum;
    answerSumCard->DATA[0] = sum16.b[1];
    answerSumCard->DATA[1] = sum16.b[0];
    answerSumCard->CHECKSUM = getCRC((uint8_t*)answerSumCard, sizeof(AnswerSumCard) - 1);
}

// Инициализация ответов для операций с картой (Запись);
void initAnswerOperationCard(AnswerOperationCard* answerOperationCard)
{
    answerOperationCard->HEAD = HEAD_CONST;
    answerOperationCard->SIZE = 0x01;
    answerOperationCard->ERR = 0x00;
}

void setWriteAnswerOperationCard(AnswerOperationCard* answerOperationCard, enum CMD_ERROR err)
{
    answerOperationCard->ERR = (uint8_t)err;
    answerOperationCard->COMMAND = WRITE_SUM;
    answerOperationCard->CHECKSUM = getCRC((uint8_t*)answerOperationCard, sizeof(AnswerOperationCard) - 1);
}

void setIncAnswerOperationCard(AnswerOperationCard* answerOperationCard, enum CMD_ERROR err)
{
    answerOperationCard->ERR = (uint8_t)err;
    answerOperationCard->COMMAND = INC_SUM;
    answerOperationCard->CHECKSUM = getCRC((uint8_t*)answerOperationCard, sizeof(AnswerOperationCard) - 1);
}

void setDecAnswerOperationCard(AnswerOperationCard* answerOperationCard, enum CMD_ERROR err)
{
    answerOperationCard->ERR = (uint8_t)err;
    answerOperationCard->COMMAND = DEC_SUM;
    answerOperationCard->CHECKSUM = getCRC((uint8_t*)answerOperationCard, sizeof(AnswerOperationCard) - 1);
}

enum CMD_INTERFACE parsingCommandInterface(uint8_t* dataCMD, uint8_t size){
    // Проверка заголовка
    if(dataCMD[0] != HEAD_CONST)
        return ERROR_PARSING;
    // Проверка CRC
    if(dataCMD[size - 1] != getCRC(dataCMD, size - 1))
        return ERROR_PARSING;
    // Определение команды
    return (enum CMD_INTERFACE)dataCMD[2];
}

ToUint16 parsingOperationSumCard(uint8_t* dataCMD){
    ToUint16 sum;
    sum.b[0] = dataCMD[4];
    sum.b[1] = dataCMD[3];
    return sum;
}


