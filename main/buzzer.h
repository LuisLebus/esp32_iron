/*
 * 	buzzer.h
 *
 *	Created on:
 *  Author:
 *  Company: D&T Embedded System Solutions
 *  Version: v1.0
 */

#ifndef _BUZZER_H_
#define _BUZZER_H_
//===================================================================
// [Inclusions] =====================================================
#include "driver/gpio.h"

//===================================================================
// [External Defines] ===============================================
#define BUZZER_GPIO			GPIO_NUM_12

//===================================================================
// [Typedef] ========================================================

//===================================================================
// [External Data Definitions] ======================================

//===================================================================
// [External Function Declarations ] ================================
void buzzer_init(void);
void buzzer_beep(uint16_t duration, uint8_t count);

#endif /* _BUZZER_H_ */