#ifndef INC_MODBUS_RTU_H_
#define INC_MODBUS_RTU_H_

#include "main.h"

#define WAIT            0
#define RCVD           	1
#define CMPT           	2
#define BUF_SIZE        300

typedef struct MODBUS_RTU
{
    UART_HandleTypeDef* m_pUart;
    TIM_HandleTypeDef*  m_pTimer;
    uint8_t*            m_pu8RxData;
    uint8_t             m_au8RxBuf[BUF_SIZE];
    uint8_t             m_au8TxBuf[BUF_SIZE];
    uint8_t             m_u8RxState;
    uint8_t             m_u8RxIndex;
    uint8_t             m_n8WrFlag;
} MODBUS_RTU;

extern MODBUS_RTU g_MODBUS_RTU;

void     ModbusInit(MODBUS_RTU* self, UART_HandleTypeDef* a_pstUart, TIM_HandleTypeDef* a_pstTimer, uint8_t* a_pn8RcvData);
void     ModbusRcv(MODBUS_RTU* self);
void 	 ModbusTimeOut(MODBUS_RTU* self);
void     ModbusFunc(MODBUS_RTU* self);
uint8_t  ModbusGetFlag(MODBUS_RTU* self);
void     ModbusResetFlag(MODBUS_RTU* self);
uint16_t CRC16(const uint8_t* nData, uint16_t wLength);
void     EndianSwap(uint8_t* a_pn8Target,const uint8_t* a_pn8Source, uint16_t a_n16Length);
void     copyData(uint8_t* a_pn8Target, uint8_t* a_pn8Source, uint16_t a_n16Length);
void     ErrorMSG(uint8_t a_n8ID, uint8_t a_n8Func, uint8_t a_n8Error, uint8_t* a_pn8Target);
void     getInfo(uint8_t* a_pn8ID, uint8_t* a_pn8Func, uint16_t* a_pn16StartAddress, uint16_t* a_pn16DataNum, const uint8_t* a_pSource);
void  	 ModbusReset(MODBUS_RTU* self);

#endif /* INC_MODBUS_RTU_H_ */
