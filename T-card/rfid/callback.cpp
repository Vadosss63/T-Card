#include "callback.h"

static std::function<void(std::vector<uint8_t>&&)> callbackWrite;
static std::function<std::vector<uint8_t>(std::vector<uint8_t>&&)> callbackRead;

void setCallbackWrite(const std::function<void (std::vector<uint8_t> &&)>& callback)
{
    callbackWrite = callback;
}

void setCallbackRead(const std::function<std::vector<uint8_t>(std::vector<uint8_t> &&)>& callback)
{
    callbackRead = callback;
}

void writeRegister(uint8_t addr, uint8_t val){
    std::vector<uint8_t> sendData = {addr, val};
    callbackWrite(std::move(sendData));
}
uint8_t readRegister(uint8_t addr)
{
    uint8_t value = 0;
    std::vector<uint8_t> readData = {addr, value};
    std::vector<uint8_t> answerData = callbackRead(std::move(readData));
    if(answerData.empty()) return value;
    return answerData.front();
}
