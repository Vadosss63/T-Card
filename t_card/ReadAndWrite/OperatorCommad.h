#ifndef OPERATOR_COMMAND_H
#define OPERATOR_COMMAND_H

#include "CommandParser.h"
#include "OperatorMFRC.h"

extern void calcPassword(uint8_t *uid, uint8_t *pwd);

class OperatorCommad {
public:
  OperatorCommad() = default;
  ~OperatorCommad() = default;

  void init();

  void executeCommand();

private:
  inline void waitCmd();

  void readCMD();

  void parsingCMD();
  void execute();

  void sendAnswer();

  uint16_t readSumFromCard();

  void writeSumToCard();

  void activationCard();
  void deactivationCard();

  OperatorMFRC m_operatorMFRC;
  ParserCommand m_parserCommand;
  String m_readCmdBuf;
  String m_writeAnswerBuf;
  bool m_noError;
};

#endif