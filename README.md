# CE226123 - LE FindMe

This code example demonstrates the implementation of a simple LE Immediate Alert Service (IAS)-based Find Me Profile (FMP) using the Infineon Bluetooth&#174; SoCs and ModusToolbox&#8482; Integrated Development Environment (IDE).
This document uses CYW20819 as the reference, but the example and instructions in this document are applicable for all supported kits. See the `makefile` file in the *example* folder for a detailed list of supported devices.

## Requirements
- **Tool**: [ModusToolbox&#8482;](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/)

- **Programming Language**: C

- **Associated Parts**: [CYW20819](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/airoc-bluetooth-le-bluetooth/cyw20819/), [CYW20820](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/airoc-bluetooth-le-bluetooth/cyw20820/), [CYW20719](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/airoc-bluetooth-le-bluetooth/cyw20719/)

## Supported Kits
See the `makefile` file for a detailed list of supported kits.

## Hardware Setup
This example uses the kit's default configuration. Refer to the [kit guide](https://www.infineon.com/cms/en/product/evaluation-boards/cyw920819evb-02/), if required, to ensure the kit is configured correctly.

## Software Setup
This code example consists of two parts: a locator and a target.

For the locator, download and install the CySmart&#8482; app for [iOS](https://itunes.apple.com/us/app/cysmart/id928939093?mt=8) or [Android](https://play.google.com/store/apps/details?id=com.cypress.cysmart&hl=en).

You can also use the [CySmart Host Emulation Tool](https://www.infineon.com/cms/en/design-support/tools/utilities/wireless-connectivity/cysmart-bluetooth-le-test-and-debug-tool/) Windows PC application if you have access to the [CY5677 CySmart BLE 4.2 USB Dongle](https://www.infineon.com/cms/en/product/evaluation-boards/cy5677/). You can also use other Android or iOS apps that support the IAS service.

Scan the following QR codes from your mobile phone to download the CySmart app.

![AppQR](./Images/QR.PNG)

## Using the Code Example

Refer to the `README` file in the root of the repo for instruction on how to build and program the code example using ModusToolBox&#8482; IDE and CLI.

## Operation
1. Connect the kit to your PC using the provided USB cable.

   The USB Serial interface on the kit provides access to the two UART interfaces of the CYW20819 device - "WICED HCI UART" and "WICED Peripheral UART" (PUART). The HCI UART is used only for downloading the application code in this code example; PUART is for printing the Bluetooth&#174; stack and application trace messages.
2. Use your favorite serial terminal application and connect to the PUART serial port. Configure the terminal application to access the serial port using the following settings.
   Baud rate : 115200 bps; Data: 8 bits; Parity : None; Stop : 1 bit; Flow control : None; New line for receive data : Line Feed(LF) or Auto setting

3. Import the code example into a new or existing workspace. See [Importing Code Example into ModusToolbox&#8482; IDE - KBA225201](https://community.infineon.com/t5/Knowledge-Base-Articles/Importing-Code-Example-into-ModusToolbox-IDE-KBA225201/ta-p/250475) for details.

4.	Build and program the application: In the project explorer, select the project. The Quick Panel will get populated with various options. Scroll to the **Launches** section, and click the **Program configuration** link as shown in [Figure 1]().

##### Figure 1 Quick Panel with Program Configuration
![Figure1](./Images/figure1.PNG)

  **Note:** If the download fails, it is possible that a previously loaded application is preventing programming. For example, the application may use a custom baud rate that the download process does not detect, or the device may be in a low-power mode. In that case, it may be necessary to put the board in recovery mode, and then try the programming operation again from the IDE.
<br/>
  To enter recovery mode, first, press and hold the Recover button (SW1), then press the Reset button (SW2), release the Reset button (SW2), and then release the Recover button (SW1).

5.	Observe the yellow LED (LED1) starts blinking after the device starts advertising.

  **Note:** Some boards only support a single LED GPIO connection between the Bluetooth&#174; SoC and the baseboard.  In those cases, there is no LED1 activity for advertising, only LED2 for alert level changes.  See the application makefile and .c source file for "SINGLE_LED".

6.	To test using the CySmart mobile app, do the following (see equivalent CySmart app screenshots in [Figure 2](#figure-2-testing-with-the-cysmart-app-on-ios) and [Figure 3](#figure-3-testing-with-the-cysmart-app-on-android)):
<br/>
    1. Turn ON Bluetooth&#174; on your Android or iOS device.

    2. Launch the CySmart app.
<br/>
    3. Press the reset switch on the kit to start sending advertisements. The yellow LED (LED1) starts blinking to indicate that advertising has started. Advertising will stop after 90 seconds if a connection has not been established.

    4. Swipe down on the CySmart app home screen to start scanning for LE Peripherals; your device ("Find Me Target") appears in the CySmart app home screen. Select your device to establish a LE connection. Once the connection is established, the yellow LED (LED1) changes from blinking state to always ON state.
<br/>
    5. Select the 'Find Me' Profile from the carousel view.
<br/>
    6. Select an Alert Level value on the Find Me Profile screen. Observe the state of the red LED (LED2) on the device changes based on the alert level.

[Figure 2](#figure-2-testing-with-the-cysmart-app-on-ios) and [Figure 3](#figure-3-testing-with-the-cysmart-app-on-android) show the steps for using CySmart App on iOS and Android respectively.

##### Figure 2. Testing with the CySmart App on iOS
![Figure2](./Images/figure2.PNG)

##### Figure 3. Testing with the CySmart App on Android
![Figure3](./Images/figure3.PNG)

7.	Use the PUART serial port to view the Bluetooth&#174; stack and application trace messages in the terminal window as shown in [Figure 4]().

##### Figure 4. Log Messages on "WICED PUART" COM Port
![Figure4](./Images/figure4.PNG)

## Design and Implementation
The 'Find Me Locator' (the LE Central device) is a LE GATT Client. The 'Find Me Target' (the Peripheral device) is a LE GATT Server with the IAS implemented, as [Figure 5]() shows.

##### Figure 5. Find Me profile (FMP) Implementation
![Figure5](./Images/figure5.PNG)

The LE Find Me profile defines what happens when the locating Central device broadcasts a change in the alert level.

The Find Me locator performs service discovery using the "GATT Discover All Primary Services" procedure. The LE Service Characteristic discovery is done by the "Discover All Characteristics of a Service" procedure. When the Find Me Locator wants to cause an alert on the Find Me Target, it writes an alert level in the Alert Level Characteristic of the IAS. When the Find Me Target receives an alert level, it indicates the level using the red LED: OFF for no alert, blinking for mild alert, and ON for high alert.

The application code and Bluetooth&#174; stack runs on the Arm&#174 Cortex&#174-M4/M33 core of the SoC. The important source files relevant for the user application level code for this code example are listed in [Table 1]().

## Additional Notes:
The CYW955572BTEVK-01 is used with the audio shield board and the CUSTOM button on the shield board is configured for input by default.

##### Table 1. Important User-Application-Related Source Files

|**File Name**|**Comments**|
|-----------------------------------|-------------------------------------------------------|
|*main.c* | Contains the `application_start()` function, which is the entry point for execution of the user application code after device startup.|
|*app_bt_cfg.c, app_bt_cfg.h* |	These files contain the runtime Bluetooth&#174; stack configuration parameters such as device name and  advertisement/ connection settings. Note that the name that the device uses for advertising ("Find Me Target") is defined in *app_bt_cfg.c*.|
|*app_bt_event_handler.c, app_bt_event_handler.h*|These files contain the code for the Bluetooth&#174; stack event handler functions. |
|app_user_interface.c, app_user_interface.h*	|These files contain the code for the application user interface (in this case, the LED) functionality.|
|*cycfg_gatt_db.c, cycfg_gatt_db.h*|	These files reside in the *GeneratedSource* folder under the application folder. They contain the GATT database information generated using the Bluetooth&#174; Configurator tool.|

See [AN225684 - Getting Started with CYW20819](https://www.infineon.com/dgdl/Infineon-AN225684_Getting_Started_with_CYW20819-ApplicationNotes-v02_00-EN.PDF?fileId=8ac78c8c7cdc391c017d0d3674d1669d) application note to understand the firmware design for this code example.

## Resources and Settings
This section explains the ModusToolbox&#8482; resources and their configuration as used in this code example. Note that all the configuration explained in this section has already been done in the code example. ModusToolbox&#8482; stores the configuration settings of the application in the *design.modus* file. This file is used by the graphical configurators, which generate the configuration firmware. This firmware is stored in the application’s *GeneratedSource* folder.

- **Device Configurator:** The Device Configurator is used to enable/configure the peripherals and the pins used in the application. For detailed information on how to use it, see
[Device Configurator Guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Device_Configurator_Guide_4-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99ab297631cb).

- **Bluetooth&#174; Configurator:** The Bluetooth&#174; Configurator is used for generating/modifying the LE GATT database and configuration settings. For detailed information on how to use it, see
[Bluetooth&#174; Configurator Guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Bluetooth_Configurator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99aaf5b231be).

## Related Resources

| **Application Notes**                                            |                                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
|[AN225684 - Getting Started with CYW20819](https://www.infineon.com/dgdl/Infineon-AN225684_Getting_Started_with_CYW20819-ApplicationNotes-v02_00-EN.PDF?fileId=8ac78c8c7cdc391c017d0d3674d1669d) | Describes CYW20819 Bluetooth&#174; SoC, software/hardware development ecosystem, and how to build your first LE application using the device in ModusToolbox&#8482; IDE.
| **Code Examples**  |
|Visit the BTSDK code examples repository in the Infineon GitHub portal for a comprehensive collection of code examples using ModusToolbox&#8482;|
|**Device Documentation**|
|[CYW20819 Device Datasheet](https://www.infineon.com/dgdl/Infineon-AIROC_CYW20819_Bluetooth_Bluetooth_LE_5_2_SoC-DataSheet-v09_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ee7dba070bf)|
|[CYW20820 Device Datasheet](https://www.infineon.com/dgdl/Infineon-CYW20820-DataSheet-v01_00-EN.pdf?fileId=8ac78c8c8412f8d301843310af461e24)|
|[CYW20719 Device Datasheet](https://www.infineon.com/dgdl/Infineon-CYW20719-DataSheet-v08_00-EN.pdf?fileId=8ac78c8c85ecb3470185f3d08c7a10d3)|
|**Development Kits** |
|[CYW920819EVB-02 Evaluation Kit](https://www.infineon.com/cms/en/product/evaluation-boards/cyw920819evb-02/)|
|[CYW920820EVB-02 Evaluation Kit]( https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/)|
|[CYW920719B2Q40EVB-01 Evaluation kit](https://community.infineon.com/t5/Resource-Library/CYW20719B2-Product-Guide/ta-p/251238)|
|**Tool Documentation** |
|[ModusToolbox&#8482;](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/)| The Infineon IDE for IoT designers|
--------------------------------------------------------------------------------------------

## BTSTACK version

BTSDK AIROC&#8482; chips contain the embedded AIROC&#8482; Bluetooth&#174; stack, BTSTACK. Different chips use different versions of BTSTACK, so some assets may contain variant sets of files targeting the different versions in COMPONENT\_btstack\_vX (where X is the stack version). Applications automatically include the appropriate folder using the COMPONENTS make variable mechanism, and all BSPs declare which stack version should be used in the BSP .mk file, with a declaration such as:<br>
> COMPONENTS+=btstack\_v1<br>
or:<br>
> COMPONENTS+=btstack\_v3

## Common application settings

Application settings below are common for all BTSDK applications and can be configured via the makefile of the application or passed in via the command line.

##### BT\_DEVICE\_ADDRESS
> Set the BDA (Bluetooth&#174; Device Address) for your device. The address is 6 bytes, for example, 20819A10FFEE. By default, the SDK will set a BDA for your device by combining the 7 hex digit device ID with the last 5 hex digits of the host PC MAC address.

##### UART
> Set to the UART port you want to use to download the application. For example 'COM6' on Windows or '/dev/ttyWICED\_HCI\_UART0' on Linux or '/dev/tty.usbserial-000154' on macOS. By default, the SDK will auto-detect the port.

##### ENABLE_DEBUG
> For HW debugging, configure ENABLE\_DEBUG=1. See the document [AIROC&#8482;-Hardware-Debugging](https://infineon.github.io/btsdk-docs/BT-SDK/AIROC-Hardware-Debugging.pdf) for more information. This setting configures GPIO for SWD.<br>
>
   - CYW920819EVB-02/CYW920820EVB-02: SWD signals are shared with D4 and D5, see SW9 in schematics.
   - CYBT-213043-MESH/CYBT-213043-EVAL/CYBT-253059-EVAL: SWD signals are routed to P12=SWDCK and P13=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-223058-EVAL/CYW920835M2EVB-01/CYBT-243053-EVAL/CYBLE-343072-EVAL-M2B/CYBLE-333074-EVAL-M2B/CYBLE-343072-MESH/Vela-IF820-INT-ANT-DVK/Vela-IF820-EXT-ANT-DVK: SWD signals are routed to P02=SWDCK and P03=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-263065-EVAL/CYBT-273063-EVAL: SWD signals are routed to P02=SWDCK and P04=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-343026-EVAL/CYBT-353027-EVAL/CYBT-333047-EVAL: SWD signals are routed to P11=SWDCK and P15=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-413055-EVAL/CYBT-413061-EVAL: SWD signals are routed to P16=SWDCK and P17=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYW989820EVB-01: SWDCK (P02) is routed to the J13 DEBUG connector, but not SWDIO. Add a wire from J10 pin 3 (PUART CTS) to J13 pin 2 to connect GPIO P10 to SWDIO.
   - CYW920719B2Q40EVB-01: PUART RX/TX signals are shared with SWDCK and SWDIO. Remove RX and TX jumpers on J10 when using SWD. PUART and SWD cannot be used simultaneously on this board unless these pins are changed from the default configuration.
   - CYW920721M2EVK-02/CYW920721M2EVB-03: The default setup uses P03 for SWDIO and P05 for SWDCK. Check the position of SW15 if using JLink with the DEBUG connector.
   - CYW920706WCDEVAL: SWD debugging requires fly-wire connections. The default setup P15 (J22 pin 3 or J24 pin 1) for SWDIO and P11 (J23 pin 5
    or J22 pin 4) for SWDCK.
   - CYW920736M2EVB-01: SWD hardware debugging requires fly-wire connections. The only option is using P14 for SWDCK and P15 for SWDIO. These route to Arduino header J2, A1 and A0. These can be fly-wired to Arduino header J4, D4 and D5. From there the signals connect to the KitProg3 SWD bridge. In addition, the debug macros (SETUP\_APP\_FOR\_DEBUG\_IF\_DEBUG\_ENABLED and BUSY\_WAIT\_TILL\_MANUAL\_CONTINUE\_IF\_DEBUG\_ENABLED) are placed in sparinit.c in code common to all applications for this device. Most applications for this device call bleprofile\_GPIOInit() in subsequent code, overwriting the SWD pin configuration. To use hardware debugging after the call to bleprofile\_GPIOInit(), place the debug macros in code after that call.
   - CYW943012B2EVK-01: SWD signals are shared with D4 and D5.
   - CYW920820M2EVB-01 & CYW920819M2EVB-01: The default setup uses P03 for SWDIO and P02 for SWDCK. Check the position of SW15 if using JLink with the DEBUG connector.
   - CYW989820M2EVB-01: SWD hardware debugging requires a fly-wire connection to use P14 for SWDIO. P2 is connected directly to SWDCK / ARD_D4. Fly-wire P14 / ARD_D8 on J3.10 to J4.3 / ARD_D5 to connect SWDIO.

   - SWD hardware debugging is not supported on the following:
      - CYW920721M2EVK-01
      - CYW920835REF-RCU-01
      - CYW9M2BASE-43012BT
      - CYBT-423054-EVAL
      - CYBT-423060-EVAL
      - CYBT-483056-EVAL
      - CYBT-483062-EVAL
      - CYW955572BTEVK-01
      - CYW943022BTEVK-01

##### DIRECT_LOAD
> BTSDK chips support downloading applications either to FLASH storage or to RAM storage. Some chips support only one or the other, and some chips support both.

> If a chip only supports one or the other, this variable is not applicable, applications will be downloaded to the appropriate storage supported by the device.

> If a chip supports both FLASH and RAM downloads, the default is to download to FLASH, and the DIRECT_LOAD make variable may be set to 1 in the application makefile (or in the command line make command) to override the default and download to RAM.

> Currently, the following chips support both FLASH and RAM download and can set DIRECT_LOAD=1 if desired:
>
   - CYW20835
   - CYW20706

## Building and downloading code examples

**Using the ModusToolbox&#8482; Eclipse IDE**

1. Install ModusToolbox&#8482; 2.4.1 (or higher).
2. In the ModusToolbox&#8482; Eclipse IDE, click the **New Application** link in the Quick Panel (or, use **File > New > ModusToolbox IDE Application**).
3. Pick your board for BTSDK under AIROC&#8482; Bluetooth&#174; BSPs.
4. Select the application in the IDE.
5. In the Quick Panel, select **Build** to build the application.
6. To program the board (download the application), select **Program** in the Launches section of the Quick Panel.

**Using command line**

1. Install ModusToolbox&#8482; 2.4.1 (or higher).
2. On Windows, use Cygwin from \ModusToolbox\tools_2.x\modus-shell\Cygwin.bat to build apps.
3. Use the tool 'project-creator-cli' under \ModusToolbox\tools_2.x\project-creator\ to create your application.<br/>
   > project-creator-cli --board-id (BSP) --app-id (appid) -d (dir) <br/>
   See 'project-creator-cli --help' for useful options to list all available BSPs, and all available apps per BSP.<br/>
   For example:<br/>
   > project-creator-cli --app-id mtb-example-btsdk-empty --board-id CYW920706WCDEVAL -d .<br/>
4. To build the app call make build. For example:<br/>
   > cd mtb-examples-btsdk-empty<br/>
   > make build<br/>
5. To program (download to) the board, call:<br/>
   > make qprogram<br/>
6. To build and program (download to) the board, call:<br/>
   > make program<br/><br>
   Note: make program = make build + make qprogram

If you have issues downloading to the board, follow the steps below:

- Press and hold the 'Recover' button on the board.
- Press and hold the 'Reset' button on the board.
- Release the 'Reset' button.
- After one second, release the 'Recover' button.

Note: this is only applicable to boards that download application images to FLASH storage. Boards that only support RAM download (DIRECT_LOAD) such as CYW9M2BASE-43012BT or CYW943022BTEVK-01 can be power cycled to boot from ROM.

## Over The Air (OTA) Firmware Upgrade
Applications that support OTA upgrade can be updated via the peer OTA app in:<br>
>\<Workspace Dir>\mtb\_shared\wiced\_btsdk\tools\btsdk-peer-apps-ota<br>

See the readme.txt file located in the above folder for instructions.<br>
To generate the OTA image for the app, configure OTA\_FW\_UPGRADE=1 in the app
makefile, or append OTA\_FW\_UPGRADE=1 to a build command line, for example:
> make PLATFORM=CYW920706WCDEVAL OTA\_FW\_UPGRADE=1 build<br>

This will the generate \<app>.bin file in the 'build' folder.

## SDK software features

- Dual-mode Bluetooth&#174; stack included in the ROM (BR/EDR and LE)
- Bluetooth&#174; stack and profile level APIs for embedded Bluetooth&#174; application development
- AIROC&#8482; HCI protocol to simplify host/MCU application development
- APIs and drivers to access on-board peripherals
- Bluetooth&#174; protocols include GAP, GATT, SMP, RFCOMM, SDP, AVDT/AVCT, LE Mesh
- LE and BR/EDR profile APIs, libraries, and sample apps
- Support for Over-The-Air (OTA) upgrade
- Device Configurator for creating custom pin mapping
- Bluetooth&#174; Configurator for creating LE GATT Database
- Peer apps based on Android, iOS, Windows, etc. for testing and reference
- Utilities for protocol tracing, manufacturing testing, etc.
- Documentation for APIs, datasheets, profiles, and features
- BR/EDR profiles: A2DP, AVRCP, HFP, HSP, HID, SPP, MAP, PBAP, OPP
- LE profiles: Mesh profiles, HOGP, ANP, BAP, HRP, FMP, IAS, ESP, LE COC
- Apple support: Apple Media Service (AMS), Apple Notification Center Service (ANCS), iBeacon, Homekit, iAP2
- Google support: Google Fast Pair Service (GFPS), Eddystone
- Amazon support: Alexa Mobile Accessories (AMA)

Note: this is a list of all features and profiles supported in BTSDK, but some AIROC&#8482; devices may only support a subset of this list.

## List of boards available for use with BTSDK

- [CYW20819A1 chip](https://github.com/Infineon/20819A1)
    - [CYW920819EVB-02](https://github.com/Infineon/TARGET_CYW920819EVB-02), [CYW920819M2EVB-01](https://github.com/Infineon/TARGET_CYW920819M2EVB-01), [CYBT-213043-MESH](https://github.com/Infineon/TARGET_CYBT-213043-MESH), [CYBT-213043-EVAL](https://github.com/Infineon/TARGET_CYBT-213043-EVAL), [CYBT-223058-EVAL](https://github.com/Infineon/TARGET_CYBT-223058-EVAL), [CYBT-263065-EVAL](https://github.com/Infineon/TARGET_CYBT-263065-EVAL), [CYBT-273063-EVAL](https://github.com/Infineon/TARGET_CYBT-273063-EVAL)
- [CYW20820A1 chip](https://github.com/Infineon/20820A1)
    - [CYW920820EVB-02](https://github.com/Infineon/TARGET_CYW920820EVB-02), [CYW989820M2EVB-01](https://github.com/Infineon/TARGET_CYW989820M2EVB-01), [CYW989820EVB-01](https://github.com/Infineon/TARGET_CYW989820EVB-01), [CYBT-243053-EVAL](https://github.com/Infineon/TARGET_CYBT-243053-EVAL), [CYBT-253059-EVAL](https://github.com/Infineon/TARGET_CYBT-253059-EVAL), [CYW920820M2EVB-01](https://github.com/Infineon/TARGET_CYW920820M2EVB-01), [Vela-IF820-INT-ANT-DVK](https://github.com/Infineon/TARGET_Vela-IF820-INT-ANT-DVK), [Vela-IF820-EXT-ANT-DVK](https://github.com/Infineon/TARGET_Vela-IF820-EXT-ANT-DVK)
- [CYW20721B2 chip](https://github.com/Infineon/20721B2)
    - [CYW920721M2EVK-01](https://github.com/Infineon/TARGET_CYW920721M2EVK-01), [CYW920721M2EVK-02](https://github.com/Infineon/TARGET_CYW920721M2EVK-02), [CYW920721M2EVB-03](https://github.com/Infineon/TARGET_CYW920721M2EVB-03), [CYBT-423060-EVAL](https://github.com/Infineon/TARGET_CYBT-423060-EVAL), [CYBT-483062-EVAL](https://github.com/Infineon/TARGET_CYBT-483062-EVAL), [CYBT-413061-EVAL](https://github.com/Infineon/TARGET_CYBT-413061-EVAL)
- [CYW20719B2 chip](https://github.com/Infineon/20719B2)
    - [CYW920719B2Q40EVB-01](https://github.com/Infineon/TARGET_CYW920719B2Q40EVB-01), [CYBT-423054-EVAL](https://github.com/Infineon/TARGET_CYBT-423054-EVAL), [CYBT-413055-EVAL](https://github.com/Infineon/TARGET_CYBT-413055-EVAL), [CYBT-483056-EVAL](https://github.com/Infineon/TARGET_CYBT-483056-EVAL)
- [CYW20706A2 chip](https://github.com/Infineon/20706A2)
    - [CYW920706WCDEVAL](https://github.com/Infineon/TARGET_CYW920706WCDEVAL), [CYBT-353027-EVAL](https://github.com/Infineon/TARGET_CYBT-353027-EVAL), [CYBT-343026-EVAL](https://github.com/Infineon/TARGET_CYBT-343026-EVAL), [CYBT-333047-EVAL](https://github.com/Infineon/TARGET_CYBT-333047-EVAL)
- [CYW20835B1 chip](https://github.com/Infineon/20835B1)
    - [CYW920835REF-RCU-01](https://github.com/Infineon/TARGET_CYW920835REF-RCU-01), [CYW920835M2EVB-01](https://github.com/Infineon/TARGET_CYW920835M2EVB-01), [CYBLE-343072-EVAL-M2B](https://github.com/Infineon/TARGET_CYBLE-343072-EVAL-M2B), [CYBLE-333074-EVAL-M2B](https://github.com/Infineon/TARGET_CYBLE-333074-EVAL-M2B), [CYBLE-343072-MESH](https://github.com/Infineon/TARGET_CYBLE-343072-MESH)
- [CYW43012C0 chip](https://github.com/Infineon/43012C0)
    - [CYW9M2BASE-43012BT](https://github.com/Infineon/TARGET_CYW9M2BASE-43012BT), [CYW943012BTEVK-01](https://github.com/Infineon/TARGET_CYW943012BTEVK-01)
- [CYW43022C1 chip](https://github.com/Infineon/43022C1)
    - [CYW943022BTEVK-01](https://github.com/Infineon/TARGET_CYW943022BTEVK-01)
- [CYW20736A1 chip](https://github.com/Infineon/20736A1)
    - [CYW920736M2EVB-01](https://github.com/Infineon/TARGET_CYW920736M2EVB-01)
- [CYW30739A0 chip](https://github.com/Infineon/30739A0)
    - [CYW930739M2EVB-01](https://github.com/Infineon/TARGET_CYW930739M2EVB-01)
- [CYW55572A1 chip](https://github.com/Infineon/55572A1)
    - [CYW955572BTEVK-01](https://github.com/Infineon/TARGET_CYW955572BTEVK-01)


## Folder structure

All BTSDK code examples need the 'mtb\_shared\wiced\_btsdk' folder to build and test the apps. 'wiced\_btsdk' includes the 'dev-kit' and 'tools' folders. The contents of the 'wiced\_btsdk' folder will be automatically populated incrementally as needed by the application being used.

**dev-kit**

This folder contains the files that are needed to build the embedded Bluetooth&#174; apps.

* baselib: Files for chips supported by BTSDK. For example CYW20819, CYW20719, CYW20706, etc.

* bsp: Files for BSPs (platforms) supported by BTSDK. For example CYW920819EVB-02, CYW920706WCDEVAL etc.

* btsdk-include: Common header files needed by all apps and libraries.

* btsdk-tools: Build tools needed by BTSDK.

* libraries: Profile libraries used by BTSDK apps such as audio, LE, HID, etc.

**tools**

This folder contains tools and utilities need to test the embedded Bluetooth&#174; apps.

* btsdk-host-apps-bt-ble: Host apps (Client Control) for LE and BR/EDR embedded apps, demonstrates the use of AIROC&#8482; HCI protocol to control embedded apps.

* btsdk-host-peer-apps-mesh: Host apps (Client Control) and Peer apps for embedded Mesh apps, demonstrates the use of AIROC&#8482; HCI protocol to control embedded apps, and configuration and provisioning from peer devices.

* btsdk-peer-apps-ble: Peer apps for embedded LE apps.

* btsdk-peer-apps-ota: Peer apps for embedded apps that support Over The Air Firmware Upgrade.

* btsdk-utils: Utilities used in BTSDK such as BTSpy, wmbt, and ecdsa256.

See README.md in the sub-folders for more information.

## Software Tools
The following tool applications are installed on your computer either with ModusToolbox&#8482;, or by creating an application in the workspace that can use the tool.

**BTSpy:**<br>
>   BTSpy is a trace viewer utility that can be used with AIROC&#8482; Bluetooth&#174; platforms to
    view protocol and application trace messages from the embedded device. The
    utility is located in the folder below. For more information, see readme.txt in the same folder.<br>
    This utility can be run directly from the filesystem, or it can be run from
    the Tools section of the ModusToolbox&#8482; QuickPanel, or by right-clicking
    a project in the Project Explorer pane and selecting the ModusToolbox&#8482;
    context menu.<br>
    It is supported on Windows, Linux and macOS.<br>
    Location:  \<Workspace Dir>\wiced_btsdk\tools\btsdk-utils\BTSpy

**Bluetooth&#174; Classic and LE Profile Client Control:**<br>
>   This application emulates host MCU applications for LE and BR/EDR profiles.
    It demonstrates AIROC&#8482; Bluetooth&#174; APIs. The application communicates with embedded
    apps over the "WICED HCI UART" interface. The application is located in the folder
    below. For more information, see readme.txt in the same folder.<br>
    This utility can be run directly from the filesystem, or it can be run from
    the Tools section of the ModusToolbox&#8482; QuickPanel, or by right-clicking
    a project in the Project Explorer pane and selecting the ModusToolbox&#8482;
    context menu.<br>
    It is supported on Windows, Linux, and macOS.<br>
    Location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-host-apps-bt-ble\client_control

**LE Mesh Client Control:**<br>
>   Similar to the above app, this application emulates host MCU applications
    for LE Mesh models. It can configure and provision mesh devices and create
    mesh networks. The application is located in the folder below. For more
    information, see readme.txt in the same folder.<br>
    This utility can be run directly from the filesystem, or it can be run from
    the Tools section of the ModusToolbox&#8482; QuickPanel (if a mesh-capable
    project is selected in the Project Explorer pane), or by right-clicking
    a mesh-capable project in the Project Explorer pane and selecting the
    ModusToolbox&#8482; context menu.<br>
    The full version is provided for Windows (VS\_ClientControl) supporting all
    Mesh models.<br>
    A limited version supporting only the Lighting model (QT\_ClientControl) is
    provided for Windows, Linux, and macOS.<br>
    Location:  \<Workspace Dir>\wiced_btsdk\tools\btsdk-host-peer-apps-mesh\host

**Peer apps:**<br>
>   Applications that run on Windows, iOS or Android and act as peer Bluetooth&#174; apps to
    demonstrate specific profiles or features, communicating with embedded apps
    over the air.<br>
    LE apps location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-peer-apps-ble<br>
    LE Mesh apps location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-host-peer-apps-mesh\peer<br>
    OTA apps location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-peer-apps-ota

**Device Configurator:**<br>
>   Use this GUI tool to create source code for a custom pin mapping for your device. Run this tool
    from the Tools section of the ModusToolbox&#8482; QuickPanel, or by
    right-clicking a project in the Project Explorer pane and selecting the
    ModusToolbox&#8482; context menu.<br>
    It is supported on Windows, Linux and macOS.<br>
    Note: The pin mapping is based on wiced\_platform.h for your board.<br>
    Location:  \<Install Dir>\tools_2.x\device-configurator

Note: Not all BTSDK chips support Device Configurator. BSPs using the following devices do not currently support Device Configurator: CYW20706, CYW20736

**Bluetooth&#174; Configurator:**<br>
>   Use this GUI tool to create and configure the LE GATT Database and the BR/EDR SDP Database, generated as source code for your
    application.<br>
    Run this tool from the Tools section of the ModusToolbox&#8482; QuickPanel, or
    by right-clicking a project in the Project Explorer pane and selecting
    the ModusToolbox&#8482; context menu.<br>
    It is supported on Windows, Linux and macOS.<br>
    Location:  \<Install Dir>\tools_2.x\bt-configurator

## Tracing
To view application traces, there are 2 methods available. Note that the
application needs to configure the tracing options.<br>

1. "WICED Peripheral UART" - Open this port on your computer using a serial port
utility such as TeraTerm or PuTTY (usually using 115200 baud rate for non-Mesh apps, and 921600 for Mesh apps).<br>
2. "WICED HCI UART" - Open this port on your computer using the Client Control
application mentioned above (usually using 3M baud rate). Then run the BTSpy
utility mentioned above.

## Using BSPs (platforms)

BTSDK BSPs are located in the \mtb\_shared\wiced\_btsdk\dev-kit\bsp\ folder by default.

#### a. Selecting an alternative BSP

The application makefile has a default BSP. See "TARGET". The makefile also has a list of other BSPs supported by the application. See "SUPPORTED_TARGETS". To select an alternative BSP, use Library Manager from the Quick Panel to deselect the current BSP and select an alternate BSP. Then right-click the newly selected BSP and choose 'Set Active'.  This will automatically update TARGET in the application makefile.

#### b. Custom BSP

To create a custom BSP from a BSP template for BTSDK devices, see the following KBA article: [KBA238530](https://community.infineon.com/t5/Knowledge-Base-Articles/Create-custom-BTSDK-BSP-using-ModusToolbox-version-3-x-KBA238530/ta-p/479355)

## Using libraries

The libraries needed by the app can be found in in the mtb\_shared\wiced\_btsdk\dev-kit\libraries folder. To add an additional library to your application, launch the Library Manager from the Quick Panel to add a library. Then update the makefile variable "COMPONENTS" of your application to include the library. For example:<br/>
   COMPONENTS += fw\_upgrade\_lib


## Documentation

BTSDK API documentation is available [online](https://infineon.github.io/btsdk-docs/BT-SDK/index.html)

Note: For offline viewing, git clone the [documentation repo](https://github.com/Infineon/btsdk-docs)

BTSDK Technical Brief and Release Notes are available [online](https://community.infineon.com/t5/Bluetooth-SDK/bd-p/ModusToolboxBluetoothSDK)

<br>
<sup>The Bluetooth&#174; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.</sup>
