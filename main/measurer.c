/**
 * @file measurer.c
 * @author
 * @date
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "measurer.h"
#include "max6675.h"
#include "nvs_application.h"
#include "nvs_keys.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================
#define MEASURER_TAG            "[MEASURER]"

#define MEASURER_PERIOD 500    //0.5 sec

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
static measurer_data_t data = {0};
static measurer_config_t config = {0};

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
static void measurer_process_task(void * pvParameter);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void measurer_read_config(void);

//=============================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

static void measurer_process_task(void * pvParameter)
{
    uint8_t error_count = 0;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(MEASURER_PERIOD));

        if (true == max6675_read(&data.temperature))
        {
            data.temperature = data.temperature + config.offset;
            data.valid = true;

            error_count = 0;
        }
        else
        {
            if (5 == error_count)
            {
                data.valid = false;
            }
            else
            {
                error_count++;
            }
        }
    }
}
// End static void measurer_process_task(void * pvParameter)

//=============================================================================

//=============================================================================
// [Local Function Definitions] ===============================================

static void measurer_read_config(void)
{
    int16_t i16_aux_0 = 0;

    nvs_read_i16(NVS_KEY_MEA_OFFSET, &i16_aux_0);
    config.offset = ((float_t)i16_aux_0) / 10.0;
}
// End static void measurer_read_params(void)

//=============================================================================

//=============================================================================
// [External Function Definition] =============================================

void measurer_init(void)
{
    measurer_read_config();

    max6675_init();

    xTaskCreate(measurer_process_task, "measurer_proc_ts", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, NULL);
}
// End void measurer_init(void)


measurer_data_t measurer_get_data(void)
{
    return data;
}
// End measurer_data_t measurer_get_data(void)


measurer_config_t measurer_get_config(void)
{
    return config;
}
// End measurer_config_t measurer_get_config(void)


void measurer_set_offset(float_t offset)
{
    if (nvs_write_i16(NVS_KEY_MEA_OFFSET, (int16_t)(offset * 10.0)))
    {
        config.offset = offset;
    }
}
// End void measurer_set_offset(float_t offset)

//=============================================================================

//====================== [End Document] =======================================
