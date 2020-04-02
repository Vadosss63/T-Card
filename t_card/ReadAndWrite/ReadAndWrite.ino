#include <SPI.h>
#include <MFRC522.h>
#include "CommandParser.h"
#include "OperatorMFRC.h"

#define RST_PIN         9
#define SS_PIN          10

extern void calcPassword(uint8_t* uid, uint8_t* pwd);

class OperatorCommad
{
  public:
    OperatorCommad() = default;
    ~OperatorCommad() = default;

    void init()
    {
      operatorMFRC.init();
    }

    // Цикл ожидания команды
    void executeCommand()
    {
      waitCmd();
      readCMD();
      parsingCMD();
      execute();
      sendAnswer();
      //Завершаем Операции
      operatorMFRC.end();
    }

  private:

    inline void waitCmd()
    {
      while (Serial.available() < 1)
      { /* Ждем пока не придут данные */
        delay(10);
      }
    }

    //Чтение соманды
    void readCMD()
    {
      readCmdBuf = Serial.readString();
    }
    // Определение команды
    void parsingCMD()
    {
      parserCommand.convertFormString(readCmdBuf);
    }
    // выполнение команды
    void execute()
    {
      writeAnswerBuf = "";
      if (! operatorMFRC.checkCard())
      {
        Serial.println(F("No card"));
        return;
      }
      switch (parserCommand.cmd)
      {
        case READ_SUM:
          readSumFromCard();
          break;
        case WRITE_SUM:
          writeSumToCard();
          break;
        case ERROR_CMD:
          break;
      }

      writeAnswerBuf = parserCommand.convertToString();

    }
    // Отправка ответа
    void sendAnswer()
    {
      Serial.println(F("Answer "));
      Serial.print(writeAnswerBuf);
    }

    uint16_t readSumFromCard()
    {
      uint16_t  s = operatorMFRC.readSumFromCard();
      Serial.println(F("Summa = "));
      Serial.println(s);
      parserCommand.sum = s;
    }

    void writeSumToCard()
    {
      uint16_t sum = parserCommand.sum;
      operatorMFRC.writeSumToCard(sum);
      Serial.println(sum);
    }


    OperatorMFRC operatorMFRC;
    ParserCommand parserCommand;
    String readCmdBuf;
    String writeAnswerBuf;
};


OperatorCommad operatorCommad;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  operatorCommad.init();
  Serial.println(F("T-Card Run."));
}

void loop()
{
  operatorCommad.executeCommand();
}
