#ifndef INIT_TYPE_H
#define INIT_TYPE_H
// Переопределение переменных
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef char int8_t;
typedef short int16_t;
///Card Reader
#ifndef	NumderCard
typedef struct NumderCard {
    uint8_t b[4];
} NumderCard;
#endif

#ifndef	ToUint16
typedef union ToUint16 {
    uint16_t Uint16;
    uint8_t b[2];
} ToUint16;
#endif

#ifndef	ActivateCard
typedef struct ActivateCard {
    uint8_t b[10];
} ActivateCard;
#endif

#ifndef	LockCardRider
typedef struct LockCardRider {
    uint8_t b[8];
} LockCardRider;
#endif

#ifndef	UnlockCardRider
typedef struct UnlockCardRider {
    uint8_t b[8];
} UnlockCardRider;
#endif


#ifndef	SumCard
typedef struct SumCard {
    uint8_t b[13];
} SumCard;
#endif

#ifndef	StatusCard
typedef struct StatusCard {
    uint8_t b[8];
} StatusCard;
#endif

#ifndef	WriteSumCard
typedef struct WriteSumCard {
    uint8_t b[29];
} WriteSumCard;
#endif

#ifndef	PasswordCard
typedef union PasswordCard {
    uint16_t b16[3];
    uint8_t b[6];
} PasswordCard;
#endif

#ifndef	PasswordSumCard
typedef struct PasswordSumCard {
    uint8_t b[19];
} PasswordSumCard;
#endif

///////////////////////
///Команды для юзер интерфейса
///////////////////////

// Вспомогательный класс
static uint8_t xdata HEAD_CONST = 0xAB;

extern enum CMD_INTERFACE{
    INSERTED_CARD = 0x00,
    SUM_CARD = 0x01,
    WRITE_SUM = 0x02,
    INC_SUM = 0x03,
    DEC_SUM = 0x04,
    ERROR_PARSING = 0x20
} xdata CMD_INTERFACE;

extern enum CMD_ERROR {
    ERROR_NO = 0x00
} xdata CMD_ERROR;

#ifndef	InsertedCard
typedef struct InsertedCard {
    uint8_t HEAD;
    uint8_t SIZE;
    uint8_t COMMAND;
    uint8_t CHECKSUM;
} InsertedCard;
#endif

#ifndef	AnswerSumCard
typedef struct AnswerSumCard {
    uint8_t HEAD;
    uint8_t SIZE;
    uint8_t COMMAND;
    uint8_t DATA[2];
    uint8_t CHECKSUM;
} AnswerSumCard;
#endif

#ifndef	AnswerOperationCard
typedef struct AnswerOperationCard {
    uint8_t HEAD;
    uint8_t SIZE;
    uint8_t COMMAND;
    uint8_t ERR;
    uint8_t CHECKSUM;
} AnswerOperationCard;
#endif

//////////////////////////////
#endif
