#ifndef CMD_INTERFACE_H
#define CMD_INTERFACE_H

#include "InitType.h"
#include "crc_xor.h"

// Карта вставлена
extern void initInsertedCard(InsertedCard* insertedCard);
// Ответ в интерфейс на запрос суммы
extern void initAnswerSumCard(AnswerSumCard* answerSumCard);
// Установка суммы
extern void setAnswerSumCard(AnswerSumCard* answerSumCard, uint16_t sum);
// Инициализация ответов для операций с картой (Запись);
extern void initAnswerOperationCard(AnswerOperationCard* answerOperationCard);
extern void setWriteAnswerOperationCard(AnswerOperationCard* answerOperationCard, enum CMD_ERROR err);
extern void setIncAnswerOperationCard(AnswerOperationCard* answerOperationCard, enum CMD_ERROR err);
extern void setDecAnswerOperationCard(AnswerOperationCard* answerOperationCard, enum CMD_ERROR err);
extern enum CMD_INTERFACE parsingCommandInterface(uint8_t* dataCMD, uint8_t size);

#endif // CMD_INTERFACE_H
