#include "OperatorMFRC.h"



OperatorMFRC::OperatorMFRC(): mfrc522(SS_PIN, RST_PIN)
{
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  setupSector(ID_PWD);
}

void OperatorMFRC::init()
{
  mfrc522.PCD_Init();
}

bool OperatorMFRC::checkCard()
{
  // Ждем новую карту
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return false;

  // Выбираем одну из карт
  if ( ! mfrc522.PICC_ReadCardSerial())
    return false;

  // Показываем подробности карты
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  // Проверяем совместимость
  if (piccType != MFRC522::PICC_TYPE_MIFARE_1K)
    return false;
  setupPwdId();
  return true;
}

void OperatorMFRC::end()
{
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

uint16_t OperatorMFRC::readSumFromCard()
{
  uint16_t sum = 0;
  //Чтение суммы с 1 сектора
  sum = readSumFromSector(SUM1);
  if (checkCRC())
    return sum;

  //Чтение суммы с 2 сектора
  sum = readSumFromSector(SUM2);
  if (checkCRC())
    return sum;

  return 0;
}

void OperatorMFRC::writeSumToCard(uint16_t sum)
{
  byte b0 = 0xFF & (sum >> 8);
  byte b1 = 0xFF &  sum;
  writeBuffer[0] = b0;
  writeBuffer[1] = b1;
  addCRCToSum();
  if (!writeAndCheck(SUM1))
  {
    Serial.println(F("Erroe write"));
  }
  if (!writeAndCheck(SUM2))
  {
    Serial.println(F("Erroe write"));
  }
}

void OperatorMFRC::dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

bool OperatorMFRC::readPwdSum()
{
  setupSector(ID_PWD);
  // Сделать установку пароля для ID
  if (!atification())
    return false;

  readFromCard();

  return true;
}

bool OperatorMFRC::writeAndCheck(byte sectorId)
{
  setupSector(sectorId);
  if (!atification())
    return false;
  writeToCard();

  if (!checkStatus())
    return false;

  readFromCard();
  if (!checkStatus())
    return false;

  return checkBeffers(3);
}

uint16_t OperatorMFRC::readSumFromSector(byte sectorId)
{
  uint16_t sum = 0;
  setupSector(sectorId);
  if (!atification())
    return sum;

  readFromCard();
  sum = ((readBuffer[0] << 8) & 0xFF00) | readBuffer[1];
  return sum;
}

inline void OperatorMFRC::addCRCToSum()
{
  writeBuffer[2] = writeBuffer[0] + writeBuffer[1];
}

inline bool OperatorMFRC::checkCRC()
{
  return (readBuffer[0] + readBuffer[1]) == readBuffer[2];
}


bool OperatorMFRC::checkBeffers(byte checkSize)
{
  for (byte i = 0; i < checkSize; ++i)
  {
    if (readBuffer[i] != writeBuffer[i])
      return false;
  }
  return true;
}

void OperatorMFRC::setupSector(byte s)
{
  sector = s;
  blockAddr      = 4 * sector;
  trailerBlock   = blockAddr + 3;
}

inline void OperatorMFRC::writeToCard() {
  // Записываем данные в блок
  status = mfrc522.MIFARE_Write(blockAddr, writeBuffer, sizeWriteBuf);
}

inline void OperatorMFRC::readFromCard() {
  // Читаем данные из блока
  size = sizeof(readBuffer);//FIX A buffer is not big enough.
  status = mfrc522.MIFARE_Read(blockAddr, readBuffer, &size);
}

bool OperatorMFRC::checkStatus()
{
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Error: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  return true;
}

bool OperatorMFRC::atification() {
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  return checkStatus();
}

void OperatorMFRC::setupPwdId()
{
  calcPassword(mfrc522.uid.uidByte, pwdId);
  Serial.print(F("Password is :"));
  dump_byte_array(pwdId, 6);
  Serial.println();
}
