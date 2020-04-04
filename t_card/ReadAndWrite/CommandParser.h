#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

enum Command
{
  READ_SUM,
  WRITE_SUM,
  NEW_CARD,
  ERROR_CMD
};

struct ParserCommand
{
    const String READ =  ":0100000000";
    const String WRITE = ":0301000000";
    const String READ_ANSWER = ":150";
    const String WRITE_ANSWER = ":130";
    
    const uint8_t cmdSize = 11;
    const uint8_t sumId = 11;
    const uint8_t crcId = 17;

    Command cmd = ERROR_CMD;
    uint16_t sum = 0;
    uint16_t crc = 0x00;

    void convertFormString(const String& cmdStr);

    String convertToString();

  private:
    void calcRequestCRC(const String& valStr);
    String calcAnswerCRC(const String& valStr);

    bool checkCRCRequest(const String& valStr);

    uint8_t toUint8(const String& valStr);

    uint16_t toUint16(const String& valStr);
};

#endif
