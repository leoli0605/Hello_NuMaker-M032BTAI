/************************************************************************
 *
 * File Name  : BLE_SERVICE_HID_KB_CS.c
 * Description: This file contains the definitions and functions of BLE HID_KB_CS
 *
 *
 ************************************************************************/
#include "ble_service_hid_kb_cs.h"
#include "ble_profile.h"

/** ATTR_VALUE_HID_General_Access
 * @note This callback receives the HID Service events.  \n
 *  Each of these events can be associated with parameters.
 */
void ATTR_VALUE_HID_General_Access(BLE_Event_AttParam *attParam);

/**************************************************************************
 * HID Service UUID Definitions
 **************************************************************************/

/** HID Service UUID.
 * @note 16-bits UUID
 * @note UUID: 1812
*/
const uint16_t ATTR_UUID_HID_PRIMARY_SERVICE[] =
{
    GATT_SERVICES_HUMAN_INTERFACE_DEVICE,
};

/** HID Information characteristic UUID.
 * @note 16-bits UUID
 * @note UUID: 2A4A
*/
const uint16_t ATTR_UUID_HID_CHARC_INFORMATION[] =
{
    GATT_SPEC_CHARC_HID_INFORMATION,
};

/** HID Control point characteristic UUID.
 * @note 16-bits UUID
 * @note UUID: 2A4C
*/
const uint16_t ATTR_UUID_HID_CHARC_CONTROL_POINT[] =
{
    GATT_SPEC_CHARC_HID_CONTROL_POINT,
};

/** HID Report map characteristic UUID.
 * @note 16-bits UUID
 * @note UUID: 2A4B
*/
const uint16_t ATTR_UUID_HID_CHARC_REPORT_MAP[] =
{
    GATT_SPEC_CHARC_REPORT_MAP,
};

/** HID Protocol mode characteristic UUID.
 * @note 16-bits UUID
 * @note UUID: 2A4E
*/
const uint16_t ATTR_UUID_HID_CHARC_PROTOCOL_MODE[] =
{
    GATT_SPEC_CHARC_PROTOCOL_MODE,
};

/** HID Report characteristic UUID.
 * @note 16-bits UUID
 * @note UUID: 2A4D
*/
const uint16_t ATTR_UUID_HID_CHARC_REPORT[] =
{
    GATT_SPEC_CHARC_REPORT,
};

/** HID BOOT keyboard input report characteristic UUID.
 * @note 16-bits UUID
 * @note UUID: 2A22
*/
const uint16_t ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_INPUT_REPORT[] =
{
    GATT_SPEC_CHARC_BOOT_KEYBOARD_INPUT_REPORT,
};

/** HID BOOT keyboard output report characteristic UUID.
 * @note 16-bits UUID
 * @note UUID: 2A32
*/
const uint16_t ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_OUTPUT_REPORT[] =
{
    GATT_SPEC_CHARC_BOOT_KEYBOARD_OUTPUT_REPORT,
};

/**************************************************************************
 * HID Service Value Definitions
 **************************************************************************/

/** HID characteristic HID Information value.
 * @note Return the "Read data" when central send "Read Request".
*/
const uint8_t HID_INFORMATION[] =
{
    0x13, 0x02,     //bcdHID: 0x0213
    0x40,           //bCountryCode: 0x40
    0x01,           //Flags: 0x_1 (providing wake-up signal to a HID host)
};

/** HID characteristic HID report map value.
 * @note Return the "Read data" when central send "Read Request".
*/
const uint8_t HID_REPORT_MAP[] =    //Device Class Definition for Human Interface Devices (HID) Version 1.11, 6.2.2 Report Descriptor
{
    0x05, 0x01,
    0x09, 0x06,
    0xa1, 0x01,
    0x85, 0x01,
    0x05, 0x08,
    0x19, 0x01,
    0x29, 0x03,
    0x75, 0x01,
    0x95, 0x03,
    0x15, 0x00,
    0x25, 0x01,
    0x91, 0x02,
    0x95, 0x05,
    0x91, 0x01,
    0x05, 0x07,
    0x19, 0xe0,
    0x29, 0xe7,
    0x95, 0x08,
    0x81, 0x02,
    0x75, 0x08,
    0x95, 0x01,
    0x81, 0x01,
    0x19, 0x00,
    0x29, 0x91,
    0x26, 0xff, 0x00,
    0x95, 0x06,
    0x81, 0x00,
    0xc0,

    0x05, 0x0C,
    0x09, 0x01,
    0xA1, 0x01,
    0x85, 0x03,
    0x19, 0x00,
    0x2A, 0x9C, 0x02,
    0x15, 0x00,
    0x26, 0x9C, 0x02,
    0x75, 0x10,
    0x95, 0x01,
    0x81, 0x00,
    0xC0,
};

const uint8_t ATT_HDL_HIDS_REPORT_KBI_REPORT_REFERENCE[] =
{
    0x01,    //Report ID
    0x01     //Report Type: Input (0x01)
};

const uint8_t ATT_HDL_HIDS_REPORT_KBO_REPORT_REFERENCE[] =
{
    0x01,    //Report ID : 01
    0x02     //Report Type: Output (0x02)
};

const uint8_t ATT_HDL_HIDS_REPORT_CSI_REPORT_REFERENCE[] =
{
    0x03,    //Report ID
    0x01     //Report Type: Input (0x01)
};
/**************************************************************************
 * HID Service/ Characteristic Definitions
 **************************************************************************/

const ATTRIBUTE_BLE ATT_HID_PRIMARY_SERVICE =
{
    (void *)ATTR_UUID_TYPE_PRIMARY_SERVICE,
    (void *)ATTR_UUID_HID_PRIMARY_SERVICE,
    sizeof(ATTR_UUID_HID_PRIMARY_SERVICE),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_INFORMATION =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_INFORMATION,
    sizeof(ATTR_UUID_HID_CHARC_INFORMATION),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_INFORMATION =
{
    (void *)ATTR_UUID_HID_CHARC_INFORMATION,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_CONTROL_POINT =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_CONTROL_POINT,
    sizeof(ATTR_UUID_HID_CHARC_CONTROL_POINT),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_CONTROL_POINT =
{
    (void *)ATTR_UUID_HID_CHARC_CONTROL_POINT,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        //GATT_DECLARATIONS_PROPERTIES_READ |
        GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_PROTOCOL_MODE =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_PROTOCOL_MODE,
    sizeof(ATTR_UUID_HID_CHARC_PROTOCOL_MODE),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_PROTOCOL_MODE =
{
    (void *)ATTR_UUID_HID_CHARC_PROTOCOL_MODE,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_REPORT_MAP =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_REPORT_MAP,
    sizeof(ATTR_UUID_HID_CHARC_REPORT_MAP),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_REPORT_MAP =
{
    (void *)ATTR_UUID_HID_CHARC_REPORT_MAP,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};




const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_BOOT_KEYBOARD_INPUT_REPORT =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_INPUT_REPORT,
    sizeof(ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_INPUT_REPORT),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_BOOT_KEYBOARD_INPUT_REPORT =
{
    (void *)ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_INPUT_REPORT,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_BOOT_KEYBOARD_INPUT_NOTIFY_CLIENT_CHARC_CONFIGURATION =
{
    (void *)ATTR_UUID_TYPE_CLIENT_CHARC_CONFIGURATION,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_BOOT_KEYBOARD_OUTPUT_REPORT =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_OUTPUT_REPORT,
    sizeof(ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_OUTPUT_REPORT),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_BOOT_KEYBOARD_OUTPUT_REPORT =
{
    (void *)ATTR_UUID_HID_CHARC_BOOT_KEYBOARD_OUTPUT_REPORT,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_KEYBOARD_INPUT_REPORT =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_REPORT,
    sizeof(ATTR_UUID_HID_CHARC_REPORT),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_KEYBOARD_INPUT_REPORT =
{
    (void *)ATTR_UUID_HID_CHARC_REPORT,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_KEYBOARD_INPUT_NOTIFY_CLIENT_CHARC_CONFIGURATION =
{
    (void *)ATTR_UUID_TYPE_CLIENT_CHARC_CONFIGURATION,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_KEYBOARD_INPUT_REPORT_REFERENCE =
{
    (void *)ATTR_UUID_TYPE_REPORT_REFERENCE,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_KEYBOARD_OUTPUT_REPORT =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_REPORT,
    sizeof(ATTR_UUID_HID_CHARC_REPORT),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_KEYBOARD_OUTPUT_REPORT =
{
    (void *)ATTR_UUID_HID_CHARC_REPORT,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_KEYBOARD_OUTPUT_REPORT_REFERENCE =
{
    (void *)ATTR_UUID_TYPE_REPORT_REFERENCE,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CHARACTERISTIC_CONSUMER_INPUT_REPORT =
{
    (void *)ATTR_UUID_TYPE_CHARACTERISTIC,
    (void *)ATTR_UUID_HID_CHARC_REPORT,
    sizeof(ATTR_UUID_HID_CHARC_REPORT),
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_NULL_Access,                       //This function should be set to ATTR_NULL_Access when lenValue or uuidValue is a null value.
};

const ATTRIBUTE_BLE ATT_HID_CONSUMER_INPUT_REPORT =
{
    (void *)ATTR_UUID_HID_CHARC_REPORT,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CONSUMER_INPUT_NOTIFY_CLIENT_CHARC_CONFIGURATION =
{
    (void *)ATTR_UUID_TYPE_CLIENT_CHARC_CONFIGURATION,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        //ATT_PERMISSION_AUTHE_READ |
        ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};

const ATTRIBUTE_BLE ATT_HID_CONSUMER_INPUT_REPORT_REFERENCE =
{
    (void *)ATTR_UUID_TYPE_REPORT_REFERENCE,
    (void *)0,
    0,
    (
        //GATT_DECLARATIONS_PROPERTIES_BROADCAST |
        GATT_DECLARATIONS_PROPERTIES_READ |
        //GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE |
        //GATT_DECLARATIONS_PROPERTIES_WRITE |
        //GATT_DECLARATIONS_PROPERTIES_NOTIFY |
        //GATT_DECLARATIONS_PROPERTIES_INDICATE |
        //GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES |
        //GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES |
        0x00
    ),
    (
        ATT_TYPE_FORMAT_16UUID |            //otherwise, 128bit UUID
        //ATT_VALUE_BOND_ENABLE |
        //ATT_PERMISSION_ENC_READ |
        //ATT_PERMISSION_ENC_WRITE |
        ATT_PERMISSION_AUTHE_READ |
        //ATT_PERMISSION_AUTHE_WRITE |
        //ATT_PERMISSION_AUTHO_READ |
        //ATT_PERMISSION_AUTHO_WRITE |
        0x00
    ),
    ATTR_VALUE_HID_General_Access,         //registered callback function
};


/**************************************************************************
 * BLE Service << HID >> Local Variable
 **************************************************************************/
#ifndef MAX_NUM_CONN_HID
// check MAX_NUM_CONN_HID if defined or set to default 1.
#define MAX_NUM_CONN_HID       1
#endif


// Service basic information
Service_Basic_Info             hidBasicInfo[MAX_NUM_CONN_HID];

// HID information
BLEATT_HID_Info                *hidInfo[MAX_NUM_CONN_HID];

// HID callback function
BleHID_EventCallBack           hidCallback[MAX_NUM_CONN_HID];

// HID registered total count
uint8_t hid_count = 0;

/**************************************************************************
 * BLE Service << HID >> Public Function
 **************************************************************************/
/** HID Service Initialization
 *
 * @param[in] hostId : the link's host id.
 * @param[in] gattRole : @ref BleGattRole "BLE GATT role".
 * @param[in] info : a pointer to HID information.
 * @param[in] callback : a pointer to a callback function that receive the service events.
 *
 * @retval BLESTACK_STATUS_ERR_INVALID_HOSTID : Error host id.
 * @retval BLESTACK_STATUS_ERR_INVALID_PARAM : Invalid parameter.
 * @retval BLESTACK_STATUS_ERR_NOT_SUPPORTED : Registered services buffer full.
 * @retval BLESTACK_STATUS_SUCCESS  : Setting success.
*/
BleStackStatus setHID_ServiceInit(uint8_t hostId, BleGattRole gattRole, BLEATT_HID_Info *info, BleHID_EventCallBack callback)
{
    BleStackStatus status;
    uint8_t config_index;

    if (info == NULL)
    {
        return BLESTACK_STATUS_ERR_INVALID_PARAM;
    }

    // init service basic information and get "config_index" & "hid_count"
    status = setBLE_ServiceBasicInit(hostId, gattRole, MAX_NUM_CONN_HID, hidBasicInfo, &config_index, &hid_count);
    BLESTACK_STATUS_CHECK(status);

    // Set service role
    info->role = gattRole;

    // Set HID data
    hidInfo[config_index] = info;

    // Register HID callback function
    hidCallback[config_index] = callback;

    // Get handles at initialization if role is set to BLE_GATT_ROLE_SERVER
    if (gattRole == BLE_GATT_ROLE_SERVER)
    {
        status = getHID_ServiceHandles(hostId, hidInfo[config_index]);
        BLESTACK_STATUS_CHECK(status);
    }

    return BLESTACK_STATUS_SUCCESS;
}



/** Get HID Service Handle Numbers
 *
 * @attention
 *            - role = <b> @ref BLE_GATT_ROLE_CLIENT </b> \n
 *              MUST call this API to get service information after received @ref BLECMD_EVENT_ATT_DATABASE_PARSING_FINISHED  \n
 *            - role = <b> @ref BLE_GATT_ROLE_SERVER </b> \n
 *              MUST call this API to get service information before connection established. \n
 *
 * @param[in] hostId : the link's host id.
 * @param[out] info : a pointer to HID information
 *
 * @retval BLESTACK_STATUS_ERR_INVALID_HOSTID : Error host id.
 * @retval BLESTACK_STATUS_ERR_INVALID_PARAM : Invalid parameter.
 * @retval BLESTACK_STATUS_SUCCESS  : Setting success.
*/
BleStackStatus getHID_ServiceHandles(uint8_t hostId, BLEATT_HID_Info *info)
{
    BleStackStatus status;

    // Get HID handles
    status = getBLEGATT_HandleNumAddr(hostId, info->role, (ATTRIBUTE_BLE *)&ATT_HID_PRIMARY_SERVICE, (void *)&info->handles);
    BLESTACK_STATUS_CHECK(status);

    return BLESTACK_STATUS_SUCCESS;
}


/**************************************************************************
 * BLE Service << HID >> General Callback Function
 **************************************************************************/
static void bleHID_PostEvent(BLE_Event_AttParam *attParam, BleHID_EventCallBack *callback)
{
    // check callback is null or not
    SERVICE_CALLBACK_NULL_CHECK(*callback);

    // post event to user
    (*callback)(attParam->hostId, attParam->cmdAccess, attParam->data, attParam->length);
}

// handle HID client GATT event
static void handle_HID_client(uint8_t index, BLE_Event_AttParam *attParam)
{
    switch (attParam->cmdAccess)
    {


    default:
        break;
    }
}

// handle HID server GATT event
static void handle_HID_server(uint8_t index, BLE_Event_AttParam *attParam)
{
    switch (attParam->cmdAccess)
    {
    case OPCODE_ATT_READ_BY_TYPE_REQUEST:
    case OPCODE_ATT_READ_REQUEST:
        //============== handle cccd ===================================================================
        if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_boot_keyboard_intput_report_cccd)
        {
            // send cccd Read or Read By Type Response
            setBLEGATT_HandleCCCDGeneralReadRequest(attParam->hostId,
                                                    attParam->cmdAccess,
                                                    hidInfo[index]->handles.hdl_hid_boot_keyboard_intput_report_cccd,
                                                    hidInfo[index]->data.hid_boot_keyboard_intput_report_cccd);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_intput_report_cccd)
        {
            // send cccd Read or Read By Type Response
            setBLEGATT_HandleCCCDGeneralReadRequest(attParam->hostId,
                                                    attParam->cmdAccess,
                                                    hidInfo[index]->handles.hdl_hid_keyboard_intput_report_cccd,
                                                    hidInfo[index]->data.hid_keyboard_intput_report_cccd);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_consumer_input_report_cccd)
        {
            // send cccd read or read by type Response
            setBLEGATT_HandleCCCDGeneralReadRequest(attParam->hostId,
                                                    attParam->cmdAccess,
                                                    hidInfo[index]->handles.hdl_hid_consumer_input_report_cccd,
                                                    hidInfo[index]->data.hid_consumer_input_report_cccd);
        }
        //============== send read rsp ===========================================================
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_information)
        {
            setBLEGATT_GeneralReadRsp(attParam->hostId, attParam->hdlNum, (uint8_t *)HID_INFORMATION, sizeof(HID_INFORMATION));
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_report_map)
        {
            setBLEGATT_GeneralReadRsp(attParam->hostId, attParam->hdlNum, (uint8_t *)HID_REPORT_MAP, sizeof(HID_REPORT_MAP));
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_input_report_reference)
        {
            setBLEGATT_GeneralReadRsp(attParam->hostId, attParam->hdlNum, (uint8_t *)ATT_HDL_HIDS_REPORT_KBI_REPORT_REFERENCE, sizeof(ATT_HDL_HIDS_REPORT_KBI_REPORT_REFERENCE));
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_output_report_reference)
        {
            setBLEGATT_GeneralReadRsp(attParam->hostId, attParam->hdlNum, (uint8_t *)ATT_HDL_HIDS_REPORT_KBO_REPORT_REFERENCE, sizeof(ATT_HDL_HIDS_REPORT_KBO_REPORT_REFERENCE));
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_consumer_input_report_reference)
        {
            setBLEGATT_GeneralReadRsp(attParam->hostId, attParam->hdlNum, (uint8_t *)ATT_HDL_HIDS_REPORT_CSI_REPORT_REFERENCE, sizeof(ATT_HDL_HIDS_REPORT_CSI_REPORT_REFERENCE));
        }
        //============== post event to user ===========================================================
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_protocol_mode)
        {
            // received read or read by type request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_PROTOCOL_MODE_READ_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_boot_keyboard_intput_report)
        {
            // received read request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_BOOT_KEYBOARD_INPUT_REPORT_READ_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_boot_keyboard_output_report)
        {
            // received read request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_BOOT_KEYBOARD_OUTPUT_REPORT_READ_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_intput_report)
        {
            // received read request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_KEYBOARD_INPUT_REPORT_READ_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_output_report)
        {
            // received read request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_KEYBOARD_OUTPUT_REPORT_READ_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_consumer_input_report)
        {
            // received read or read by type request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_CONSUMER_INPUT_REPORT_READ_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        break;

    case OPCODE_ATT_READ_BLOB_REQUEST:
        if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_report_map)
        {
            setBLEGATT_HandleReadBlobRequest(attParam, (uint8_t *)HID_REPORT_MAP, sizeof(HID_REPORT_MAP));
        }
        break;

    case OPCODE_ATT_WRITE_REQUEST:
    case OPCODE_ATT_RESTORE_BOND_DATA_COMMAND:
        //============== handle cccd ===================================================================
        if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_boot_keyboard_intput_report_cccd)
        {
            // update server defined cccd value
            setBLEGATT_HandleCCCDWriteRequest(attParam->data, attParam->length, &hidInfo[index]->data.hid_boot_keyboard_intput_report_cccd);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_intput_report_cccd)
        {
            // update server defined cccd value
            setBLEGATT_HandleCCCDWriteRequest(attParam->data, attParam->length, &hidInfo[index]->data.hid_keyboard_intput_report_cccd);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_consumer_input_report_cccd)
        {
            // update server defined cccd value
            setBLEGATT_HandleCCCDWriteRequest(attParam->data, attParam->length, &hidInfo[index]->data.hid_consumer_input_report_cccd);
        }
        //============== post event to user =============================================================
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_boot_keyboard_intput_report)
        {
            // received write request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_BOOT_KEYBOARD_INPUT_REPORT_WRITE_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_boot_keyboard_output_report)
        {
            // received write request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_BOOT_KEYBOARD_OUTPUT_REPORT_WRITE_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_intput_report)
        {
            // received write request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_KEYBOARD_INPUT_REPORT_WRITE_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_keyboard_output_report)
        {
            // received write request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_KEYBOARD_OUTPUT_REPORT_WRITE_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_consumer_input_report)
        {
            // received write request from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_CONSUMER_INPUT_REPORT_WRITE_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        break;

    case OPCODE_ATT_WRITE_COMMAND:
        if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_control_point)
        {
            // received write command from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_CONTROL_POINT_WRITE_EVNET;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        else if (attParam->hdlNum == hidInfo[index]->handles.hdl_hid_protocol_mode)
        {
            // received write command from client -> post to user
            attParam->cmdAccess = BLESERVICE_HID_PROTOCOL_MODE_WRITE_EVENT;
            bleHID_PostEvent(attParam, &hidCallback[index]);
        }
        break;

    default:
        break;
    }
}

void ATTR_VALUE_HID_General_Access(BLE_Event_AttParam *attParam)
{
    uint8_t index;

    if (queryIndexByHostIdGattRole(attParam->hostId, attParam->gattRole, MAX_NUM_CONN_HID, hidBasicInfo, &index) != BLESTACK_STATUS_SUCCESS)
    {
        // Host id has not registered so there is no callback function -> do nothing
        return;
    }

    if (attParam->gattRole == BLE_GATT_ROLE_CLIENT)
    {
        // handle HID client GATT event
        handle_HID_client(index, attParam);
    }

    if (attParam->gattRole == BLE_GATT_ROLE_SERVER)
    {
        // handle HID server GATT event
        handle_HID_server(index, attParam);
    }
}
