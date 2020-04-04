#include <SPI.h>
#include <MFRC522.h>
#include "CommandParser.h"
#include "OperatorMFRC.h"

#define RST_PIN 9
#define SS_PIN 10

extern void calcPassword(uint8_t *uid, uint8_t *pwd);

class OperatorCommad
{
public:
  OperatorCommad() = default;
  ~OperatorCommad() = default;

  void init()
  {
    m_operatorMFRC.initConnection();
  }

  void executeCommand()
  {
    waitCmd();
    readCMD();
    parsingCMD();
    execute();
    sendAnswer();
  }

private:
  inline void waitCmd()
  {
    while (Serial.available() < 1)
    { 
      delay(10);
    }
  }

  void readCMD()
  {
    m_readCmdBuf = Serial.readString();
  }

  void parsingCMD()
  {
    m_parserCommand.convertFormString(m_readCmdBuf);
  }

  void execute()
  {
    m_writeAnswerBuf = "";
    m_noError = m_operatorMFRC.checkCard();

    switch (m_parserCommand.cmd)
    {
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
    if (!m_noError)
    {
      m_parserCommand.sum = 0;
    }

    m_writeAnswerBuf = m_parserCommand.convertToString();
  }

  void sendAnswer()
  {
    Serial.print(m_writeAnswerBuf);
  }

  uint16_t readSumFromCard()
  {
    uint16_t s = m_operatorMFRC.readSumFromCard();
    m_parserCommand.sum = s;
  }

  void writeSumToCard()
  {
    uint16_t sum = m_parserCommand.sum;
    m_noError = m_operatorMFRC.writeSumToCard(sum);
  }

  OperatorMFRC m_operatorMFRC;
  ParserCommand m_parserCommand;
  String m_readCmdBuf;
  String m_writeAnswerBuf;
  bool m_noError;
};

OperatorCommad operatorCommad;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  SPI.begin();
  operatorCommad.init();
  Serial.println(F("T-Card Run."));
}

void loop()
{
  operatorCommad.executeCommand();
}
