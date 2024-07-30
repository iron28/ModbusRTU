#include <Ctrl/GPIO.h>
#include <Ctrl/Timer.h>

uint8_t g_u8Elapse5ms 		= _FALSE;
uint8_t g_u8Elapse500ms 	= _FALSE;
uint8_t g_u8Elapse1s 		= _FALSE;
uint8_t g_u8Cnt500ms 		= 0;
uint8_t g_u8Cnt1s 			= 0;

void tick5ms(void) {
	g_u8Elapse5ms = _TRUE;
	g_u8Cnt500ms++;
	g_u8Cnt1s++;

	if(g_u8Cnt500ms == 100) {
		g_u8Elapse500ms = _TRUE;
		g_u8Cnt500ms = 0;
	}
	if(g_u8Cnt1s == 200) {
		g_u8Elapse1s = _TRUE;
		g_u8Cnt1s = 0;
	}
}

int IsElapsed5ms(void) {
	return g_u8Elapse5ms;
}

int IsElapsed500ms(void) {
	return g_u8Elapse500ms;
}

int IsElapsed1s(void) {
	return g_u8Elapse1s;
}

void reset5ms(void) {
	g_u8Elapse5ms = _FALSE;
}

void reset500ms(void) {
	g_u8Elapse500ms = _FALSE;
}

void reset1s(void) {
	g_u8Elapse1s = _FALSE;
}
