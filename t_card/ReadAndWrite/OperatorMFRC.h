#ifndef OPERATORMFRC_H
#define OPERATORMFRC_H

class OperatorMFRC
{
    enum SectorValue
    {
      ID_PWD = 1,
      SUM1,
      SUM2
    };

  public:

    OperatorMFRC();
    ~OperatorMFRC() = default;

    void init();

    bool checkCard();

    void end();

    uint16_t readSumFromCard();

    void writeSumToCard(uint16_t sum);

    static void dump_byte_array(byte *buffer, byte bufferSize);

  private:

    bool readPwdSum();

    bool writeAndCheck(byte sectorId);

    uint16_t readSumFromSector(byte sectorId);

    inline void addCRCToSum();

    inline bool checkCRC();

    bool checkBeffers(byte checkSize);

    void setupSector(byte s);

    inline void writeToCard();

    inline void readFromCard();

    bool checkStatus();

    bool atification();

    void setupPwdId();

    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;

    byte sector;
    byte blockAddr;
    byte trailerBlock;
    static const byte sizeWriteBuf = 16;
    byte writeBuffer[sizeWriteBuf]    = { // Данные, которые мы запишем на карту
      0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF
    };

    uint8_t pwdId[6] = {};
    uint8_t pwdSUM[6] = {};

    byte status;
    byte readBuffer[18];
    byte size = sizeof(readBuffer);

};

#endif
