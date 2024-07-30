#include <Ctrl/FLASH.h>
#include "string.h"

FLASH_REG g_FlashHoldingReg;

FLASH_REG* getFlashReg(void) {
	return &g_FlashHoldingReg;
}

void initFlash(void) {
    memcpy(&g_FlashHoldingReg, (uint32_t*)FLASH_ADDRESS, sizeof(FLASH_REG));
    if (g_FlashHoldingReg.u16Signiture != FLASH_SIGNITURE) {
    	g_FlashHoldingReg.fVersion     		= 1.0;
    	g_FlashHoldingReg.fTemp            	= 0;
    	g_FlashHoldingReg.fHumi             = 0;
    	g_FlashHoldingReg.fVolt	         	= 0;
    	g_FlashHoldingReg.fGain             = 1;
    	g_FlashHoldingReg.fOffset           = 0;
    	g_FlashHoldingReg.u16SenID       	= 1;
    	g_FlashHoldingReg.u16Signiture      = FLASH_SIGNITURE;
    	writeRecord(&g_FlashHoldingReg, FLASH_ADDRESS, sizeof(FLASH_REG));
    }
}

void writeRecord(void* a_pvSource, uint32_t a_uAddress, uint32_t a_uLenBytes) {
    uint8_t   i;
    uint16_t  u16LenAddr = (a_uLenBytes % 4 == 0) ? (a_uLenBytes >> 2) : (a_uLenBytes >> 2) + 1;
    uint32_t* puRecord = (uint32_t*)a_pvSource;
    uint32_t  pageErr = 0;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = a_uAddress;
    EraseInitStruct.NbPages     = 1;

    HAL_FLASHEx_Erase(&EraseInitStruct, &pageErr);

    for (i = 0; i < u16LenAddr; i++, puRecord++, a_uAddress += 4){
    	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, a_uAddress, *puRecord);
    }

    HAL_FLASH_Lock();
}

void writeFlash(void) {
	writeRecord(&g_FlashHoldingReg, FLASH_ADDRESS, sizeof(FLASH_REG));
}


