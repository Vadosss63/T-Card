#include "CommandParser.h"

void ParserCommand::convertFormString(const String &cmdStr)
{
  String cmdParsing = cmdStr.substring(0, CMD_SIZE);
  if (cmdParsing.equals(READ))
  {
    cmd = READ_SUM;
    calcRequestCRC(cmdStr);
    if (!checkCRCRequest(cmdStr.substring(CRC_ID, CRC_ID + 2)))
      cmd = ERROR_CMD;
    return;
  }

  if (cmdParsing.equals(WRITE))
  {
    cmd = WRITE_SUM;
    calcRequestCRC(cmdStr);
    if (checkCRCRequest(cmdStr.substring(CRC_ID, CRC_ID + 2)))
      sum = toUint16(cmdStr.substring(SUM_ID, SUM_ID + 4));
    else
      cmd = ERROR_CMD;
    return;
  }

  cmd = ERROR_CMD;
}

String ParserCommand::convertToString()
{
  String answer;
  switch (cmd)
  {
  case READ_SUM:
    answer += READ_ANSWER;
    break;

  case WRITE_SUM:
    answer += WRITE_ANSWER;
    break;
  default:
    return answer;
  }

  String sumStr(sum);
  for (uint8_t i = 4; i != 0; --i)
  {
    if (i - 1 < sumStr.length())
      answer += sumStr.charAt(i - 1);
    else
      answer += "0";
  }
  answer += "000";
  answer += calcAnswerCRC(answer);
  answer += "\r\n";
  return answer;
}

void ParserCommand::calcRequestCRC(const String &valStr)
{
  crc = 256;
  for (uint8_t i = 1; i < CRC_ID; i += 2)
    crc -= toUint8(valStr.substring(i, i + 2));
}

String ParserCommand::calcAnswerCRC(const String &valStr)
{
  uint16_t crcL = 256 - toUint8(valStr.substring(1, 3));
  String sumStr(sum);
  uint8_t len = 4 - sumStr.length();
  while (len)
  {
    sumStr = "0" + sumStr;
    --len;
  }
  crcL -= toUint8(sumStr.substring(0, 2));
  crcL -= toUint8(sumStr.substring(2, 4));

  String crcStr = String(crcL, HEX);
  crcStr.toUpperCase();
  return crcStr;
}

bool ParserCommand::checkCRCRequest(const String &valStr)
{
  return valStr.equalsIgnoreCase(String(crc, HEX));
}

uint8_t ParserCommand::toUint8(const String &valStr)
{
  return valStr.toInt();
}

uint16_t ParserCommand::toUint16(const String &valStr)
{
  return valStr.toInt();
}
