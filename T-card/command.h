#ifndef COMMAND_H
#define COMMAND_H
#include <algorithm>
#include <sstream>
#include <string>

const std::string READ_ANSWER = ":150";
const std::string WRITE = ":0301000000";
const uint8_t CRC_ID = 17;

std::string getSumCMD() { return ":0100000000000000FF\r\n\n"; }

inline uint8_t toUint8(std::string valStr) {
  return static_cast<uint8_t>(std::stoi(valStr));
}

inline uint16_t toUint16(std::string valStr) {
  return static_cast<uint16_t>(std::stoi(valStr));
}

uint16_t getSum(const std::string& valStr) {
  // Проверка заголовка
  if (valStr.compare(0, READ_ANSWER.size(), READ_ANSWER)) return 0;

  std::string sumStr = valStr.substr(READ_ANSWER.size(), 4);
  std::reverse(sumStr.begin(), sumStr.end());
  return toUint16(sumStr);
}

std::string int_to_hex(uint16_t i) {
  std::stringstream stream;
  stream << std::hex << i;
  return stream.str();
}

std::string calcCrcForWriteSum(std::string valStr) {
  uint16_t crc = 256;
  for (uint8_t i = 1; i < CRC_ID; i += 2) crc -= toUint8(valStr.substr(i, 2));

  return int_to_hex(crc);
}

std::string getWriteSumCMD(uint16_t sum) {
  std::string sumStr = WRITE;
  std::string s = std::to_string(sum);

  for (size_t i = 0; i < 4 - s.length(); ++i) sumStr += "0";

  sumStr += s;
  sumStr += "03";
  sumStr += calcCrcForWriteSum(sumStr);
  sumStr += "\r\n\n";
  return sumStr;
}

#endif  // COMMAND_H
