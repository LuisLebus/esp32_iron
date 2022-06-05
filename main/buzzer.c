/*
 * 	buzzer.c
 *
 *	Created on:
 *  Author:
 *  Company: D&T Embedded System Solutions
 *  Version: v1.0
 */

//===================================================================
// [Inclusions] =====================================================
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "buzzer.h"

//===================================================================
// [Private Defines] ================================================

//===================================================================
// [Local Functions Declaration] ====================================

//===================================================================
// [External Data Definitions] ======================================

// Const ---------------------------------------------

// Vars ----------------------------------------------

// Flags ---------------------------------------------

// TypeDefs ------------------------------------------

// Enums ---------------------------------------------

// Queue Handlers ------------------------------------

//===================================================================
// [Local Data Declaration] =========================================

// Vars ----------------------------------------------

// Const --------------------------------------------

// Flags ---------------------------------------------

// TypeDefs ------------------------------------------

// Enums ---------------------------------------------

// Task Handlers -------------------------------------

// Queue Handlers ------------------------------------

//===================================================================
// [Local Functions Definition] =====================================

//===================================================================
// [FreeRTOs Task Definition] =======================================

//===================================================================
// [External Function Definition] ===================================
void buzzer_init(void)
{
	gpio_config_t gpio_Config = {0};

	gpio_set_level(BUZZER_GPIO, false);

	gpio_Config.intr_type = GPIO_PIN_INTR_DISABLE;
	gpio_Config.mode = GPIO_MODE_OUTPUT;
	gpio_Config.pin_bit_mask = 1ULL << BUZZER_GPIO;
	gpio_Config.pull_down_en = 0;
	gpio_Config.pull_up_en = 0;
	gpio_config(&gpio_Config);
}

void buzzer_beep(uint16_t duration, uint8_t count)
{
	uint8_t i = 0;

	for (i = 0; i < count; i++)
	{
		gpio_set_level(BUZZER_GPIO, true);
		vTaskDelay(duration / portTICK_RATE_MS);
		gpio_set_level(BUZZER_GPIO, false);
		vTaskDelay( (duration / 2) / portTICK_RATE_MS);
	}
}
