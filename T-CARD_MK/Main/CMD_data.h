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

// Расчет CRC
extern uint8_t getCRC(uint8_t* d, uint8_t size);
// Получение номера карточки
extern NumderCard getNumberCard(uint8_t* d);
// Блокировка картридера
extern LockCardRider getLock();
// Разблокировка картридера
extern UnlockCardRider getUnlock();

// Проверка ответа
bit isValidAnswer(uint8_t d);
// Проверка пакета
bit isValidData(uint8_t* d, uint8_t size);

/*
uint16_t convertToInt(const std::vector<uint8_t>& data);

std::vector<uint8_t> getSumCard(const std::vector<uint8_t>& data);

std::vector<uint8_t> getActivate();

std::vector<uint8_t> getPasswordSum(std::vector<uint8_t> password = std::vector<uint8_t>{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF});

std::vector<uint8_t> getWriteSum(uint16_t sum);

std::vector<uint8_t> getSum();

std::vector<uint8_t> getStatusCard();

bool checkStatusCard(std::vector<uint8_t>& data);
*/

#endif
