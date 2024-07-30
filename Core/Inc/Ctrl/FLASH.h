#ifndef INC_CTRL_FLASH_H_
#define INC_CTRL_FLASH_H_

#include "main.h"

#define FLASH_ADDRESS ((uint32_t)0x08007c00)
#define FLASH_SIGNITURE 0xccdd

typedef struct __attribute__((aligned(sizeof(uint16_t))))
{
    float    fVersion;
    float    fTemp;
    float    fHumi;
    float    fVolt;
    float    fGain;
    float    fOffset;
    uint16_t u16SenID;
    uint16_t u16Signiture;
} FLASH_REG;

extern FLASH_REG g_FlashHoldingReg;

FLASH_REG* 	getFlashReg(void);
void        initFlash(void);
void        writeRecord(void* a_pvSource, uint32_t a_uAddress, uint32_t a_uLenBytes);
void        writeFlash(void);

#endif /* INC_CTRL_FLASH_H_ */
