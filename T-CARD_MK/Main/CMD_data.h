#ifndef CMD_H
#define CMD_H

#include "InitType.h"
#include "crc_xor.h"
// ���� ������� �����������������
// ������� ��� ������ �� ��������


static uint8_t xdata CMD_OK = 0x06;
static uint8_t xdata CMD_STX = 0xF2;
static uint8_t xdata CMD_ETX = 0x03;
static uint8_t xdata CMD_C = 0x43;
static uint8_t xdata CMD_P = 0x50;
static uint8_t xdata CMD_N = 0x4E;
#define SIZE_DATA 3

// ��������������� �����
static ToUint16 xdata convert;


// ��������� ������ ��������
extern NumderCard getNumberCard(uint8_t* d);
// ���������� ����������
extern LockCardRider getLock(void);
// ������������� ����������
extern UnlockCardRider getUnlock(void);
// �������� ������
extern bit isValidAnswer(uint8_t d);
// �������� ������
extern bit isValidData(uint8_t* d, uint8_t size);
// �������� ������� �����
extern StatusCard getStatusCard(void);
// ������ ����� �� �����
extern WriteSumCard getWriteSum(uint16_t sum);
// ������������ ������ ��� �������������
extern PasswordSumCard getPasswordSum(PasswordCard password);

/*
bool checkStatusCard(std::vector<uint8_t>& data);
*/

#endif
