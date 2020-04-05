#ifndef OPERATOR_MFRC_H
#define OPERATOR_MFRC_H

class OperatorMFRC {
  enum SectorValue { ID_PWD = 1, SUM1, SUM2 };

public:
  OperatorMFRC();
  ~OperatorMFRC() = default;

  void initConnection();

  bool checkCard();

  void closeConnection();

  uint16_t readSumFromCard();

  bool writeSumToCard(uint16_t sum);

  static void dump_byte_array(byte *buffer, byte bufferSize);

private:
  bool readPwdSum();

  bool writeAndCheck(byte sectorId);

  uint16_t readSumFromSector(byte sectorId);

  inline void addCRCToSum();

  inline bool checkCRC();

  bool checkBuffers(byte checkSize);

  void setupSector(byte s);

  inline void writeToCard();

  inline void readFromCard();

  bool checkStatus();

  bool loginIn();

  void setupPasswordId();

  MFRC522 m_mfrc522;
  MFRC522::MIFARE_Key key;

  byte m_sector;
  byte m_blockAddr;
  byte m_trailerBlock;
  static const byte SIZE_WRITE_BUF = 16;
  byte m_writeBuffer[SIZE_WRITE_BUF] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                        0xFF, 0xFF, 0xFF, 0xFF};

  uint8_t m_pwdId[6] = {};
  uint8_t m_pwdSUM[6] = {};

  byte m_status;
  byte m_readBuffer[18];
  byte m_size = sizeof(m_readBuffer);

  bool m_isInitNewCard = false;
};

#endif
