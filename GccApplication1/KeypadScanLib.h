/*
 * KeypadScanLib.h
 *
 * Created: 19-Apr-21 4:52:13 PM
 *  Author: fedea
 */ 


#ifndef KEYPADSCANLIB_H_
#define KEYPADSCANLIB_H_
#include <inttypes.h>

uint8_t KEYPAD_Scan(uint8_t *key);
uint8_t KEYPAD_Update (uint8_t *pkey);


#endif /* KEYPADSCANLIB_H_ */