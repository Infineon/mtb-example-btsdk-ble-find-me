/*
 * Copyright 2016-2021, Cypress Semiconductor Corporation (an Infineon company) or
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

/* This file is gatts functions for all devices with BTSTACK version lower than 3.0, i.e. 20xxx and 43012C0 */

#include "wiced_bt_gatt.h"
#include "wiced_bt_trace.h"
#include "app_bt_event_handler.h"

/**************************************************************************************************
* Function Name: app_bt_read_handle_value()
***************************************************************************************************
* Summary:
*   This function handles reading of the attribute value from the GATT database and passing the
*   data to the BT stack. The value read from the GATT database is stored in a buffer whose
*   starting address is passed as one of the function parameters
*
* Parameters:
*   uint16_t handle     : Attribute handle for read operation
*   uint16_t offset     : offset to read
*   uint8_t *buff       : Pointer to the buffer to store read data (need to make sure butter is large enough to hold len data
*   uint16_t *p_len     : input & output of length pointer
*
* Return:
*   wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t app_bt_read_handle_value(uint16_t handle, uint8_t offset, uint8_t *buff, uint16_t *p_len)
{
    gatt_db_lookup_table_t *p_attribute = app_get_attribute(handle);
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;

    if (p_attribute != NULL)
    {
        if (offset < p_attribute->max_len)
        {
            // if requested len is larger than available data, change copy length to available data
            if (*p_len > p_attribute->max_len - offset)
            {
                *p_len = p_attribute->max_len - offset;
            }

            /* Value fits within the supplied buffer; copy over the value */
            memcpy(buff, p_attribute->p_data+offset, *p_len);
            res = WICED_BT_GATT_SUCCESS;
        }
        else
        {
            WICED_BT_TRACE("offset:%d larger than attribute length:%d", offset, p_attribute->max_len);
            res = WICED_BT_GATT_INVALID_OFFSET;
        }
    }
    else
    {
        WICED_BT_TRACE("Read Request to Invalid Handle: 0x%x\n\r", handle);
    }
    return res;
}

/**************************************************************************************************
* Function Name: app_gatt_req_event()
***************************************************************************************************
* Summary:
*   This function handles GATT server events from the BT stack.
*
* Parameters:
*   uint16_t conn_id                            : Connection ID
*   wiced_bt_gatt_request_type_t type           : Type of GATT server event
*   wiced_bt_gatt_request_data_t *p_data        : Pointer to GATT server event data
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t app_gatt_req_event(uint16_t conn_id, wiced_bt_gatt_request_type_t type, wiced_bt_gatt_request_data_t *p_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    switch ( type )
    {
        case GATTS_REQ_TYPE_READ:
            /* Attribute read request */
            status = app_bt_read_handle_value(p_data->read_req.handle, p_data->read_req.offset, p_data->read_req.p_val, p_data->read_req.p_val_len);
            break;
        case GATTS_REQ_TYPE_WRITE:
            /* Attribute write request */
            status = app_bt_write_handle_value(p_data->write_req.handle, p_data->write_req.offset, p_data->write_req.p_val, p_data->write_req.val_len);
            break;
    }

    return status;
}


/**************************************************************************************************
* Function Name: app_gatt_event_callback()
***************************************************************************************************
* Summary:
*   This function handles GATT events from the BT stack.
*
* Parameters:
*   wiced_bt_gatt_evt_t event                   : BLE GATT event code of one byte length
*   wiced_bt_gatt_event_data_t *p_event_data    : Pointer to BLE GATT event structures
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_gatt_event_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;
    wiced_bt_gatt_connection_status_t *p_conn_status = NULL;
    wiced_bt_gatt_attribute_request_t *p_attr_req = NULL;

    /* Call the appropriate callback function based on the GATT event type, and pass the relevant event
     * parameters to the callback function */
    switch ( event )
    {
        case GATT_CONNECTION_STATUS_EVT:
            status = app_bt_event_connect( &p_event_data->connection_status );
            break;

        case GATT_ATTRIBUTE_REQUEST_EVT:
            p_attr_req = &p_event_data->attribute_request;
            status = app_gatt_req_event( p_attr_req->conn_id, p_attr_req->request_type, &p_attr_req->data );
            break;

        default:
            status = WICED_BT_GATT_SUCCESS;
            break;
    }

    return status;
}
