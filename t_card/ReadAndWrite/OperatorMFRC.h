#ifndef OPERATOR_MFRC_H
#define OPERATOR_MFRC_H

class OperatorMFRC {
  enum SectorValue { ID_PWD = 6, SUM1, SUM2 };

public:
  OperatorMFRC();
  ~OperatorMFRC() = default;

  void initConnection();

  bool checkCard();

  void closeConnection();

  uint16_t readSumFromCard();

  bool writeSumToCard(uint16_t sum);

  static void dump_byte_array(byte *buffer, byte bufferSize);

  void resetKey();

  bool activateCard();
  bool deactivateCard();

  MFRC522::StatusCode getStatus() { return (MFRC522::StatusCode)m_status; }

private:
  bool setupKey(byte *keyIn, byte length);

  void readPWDSector();

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

  bool setKeys(MFRC522::MIFARE_Key *oldKeyA, MFRC522::MIFARE_Key *oldKeyB,
               MFRC522::MIFARE_Key *newKeyA, MFRC522::MIFARE_Key *newKeyB,
               int sector);

  MFRC522 m_mfrc522;
  MFRC522::MIFARE_Key key;
  MFRC522::MIFARE_Key keyB;

  byte m_sector;
  byte m_blockAddr;
  byte m_trailerBlock;
  static const byte SIZE_WRITE_BUF = 16;
  byte m_writeBuffer[SIZE_WRITE_BUF] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                        0xFF, 0xFF, 0xFF, 0xFF};

  uint8_t m_pwdSUM[6] = {};

  byte m_status;
  byte m_readBuffer[18];
  byte m_size = sizeof(m_readBuffer);

  bool m_isInitNewCard = false;
};

#endif
