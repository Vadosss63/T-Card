
const char *msg = "T-card System 0x01";

static uint16_t crc16(uint8_t *pcBlock, const uint16_t polinom, uint8_t len = 18)
{
    uint16_t crc = 0xFFFF;
    uint8_t i;
    while (len--)
    {
        crc ^= *pcBlock++ << 8;
        for (i = 0; i < 8; i++)
        {
            crc = crc & 0x8000 ? (crc << 1) ^ polinom : crc << 1;
        }
    }
    return crc;
}

void calcPassword(uint8_t *uid, uint8_t *pwd)
{
    uint16_t pwdB = crc16((uint8_t *)msg, uid[0]);
    pwd[0] = 0xFF & (pwdB >> 8);
    pwd[1] = 0xFF & pwdB;

    pwdB = crc16((uint8_t *)msg, uid[1]);
    pwd[2] = 0xFF & (pwdB >> 8);
    pwd[3] = 0xFF & pwdB;

    pwdB = crc16((uint8_t *)msg, uid[2] + uid[3]);
    pwd[4] = 0xFF & (pwdB >> 8);
    pwd[5] = 0xFF & pwdB;
}
