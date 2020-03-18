#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <stdint.h>
#include <vector>
void setCallbackWrite(const std::function<void (std::vector<uint8_t> &&)>& callback);
void setCallbackRead(const std::function<std::vector<uint8_t>(std::vector<uint8_t> &&)>& callback);
void writeRegister(uint8_t addr, uint8_t val);
uint8_t readRegister(uint8_t addr);

#endif // CALLBACK_H
