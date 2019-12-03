#include "CMD_data.h"

namespace CMD {

uint8_t getCRC(uint8_t* data, int size)
{
    uint8_t crc = 0;
    for (int i = 0; i < size; ++i)
        crc = crc ^ data[i];
    return crc;
}

std::vector<uint8_t> getNumberCard(std::vector<uint8_t>& data)
{
    return std::vector<uint8_t>(data.begin() + 12, data.begin() + 16);
}

uint16_t convertToInt(const std::vector<uint8_t>& data)
{
    if(data.size() > 8)
        return 0;
    size_t dataInt = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        size_t nb = data[i] << (8 * (uint8_t)i);
        dataInt = dataInt | nb;
    }
    return static_cast<uint16_t>(dataInt);
}

std::vector<uint8_t> getSumCard(const std::vector<uint8_t>& data)
{
    return std::vector<uint8_t>(data.begin() + 8, data.begin() + 10);
}

std::vector<uint8_t> getActivate()
{
    return std::vector<uint8_t>{STX, 0x00, 0x05, C, 0x60, 0x30, 0x41, 0x42, ETX, 0xE4};
}

std::vector<uint8_t> getLock()
{
    return std::vector<uint8_t>{STX, 0x00, 0x03, C, 0xB0, 0x30, ETX, 0x31};
}

std::vector<uint8_t> getUnlock()
{
    return std::vector<uint8_t>{STX, 0x00, 0x03, C, 0xB0, 0x31, ETX, 0x30};
}

std::vector<uint8_t> getPasswordSum(std::vector<uint8_t> password)
{
    std::vector<uint8_t> cmdPassword {STX, 0x00, 0x0E, C, 0x60, 0x33, 0x00, 0x20, 0x00, 0x02, 0x06 };
    cmdPassword.insert(cmdPassword.end(), password.begin(), password.end());
    cmdPassword.push_back(ETX);
    uint8_t crc = getCRC(cmdPassword.data(), cmdPassword.size());
    cmdPassword.push_back(crc);
    return cmdPassword;
}

std::vector<uint8_t> getWriteSum(uint16_t sum)
{
    std::vector<uint8_t> cmdWrite {0xF2, 0x00, 0x18, 0x43, 0x60, 0x33, 0x00, 0xD1, 0x02, 0x00, 0x01};
    uint8_t hb = sum >> 8;
    uint8_t lb = static_cast<uint8_t>(sum);
    cmdWrite.push_back(lb);
    cmdWrite.push_back(hb);
    std::vector<uint8_t> zeroByte {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    cmdWrite.insert(cmdWrite.end(), zeroByte.begin(), zeroByte.end());
    cmdWrite.push_back(ETX);
    uint8_t crc = getCRC(cmdWrite.data(), cmdWrite.size());
    cmdWrite.push_back(crc);
    return cmdWrite;
}

std::vector<uint8_t> getSum()
{
    return std::vector<uint8_t>{STX, 0x00, 0x08, C, 0x60, 0x33,
                0x00, 0xB0, 0x02, 0x00, 0x01, ETX, 0x5A};
}

std::vector<uint8_t> getStatusCard()
{
    return std::vector<uint8_t>{STX, 0x00, 0x03, 0x43, 0x50, 0x31, ETX, 0xD0};
}

bool checkStatusCard(std::vector<uint8_t>& data)
{
    if(data.size() < 5)
        return false;
    return data[3] == P;
}

};
