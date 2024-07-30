#ifndef INC_CTRL_TIMER_H_
#define INC_CTRL_TIMER_H_

#include "main.h"

extern uint8_t g_u8Elapse5ms;
extern uint8_t g_u8Elapse500ms;
extern uint8_t g_u8Elapse1s;
extern uint8_t g_u8Cnt500ms;
extern uint8_t g_u8Cnt1s;

void tick5ms(void);
int  IsElapsed5ms(void);
int  IsElapsed500ms(void);
int  IsElapsed1s(void);
void reset5ms(void);
void reset500ms(void);
void reset1s(void);

#endif /* INC_CTRL_TIMER_H_ */
