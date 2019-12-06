#include "CMD_data.h"


///==================Parsing================================///



uint8_t getCRC(uint8_t* d, uint8_t size)
{
	uint8_t crc = 0;
	uint8_t i;
	for (i = 0; i < size; i++){	
		crc = crc ^ d[i];	
	}
	return crc;
}

bit isValidData(uint8_t* d, uint8_t size)
{
	// Прверка стартового байта
	if(d[0] != CMD_STX)
		return 0;
	// Проверки стоп байта
	if(d[size - 2] != CMD_ETX)
		return 0;
	// Проверка размерности
	if(size != (d[SIZE_DATA] + 5))
		return 0;	
	// Проверка СКС
	if(getCRC(d, size) == d[size - 1])
		return 1;
}

bit isValidAnswer(uint8_t d)
{
	return d == CMD_OK;
}


NumderCard getNumberCard(uint8_t* d)
{
    NumderCard nc;
	nc.b[0] = d[12];
	nc.b[1] = d[13];
	nc.b[2] = d[14];
	nc.b[3] = d[15];	
	return nc;
}


uint16_t convertToUint16(uint8_t* d, uint8_t startByte)
{	
	ToUint16 convert;
	convert.b[0] = d[startByte];
	convert.b[1] = d[startByte + 1];
    return convert.Uint16;
}

uint16_t getSumCard(uint8_t* d)
{
    return convertToUint16(d, 8);
}

bit checkStatusCard(uint8_t* d, uint8_t size)
{
    if(size < 5)
        return 0;

    return d[3] == CMD_P;
}


ActivateCard getActivate()
{
	static ActivateCard xdata ac;
	ac.b[0] = CMD_STX;
	ac.b[1] = 0x00;
	ac.b[2] = 0x05;
	ac.b[3] = CMD_C;
	ac.b[4] = 0x60;
	ac.b[5] = 0x30;
	ac.b[6] = 0x41;
	ac.b[7] = 0x42;
	ac.b[8] = CMD_ETX;
	ac.b[9] = 0xE4;
	return ac;

   // {STX, 0x00, 0x05, C, 0x60, 0x30, 0x41, 0x42, ETX, 0xE4};
}



LockCardRider getLock()
{
    static LockCardRider xdata lockCR;
	lockCR.b[0] = CMD_STX;
	lockCR.b[1] = 0x00;
	lockCR.b[2] = 0x03;
	lockCR.b[3] = CMD_C;
	lockCR.b[4] = 0xB0;
	lockCR.b[5] = 0x30;
	lockCR.b[6] = CMD_ETX;
	lockCR.b[7] = 0x31;
	return lockCR;	
	//{STX, 0x00, 0x03, C, 0xB0, 0x30, ETX, 0x31};
}

UnlockCardRider getUnlock()
{
    static UnlockCardRider xdata unlockCR;
	unlockCR.b[0] = CMD_STX;
	unlockCR.b[1] = 0x00;
	unlockCR.b[2] = 0x03;
	unlockCR.b[3] = CMD_C;
	unlockCR.b[4] = 0xB0;
	unlockCR.b[5] = 0x31;
	unlockCR.b[6] = CMD_ETX;
	unlockCR.b[7] = 0x30;
	return unlockCR;
	//{STX, 0x00, 0x03, C, 0xB0, 0x31, ETX, 0x30};
}

/*
std::vector<uint8_t> getPasswordSum(std::vector<uint8_t> password)
{
    std::vector<uint8_t> cmdPassword {STX, 0x00, 0x0E, C, 0x60, 0x33, 0x00, 0x20, 0x00, 0x02, 0x06 };
    cmdPassword.insert(cmdPassword.end(), password.begin(), password.end());
    cmdPassword.push_back(ETX);
    uint8_t crc = getCRC(cmdPassword.data(), cmdPassword.size());
    cmdPassword.push_back(crc);
    return cmdPassword;
}

std::vector<uint8_t> getWriteSum(uint16_t sum)
{
    std::vector<uint8_t> cmdWrite {0xF2, 0x00, 0x18, 0x43, 0x60, 0x33, 0x00, 0xD1, 0x02, 0x00, 0x01};
    uint8_t hb = sum >> 8;
    uint8_t lb = static_cast<uint8_t>(sum);
    cmdWrite.push_back(lb);
    cmdWrite.push_back(hb);
    std::vector<uint8_t> zeroByte {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    cmdWrite.insert(cmdWrite.end(), zeroByte.begin(), zeroByte.end());
    cmdWrite.push_back(ETX);
    uint8_t crc = getCRC(cmdWrite.data(), cmdWrite.size());
    cmdWrite.push_back(crc);
    return cmdWrite;
}

std::vector<uint8_t> getSum()
{
    return std::vector<uint8_t>{STX, 0x00, 0x08, C, 0x60, 0x33,
                0x00, 0xB0, 0x02, 0x00, 0x01, ETX, 0x5A};
}

std::vector<uint8_t> getStatusCard()
{
    return std::vector<uint8_t>{STX, 0x00, 0x03, 0x43, 0x50, 0x31, ETX, 0xD0};
}


*/