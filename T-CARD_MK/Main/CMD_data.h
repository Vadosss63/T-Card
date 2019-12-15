#ifndef _CMD_H      
#define _CMD_H

#include "InitType.h"

static uint8_t xdata CMD_OK = 0x06;
static uint8_t xdata CMD_STX = 0xF2;
static uint8_t xdata CMD_ETX = 0x03;
static uint8_t xdata CMD_C = 0x43;
static uint8_t xdata CMD_P = 0x50;
static uint8_t xdata CMD_N = 0x4E;
#define SIZE_DATA 3

// Вспомогательный класс
static ToUint16 xdata convert;

// Расчет CRC
extern uint8_t getCRC(uint8_t* d, uint8_t size);
// Получение номера карточки
extern NumderCard getNumberCard(uint8_t* d);
// Блокировка картридера
extern LockCardRider getLock(void);
// Разблокировка картридера
extern UnlockCardRider getUnlock(void);
// Проверка ответа
extern bit isValidAnswer(uint8_t d);
// Проверка пакета
extern bit isValidData(uint8_t* d, uint8_t size);
// Проверка статуса карты
extern StatusCard getStatusCard(void);
// Запись суммы на карту
extern WriteSumCard getWriteSum(uint16_t sum);
// Формирование пароля для разблокировки
extern PasswordSumCard getPasswordSum(PasswordCard password);

/*
bool checkStatusCard(std::vector<uint8_t>& data);
*/

#endif
