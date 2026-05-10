//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \copyright
/// COPYRIGHT (c) 2017
/// HONEYWELL INTERNATIONAL INC.
/// ALL RIGHTS RESERVED
///
/// Legal rights of Honeywell International Inc. in this software is distinct from ownership of any medium in which the software is embodied.
/// Copyright notices must be reproduced in any copies authorized by Honeywell International Inc.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file  STT850.js
///
/// \brief Java script file to contain enumerations and functions that are specific to the STT850 transmitter.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// History:
///
/// EXP510.1-35		May   27, 2018		Developer Label D_EXP510.1-35.0-06
/// EXP510.1-34		May   21, 2018		Developer Label D_EXP510.1-34.0-12
/// [RLEAPFI-642]	April 30, 2018		Add support for building a STT850 specific device model.
/// EXP510.1-31		April 24, 2018		Initial Experion R510 testbed release
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_NumberOfStatusBytes = [{ Revision: , Cmd48: , Cmd220: }]
///
/// This array indicates the number of status bytes that have been defined for the array of device specific status bytes that are read through
/// the HART Universal Command 48 and the SmartLine Device Specific Command 220 for each of the released revisions of the transmitter firmware.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_NumberOfStatusBytes =
[
    { "Revision": 1, "Cmd48": 4, "Cmd220": 8 },
    { "Revision": 2, "Cmd48": 4, "Cmd220": 8 },
    { "Revision": 3, "Cmd48": 4, "Cmd220": 8 },
    { "Revision": 4, "Cmd48": 4, "Cmd220": 8 }
];

var STT850_DeviceRevisions =
{
    1: new SL_DeviceRevision(1, 4, 8),
    2: new SL_DeviceRevision(2, 4, 8),
    3: new SL_DeviceRevision(3, 4, 8),
    4: new SL_DeviceRevision(4, 4, 8)
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_Cmd48StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the STT850 transmitter
/// in the response to HART Universal Command 48. The location within the returned status byte array is defined by the combination of the byte
/// number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_Cmd48StatusBits =
[
    {
        "byte": 0,
        "bit": 7,
        "criticality": 0,
        "name": "Comm VCC Fault",
        "description": "This bit is set when the Communication module supply voltage goes outside the 2.8 to 3.2 V operating range.",
        "resolution": "The Communications module is not receiving the proper supply voltage from the Sensor module.  If this problem persists, try replacing the sensor module/Terminal Block assembly."
    },
    {
        "byte": 0,
        "bit": 6,
        "criticality": 0,
        "name": "Excess Delta Temperature",
        "description": "Excess Delta Alarm is Enabled and set as Critical Alarm.",
        "resolution": "Check your sensors to see if they are reporting bad values. If the values are good, and the difference between values is expected, increase your Delta Limit for alarming."
    },
    {
        "byte": 0,
        "bit": 5,
        "criticality": 0,
        "name": "Sensor Communication Timeout",
        "description": "There is no communication between communication module and sensor module, or invalid data is encountered on communication line due to noise.",
        "resolution": "Verify the connection from the Communication module to the housing and from the Sensor module/Terminal Block Assembly to the housing.  Especially check for bent pins in the connectors.\n\nIf the cable or connectors is not the problem, try replacing the Communication module. If this does not fix the problem, replace the Sensor module/Terminal Block assembly."
    },
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "Sensor Critical Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present:\n\n  - Input Sensor Fault\n  - Suspect Input\n  - Char Table CRC Fault\n  - Sensor NVM Corrupt\n  - RAM Failure\n  - Flash CRC Failure\n  - Program Flow Failure",
        "resolution": "For conditions 1, 2, and 3 check your sensor wiring and connections. If the connections are ok, and problem persists, replace the Sensor module/Terminal Block Assembly.\n\nFor conditions 4 through 8, if following a power cycle the failure recurs, replace the Sensor module/Terminal Block assembly."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "Comm Module Diag Failure",
        "description": "SIL Diagnostics failures have been detected. This is a roll-up status bit that is set when any of the following critical status conditions are present:\n\n  - RAM Failure\n  - ROM Failure\n  - Program Flow Failure",
        "resolution": "Reset the device. If the problem persists replace the Communication module."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 0,
        "name": "Comm Module Config Data Corrupt",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present:\n\n  - Common DB Corrupt\n  - Vital DB Corrupt\n  - General DB Corrupt",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 0,
        "bit": 1,
        "criticality": 0,
        "name": "Sensor NVM Corrupt",
        "description": "Sensor NVM data is bad.",
        "resolution": "If following a power cycle the failure recurs, then replace the Sensor module/Terminal Block assembly."
    },
    {
        "byte": 0,
        "bit": 0,
        "criticality": 0,
        "name": "Electronic Module DAC Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present:\n\n  - DAC SPI Interface Failure\n  - DAC Packet Error\n  - DAC Over Current\n  - DAC Under Current\n  - DAC Temperature Above 140°C\n  - DAC Control Word Write Fault",
        "resolution": "Refer to the resolution for each condition present."
    },

    {
        "byte": 1,
        "bit": 7,
        "criticality": 1,
        "name": "No DAC Compensation",
        "description": "Communication module DAC compensation has not been performed. Loop accuracy may be slightly compromised. The effect will be minor degradation of ambient temperature influence specifications.",
        "resolution": "Replace the Communication module to achieve the maximum current loop accuracy or return the device to factory for DAC compensation."
    },
    {
        "byte": 1,
        "bit": 6,
        "criticality": 1,
        "name": "No Factory Calibration",
        "description": "Factory calibration is missing. Accuracy will be compromised.",
        "resolution": "Replace the Sensor module/Terminal Block assembly or return the device for factory calibration."
    },
    {
        "byte": 1,
        "bit": 5,
        "criticality": 1,
        "name": "Primary Variable Out of Range",
        "description": "Loop PV is outside the configured span for the process (LRV to URV). This roll-up status bit may be set when any of the following non-critical status conditions are present:\n\n  - Sensor Overload or Fault\n  - Redundant Characterization Calculation Error",
        "resolution": "Check range and, if required, change to a sensor type with a greater temperature range. Sensor may have been damaged.\nCheck the transmitter for accuracy and linearity.\nReplace the sensor and recalibrate if needed."
    },
    {
        "byte": 1,
        "bit": 4,
        "criticality": 1,
        "name": "Fixed Current Mode",
        "description": "Output current is fixed and not varying as per the process input. Loop current mode is disabled or Loop Test is active.",
        "resolution": "Enable loop current mode if it is disabled or exit the Loop Test mode if active."
    },
    {
        "byte": 1,
        "bit": 3,
        "criticality": 1,
        "name": "CJ Temperature Out of Limits",
        "description": "Sensor Cold Junction temperature is too high.",
        "resolution": " If the temperature is > 85°C and if compensation is Internal CJ (range is -40 to 85°C), accuracy and life span may decrease if internal CJ remains overrange.\n\nExternal (Fixed) CJ range is -200 to 850°C. As long as transmitter is isolated from the temperature source, there will not be any degradation. Take steps to isolate the device from the temperature source."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 1,
        "name": "Sensor Non-critical Failure",
        "description": "This is a roll-up status bit that is set when any of the following non-critical status conditions are present:\n\n  - Excess Cal1 correction\n  - Excess Cal2 correction\n",
        "resolution": "Conditions 1 and 2 indicate that the value applied during Calibration is too far away from the CAL point for Sensor1 and Sensor 2.\nCould be that INPUT was incorrect or transmitter was in output mode during a CORRECT procedure.\nCheck input temperature and be sure that it matches calibrated range values (Lower Calib Point, Upper Calib Point). Perform an LRV CORRECT procedure followed by a URV CORRECT procedure."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "Comm Module Non-critical Failure",
        "description": "This is a roll-up status bit that is set when any of the following non-critical status conditions are present.\n\n  - Advanced Diagnostic database corrupt\n  - Configuration Change database corrupt\n  - Environmental temperature above 100°C\n",
        "resolution": "Power cycle the device. If the problem still persists, verify the environment is within spec. If it is, then replace the Communication Module."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 1,
        "name": "Local Display Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n  - Display Communication Timeout\n  - Display NVM Corrupt\n  - Display View 1 corrupt\n  - Display View 2 corrupt\n  - Display View 3 corrupt\n  - Display View 4 corrupt\n  - Display View 5 corrupt\n  - Display View 6 corrupt\n  - Display View 7 corrupt\n  - Display View 8 corrupt",
        "resolution": "Refer to the resolution for each condition present."
    },

    {
        "byte": 2,
        "bit": 7,
        "criticality": 1,
        "name": "Low Supply Voltage",
        "description": "The supply voltage to the transmitter terminals is too low or the supply voltage to the Sensor module is too low.",
        "resolution": "Check that the power supply and loop resistance are within specification. If possible, try to increase the voltage level of the supply. If supply voltage and loop resistance are adequate and the problem persists, replace the Communication Module. If the problem still persists, replace the sensor module/Terminal Block assembly."
    },
    {
        "byte": 2,
        "bit": 6,
        "criticality": 1,
        "name": "No DAC Calibration",
        "description": "No DAC calibration has been performed on the device.",
        "resolution": "Perform DAC calibration for precise analog output measurement and the device will be usable."
    },
    {
        "byte": 2,
        "bit": 5,
        "criticality": 1,
        "name": "Tamper Alarm",
        "description": "The Tamper Alarm is set if more than a specified number of attempts or actual configuration changes are made, when Tamper Alarm is enabled (Device can be either Write Protected or not). The warning stays active until the specified latency period has elapsed.",
        "resolution": "See User's Manual for Tamper Alarm configuration. Make sure configuration changes are made by the authorized users."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 1,
        "name": "Unreliable Sensor Communication",
        "description": "Unreliable sensor communication.",
        "resolution": " Either the transmitter is installed in a noisy environment or internal communication quality between the Communication Module and Sensor module is degrading.\nSecure the connection between Terminal Block Assembly and the connector on the transmitter housing and also between communication module and the connector on the housing.  Check especially for bent pins in the connectors.\nCall service person if the connections are secure."
    },
    {
        "byte": 2,
        "bit": 3,
        "criticality": 1,
        "name": "Loop Current Noise",
        "description": "DAC is not able to regulate loop current. If this condition is observed frequently, it could be an early indication of critical under or over-current failure.",
        "resolution": "Try to find the source of the noise. It could be bad loop wires, a hardware problem inside the transmitter, loop wires running close to a noise source, etc. If the environment is ok, replace the Communication Module."
    },
    {
        "byte": 2,
        "bit": 2,
        "criticality": 1,
        "name": "Analog Output Saturated",
        "description": "The calculated analog output is outside the 0 to 100% range based on the configured span (LRV to URV).",
        "resolution": "Verify that the LRV and URV values are configured properly for the desired process range."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "URV Set Error - Span Config Button",
        "description": "Set operation using external Span button was rejected.",
        "resolution": "Please check whether the inputs are valid for the intended operation."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 1,
        "name": "LRV Set Error - Zero Config Button",
        "description": "Set operation using external Zero button was rejected.",
        "resolution": "Please check whether the inputs are valid for the intended operation."
    },

    {
        "byte": 3,
        "bit": 7,
        "criticality": 1,
        "name": "Sensor 2 Input Out of Range",
        "description": "Sensor 2 input under or over range.",
        "resolution": "Try to re-range the transmitter for the allowed range for the connected sensor type."
    },
    {
        "byte": 3,
        "bit": 6,
        "criticality": 1,
        "name": "Sensor 1 Input Out of Range",
        "description": "Sensor 1 input under or over range.",
        "resolution": "Try to re-range the transmitter for the allowed range for the connected sensor type."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 1,
        "name": "Sensor 2 Input Open",
        "description": "Sensor 2 Input Open",
        "resolution": "Make sure sensor wiring for temperature sensor 2 is connected to the correct terminals and is secure."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 1,
        "name": "Sensor 1 Input Open",
        "description": "Sensor 1 Input Open",
        "resolution": "Make sure sensor wiring for temperature sensor 1 is connected to the correct terminals and is secure."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 1,
        "name": "Sensor Over Temperature",
        "description": "Specifies that the sensor internal CPU temperature is going out of limits. Valid Range (-40°C to 85°C).",
        "resolution": " Power cycle the device. If the problem still persists make sure the environment is within spec."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "Excess Delta Detected",
        "description": "When Excess Delta Alarm is Enabled, and device is in non-redundant mode, this status indicates that the difference in Sensor 1 and Sensor 2 values exceeds the configured Delta Limit.",
        "resolution": "Make sure desired limit value for the PV Delta is correct. If the limit is correct, check Input 1 and Input 2 readings."
    }
 ];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_Cmd220StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the STT850 transmitter
/// in the response to SmartLine specific Command 220. The location within the returned status byte array is defined by the combination of the
/// byte number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_Cmd220StatusBits =
[
    {
        "byte": 0,
        "bit": 7,
        "criticality": 2,
        "name": "DAC SPI Interface Failure",
        "description": "Failure of the DAC SPI Interface.",
        "resolution": "Power cycle the device. Check that the power supply and loop resistance are within specifications. If they are, then replace the Communication module."
    },
    {
        "byte": 0,
        "bit": 6,
        "criticality": 0,
        "name": "DAC Packet Error",
        "description": "The DAC or micro-controller chip on the communication module may be damaged.",
        "resolution": "Power cycle the device. Check that the power supply and loop resistance are within specifications. If they are, then replace the Communication module."
    },
    {
        "byte": 0,
        "bit": 5,
        "criticality": 0,
        "name": "DAC Over Current",
        "description": "The loop current value is greater than actual requested value. This might be due to some damaged component taking more current than expected.",
        "resolution": "Power cycle the device. If the problem persists, try replacing the sensor module/Terminal Block Assembly, then the Communication module in that order."
    },
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "DAC Under Current",
        "description": "The loop current value is below the actual requested value.",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "DAC Temperature Above 140°C",
        "description": "The communication module temperature is dangerously high - above 140°C.",
        "resolution": "Verify that the environment is within the operating specification of the transmitter. If it is, then replace the Communication module."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 1,
        "name": "DAC Temperature Above 100°C",
        "description": "The communication module temperature is high - above 100°C.",
        "resolution": "Check the ambient temperature. If the ambient temperature is well below 100°C, check if there are other DAC faults reported by the device. Power cycle the device and if the problem persists, replace the communication module."
    },
    {
        "byte": 0,
        "bit": 0,
        "criticality": 0,
        "name": "Loop Voltage Low",
        "description": "Incorrect supply voltage.",
        "resolution": "Check the power supply and loop resistance are within spec. If they are then replace the Communication Module."
    },

    {
        "byte": 1,
        "bit": 3,
        "criticality": 0,
        "name": "Brownout Status",
        "description": "Warm reset caused by low power supply.",
        "resolution": "Check Loop resistor and supply voltage values. If they are within spec, replace the Communication module."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 0,
        "name": "Program Flow Failure",
        "description": "SIL diagnostic failure.",
        "resolution": "Reset the device. If problem persist, replace the Communication module."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "ROM Failure",
        "description": "SIL diagnostic failure.",
        "resolution": "Reset the device. If problem persist, replace the Communication module."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 0,
        "name": "RAM Failure",
        "description": "SIL diagnostic failure.",
        "resolution": "Reset the device. If problem persist, replace the Communication module."
    },

    {
        "byte": 2,
        "bit": 7,
        "criticality": 0,
        "name": "Open Circuit on TB7",
        "description": "Input 2 Terminal TB7 is open. Applicable to RTD and Ohm sensors only.",
        "resolution": "Check if sensor input 2 is connected to terminal 7 and is secured."
    },
    {
        "byte": 2,
        "bit": 6,
        "criticality": 0,
        "name": "Open Circuit on TB7",
        "description": "Input 1 Terminal TB7 is open. Applicable to RTD and Ohm sensors only.",
        "resolution": "Check if sensor input 1 is connected to terminal 7 and is secured."
    },
    {
        "byte": 2,
        "bit": 5,
        "criticality": 0,
        "name": "Open Circuit on TB9",
        "description": "Input 2 terminal TB9 is open. Applicable to dual input models only.",
        "resolution": "Check if sensor input 2 is connected to terminal 9 and is secured."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 0,
        "name": "Open Circuit on TB8",
        "description": "Input terminal TB8 is open. Applicable to dual input models only.",
        "resolution": "Check if sensor input is connected to terminal 8 and is secured."
    },
    {
        "byte": 2,
        "bit": 3,
        "criticality": 0,
        "name": "Open Circuit on TB6",
        "description": "Input terminal TB6 is open.",
        "resolution": "Check if sensor input is connected to terminal 6 and is secured."
    },
    {
        "byte": 2,
        "bit": 2,
        "criticality": 1,
        "name": "Display Config Data Corrupt",
        "description": "Display configuration data has been corrupted.",
        "resolution": "Reset the device. Device will try to restore last saved configuration. If CRC still doesn't match with last configuration, the NVM block will be defaulted. User will need to reconfigure display Screens."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "Display Comm Timeout",
        "description": "Communication between the Communication Module and Display has failed.",
        "resolution": "Secure Display connections and recheck. If problem persists, replace the Display."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 0,
        "name": "Open Circuit on TB5",
        "description": "Input Terminal TB5 is open.",
        "resolution": "Check if sensor input is connected to terminal 5 and is secured."
    },

    {
        "byte": 3,
        "bit": 5,
        "criticality": 0,
        "name": "ADC Reference Fault",
        "description": "One of the sensor internal ADC reference voltages is bad.",
        "resolution": "Power cycle the device. If the problem persisits replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 0,
        "name": "ADC1 Range Fault",
        "description": "Input value supplied to the ADC1 is > ADC Range.",
        "resolution": "Reset the device, replace the sensor module/Terminal Block Assembly if issue persists."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 0,
        "name": "ADC0 Range Fault",
        "description": "Input value supplied to the ADC0 is > ADC Range.",
        "resolution": "Reset the device, replace the sensor module/Terminal Block Assembly if issue persists."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 0,
        "name": "CJ CT Delta Warning",
        "description": "The internal Cold Junction (CJ) temperature and the Processor Core (CT) temperature difference is > 10°C.",
        "resolution": "Verify environment is within specification."
    },
    {
        "byte": 3,
        "bit": 1,
        "criticality": 1,
        "name": "Low Transmitter Supply",
        "description": "This status bit is set when a low voltage supply is detected at:\n\n  - Transmitter terminals\n  - Sensor module\n  - Communication module",
        "resolution": "For condition 1, check the power supply and loop resistance are within spec. If they are, then replace the Communication Module. Conditions 2 and 3 can occur with low supply voltage from the Communication module or by loading by the Sensor or Communication module. Try to increase the voltage level of supply. If problem persists, try replacing the Communication module. If the problem still persists, replace the Sensor module."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 0,
        "name": "Control Word Write Fault",
        "description": "DAC or micro-controller chip may be damaged.",
        "resolution": "Power cycle the device. If problem persists, replace the Communication module."
    },

    {
        "byte": 4,
        "bit": 7,
        "criticality": 0,
        "name": "Program Flow Failure",
        "description": "Sensor module is reporting corruption in the program code flow.",
        "resolution": "Power cycle the device. If the problem persisits replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 6,
        "criticality": 0,
        "name": "Flash CRC Failure",
        "description": "Sensor module is reporting CRC failure in the Read-only Memory (ROM).",
        "resolution": "Power cycle the device. If the problem persists replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 5,
        "criticality": 0,
        "name": "RAM Failure",
        "description": "Sensor module is reporting corruption in the Random Access Memory (RAM).",
        "resolution": "Power cycle the device. If the problem persists, replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 4,
        "criticality": 0,
        "name": "Suspect Temperature Input",
        "description": "Reported temperature input data greatly exceeds measurable range and may be invalid. Could be a process problem, but it could also be a sensor problem.",
        "resolution": "Power cycle the device. Put transmitter in output mode and check transmitter status. Diagnostic messages should identify where problem is. If no other diagnostic message is given, condition is most likely sensor module/Terminal Block Assembly related. Check installation and replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 3,
        "criticality": 0,
        "name": "Input 1/Input 2/CJ Val Bad",
        "description": "Input 1 or Input 2 or Cold Junction value is bad.",
        "resolution": "Check if the inputs connected to Input 1 and 2 are as expected. If it is, check the sensor ranges configured are right for the process. If they are, power cycle the device. If the issue persists, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 2,
        "criticality": 0,
        "name": "Char Table CRC Fault",
         "description": "Sensor characterization is corrupt or there is a Sensor firmware failure.",
        "resolution": "If following a power cycle the failure recurs, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 1,
        "criticality": 1,
        "name": "Sensor NVM corrupt",
        "description": "Sensor NVM database is corrupted",
        "resolution": "If following a power cycle the failure recurs, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 0,
        "criticality": 0,
        "name": "Low Sensor Supply",
        "description": "Incorrect supply voltage. This can be caused by low supply voltage from the Communication module or by loading by the Sensor or Communication module.",
        "resolution": "If possible try to increase the voltage level of supply.Try replacing the Communication module. If that does not fix the problem replace the sensor module/Terminal Block Assembly."
    },

    {
        "byte": 5,
        "bit": 7,
        "criticality": 0,
        "name": "Input 2 Fault",
        "description": "Temperature Input 2 reading is bad.",
        "resolution": "Check if the temperature sensor connected to Input 2 matches the configuration. If it does, check that the configured sensor range is right for the process. If it is, power cycle the device. If the issue persists, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 6,
        "criticality": 0,
        "name": "Input 1 Fault",
        "description": "Temperature Input 1 reading is bad.",
        "resolution": "Check if the temperature sensor connected to Input 1 matches the configuration. If it does, check that the configured sensor range is right for the process. If it is, power cycle the device. If the issue persists, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 5,
        "criticality": 0,
        "name": "Cold Junction Fault",
        "description": "Cold Junction temperature reading is bad.",
        "resolution": "Verify the reported Cold Junction temperature against actual (ambient for Internal CJ, configured value for Fixed CJ).  If value is inaccurate or corrupted, power cycle the device. If the issue persists, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 4,
        "criticality": 0,
        "name": "Input 2 Val Bad",
        "description": "Input 2 value is bad.",
        "resolution": "Check if the sensor connection to Input 2 is as expected. If it is, check the sensor range configured is right for the process. If it is, power cycle the device. If the issue persists, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 3,
        "criticality": 0,
        "name": "Input 1 Val Bad",
        "description": "Input 1 value is bad.",
        "resolution": "Check if the sensor connection to Input 1 is as expected. If it is, check the sensor range configured is right for the process. If it is, power cycle the device. If the issue persists, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 2,
        "criticality": 0,
        "name": "User Corrects Activated",
        "description": "User Corrects for Sensor 1 or Sensor 2 activated.",
        "resolution": "This status indicates User sensor calibration is being used instead of factory calibration."
    },
    {
        "byte": 5,
        "bit": 1,
        "criticality": 0,
        "name": "Input 2 Excess Correction",
        "description": "The value applied during Calibration is too far away from the CAL point of Sensor 2.",
        "resolution": "Perform a Reset Corrects. Perform an LRV Correct on Sensor 2 first, checking that the applied input matches the Sensor 2 Lower calib Point value.  Next, perform the URV Correct on Sensor 2, checking that the input matches the Sensor 2 Upper Calib Point value."
    },
    {
        "byte": 5,
        "bit": 0,
        "criticality": 0,
        "name": "Input 1 Excess Correction",
        "description": "The value applied during Calibration is too far away from the CAL point of Sensor 1.",
        "resolution": "Perform a Reset Corrects. Perform an LRV Correct on Sensor 1 first, checking that the applied input matches the Sensor 1 Lower calib Point value.  Next, perform the URV Correct on Sensor 1, checking that the input matches the Sensor 1 Upper Calib Point value."
    },

    {
        "byte": 6,
        "bit": 6,
        "criticality": 0,
        "name": "Display Common Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the Display common configuration portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 6,
        "bit": 5,
        "criticality": 0,
        "name": "Display Screen Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the Display screen configuration portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 6,
        "bit": 4,
        "criticality": 0,
        "name": "Adv Diag DB Corrupt",
        "description": "The Communication module is reporting corruption in the advanced diagnostic portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 6,
        "bit": 3,
        "criticality": 0,
        "name": "Config Change DB Corrupt",
        "description": "The Communication module is reporting corruption in the configuration change parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 6,
        "bit": 2,
        "criticality": 0,
        "name": "General Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the general configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module,"
    },
    {
        "byte": 6,
        "bit": 1,
        "criticality": 0,
        "name": "Vital Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the vital configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 6,
        "bit": 0,
        "criticality": 0,
        "name": "Common DB Corrupt",
        "description": "The Communication module is reporting corruption in the common parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },

    {
        "byte": 7,
        "bit": 7,
        "criticality": 0,
        "name": "Display Screen 8 Corrupt",
        "description": "Display View 8 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 8."
    },
    {
        "byte": 7,
        "bit": 6,
        "criticality": 0,
        "name": "Display Screen 7 Corrupt",
        "description": "Display View 7 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 7."
    },
    {
        "byte": 7,
        "bit": 5,
        "criticality": 0,
        "name": "Display Screen 6 Corrupt",
        "description": "Display View 6 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 6."
    },
    {
        "byte": 7,
        "bit": 4,
        "criticality": 0,
        "name": "Display Screen 5 Corrupt",
        "description": "Display View 5 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 5."
    },
    {
        "byte": 7,
        "bit": 3,
        "criticality": 0,
        "name": "Display Screen 4 Corrupt",
        "description": "Display View 4 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 4."
    },
    {
        "byte": 7,
        "bit": 2,
        "criticality": 0,
        "name": "Display Screen 3 Corrupt",
        "description": "Display View 3 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 3."
    },
    {
        "byte": 7,
        "bit": 1,
        "criticality": 0,
        "name": "Display Screen 2 Corrupt",
        "description": "Display View 1 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 2."
    },
    {
        "byte": 7,
        "bit": 0,
        "criticality": 0,
        "name": "Display Screen 1 Corrupt",
        "description": "Display View 1 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 1."
    }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_InputSensorTypes = { Type: }
///
/// This enumeration contains the valid definitions for the style of temperatue sensing elements that can be connected to the STT850 temperature
/// transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     sensor1_type_code variable as read through device specific command 136
///     sensor2_type_code variable as read through device specific command 137
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_InputSensorTypes =
{
    0: { Type: "milliVolts" },
    1: { Type: "Thermocouple" } ,
    2: { Type: "RTD" },
    3: { Type: "Ohms" },
    4: { Type: "None" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_InputSensorIDs = { Type: }
///
/// This enumeration contains the valid definitions for the identification of the specific construction details of the temperatue sensing elements
/// that can be connected to the STT850 temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is
/// expected to be applied to:
///     sensor1_id variable as read through device specific command 136
///     sensor2_id variable as read through device specific command 137
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_InputSensorIDs =
{
    0:  { Type: "milliVolts", Model: "125 mV" },
    1:  { Type: "milliVolts", Model: "1200 mV" },
    2:  { Type: "Thermocouple", Model: "Type E" },
    3:  { Type: "Thermocouple", Model: "Type J" },
    4:  { Type: "Thermocouple", Model: "Type K" },
    5:  { Type: "Thermocouple", Model: "Type N" },
    6:  { Type: "Thermocouple", Model: "Type T" },
    7:  { Type: "Thermocouple", Model: "Type S" },
    8:  { Type: "Thermocouple", Model: "Type R" },
    9:  { Type: "Thermocouple", Model: "Type B" },
    10: { Type: "Thermocouple", Model: "Type C (W5W26)" },
    11: { Type: "RTD", Model: "Pt 25, a=385" },
    12: { Type: "RTD", Model: "Pt 100, a=385" },
    13: { Type: "RTD", Model: "Pt 200, a=385" },
    14: { Type: "RTD", Model: "Pt 500, a=385" },
    15: { Type: "RTD", Model: "Pt 1000, a=385" },
    16: { Type: "Ohms", Model: "500 Ω" },
    17: { Type: "Ohms", Model: "2K Ω" },
    18: { Type: "Ohms", Model: "3K Ω" },
    19: { Type: "RTD", Model: "Ni 120, a=672" },
    20: { Type: "RTD", Model: "Cu 10, a=427" },
    22: { Type: "RTD", Model: "Cu 50, a=428" },
    23: { Type: "RTD", Model: "Cu 100, a=426" },
    24: { Type: "RTD", Model: "Cu 100, a=428" },
    25: { Type: "RTD", Model: "Pt 50, a=391" },
    26: { Type: "RTD", Model: "Pt 100, a=391" },
    27: { Type: "Thermocouple", Model: "Type L GOST" },
    28: { Type: "RTD", Model: "Cu 100, Roxspur" },
    31: { Type: "None", Model: "None" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_InputSensorWires = { Wires: , Description: }
///
/// This enumeration contains the valid definitions for the number of wires associated with the RTD and Resistance style of temperatue sensing
/// elements that can be connected to the STT850 temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor
/// firmware and is expected to be applied to:
///     rtd1_type  variable as read through device specific command 136
///     rtd2_type  variable as read through device specific command 137
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_InputSensorWires =
{
    0: { Wires: 2, Description: "2 wire" },
    1: { Wires: 3, Description: "3 wire" },
    2: { Wires: 4, Description: "4 wire" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_ColdJunctionTemperatureSources = { Source: }
///
/// This enumeration contains the valid definitions for the source of the cold junction temperature input that can be used by the STT850
/// temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     cj_selection variable as read through device specific command 135
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_ColdJunctionTemperatureSources =
{
    0: { Source: "Internal" },
    1: { Source: "External" } ,
    2: { Source: "Fixed Value" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_BreakDetectionModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of the Break Detection function that can be used by the STT850
/// temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     break_detect_mode variable as read through device specific command 135
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_BreakDetectionModes =
{
    0: { Mode: "Disabled" },
    1: { Mode: "Enabled" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_PVCalculationModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of calculating the output PV Temperature in the STT850
/// temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     loop_control_choice variable as read through device specific command 188
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_PVCalculationModes =
{
    0: { Mode: "Average" },
    1: { Mode: "Differential" },
    2: { Mode: "Sensor 1" },
    3: { Mode: "Sensor 2" },
    4: { Mode: "Redundant" },
    5: { Mode: "Split Range" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_PVCalculationSources = { Source: }
///
/// This enumeration contains the valid definitions for the different input sensors that can be used as the source for the PV calculaition routine
/// in the STT850 temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be
/// applied to:
///     contolling_var variable as read through device specific command 188
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_PVCalculationSources =
{
    0: { Source: "Sensors 1 & 2" },
    1: { Source: "Sensor 1" },
    2: { Source: "Sensor 2" },
    4: { Source: "Single Input" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_ModuleInputTypes = { Type: }
///
/// This enumeration contains the valid definitions for the different module types in terms of number of inputs of the STT850 temperature transmitter.
/// This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be
/// applied to:
///     First character of table_I variable as read through device specific command 139
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_ModuleInputTypes =
{
    "S": { Type: "Single Input" },
    "T": { Type: "Dual Input" }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_MatchSensorsModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of the Match Sensors function that can be used by the STT850
/// temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     match_pvs variable as read through device specific command 135
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_MatchSensorsModes =
{
    0: { Mode: "Disabled" },
    1: { Mode: "Enabled" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_ExcessDeltaDetectionModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of the Excess Delta temperature detection function that can be used by
/// the STT850 temperature transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     xs_delta_detection variable as read through device specific command 223
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_ExcessDeltaDetectionModes =
{
    0: { Mode: "Disabled" },
    1: { Mode: "Enabled" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_LocalDisplayTypes = { Mode: }
///
/// This enumeration contains the valid definitions for the different locsl display types that can be connected to the STT850 temperature
/// transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     display_type_code  variable as read through device specific command 134
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_LocalDisplayTypes =
{
    0: { Mode: "None" },
    1: { Mode: "Advanced" },
    2: { Mode: "Basic" },
    3: { Mode: "None" }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850_DeviceVariables
///
/// This array contains the names corresponding to the 4 possible device variables that the transmitter supports.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT850_DeviceVariables =
[
    { code: 0, classification: "Temperature", name: "Process Temperature" },
    { code: 1, classification: "Temperature", name: "Cold Junction Temperature" },
    { code: 2, classification: "Temperature", name: "Sensor 1 Temperature" },
	{ code: 3, classification: "Temperature", name: "Sensor 2 Temperature" }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT850 Device Model
///
/// Initialize the global SL_Transmitter object to represent a STT850 transmitter. This global device model will be accessed by numerous global
/// functions to implement functionality that is common to all SmartLine transmitter types. It also provides storage for those display elements
/// that are not bound directly to a point parameter database item but rather made up of a number of components that can be updated asyncronously.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SL_Device.expectedModel = "STT850";
SL_Device.deviceVariables = STT850_DeviceVariables;

SL_Device.analogPV.classification = "Temperature";
SL_Device.analogPV.name = "Process Temperature";
SL_Device.analogPV.visibility = "visible";
SL_Device.analogPV.ignoreStatus = "false";

SL_Device.dynamicVariables.PV.classification = "Temperature";
SL_Device.dynamicVariables.PV.name = "Process Temperature";
SL_Device.dynamicVariables.PV.visibility = "visible";
SL_Device.dynamicVariables.PV.ignoreStatus = "true";

SL_Device.dynamicVariables.SV.classification = "Temperature";
SL_Device.dynamicVariables.SV.name = "Cold Junction Temperature";
SL_Device.dynamicVariables.SV.visibility = "visible";
SL_Device.dynamicVariables.SV.ignoreStatus = "true";

SL_Device.dynamicVariables.TV.classification = "Temperature";
SL_Device.dynamicVariables.TV.name = "Sensor Temperature";
SL_Device.dynamicVariables.TV.visibility = "visible";
SL_Device.dynamicVariables.TV.ignoreStatus = "true";

SL_Device.dynamicVariables.QV.classification = "Temperature";
SL_Device.dynamicVariables.QV.name = "";
SL_Device.dynamicVariables.QV.visibility = "hidden";
SL_Device.dynamicVariables.QV.ignoreStatus = "true";

SL_Device.deviceRevisions = STT850_DeviceRevisions;

SL_Device.command48.numberOfBytes = STT850_DeviceRevisions[1].command48StatusBytes;
SL_Device.command48.statusBits = STT850_Cmd48StatusBits;

SL_Device.command220.numberOfBytes = STT850_DeviceRevisions[1].command220StatusBytes;
SL_Device.command220.statusBits = STT850_Cmd220StatusBits;

SL_Device.command220.statusBytes.length = SL_Device.command220.numberOfBytes;

STT850_TemperatureModule = new SL_TemperatureModule();
STT850_TemperatureModule.PvCalculationSources = STT850_PVCalculationSources;
STT850_TemperatureModule.inputSensorTypes = STT850_InputSensorTypes;
STT850_TemperatureModule.inputSensorIDs = STT850_InputSensorIDs;
STT850_TemperatureModule.inputSensorWires = STT850_InputSensorWires;

SL_Device.module = STT850_TemperatureModule;