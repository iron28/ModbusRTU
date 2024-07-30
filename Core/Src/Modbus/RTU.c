#include <Ctrl/GPIO.h>
#include <Ctrl/FLASH.h>
#include <Modbus/RTU.h>

MODBUS_RTU g_MODBUS_RTU;

void ModbusInit(MODBUS_RTU* self, UART_HandleTypeDef* a_pUart, TIM_HandleTypeDef* a_pTimer, uint8_t* a_pu8RxData) {
    self->m_pUart     	= a_pUart;
    self->m_pTimer    	= a_pTimer;
    self->m_pu8RxData  	= a_pu8RxData;
    self->m_u8RxState   = WAIT;
    self->m_u8RxIndex  	= 0;
    self->m_n8WrFlag 	= _OFF;

    HAL_UART_Receive_IT(self->m_pUart, self->m_pu8RxData, 1);
    HAL_TIM_Base_Start_IT(self->m_pTimer);
}

void ModbusRcv(MODBUS_RTU* self) {
    self->m_u8RxState                     	= RCVD;
    self->m_pTimer->Instance->CNT        	= 0;
    self->m_au8RxBuf[self->m_u8RxIndex++] 	= *self->m_pu8RxData;
    HAL_UART_Receive_IT(self->m_pUart, self->m_pu8RxData, 1);
}

uint8_t ModbusGetFlag(MODBUS_RTU* self) {
	return self->m_u8RxState;
}

void ModbusReset(MODBUS_RTU* self) {
    self->m_u8RxIndex = 0;
    HAL_UART_Receive_IT(self->m_pUart, self->m_pu8RxData, 1);
    self->m_u8RxState = WAIT;
}

void ModbusFunc(MODBUS_RTU* self) {
    uint8_t  u8ID;
    uint8_t  u8Function;
    uint8_t  u8DataSize;
    uint16_t u16StartAddr;
    uint16_t u16StartAddrDefault;
    uint16_t u16DataLen;

    if (CRC16(self->m_au8RxBuf, self->m_u8RxIndex) == 0) {
        getInfo(&u8ID, &u8Function, &u16StartAddr, &u16DataLen, self->m_au8RxBuf);
        u16StartAddrDefault = u16StartAddr;
        u16StartAddr *= 2;

        if ((u8ID == getID()) || (u8ID == 0)) {
            togRX();
            switch (u8Function) {
            case 3: // READ HOLDING REGISTER
                if (u8ID == 0) {
                    togTX();
                    return;
                }

                if (u16StartAddr > (sizeof(FLASH_REG) - 2)) {
                	ErrorMSG(u8ID, u8Function, 2, self->m_au8TxBuf);
                    HAL_UART_Transmit_DMA(self->m_pUart, self->m_au8TxBuf, 5);
                    return;
                }

                u8DataSize = u16DataLen * 2;

                self->m_au8TxBuf[0] = u8ID;
                self->m_au8TxBuf[1] = u8Function;
                self->m_au8TxBuf[2] = u8DataSize;

                EndianSwap(&self->m_au8TxBuf[3], &((uint8_t*)getFlashReg())[u16StartAddr], u16DataLen);
                uint16_t u16SendCRC = CRC16(self->m_au8TxBuf, u8DataSize + 3);
                copyData(&self->m_au8TxBuf[3 + u8DataSize], (uint8_t*)&u16SendCRC, 1);

                HAL_UART_Transmit_DMA(self->m_pUart, self->m_au8TxBuf, 5 + u8DataSize);
                togTX();
                break;

            case 16: // WRITE HOLDING REGISTER

                if (u16StartAddr > (sizeof(FLASH_REG) - 2)) {
                	ErrorMSG(u8ID, u8Function, 2, self->m_au8TxBuf);
                    HAL_UART_Transmit_DMA(self->m_pUart, self->m_au8TxBuf, 5);
                    return;
                }

                EndianSwap(&((uint8_t*)getFlashReg())[u16StartAddr], &self->m_au8RxBuf[7], u16DataLen);
                self->m_n8WrFlag = _ON;

                if (u8ID == 0) { return; }

                self->m_au8TxBuf[0] = u8ID;
                self->m_au8TxBuf[1] = u8Function;
                EndianSwap(&self->m_au8TxBuf[2], (uint8_t*)&u16StartAddrDefault, 1);
                EndianSwap(&self->m_au8TxBuf[4], (uint8_t*)&u16DataLen, 1);
                u16SendCRC = CRC16(self->m_au8TxBuf, 6);
                copyData(&self->m_au8TxBuf[6], (uint8_t*)&u16SendCRC, 1);
                HAL_UART_Transmit_DMA(self->m_pUart, self->m_au8TxBuf, 8);
                togTX();
                break;

            default:
            	ErrorMSG(u8ID, u8Function, 1, self->m_au8TxBuf);
            	HAL_UART_Transmit_DMA(self->m_pUart, self->m_au8TxBuf, 5);
            }
        }
    }
}

void ModbusTimeOut(MODBUS_RTU* self) {
    if (self->m_u8RxState == RCVD) self->m_u8RxState = CMPT;
}

void getInfo(uint8_t* a_pn8ID, uint8_t* a_pn8Func, uint16_t* a_pn16StartAddress, uint16_t* a_pu16DataLen, const uint8_t* a_pSource) {
    a_pn8ID[0]   = a_pSource[0];
    a_pn8Func[0] = a_pSource[1];
    EndianSwap((uint8_t*)a_pn16StartAddress, (uint8_t*)&a_pSource[2], 1);
    EndianSwap((uint8_t*)a_pu16DataLen, (uint8_t*)&a_pSource[4], 1);
}

void ErrorMSG(uint8_t a_n8ID, uint8_t a_n8Func, uint8_t a_n8Error, uint8_t* a_pn8Target) {
    a_pn8Target[0]      = a_n8ID;
    a_pn8Target[1]      = a_n8Func | 0x80;
    a_pn8Target[2]      = a_n8Error;
    uint16_t n16SendCRC = CRC16(a_pn8Target, 3);
    copyData(&a_pn8Target[3], (uint8_t*)&n16SendCRC, 1);
}

void EndianSwap(uint8_t* a_pn8Target, const uint8_t* a_pn8Source, uint16_t a_n16Length) {
    for (uint16_t i = 0; i < a_n16Length; i++) {
        a_pn8Target[i * 2]     = a_pn8Source[i * 2 + 1];
        a_pn8Target[i * 2 + 1] = a_pn8Source[i * 2];
    }
}

void copyData(uint8_t* a_pn8Target, uint8_t* a_pn8Source, uint16_t a_n16Length) {
    for (uint16_t i = 0; i < a_n16Length; i++) {
        a_pn8Target[i * 2]     = a_pn8Source[i * 2];
        a_pn8Target[i * 2 + 1] = a_pn8Source[i * 2 + 1];
    }
}

uint16_t CRC16(const uint8_t* nData, uint16_t wLength) {
    static const uint16_t wCRCTable[] = {
    		0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241, 0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440, 0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40, 0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
            0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40, 0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41, 0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641, 0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
            0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240, 0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441, 0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41, 0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
            0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41, 0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40, 0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640, 0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
            0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240, 0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441, 0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41, 0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
            0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41, 0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40, 0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640, 0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
            0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241, 0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, 0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40, 0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
            0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40, 0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41, 0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641, 0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
    };

    uint8_t  nTemp;
    uint16_t wCRCWord = 0xFFFF;

    while (wLength--) {
        nTemp = *nData++ ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord ^= wCRCTable[nTemp];
    }
    return wCRCWord;
}
