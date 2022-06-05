/**
 * @file output.c
 * @author
 * @date
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"

#include "output.h"
#include "measurer.h"
#include "buzzer.h"
#include "nvs_application.h"
#include "nvs_keys.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================
#define OUTPUT_TAG              "[OUTPUT]"

#define OUTPUT_GPIO             GPIO_NUM_16

#define OUTPUT_STATE_INIT       0
#define OUTPUT_STATE_RAMP       1
#define OUTPUT_STATE_PREHEAT    2
#define OUTPUT_STATE_REFLOW     3
#define OUTPUT_STATE_COOLING    4

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
static bool output_state = false;
static uint16_t output_global_time = 0;
static uint16_t output_start_time = 0;
static uint8_t output_fsm_state = OUTPUT_STATE_INIT;

output_config_t output_config = {
        .preheat_temp = 175,
        .preheat_hist = 2,
        .preheat_time = 100,
        .preheat_brake = 10,
        .reflow_temp = 235,
        .reflow_brake = 10,
        .cooling_time = 10
};

static uint16_t array_temperature[512] = {0};

//----------------------------------------------------

// Task Handlers -------------------------------------
TaskHandle_t output_task_handle = NULL;

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
static void output_process_task(void * pvParameter);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void output_read_params(void);

//=============================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

static void output_process_task(void * pvParameter)
{
    uint16_t temperature = 0;

    ESP_LOGW(OUTPUT_TAG, "----  START  ----");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        temperature = (uint16_t)measurer_get_data().temperature;

        array_temperature[output_global_time] = temperature;

        switch (output_fsm_state)
        {
            case OUTPUT_STATE_INIT:
                output_set_state(true);
                output_fsm_state = OUTPUT_STATE_RAMP;
                ESP_LOGW(OUTPUT_TAG, "----  OUTPUT_STATE_RAMP  ----");
                buzzer_beep(200, 1);
            break;

            case OUTPUT_STATE_RAMP:
                if ((temperature >= (output_config.preheat_temp - output_config.preheat_brake)) && (true == output_state ))
                {
                    output_set_state(false);
                }

                if (temperature >= output_config.preheat_temp)
                {
                    output_start_time = output_global_time;
                    output_fsm_state = OUTPUT_STATE_PREHEAT;
                    ESP_LOGW(OUTPUT_TAG, "----  OUTPUT_STATE_PREHEAT  ----");
                    buzzer_beep(200, 2);
                }
            break;

            case OUTPUT_STATE_PREHEAT:
                if (temperature >= output_config.preheat_temp)
                {
                    output_set_state(false);
                }

                if (temperature <= (output_config.preheat_temp - output_config.preheat_hist))
                {
                    output_set_state(true);
                }

                if ((output_global_time - output_start_time) >= output_config.preheat_time)
                {
                    output_set_state(true);
                    output_fsm_state = OUTPUT_STATE_REFLOW;
                    ESP_LOGW(OUTPUT_TAG, "----  OUTPUT_STATE_REFLOW  ----");
                    buzzer_beep(200, 3);
                }
            break;

            case OUTPUT_STATE_REFLOW:
                if ((temperature >= (output_config.reflow_temp - output_config.reflow_brake)) && (true == output_state ))
                {
                    output_set_state(false);
                }

                if (temperature >= output_config.reflow_temp)
                {
                    output_start_time = output_global_time;
                    output_fsm_state = OUTPUT_STATE_COOLING;
                    ESP_LOGW(OUTPUT_TAG, "----  OUTPUT_STATE_COOLING  ----");
                    buzzer_beep(200, 4);
                }
            break;

            case OUTPUT_STATE_COOLING:
                if ((output_global_time - output_start_time) >= output_config.cooling_time)
                {
                    ESP_LOGW(OUTPUT_TAG, "----  STOP  ----");
                    buzzer_beep(1000, 3);
                    output_set_state(false);
                    output_global_time = 0;
                    output_start_time = 0;
                    output_fsm_state = OUTPUT_STATE_INIT;
                    output_task_handle = NULL;

                    vTaskDelete(NULL);
                }
            break;

            default:
                output_set_state(false);
                output_global_time = 0;
                output_start_time = 0;
                output_fsm_state = OUTPUT_STATE_INIT;
                output_task_handle = NULL;

                vTaskDelete(NULL);
            break;
        }

        output_global_time++;
    }
}
// End static void output_process_task(void * pvParameter)

//=============================================================================

//=============================================================================
// [Local Function Definitions] ===============================================

static void output_read_params(void)
{
    nvs_read_u16(NVS_KEY_OUT_PREHEAT_TEMP, &output_config.preheat_temp);
    nvs_read_u16(NVS_KEY_OUT_PREHEAT_HIST, &output_config.preheat_hist);
    nvs_read_u16(NVS_KEY_OUT_PREHEAT_TIME, &output_config.preheat_time);
    nvs_read_u16(NVS_KEY_OUT_PREHEAT_BRAKE, &output_config.preheat_brake);
    nvs_read_u16(NVS_KEY_OUT_REFLOW_TEMP, &output_config.reflow_temp);
    nvs_read_u16(NVS_KEY_OUT_REFLOW_BRAKE, &output_config.reflow_brake);
    nvs_read_u16(NVS_KEY_OUT_COOLING_TIME, &output_config.cooling_time);
}
// End static void output_read_params(void)

//=============================================================================

//=============================================================================
// [External Function Definition] =============================================

void output_init(void)
{
    gpio_config_t gpio_Config = {0};

    gpio_Config.pin_bit_mask =  1ULL << OUTPUT_GPIO;
    gpio_Config.mode = GPIO_MODE_OUTPUT;
    gpio_config(&gpio_Config);

    output_read_params();
}
// End void output_init(void)


void output_set_state(bool state)
{
    output_state = state;
    gpio_set_level(OUTPUT_GPIO, output_state);
}
// End void output_set_state(bool state)


void output_toggle_state(void)
{
    if(true == output_state)
    {
        output_state = false;
    }
    else
    {
        output_state = true;
    }

    gpio_set_level(OUTPUT_GPIO, output_state);
}
// End void output_toggle_state(void)


void output_start(void)
{
    if (NULL == output_task_handle)
    {
        xTaskCreate(output_process_task, "output_proc_ts", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, &output_task_handle);
    }
}
// End void output_start(void)


void output_stop(void)
{
    if (output_task_handle != NULL)
    {
        vTaskDelete(output_task_handle);
        output_task_handle = NULL;
    }

    output_set_state(false);
    output_global_time = 0;
    output_start_time = 0;
    output_fsm_state = OUTPUT_STATE_INIT;
}
// End void output_stop(void)


output_config_t output_get_config(void)
{
    return output_config;
}
// End output_config_t output_get_config(void)


void output_set_preheat_temp(uint16_t preheat_temp)
{
    if (nvs_write_u16(NVS_KEY_OUT_PREHEAT_TEMP, preheat_temp))
    {
        output_config.preheat_temp = preheat_temp;
    }
}
// End void output_set_preheat_temp(uint16_t preheat_temp)


void output_set_preheat_hist(uint16_t preheat_hist)
{
    if (nvs_write_u16(NVS_KEY_OUT_PREHEAT_HIST, preheat_hist))
    {
        output_config.preheat_hist = preheat_hist;
    }
}
// End void output_set_preheat_hist(uint16_t preheat_hist)


void output_set_preheat_time(uint16_t preheat_time)
{
    if (nvs_write_u16(NVS_KEY_OUT_PREHEAT_TIME, preheat_time))
    {
        output_config.preheat_time = preheat_time;
    }
}
// End void output_set_preheat_time(uint16_t preheat_time)


void output_set_preheat_brake(uint16_t preheat_brake)
{
    if (nvs_write_u16(NVS_KEY_OUT_PREHEAT_BRAKE, preheat_brake))
    {
        output_config.preheat_brake = preheat_brake;
    }
}
// End void output_set_preheat_brake(uint16_t preheat_brake)


void output_set_reflow_temp(uint16_t reflow_temp)
{
    if (nvs_write_u16(NVS_KEY_OUT_REFLOW_TEMP, reflow_temp))
    {
        output_config.reflow_temp = reflow_temp;
    }
}
// End void output_set_reflow_temp(uint16_t reflow_temp)


void output_set_reflow_brake(uint16_t reflow_brake)
{
    if (nvs_write_u16(NVS_KEY_OUT_REFLOW_BRAKE, reflow_brake))
    {
        output_config.reflow_brake = reflow_brake;
    }
}
// End void output_set_reflow_brake(uint16_t reflow_brake)


void output_set_cooling_time(uint16_t cooling_time)
{
    if (nvs_write_u16(NVS_KEY_OUT_COOLING_TIME, cooling_time))
    {
        output_config.cooling_time = cooling_time;
    }
}
// End void output_set_cooling_time(uint16_t cooling_time)


uint16_t * output_get_temp_array(void)
{
    return array_temperature;
}
// End uint16_t * output_get_temp_array(void)


uint16_t output_get_num_array(void)
{
    return output_global_time;
}
// End uint16_t output_get_num_array(void)


void output_2_sec(void)
{
    gpio_set_level(OUTPUT_GPIO, true);
    vTaskDelay(pdMS_TO_TICKS(2000));
    gpio_set_level(OUTPUT_GPIO, false);
}
// End void output_2_sec(void)

//=============================================================================

//====================== [End Document] =======================================
