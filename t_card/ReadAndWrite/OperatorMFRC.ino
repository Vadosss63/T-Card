#include "OperatorMFRC.h"

OperatorMFRC::OperatorMFRC() : m_mfrc522(SS_PIN, RST_PIN) {
  resetKey();
  setupSector(ID_PWD);
}

void OperatorMFRC::initConnection() { m_mfrc522.PCD_Init(); }

bool OperatorMFRC::checkCard() {
  if (m_isInitNewCard)
    return true;
  // Ждем новую карту
  if (!m_mfrc522.PICC_IsNewCardPresent()) {
    closeConnection();
    return false;
  }

  // Выбираем одну из карт
  if (!m_mfrc522.PICC_ReadCardSerial()) {
    closeConnection();
    return false;
  }
  // Показываем подробности карты
  // Serial.print(F("Card UID:"));
  // dump_byte_array(m_mfrc522.uid.uidByte, m_mfrc522.uid.size);
  // Serial.println();
  byte piccType = m_mfrc522.PICC_GetType(m_mfrc522.uid.sak);
  // Проверяем совместимость
  if (piccType != MFRC522::PICC_TYPE_MIFARE_1K)
    return false;
  setupPasswordId();

  setupKey(m_pwdId, 6);

  if (!readPwdSum()) {
    return false;
  }
  resetKey();
  m_isInitNewCard = true;
  return true;
}

void OperatorMFRC::closeConnection() {
  m_mfrc522.PICC_HaltA();
  m_mfrc522.PCD_StopCrypto1();
  m_isInitNewCard = false;
}

uint16_t OperatorMFRC::readSumFromCard() {
  uint16_t sum = 0;
  //Чтение суммы с 1 сектора
  sum = readSumFromSector(SUM1);
  if (checkCRC())
    return sum;

  //Чтение суммы с 2 сектора
  sum = readSumFromSector(SUM2);
  if (checkCRC())
    return sum;

  m_status = 2;
  return 0;
}

bool OperatorMFRC::writeSumToCard(uint16_t sum) {
  byte b0 = 0xFF & (sum >> 8);
  byte b1 = 0xFF & sum;
  m_writeBuffer[0] = b0;
  m_writeBuffer[1] = b1;
  addCRCToSum();
  bool ok = writeAndCheck(SUM1);
  ok &= writeAndCheck(SUM2);
  return ok;
}

void OperatorMFRC::dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void OperatorMFRC::resetKey() {
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

bool OperatorMFRC::activateCard() {
  resetKey();
  MFRC522::MIFARE_Key keyPwd;
  for (byte i = 0; i < 6; i++) {
    keyPwd.keyByte[i] = m_pwdId[i];
  }
  setKeys(&key, &key, &keyPwd, &key, ID_PWD);
}

bool OperatorMFRC::setupKey(byte *keyIn, byte length) {
  for (byte i = 0; i < length; i++) {
    key.keyByte[i] = keyIn[i];
  }
}

bool OperatorMFRC::readPwdSum() {
  setupSector(ID_PWD);
  // Сделать установку пароля для ID
  if (!loginIn())
    return false;

  readFromCard();

  dump_byte_array(m_readBuffer, m_size);
  return true;
}

bool OperatorMFRC::writeAndCheck(byte sectorId) {
  setupSector(sectorId);
  if (!loginIn())
    return false;

  writeToCard();
  if (!checkStatus())
    return false;

  readFromCard();
  if (!checkStatus())
    return false;

  return checkBuffers(3);
}

uint16_t OperatorMFRC::readSumFromSector(byte sectorId) {
  uint16_t sum = 0;
  setupSector(sectorId);
  if (!loginIn())
    return sum;

  readFromCard();
  sum = ((m_readBuffer[0] << 8) & 0xFF00) | m_readBuffer[1];
  return sum;
}

inline void OperatorMFRC::addCRCToSum() {
  m_writeBuffer[2] = m_writeBuffer[0] + m_writeBuffer[1];
}

inline bool OperatorMFRC::checkCRC() {
  bool ok = (byte)(m_readBuffer[0] + m_readBuffer[1]) == m_readBuffer[2];
  m_readBuffer[2] += 0xF0;
  return ok;
}

bool OperatorMFRC::checkBuffers(byte checkSize) {
  for (byte i = 0; i < checkSize; ++i) {
    if (m_readBuffer[i] != m_writeBuffer[i])
      return false;
  }
  return true;
}

void OperatorMFRC::setupSector(byte s) {
  m_sector = s;
  m_blockAddr = 4 * m_sector;
  m_trailerBlock = m_blockAddr + 3;
}

inline void OperatorMFRC::writeToCard() {
  // Записываем данные в блок
  m_status = m_mfrc522.MIFARE_Write(m_blockAddr, m_writeBuffer, SIZE_WRITE_BUF);
}

inline void OperatorMFRC::readFromCard() {
  // Читаем данные из блока
  m_size = sizeof(m_readBuffer); // FIX A buffer is not big enough.
  m_status = m_mfrc522.MIFARE_Read(m_blockAddr, m_readBuffer, &m_size);
}

bool OperatorMFRC::checkStatus() {
  if (m_status != MFRC522::STATUS_OK) {
    //    Serial.print(F("Error: "));
    //    Serial.println(mfrc522.GetStatusCodeName(status));
    closeConnection();
    return false;
  }
  return true;
}

bool OperatorMFRC::loginIn() {
  m_status = m_mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                        m_trailerBlock, &key, &(m_mfrc522.uid));
  return checkStatus();
}

void OperatorMFRC::setupPasswordId() {
  calcPassword(m_mfrc522.uid.uidByte, m_pwdId);
  Serial.print(F("Password is :"));
  dump_byte_array(m_pwdId, 6);
  Serial.println();
}

bool OperatorMFRC::setKeys(MFRC522::MIFARE_Key *oldKeyA,
                           MFRC522::MIFARE_Key *oldKeyB,
                           MFRC522::MIFARE_Key *newKeyA,
                           MFRC522::MIFARE_Key *newKeyB, int sector) {

  byte trailerBlock = sector * 4 + 3;
  byte buffer[18];
  byte size = sizeof(buffer);

  m_status = (MFRC522::StatusCode)m_mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, oldKeyA, &(m_mfrc522.uid));
  if (m_status != MFRC522::STATUS_OK)
    return false;

  m_status =
      (MFRC522::StatusCode)m_mfrc522.MIFARE_Read(trailerBlock, buffer, &size);
  if (m_status != MFRC522::STATUS_OK)
    return false;

  m_status = (MFRC522::StatusCode)m_mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, oldKeyB, &(m_mfrc522.uid));
  if (m_status != MFRC522::STATUS_OK)
    return false;

  if (newKeyA != nullptr || newKeyB != nullptr) {
    for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
      if (newKeyA != nullptr) {
        buffer[i] = newKeyA->keyByte[i];
      }
      if (newKeyB != nullptr) {
        buffer[i + 10] = newKeyB->keyByte[i];
      }
    }
  }

  // Write data to the block
  m_status =
      (MFRC522::StatusCode)m_mfrc522.MIFARE_Write(trailerBlock, buffer, 16);
  if (m_status != MFRC522::STATUS_OK)
    return false;

  // Read data from the block (again, should now be what we have written)
  m_status =
      (MFRC522::StatusCode)m_mfrc522.MIFARE_Read(trailerBlock, buffer, &size);
  if (m_status != MFRC522::STATUS_OK)
    return false;

  return true;
}
