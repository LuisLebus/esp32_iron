/**
 * @file display.c
 * @author
 * @date
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"

#include "display.h"
#include "measurer.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================

#define DISPLAY_DELAY_US        5000

#define DISPLAY_DIG0_GPIO       GPIO_NUM_23
#define DISPLAY_DIG1_GPIO       GPIO_NUM_27
#define DISPLAY_DIG2_GPIO       GPIO_NUM_14
#define DISPLAY_DIG3_GPIO       GPIO_NUM_26

#define DISPLAY_A_GPIO          GPIO_NUM_22
#define DISPLAY_B_GPIO          GPIO_NUM_13
#define DISPLAY_C_GPIO          GPIO_NUM_32
#define DISPLAY_D_GPIO          GPIO_NUM_18
#define DISPLAY_E_GPIO          GPIO_NUM_19
#define DISPLAY_F_GPIO          GPIO_NUM_21
#define DISPLAY_G_GPIO          GPIO_NUM_25
#define DISPLAY_DP_GPIO         GPIO_NUM_33

//=============================================================================

//=============================================================================
// [Local Typedef] ============================================================

//=============================================================================

//=============================================================================
// [External Data Definitions] ================================================

// Const ---------------------------------------------
//----------------------------------------------------

// Vars ----------------------------------------------
//----------------------------------------------------

// Task Handlers -------------------------------------
//----------------------------------------------------

// Queue Handlers ------------------------------------
//----------------------------------------------------

// Event Group Handlers ------------------------------
//----------------------------------------------------

// Semaphore Handlers --------------------------------
//----------------------------------------------------

//=============================================================================

//=============================================================================
// [Local Data Declarations] ==================================================

// Const ---------------------------------------------
//----------------------------------------------------

// Vars ----------------------------------------------
//----------------------------------------------------

// Task Handlers -------------------------------------
//----------------------------------------------------

// Queue Handlers ------------------------------------
//----------------------------------------------------

// Event Group Handlers ------------------------------
//----------------------------------------------------

// Semaphore Handlers --------------------------------
//----------------------------------------------------

//=============================================================================

//=============================================================================
// [Local Function Declarations] ==============================================

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void display_process_task(void * pvParameter);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void display_set_dig(uint8_t dig);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void display_set_off(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void display_set_symbol(char symbol, bool dp);

//=============================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

static void display_process_task(void * pvParameter)
{
    measurer_data_t measurer_data = {0};
    uint8_t i = 0;
    char str_val[12] = {0};

    while (1)
    {
        measurer_data = measurer_get_data();

        if (true == measurer_data.valid)
        {
            sprintf(str_val, "%04u", (uint16_t)measurer_data.temperature);
        }
        else
        {
            strcpy(str_val, "Err \0");
        }

        for (i = 0; i < 10; i++)
        {
            display_set_symbol(str_val[0], false);
            display_set_dig(0);
            ets_delay_us(DISPLAY_DELAY_US);
            display_set_off();

            display_set_symbol(str_val[1], false);
            display_set_dig(1);
            ets_delay_us(DISPLAY_DELAY_US);
            display_set_off();

            display_set_symbol(str_val[2], false);
            display_set_dig(2);
            ets_delay_us(DISPLAY_DELAY_US);
            display_set_off();

            display_set_symbol(str_val[3], false);
            display_set_dig(3);
            ets_delay_us(DISPLAY_DELAY_US);
            display_set_off();
        }

        vTaskDelay(1);
    }
}
// End static void display_process_task(void * pvParameter)

//=============================================================================

//=============================================================================
// [Local Function Definitions] ===============================================

static void display_set_dig(uint8_t dig)
{
    switch (dig)
    {
        case 0:
            gpio_set_level(DISPLAY_DIG0_GPIO, true);
        break;

        case 1:
            gpio_set_level(DISPLAY_DIG1_GPIO, true);
        break;

        case 2:
            gpio_set_level(DISPLAY_DIG2_GPIO, true);
        break;

        case 3:
            gpio_set_level(DISPLAY_DIG3_GPIO, true);
        break;

        default:
            //Nothing to do.
        break;
    }
}
// End


static void display_set_off(void)
{
    gpio_set_level(DISPLAY_DIG0_GPIO, false);
    gpio_set_level(DISPLAY_DIG1_GPIO, false);
    gpio_set_level(DISPLAY_DIG2_GPIO, false);
    gpio_set_level(DISPLAY_DIG3_GPIO, false);

    gpio_set_level(DISPLAY_A_GPIO, false);
    gpio_set_level(DISPLAY_B_GPIO, false);
    gpio_set_level(DISPLAY_C_GPIO, false);
    gpio_set_level(DISPLAY_D_GPIO, false);
    gpio_set_level(DISPLAY_E_GPIO, false);
    gpio_set_level(DISPLAY_F_GPIO, false);
    gpio_set_level(DISPLAY_G_GPIO, false);
    gpio_set_level(DISPLAY_DP_GPIO, false);
}
// End


static void display_set_symbol(char symbol, bool dp)
{
    switch (symbol)
    {
        case '0':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
            gpio_set_level(DISPLAY_E_GPIO, true);
            gpio_set_level(DISPLAY_F_GPIO, true);
        break;

        case '1':
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
        break;

        case '2':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
            gpio_set_level(DISPLAY_E_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
        break;

        case '3':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
        break;

        case '4':
            gpio_set_level(DISPLAY_F_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
        break;

        case '5':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_F_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
        break;

        case '6':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_F_GPIO, true);
            gpio_set_level(DISPLAY_E_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
        break;

        case '7':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
        break;

        case '8':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
            gpio_set_level(DISPLAY_E_GPIO, true);
            gpio_set_level(DISPLAY_F_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
        break;

        case '9':
            gpio_set_level(DISPLAY_F_GPIO, true);
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_B_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
            gpio_set_level(DISPLAY_C_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
        break;

        case 'E':
            gpio_set_level(DISPLAY_A_GPIO, true);
            gpio_set_level(DISPLAY_F_GPIO, true);
            gpio_set_level(DISPLAY_E_GPIO, true);
            gpio_set_level(DISPLAY_D_GPIO, true);
            gpio_set_level(DISPLAY_G_GPIO, true);
        break;

        case 'r':
            gpio_set_level(DISPLAY_G_GPIO, true);
            gpio_set_level(DISPLAY_E_GPIO, true);
        break;

        case '-':
            gpio_set_level(DISPLAY_G_GPIO, true);
        break;
            //Nothing to do.
        default:

        break;
    }

    if (true == dp)
    {
        gpio_set_level(DISPLAY_DP_GPIO, true);
    }
}
// End

//=============================================================================

//=============================================================================
// [External Function Definition] =============================================

void display_init(void)
{
    gpio_config_t gpio_Config = {0};

    gpio_Config.pin_bit_mask =
            (1ULL << DISPLAY_DIG0_GPIO) | (1ULL << DISPLAY_DIG1_GPIO) | (1ULL << DISPLAY_DIG2_GPIO) |
            (1ULL << DISPLAY_DIG3_GPIO) | (1ULL << DISPLAY_A_GPIO) | (1ULL << DISPLAY_B_GPIO) |
            (1ULL << DISPLAY_C_GPIO) | (1ULL << DISPLAY_D_GPIO) | (1ULL << DISPLAY_E_GPIO) |
            (1ULL << DISPLAY_F_GPIO) | (1ULL << DISPLAY_G_GPIO) | (1ULL << DISPLAY_DP_GPIO);
    gpio_Config.mode = GPIO_MODE_OUTPUT;
    gpio_config(&gpio_Config);

    display_set_off();

    xTaskCreate(display_process_task, "display_proc_ts", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, NULL);
}
// End void display_init(void)

//=============================================================================

//====================== [End Document] =======================================
