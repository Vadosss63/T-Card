/**
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 *
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 *
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * This sample shows how to read and write data blocks on a MIFARE Classic PICC
 * (= card/tag).
 *
 * BEWARE: Data will be written to the PICC, in sector #1 (blocks #4 to #7).
 *
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */
#include <iostream>
#include "MFRC522.h"

class MFRC522Test
{

public:
    void setsup();
    void loop();
    bool PICC_IsNewCardPresent();
    bool PICC_ReadCardSerial();
    void dump_byte_array(byte *buffer, byte bufferSize);

    void initConnect();
    void findCard();
    void setCallbackWrite(const std::function<void (std::vector<uint8_t>&&)> &callbackWrite);
    void setCallbackRead(const std::function<std::vector<uint8_t>(std::vector<uint8_t> &&)> &callbackRead);
private:
    MFRC522 m_mfrc522;   // Create MFRC522 instance.
    MFRC522::MIFARE_Key m_key;
};


