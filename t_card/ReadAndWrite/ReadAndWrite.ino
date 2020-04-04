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
    operatorMFRC.init();
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
    { /* Ждем пока не придут данные */
      delay(10);
    }
  }

  void readCMD()
  {
    readCmdBuf = Serial.readString();
  }

  void parsingCMD()
  {
    parserCommand.convertFormString(readCmdBuf);
  }

  void execute()
  {
    writeAnswerBuf = "";
    noError = operatorMFRC.checkCard();

    switch (parserCommand.cmd)
    {
    case READ_SUM:
      if (noError)
        readSumFromCard();
      break;
    case WRITE_SUM:
      if (noError)
        writeSumToCard();
      break;
    case ERROR_CMD:
      break;
    }
    if (!noError)
    {
      parserCommand.sum = 0;
    }

    writeAnswerBuf = parserCommand.convertToString();
  }

  void sendAnswer()
  {
    Serial.print(writeAnswerBuf);
  }

  uint16_t readSumFromCard()
  {
    uint16_t s = operatorMFRC.readSumFromCard();
    parserCommand.sum = s;
  }

  void writeSumToCard()
  {
    uint16_t sum = parserCommand.sum;
    noError = operatorMFRC.writeSumToCard(sum);
  }

  OperatorMFRC operatorMFRC;
  ParserCommand parserCommand;
  String readCmdBuf;
  String writeAnswerBuf;
  bool noError;
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
