/**
 * @file output.h
 * @author
 * @date
 * @brief
 */

#ifndef _OUTPUT_H_
#define _OUTPUT_H_

//=============================================================================
// [Inclusions] ===============================================================

//=============================================================================

//=============================================================================
// [External Defines] =========================================================

//=============================================================================

//=============================================================================
// [External Typedef] =========================================================

typedef struct
{
    uint16_t preheat_temp;
    uint16_t preheat_hist;
    uint16_t preheat_time;
    uint16_t preheat_brake;
    uint16_t reflow_temp;
    uint16_t reflow_brake;
    uint16_t cooling_time;
} output_config_t;

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
// [External Function Declarations] ===========================================

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_init(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_state(bool state);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_toggle_state(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_start(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_stop(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_preheat_temp(uint16_t preheat_temp);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_preheat_hist(uint16_t preheat_hist);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_preheat_time(uint16_t preheat_time);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_preheat_brake(uint16_t preheat_brake);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_reflow_temp(uint16_t reflow_temp);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_reflow_brake(uint16_t reflow_brake);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_set_cooling_time(uint16_t cooling_time);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
uint16_t * output_get_temp_array(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
uint16_t output_get_num_array(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
output_config_t output_get_config(void);


/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void output_2_sec(void);

//=============================================================================

#endif /* _OUTPUT_H_ */
