#include "ReadAndWrite.h"

void MFRC522Test::loop() {

    if(!PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    std::cout << "Card UID:" << std::endl;
    dump_byte_array(m_mfrc522.uid.uidByte, m_mfrc522.uid.size);

    std::cout <<  "PICC type: " << std::endl;
    MFRC522::PICC_Type piccType = m_mfrc522.PICC_GetType(m_mfrc522.uid.sak);
    std::cout << m_mfrc522.PICC_GetTypeName(piccType);

    // Check for compatibility
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        std::cout << "This sample only works with MIFARE Classic cards." << std::endl;
        return;
    }

    // In this sample we use the second sector,
    // that is: sector #1, covering block #4 up to and including block #7
    byte sector         = 1;
    byte blockAddr      = 4;
    byte dataBlock[]    = {
        0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
        0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
        0x09, 0x0a, 0xff, 0x0b, //  9, 10, 255, 11,
        0x0c, 0x0d, 0x0e, 0x0f  // 12, 13, 14, 15
    };
    byte trailerBlock   = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);

    // Authenticate using key A
    std::cout << "Authenticating using key A..." << std::endl;
    status = (MFRC522::StatusCode) m_mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &m_key, &(m_mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        std::cout << "PCD_Authenticate() failed: ";
        std::cout << (m_mfrc522.GetStatusCodeName(status));
        return;
    }

    // Show the whole sector as it currently is
    std::cout << "Current data in sector:" << std::endl;
    m_mfrc522.PICC_DumpMifareClassicSectorToSerial(&(m_mfrc522.uid), &m_key, sector);
    std::cout << std::endl;

    // Read data from the block
    std::cout <<"Reading data from block ";
    std::cout << blockAddr;
    std::cout << " ..." << std::endl;
    status = (MFRC522::StatusCode) m_mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        std::cout <<  "MIFARE_Read() failed: ";
        std::cout << (m_mfrc522.GetStatusCodeName(status));
    }
    std::cout << "Data in block ";
    std::cout << blockAddr;
    std::cout << ":" << std::endl;
    dump_byte_array(buffer, 16);
    std::cout << std::endl;
    std::cout << std::endl;

//    // Authenticate using key B
//    std::cout << "Authenticating again using key B..." << std::endl;
//    status = (MFRC522::StatusCode) m_mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &m_key, &(m_mfrc522.uid));
//    if (status != MFRC522::STATUS_OK) {
//        std::cout << "PCD_Authenticate() failed: ";
//        std::cout << (m_mfrc522.GetStatusCodeName(status));
//        return;
//    }

//    // Write data to the block
//    std::cout << "Writing data into block ";
//    std::cout << blockAddr;
//    std::cout << " ..." << std::endl;
//    dump_byte_array(dataBlock, 16);
//    std::cout << std::endl;
//    status = (MFRC522::StatusCode) m_mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
//    if (status != MFRC522::STATUS_OK) {
//        std::cout << "MIFARE_Write() failed: ";
//        std::cout << m_mfrc522.GetStatusCodeName(status) << std::endl;
//    }
//    std::cout << std::endl;

//    // Read data from the block (again, should now be what we have written)
//    std::cout << "Reading data from block ";
//    std::cout << blockAddr;
//    std::cout << " ..." << std::endl;
//    status = (MFRC522::StatusCode) m_mfrc522.MIFARE_Read(blockAddr, buffer, &size);
//    if (status != MFRC522::STATUS_OK) {
//        std::cout << "MIFARE_Read() failed: ";
//        std::cout << (m_mfrc522.GetStatusCodeName(status));
//    }
//    std::cout << "Data in block ";
//    std::cout << blockAddr;
//    std::cout << ":" << std::endl;
//    dump_byte_array(buffer, 16);
//    std::cout << std::endl;

//    // Check that data in block is what we have written
//    // by counting the number of bytes that are equal
//    std::cout << "Checking result..." << std::endl;
//    byte count = 0;
//    for (byte i = 0; i < 16; i++) {
//        // Compare buffer (= what we've read) with dataBlock (= what we've written)
//        if (buffer[i] == dataBlock[i])
//            count++;
//    }
//    std::cout << "Number of bytes that match = ";
//    std::cout << count << std::endl;
//    if (count == 16) {
//        std::cout << "Success :-)"<< std::endl;
//    } else {
//        std::cout << "Failure, no match :-(" << std::endl;
//        std::cout << "  perhaps the write didn't work properly..."<< std::endl;
//    }
//    std::cout << std::endl;

//    // Dump the sector data
//    std::cout << "Current data in sector:"<< std::endl;
//    m_mfrc522.PICC_DumpMifareClassicSectorToSerial(&(m_mfrc522.uid), &m_key, sector);
//    std::cout << std::endl;

    // Halt PICC
    m_mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    m_mfrc522.PCD_StopCrypto1();
}

bool MFRC522Test::PICC_IsNewCardPresent()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    return m_mfrc522.PICC_IsNewCardPresent();
}

bool MFRC522Test::PICC_ReadCardSerial()
{
    return m_mfrc522.PICC_ReadCardSerial();
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void MFRC522Test::dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        std::cout << (buffer[i] < 0x10 ? " 0" : " ");
        std::cout << std::hex << buffer[i];
    }
}

void MFRC522Test::initConnect()
{
//    m_mfrc522.initMIFARE();
    setsup();
}

void MFRC522Test::findCard()
{
//    if(m_mfrc522.findCard())
//    {
//        m_mfrc522.readUIDCard();
//    }

    loop();

//    // Prepare the key (used both as key A and as key B)
//    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
//    for (byte i = 0; i < 6; i++) {
//        m_key.keyByte[i] = 0xFF;
//    }

//    // Select one of the cards
//    if (!PICC_ReadCardSerial())
//        return;

//    // Show some details of the PICC (that is: the tag/card)
//    std::cout << "Card UID:" << std::endl;
//    dump_byte_array(m_mfrc522.uid.uidByte, m_mfrc522.uid.size);
}

void MFRC522Test::setCallbackWrite(const std::function<void (std::vector<uint8_t> &&)> &callbackWrite)
{
    m_mfrc522.setCallbackWrite(callbackWrite);
}

void MFRC522Test::setCallbackRead(const std::function<std::vector<uint8_t>(std::vector<uint8_t>&&)> &callbackRead)
{
    m_mfrc522.setCallbackRead(callbackRead);
}

void MFRC522Test::setsup()
{
    m_mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        m_key.keyByte[i] = 0xFF;
    }
}
