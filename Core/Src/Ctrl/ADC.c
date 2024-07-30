
#include <Ctrl/ADC.h>
#include <Ctrl/FLASH.h>
#include <math.h>

void AdcProcess(uint32_t* pu32AdcData) {
	FLASH_REG* curData = getFlashReg();

	curData->fVolt   = (1.2 / pu32AdcData[2]) * pu32AdcData[0];
}
