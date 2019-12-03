#ifndef CMD_DATA_H
#define CMD_DATA_H

#include <vector>
#include <stdint.h>

namespace CMD {
//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;

static constexpr uint8_t OK = 0x06;
static constexpr uint8_t STX = 0xF2;
static constexpr uint8_t ETX = 0x03;
static constexpr uint8_t C = 0x43;
static constexpr uint8_t P = 0x50;
static constexpr uint8_t N = 0x4E;

uint8_t getCRC(uint8_t* data, int size);

std::vector<uint8_t> getNumberCard(std::vector<uint8_t>& data);

uint16_t convertToInt(const std::vector<uint8_t>& data);

std::vector<uint8_t> getSumCard(const std::vector<uint8_t>& data);

std::vector<uint8_t> getActivate();

std::vector<uint8_t> getLock();

std::vector<uint8_t> getUnlock();

std::vector<uint8_t> getPasswordSum(std::vector<uint8_t> password = std::vector<uint8_t>{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF});

std::vector<uint8_t> getWriteSum(uint16_t sum);

std::vector<uint8_t> getSum();

std::vector<uint8_t> getStatusCard();

bool checkStatusCard(std::vector<uint8_t>& data);

};

#endif // CMD_DATA_H
