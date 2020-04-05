#include "OperatorCommad.h"

void OperatorCommad::init() { m_operatorMFRC.initConnection(); }

void OperatorCommad::executeCommand() {
  waitCmd();
  readCMD();
  parsingCMD();
  execute();
  sendAnswer();
}

inline void OperatorCommad::waitCmd() {
  while (Serial.available() < 1) {
    delay(10);
  }
}

void OperatorCommad::readCMD() { m_readCmdBuf = Serial.readString(); }

void OperatorCommad::parsingCMD() {
  m_parserCommand.convertFormString(m_readCmdBuf);
}

void OperatorCommad::execute() {
  m_writeAnswerBuf = "";
  m_noError = m_operatorMFRC.checkCard();

  switch (m_parserCommand.cmd) {
  case READ_SUM:
    if (m_noError)
      readSumFromCard();
    break;
  case WRITE_SUM:
    if (m_noError)
      writeSumToCard();
    break;
  case ERROR_CMD:
    break;
  }
  if (!m_noError) {
    m_parserCommand.sum = 0;
  }

  m_writeAnswerBuf = m_parserCommand.convertToString();
}

void OperatorCommad::sendAnswer() { Serial.print(m_writeAnswerBuf); }

uint16_t OperatorCommad::readSumFromCard() {
  uint16_t s = m_operatorMFRC.readSumFromCard();
  m_parserCommand.sum = s;
}

void OperatorCommad::writeSumToCard() {
  uint16_t sum = m_parserCommand.sum;
  m_noError = m_operatorMFRC.writeSumToCard(sum);
}
