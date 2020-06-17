#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

enum Command {
  READ_SUM,
  WRITE_SUM,
  ACTIVATION_CARD,
  ACTIVATION_CARD_OK,
  DEACTIVATION_CARD,
  DEACTIVATION_CARD_OK,
  ERROR_CMD
};

struct ParserCommand {
  const String READ = ":0100000000";
  const String WRITE = ":0301000000";
  const String ACTIVATION = ":050000000000000000\r\n\n";
  const String DEACTIVATION = ":060000000000000000\r\n\n";
  const String READ_ANSWER = ":150";
  const String WRITE_ANSWER = ":130";
  // If ok to send else repeat request
  const String ACTIVATION_ANSWER_OK = ":051000000000000000\r\n\n";
  const String DEACTIVATION_ANSWER_OK = ":051000000000000000\r\n\n";

  const uint8_t CMD_SIZE = 11;
  const uint8_t SUM_ID = 11;
  const uint8_t CRC_ID = 17;

  Command cmd = ERROR_CMD;
  uint16_t sum = 0;
  uint16_t crc = 0x00;

  void convertFormString(const String &cmdStr);

  String convertToString();

private:
  void calcRequestCRC(const String &valStr);
  String calcAnswerCRC(const String &valStr);

  bool checkCRCRequest(const String &valStr);

  uint8_t toUint8(const String &valStr);
  uint16_t toUint16(const String &valStr);
};

#endif
