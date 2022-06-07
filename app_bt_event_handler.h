/*
 * Copyright 2016-2022, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/*******************************************************************************
* File Name: app_bt_event_handler.h
* Version: 1.0
*
* Description:
*   Header file for handling Bluetooth stack events at the application level
*
*******************************************************************************/

#ifndef APP_BT_EVENT_HANDLER_H_
#define APP_BT_EVENT_HANDLER_H_

/*******************************************************************************
*        Header Files
*******************************************************************************/
#include "wiced_bt_dev.h"
#include "wiced_bt_gatt.h"
#include "cycfg_gatt_db.h"


/*******************************************************************************
*        Macro Definitions
*******************************************************************************/
/* This enumeration combines the advertising, connection states from two different
 * callbacks to maintain the status in a single state variable */
typedef enum
{
    APP_BT_ADV_OFF_CONN_OFF,
    APP_BT_ADV_ON_CONN_OFF,
    APP_BT_ADV_OFF_CONN_ON
} app_bt_adv_conn_mode_t;

/*******************************************************************************
*        External Variable Declarations
*******************************************************************************/
/* State variable to track BLE advertising/connection state at the application
 * level */
extern app_bt_adv_conn_mode_t app_bt_adv_conn_state;

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
/* Callback function for Bluetooth stack management type events */
wiced_bt_dev_status_t app_bt_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);

/**************************************************************************************************
* Function Name: app_bt_event_connect()
***************************************************************************************************
* Summary:
*   This callback function handles connection status changes.
*
* Parameters:
*   wiced_bt_gatt_connection_status_t *p_conn_status  : Pointer to data that has connection details
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_bt_event_connect       (wiced_bt_gatt_connection_status_t *p_conn_status);

/**************************************************************************************************
* Function Name: app_bt_write_handle_value()
***************************************************************************************************
* Summary:
*   This function handles writing to the attribute handle in the GATT database using the
*   data passed from the BT stack. The value to write is stored in a buffer
*   whose starting address is passed as one of the function parameters
*
* Parameters:
*   uint16_t handle                    : Attribute handle for write operation
*   uint16_t offset                    : attribute offset to write
*   uint8_t *buffer                    : Pointer to the buffer that stores the data to be written
*   uint16_t len                       : Length of data to be written
*
* Return:
*   wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_bt_write_handle_value(uint16_t handle, uint16_t offset, uint8_t *buff, uint16_t len);

/**************************************************************************************************
* Function Name: app_get_attribute()
***************************************************************************************************
* Summary:
*   This function returns attribute lookup pointer by given handle
*
* Parameters:
*   uint16_t attr_handle                    : Attribute handle for read operation
*
* Return:
*   NULL if handle not found, otherwise, it returns pointer to the attribute lookup item
*
**************************************************************************************************/
gatt_db_lookup_table_t * app_get_attribute(uint16_t handle);

#endif /* APP_BT_EVENT_HANDLER_H_ */
