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
* File Name: app_bt_event_handler.c
* Version: 1.0
*
* Description:
*   Source file for handling Bluetooth stack events at the application level
*
*******************************************************************************/

/*******************************************************************************
*        Header Files
*******************************************************************************/
#include "app_user_interface.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_trace.h"
#include "wiced_bt_ble.h"
#include "app_bt_event_handler.h"
#include "app_gatts.h"
#include "app_cfg.h"

/*******************************************************************************
*        Variable Definitions
*******************************************************************************/
uint16_t bt_connection_id = 0;
app_bt_adv_conn_mode_t app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_OFF;

/*******************************************************************************
*        External Variable Declarations
*******************************************************************************/

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
static void                   ble_app_init               (void);
static void                   ble_app_set_advertisement_data (void);

/*******************************************************************************
*        Function Definitions
*******************************************************************************/

/**************************************************************************************************
* Function Name: app_bt_management_callback()
***************************************************************************************************
* Summary:
*   This is a Bluetooth stack event handler function to receive management events from
*   the LE stack and process as per the application.
*
* Parameters:
*   wiced_bt_management_evt_t event             : LE event code of one byte length
*   wiced_bt_management_evt_data_t *p_event_data: Pointer to LE management event structures
*
* Return:
*  wiced_result_t: Error code from WICED_RESULT_LIST or BT_RESULT_LIST
*
*************************************************************************************************/
wiced_result_t app_bt_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_result_t status = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda = { 0 };
    wiced_bt_dev_ble_pairing_info_t *p_ble_info = NULL;
    wiced_bt_ble_advert_mode_t *p_adv_mode = NULL;

    switch (event)
    {
        case BTM_ENABLED_EVT:

            /* Bluetooth Controller and Host Stack Enabled */
            WICED_BT_TRACE("Bluetooth Enabled (%s)\n\r",
                    ((WICED_BT_SUCCESS == p_event_data->enabled.status) ? "success" : "failure"));

            if (WICED_BT_SUCCESS == p_event_data->enabled.status)
            {
                /* Bluetooth is enabled */
                wiced_bt_dev_read_local_addr(bda);
                WICED_BT_TRACE("Local Bluetooth Address: [%B]\n\r", bda);

                /* Perform application-specific initialization */
                ble_app_init();
            }

            break;

        case BTM_BLE_ADVERT_STATE_CHANGED_EVT:

            /* Advertisement State Changed */
            p_adv_mode = &p_event_data->ble_advert_state_changed;
            WICED_BT_TRACE("Advertisement State Change: %d\n\r", *p_adv_mode);

            if (BTM_BLE_ADVERT_OFF == *p_adv_mode)
            {
                /* Advertisement Stopped */
                WICED_BT_TRACE("Advertisement stopped\n\r");

                /* Check connection status after advertisement stops */
                if(bt_connection_id == 0)
                {
                    app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_OFF;
                }
                else
                {
                    app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_ON;
                }
            }
            else
            {
                /* Advertisement Started */
                WICED_BT_TRACE("Advertisement started\n\r");
                app_bt_adv_conn_state = APP_BT_ADV_ON_CONN_OFF;
            }

            /* Update Advertisement LED to reflect the updated state */
            adv_led_update();

            break;

        default:
            WICED_BT_TRACE("Unhandled Bluetooth Management Event: 0x%x (%d)\n\r", event, event);
            break;
    }

    return status;
}

/*
 * This function is executed in the BTM_ENABLED_EVT management callback.
 */

/**************************************************************************************************
* Function Name: ble_app_init()
***************************************************************************************************
* Summary:
*   This function handles application level initialization tasks and is called from the BT
*   management callback once the LE stack enabled event (BTM_ENABLED_EVT) is triggered
*
* Parameters:
*   None
*
* Return:
*  None
*
*************************************************************************************************/
static void ble_app_init(void)
{
    /* User interface initialization for LEDs, buttons */
    app_user_interface_init();

    /* Disable pairing for this application */
    wiced_bt_set_pairable_mode(WICED_FALSE, 0);

    /* Set Advertisement Data */
    ble_app_set_advertisement_data();

    /* Register with BT stack to receive GATT callback */
    wiced_bt_gatt_register(app_gatt_event_callback);

    /* Initialize GATT Database */
    app_bt_gatt_db_init(gatt_database, gatt_database_len);

    /* Start Undirected LE Advertisements on device startup.
     * The corresponding parameters are contained in 'app_bt_cfg.c' */
    wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
}

/**************************************************************************************************
* Function Name: ble_app_set_advertisement_data()
***************************************************************************************************
* Summary:
*   This function configures the advertisement packet data
*
* Parameters:
*   None
*
* Return:
*   None
*
**************************************************************************************************/
static void ble_app_set_advertisement_data(void)
{
    wiced_bt_ble_advert_elem_t adv_elem[3] = { 0 };
    uint8_t adv_flag = BTM_BLE_GENERAL_DISCOVERABLE_FLAG | BTM_BLE_BREDR_NOT_SUPPORTED;
    uint8_t adv_appearance[] = { BIT16_TO_8( APPEARANCE_GENERIC_KEYRING ) };
    uint8_t num_elem = 0;

    /* Advertisement Element for Flags */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len = sizeof(uint8_t);
    adv_elem[num_elem].p_data = &adv_flag;
    num_elem++;

    /* Advertisement Element for Name */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len = app_gap_device_name_len;
    adv_elem[num_elem].p_data = app_gap_device_name;
    num_elem++;

    /* Advertisement Element for Appearance */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_APPEARANCE;
    adv_elem[num_elem].len = sizeof(adv_appearance);
    adv_elem[num_elem].p_data = adv_appearance;
    num_elem++;

    /* Set Raw Advertisement Data */
    wiced_bt_ble_set_raw_advertisement_data(num_elem, adv_elem);
}

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
gatt_db_lookup_table_t * app_get_attribute(uint16_t handle)
{
    /* Check for a matching handle entry */
    for (int i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == handle)
        {
            return &app_gatt_db_ext_attr_tbl[i];
        }
    }
    return NULL;
}

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
wiced_bt_gatt_status_t app_bt_write_handle_value(uint16_t handle, uint16_t offset, uint8_t *p_val, uint16_t len)
{
    gatt_db_lookup_table_t *p_attribute = app_get_attribute(handle);
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;

    if (p_attribute != NULL)
    {
        if (p_attribute->max_len - offset >= len)
        {
            memcpy(p_attribute->p_data + offset, p_val, len);
            res = WICED_BT_GATT_SUCCESS;

            /* Add code for any action required when this attribute is written.
             * In this case, we update the IAS led based on the IAS alert
             * level characteristic value */

            switch ( handle )
            {
                case HDLC_IAS_ALERT_LEVEL_VALUE:
                    WICED_BT_TRACE("Alert Level = %d\n\r", app_ias_alert_level[0]);
                    ias_led_update();
                    break;
            }
        }
        else
        {
            /* Value to write does not meet size constraints */
            res = WICED_BT_GATT_INVALID_ATTR_LEN;
        }
    }
    else
    {
        WICED_BT_TRACE("Write Request to Invalid Handle: 0x%x\n\r", handle);
    }
    return res;
}

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
wiced_bt_gatt_status_t app_bt_event_connect(wiced_bt_gatt_connection_status_t *p_conn_status)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    if ( NULL != p_conn_status )
    {
        if ( p_conn_status->connected )
        {
            /* Device has connected */
            WICED_BT_TRACE("Connected : BDA '%B', Connection ID '%d'\n\r", p_conn_status->bd_addr, p_conn_status->conn_id );

            /* Store the connection ID */
            bt_connection_id = p_conn_status->conn_id;

            /* Update the adv/conn state */
            app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_ON;
        }
        else
        {
            /* Device has disconnected */
            WICED_BT_TRACE("Disconnected : BDA '%B', Connection ID '%d', Reason '%d'\n\r", p_conn_status->bd_addr, p_conn_status->conn_id, p_conn_status->reason );

            /* Set the connection id to zero to indicate disconnected state */
            bt_connection_id = 0;

            /* Restart the advertisements */
            wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);

            /* Update the adv/conn state */
            app_bt_adv_conn_state = APP_BT_ADV_ON_CONN_OFF;

            /* Turn Off the IAS LED on a disconnection */
            ias_led_update();
        }

        /* Update the advertisement LED to reflect updated state */
        adv_led_update();

        status = WICED_BT_GATT_SUCCESS;
    }

    return status;
}


/* [] END OF FILE */
