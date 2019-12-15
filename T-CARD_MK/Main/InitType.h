#ifndef _INIT_TYPE_H
#define _INIT_TYPE_H

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


#endif
