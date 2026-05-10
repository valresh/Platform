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
/// \file  SLG700.js
///
/// \brief Java script file to contain data arrays that are specific to the SLG700 transmitter.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// History:
///
/// EXP510.1-35		May   27, 2018		Developer Label D_EXP510.1-35.0-06
/// EXP510.1-34		May   21, 2018		Developer Label D_EXP510.1-34.0-12
/// [RLEAPFI-642]   April 30, 2018      Add support for building a SLG700 specific device model.
/// EXP510.1-31     April 24, 2018      Initial Experion R510 testbed release
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  SLG700_NumberOfStatusBytes = [{ Revision: , Cmd48: , Cmd220: }]
///
///  This array indicates the number of status bytes that have been defined for the array of device specific status bytes that are read through
///  the HART Universal Command 48 and the SmartLine Device Specific Command 220 for each of the released revisions of the transmitter firmware.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SLG700_NumberOfStatusBytes =
[
    { "Revision": 1, "Cmd48": 3, "Cmd220": 11 },
    { "Revision": 2, "Cmd48": 5, "Cmd220": 11 },
    { "Revision": 3, "Cmd48": 5, "Cmd220": 11 },
    { "Revision": 4, "Cmd48": 5, "Cmd220": 11 }
];


var SLG700_DeviceRevisions =
{
    1: new SL_DeviceRevision(1, 3, 11),
    2: new SL_DeviceRevision(2, 5, 11),
    3: new SL_DeviceRevision(3, 5, 11),
    4: new SL_DeviceRevision(4, 5, 11)
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  SLG700_Cmd48StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
///  This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the SLG700 transmitter
///  in the response to HART Universal Command 48. The location within the returned status byte array is defined by the combination of the byte
///  number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SLG700_Cmd48StatusBits =
[
    {
        "byte": 0,
        "bit": 6,
        "criticality": 0,
        "name": "Reset Required",
        "description": "In order to ensure that all tuning parameter modifications have been activated in the sensor board, a reset is required.",
        "resolution": "Navigate to the Services page of the Advanced Configuration menu and select the Soft Reset option or cycle the power to the transmitter OFF and ON."
    },
    {
        "byte": 0,
        "bit": 5,
        "criticality": 0,
        "name": "Measurement Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n\n  - Primary Variable Bad\n  - Device variable selected as Primary Variable (PV) is not applicable for configured measured product.\n  - PV selected is any of Volume device variables and the value is either infinite or not a valid number.\n  - PV Selected is derived from a measured level which is either infinite or not a valid number.",
        "resolution": "If the non-critical Device Variable Not Supported status is set, check if the device variable selected as Primary Variable is applicable for the configured measured product type.\n\nIf the non-critical Volume Calculation Failure status is set, check if Volume Calculation Type is configured as Strapping Table, and check the Strapping table entries for invalid data. If Volume calculation type is selected as Ideal Tank Shape check the tank dimensions configured are valid and correct.\n\nIf the non-critical Linearization Table Error Status is set, check the Linearization table for valid entries corresponding to Measured and Adjusted level values."
    },
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "Sensor Communication Timeout",
        "description": "There is no communication between communication board and sensor board, or invalid data is encountered on communication line due to noise.",
        "resolution": "Verify the cable/connector between communication and sensor board to ensure that it is not damaged.\n\nIf cable/connector is not the problem, try replacing the communication module.\nIf it does not fix the problem, replace the sensor housing."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "Sensor Critical Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n\n  - Reference Reflection Not Found\n  - Power Accumulator Fault\n  - Sensor Board Oscillator Failure\n  - Sensor Code Flow Fault\n  - Sensor External RAM Failure\n  - Sensor Internal RAM Failure\n  - Sensor Flash CRC Failure\n  - Sensor in Factory Mode\n  - Sensor in Low Power Mode\n  - Sensor Power Supply 2.5V Fault\n  - Sensor Power Supply 2.5V OSC Fault\n  - Sensor Power Supply 3.3V status Fault",
        "resolution": "Refer to the resolution for each condition."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 0,
        "name": "Comm Module Critical Failure 2",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n\n  - Program Flow Failure\n\n  - Communication Board Vcc (3.3V) Failure\n\n  - RAM CRC Failure\n\n  - RAM Walk Test Failure\n\n  - ROM Failure",
        "resolution": "Refer to the resolution for each condition present."
    },
    {
        "byte": 0,
        "bit": 1,
        "criticality": 0,
        "name": "Device Configuration Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n  - Algorithm Configuration DB Corrupt\n  - Common DB Corrupt\n  - Sensor and Comm. Board DB Data Mismatch\n  - Sensor and Comm. Board DB Version Mismatch\n  - Sensor Characterization Data Failure\n  - Sensor Configuration DB Corrupt\n  - Sensor Characterization Data Failure\n  - Sensor Parameter Write Failure\n  - Vital Configuration DB Corrupt\n  - Miscellaneous DB Corrupt\n  - General Configuration DB Corrupt\n\nThe following non-critical conditions can also cause this to be set.\n  - Linearization Table 1 (or Table 2) Config DB Corrupt (when linearization is enabled)\n  - Volume Strapping Table 1 (or Table 2) Config DB Corrupt (when volume calculation type is Strapping Table)",
        "resolution": "Refer to the resolution for each condition present."
    },
    {
        "byte": 0,
        "bit": 0,
        "criticality": 0,
        "name": "Comm Module Critical Failure 1",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n\n  - DAC SPI Interface Failure\n  - DAC Packet Error\n  - DAC Over Current\n  - DAC Under Current\n  - DAC Temperature Above 140°C\n  - DAC Control Word Write Fault",
        "resolution": "Refer to the resolution for each condition present."
    },

    {
        "byte": 1,
        "bit": 7,
        "criticality": 1,
        "name": "No DAC Compensation",
        "description": "Communication board DAC compensation is not performed. Loop accuracy may be slightly compromised. Effect will be minor degradation of ambient temperature influence specifications.",
        "resolution": "Replace the communication module to achieve the maximum current loop accuracy or return the device to factory for DAC compensation."
    },
    {
        "byte": 1,
        "bit": 6,
        "criticality": 1,
        "name": "Unreliable Sensor Communication",
        "description": "Communication between sensor and communication modules is not proper due to damaged connector or hardware issue with communication module or sensor module.",
        "resolution": "Verify the cable/connector between communication and sensor housing to ensure that it is not damaged.\nEither the transmitter is installed in a noisy environment or internal communication quality between the Electronics Module and Sensor is degrading.\nTry replacing either the communication module or sensor housing."
    },
    {
        "byte": 1,
        "bit": 5,
        "criticality": 1,
        "name": "Primary Variable Out of Range",
        "description": "PV value is not within LRV and URV.",
        "resolution": "Check if digital value of PV is out of configured LRV and URV.\nRead the echo curve to see if stem markers are valid for distance to product and distance to interface (if applicable based on measured product type).\nIf PV selected is derived from either strapping table or linearization table, check the tables for validity of entries."
    },
    {
        "byte": 1,
        "bit": 4,
        "criticality": 1,
        "name": "Fixed Current Mode",
        "description": "Output current is fixed and not varying as per input.\nLoop current mode is disabled or Loop Test is active.\nThis is just information for the user.",
        "resolution": "Enable loop current mode if it is disabled or exit the Loop Test mode if active."
    },
    {
        "byte": 1,
        "bit": 3,
        "criticality": 1,
        "name": "Low Supply Voltage (DAC)",
        "description": "Incorrect supply voltage at the transmitter terminals.\nFault can occur, If the terminal voltage is less than 13.5 V.",
        "resolution": "Check that the power supply and loop resistance are within specification. If they are correct, then replace the terminal block assembly."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 1,
        "name": "Sensor Non-Critical Failure",
        "description": "This is a roll-up status bit that is set when any of the following non critical status conditions are present.\n  - Sensor Electronic Over Temperature\n  - Distance in blocking higher zone\n  - Distance in blocking lower zone\n  - Sensor Not Characterized\n  - Sensor Not Calibrated",
        "resolution": "Refer to the resolution for each condition present."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "Electronic Module Comm Failure",
        "description": "This is a roll-up status bit that is set when any of the following non critical status conditions are present.\n\n  - DAC Temperature above 100°C\n\n  - Config change database block is corrupted in Communication board\n\n  - Advance diagnostics database block is corrupted in Communication board\n\n  - Brownout Status",
        "resolution": "Refer to the resolution for each condition present."
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
        "name": "Device in Startup Mode",
        "description": "Device is starting up after a power cycle.",
        "resolution": "No action required."
    },
    {
        "byte": 2,
        "bit": 6,
        "criticality": 1,
        "name": "Device Variable Out of Range",
        "description": "This alarm may be triggered if distance to interface is bad but device variable selected as PV is derived from distance to product which is valid.",
        "resolution": "Read the echo curve and verify valid stem markers for distance to interface, and configure the correlation algorithm parameters for interface peak reflections."
    },
    {
        "byte": 2,
        "bit": 5,
        "criticality": 1,
        "name": "Tamper Alarm",
        "description": "The Tamper Alarm is set if more than a specified number of attempted or actual configuration changes are made, when Tamper Alarm is enabled (Device can be either Write Protected or not). The warning stays active until the specified latency period has elapsed.",
        "resolution": "See User's Manual for Tamper Alarm configuration."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 1,
        "name": "Sensor in Sleep Mode",
        "description": "Sensor is in Sleep Mode due to Configuration Change.\nSensor module is adjusting model parameters and waiting for sufficient power to measure level.",
        "resolution": "Power cycle the device, and if problem persists for more than 4 minutes then replace the sensor housing."
    },
    {
        "byte": 2,
        "bit": 3,
        "criticality": 1,
        "name": "Loop Current Noise",
        "description": "DAC is not able to regulate loop current. If this condition is observed frequently, it could be an early indication of critical under or over-current failure.",
        "resolution": "Try to find the source of the noise. It could be bad loop wires, a hardware problem inside the transmitter, loop wires running close to a noise source, etc. If the environment is ok, replace the Electronics Module."
    },
    {
        "byte": 2,
        "bit": 2,
        "criticality": 1,
        "name": "Analog Output Saturated",
        "description": "Calculated analog output is either above or below the specified loop current limits (LRV and URV).",
        "resolution": "Verify that the LRV and URV values are in specified range or not.\nCheck if distance to product or distance to interface is in blocking zones."
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
        "bit": 6,
        "criticality": 1,
        "name": "Interface Rate of Change Exceeded",
        "description": "The Distance to Interface device variable was found to have changed faster than expected based on the current Maximum Filling Rate setting. This may be due to turbulence in the tank, incorrect settings, reflection model mismatch, or reflections from obstacles or secondary reflections being mistaken for the interface reflection.",
        "resolution": "Check that the Maximum Filling Rate parameter is set correctly. Adjust the Interface reflection model to obtain a high correlation to the true reflection and good rejection of other reflections that might be present."
    },
    {
        "byte": 3,
        "bit": 5,
        "criticality": 1,
        "name": "Surface Rate of Change Exceeded",
        "description": "The Distance to Product device variable was found to have changed faster than expected based on the current Maximum Filling Rate setting. This may be due to turbulence in the tank, incorrect settings, reflection model mismatch, or reflections from obstacles or secondary reflections being mistaken for the surface reflection.",
        "resolution": "Check that the Maximum Filling Rate parameter is set correctly. Adjust the Surface reflection model to obtain a high correlation to the true reflection and good rejection of other reflections that might be present."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 1,
        "name": "Field Background Load Error",
        "description": "The Field Background array failed to load because either there was no field background available or a checksum error was detected.",
        "resolution": "Initiate a new field background capture."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 1,
        "name": "Sensor not Calibrated",
        "description": "Sensor was not calibrated at the factory.",
        "resolution": "Contact your local representative for additional support."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 1,
        "name": "Sensor not Characterized",
        "description": "Sensor was not characterized at the factory.",
        "resolution": "Contact your local representative for additional support."
    },
    {
        "byte": 3,
        "bit": 1,
        "criticality": 1,
        "name": "Field Background is not Compatible",
        "description": "The available field captured Background array to model the ambient reflections in the near zone was obtained with a different mounting configuration.",
        "resolution": "Please check that all of the configuration parameters on the Mounting and Probe pages of the Advanced Configuration menu are correct. If necessary perform a field background capture to model the current conditions."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "Background Not Set",
        "description": "A suitable Background array to model the ambient reflections in the near zone could not be obtained for the current mounting conditions.",
        "resolution": "Please check that all of the configuration parameters on the Mounting and Probe pages of the Advanced Configuration menu are correct. If necessary perform a field background capture to model the current conditions."
    },

    {
        "byte": 4,
        "bit": 3,
        "criticality": 1,
        "name": "Interface in Low Blocking Zone",
        "description": "This indicates that the interface reflection has been tracked into the lower zone near the End of Probe where measurements are not accurate.",
        "resolution": "This is a condition that can occur during normal operation and does not generally require corrective action. If this condition is triggered when it is not expected, verify that the Blocking Distance Low parameter is set correctly for the current conditions.\nIf the interface is in the Lower zone then the status associated with device variables derived from distance to interface will be shown as unknown in local display and on HART host the status would be poor accuracy."
    },
    {
        "byte": 4,
        "bit": 2,
        "criticality": 1,
        "name": "Interface in High Blocking Zone",
        "description": "This indicates that the interface reflection has been tracked into the upper zone near the Reference Plane where measurements are not accurate.",
        "resolution": "This is a condition that can occur during normal operation and does not generally require corrective action. If this condition is triggered when it is not expected, verify that the Blocking Distance High parameter is set correctly for the current conditions.\nIf the interface is in the Higher zone then the status associated with device variables derived from distance to interface will be shown as unknown in local display and on HART host the status would be poor accuracy."
    },
    {
        "byte": 4,
        "bit": 1,
        "criticality": 1,
        "name": "Surface in Low Blocking Zone",
        "description": "This indicates that the surface reflection has been tracked into the lower zone near the End of Probe where measurements are not accurate.",
        "resolution": "This is a condition that can occur during normal operation and does not generally require corrective action. If this condition is triggered when it is not expected, verify that the Blocking Distance Low parameter is set correctly for the current conditions.\nIf the surface is in the Lower zone then the status associated with device variables derived from distance to surface will be shown as unknown in local display and on HART host the status would be poor accuracy."
    },
    {
        "byte": 4,
        "bit": 0,
        "criticality": 1,
        "name": "Surface in High Blocking Zone",
        "description": "This indicates that the surface reflection has been tracked into the upper zone near the Reference Plane where measurements are not accurate.",
        "resolution": "This is a condition that can occur during normal operation and does not generally require corrective action. If this condition is triggered when it is not expected, verify that the Blocking Distance High parameter is set correctly for the current conditions.\nIf the surface is in the Higher zone then the status associated with device variables derived from distance to surface will be shown as unknown in local display and on HART host the status would be poor accuracy."
    }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  SLG700_Cmd220StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
///  This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the SLG700 transmitter
///  in the response to SmartLine specific Command 220. The location within the returned status byte array is defined by the combination of the
///  byte number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SLG700_Cmd220StatusBits =
[
    {
        "byte": 0,
        "bit": 7,
        "criticality": 0,
        "name": "DAC SPI Interface Failure",
        "description": "Failure of the DAC SPI Interface.",
        "resolution": "Power cycle the device.\nCheck that the power supply and loop resistance are within specifications. If they are, then replace the communication module."
    },
    {
        "byte": 0,
        "bit": 6,
        "criticality": 0,
        "name": "DAC Packet Error",
        "description": "The DAC or micro-controller chip on the communication modulr may be damaged",
        "resolution": "Power cycle the device.\nCheck that the power supply and loop resistance are within specifications. If they are, then replace the communication module."
    },
    {
        "byte": 0,
        "bit": 5,
        "criticality": 0,
        "name": "DAC Over Current",
        "description": "The loop current value is greater than actual requested value. This might be due to some damaged component taking more current than expected.",
        "resolution": "Power cycle the device. If the problem persists, try replacing the terminal block assembly, communication module and/or sensor module in that order."
    },
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "DAC Under Current",
        "description": "The loop current value is below the actual requested value.",
        "resolution": "Power cycle the device. If the problem persists, replace the communication module."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "DAC Temperature Above 140°C",
        "description": "The communication board temperature is dangerously high - above 140°C.",
        "resolution": "Verify that the environment is within the operating specification of the transmitter. If it is, then replace the communication module."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 1,
        "name": "DAC Temperature Above 100°C",
        "description": "The communication board temperature is high - above 100°C.",
        "resolution": "Check the ambient temperature. If the ambient temperature is well below 100°C, check if there are other DAC faults reported by the device. Power cycle the device and if the problem persists, replace the communication module."
    },
    {
        "byte": 0,
        "bit": 1,
        "criticality": 0,
        "name": "DAC Control Word Write Fault",
        "description": "An attempt to write to the DAC controlling the loop current has failed. The DAC or micro-controller chip on the communication board may be damaged.",
        "resolution": "Power cycle the device. If the problem persists, replace the communication module."
    },

    {
        "byte": 1,
        "bit": 7,
        "criticality": 0,
        "name": "Comm. Board Vcc 3.3V Failure",
        "description": "The communication board power supply is bad.",
        "resolution": "Power cycle the device.\nIf the problem persists, check if there are any critical faults related to the power accumulator module. If there are power accumulator faults, replace the sensor housing.\nIf the problem still persists, replace the terminal block assembly.\nIf the problem still persists, replace the communication module."
    },
    {
        "byte": 1,
        "bit": 6,
        "criticality": 0,
        "name": "Device Variable Not Supported",
        "description": "This condition indicates that one of the Dynamic Variables is mapped to a Device Variable that is not supported by the current application as indicated by the Measured Product setting. This condition is considered non-critical for the SV, TV and QV Dynamic Variables. However, if this condition is true for the PV Dynamic Variable, then the critical Measurement Failure condition will also be set.",
        "resolution": "Reconfigure the dynamic variables PV, SV, TV, QV correctly for the configured measured product type."
    },
    {
        "byte": 1,
        "bit": 5,
        "criticality": 0,
        "name": "Volume Calculation Failure",
        "description": "This condition indicates that one of the Dynamic Variables is mapped to a Device Variable that calculates a volume and the volume calculation failed. This condition is considered non-critical for the SV, TV and QV Dynamic Variables. However, if this condition is true for the PV Dynamic Variable, then the critical Measurement Failure condition will also be set.",
        "resolution": "If the Volume Calculation method is using the Volume Strapping Table, verify that all of the entries in the table correctly match the current process conditions. If the Volume Calculation method is using an ideal tank shape, verify that the tank style and nominal dimensions are correct."
    },
    {
        "byte": 1,
        "bit": 4,
        "criticality": 0,
        "name": "RAM CRC Failure",
        "description": "This indicates a SIL diagnostic failure. The checksum of the critical parameters stored in RAM does not match the value calculated from the data.",
        "resolution": "Power cycle the device. If the problem still persists, then the RAM might be damaged so need to replace the communication module."
    },
    {
        "byte": 1,
        "bit": 3,
        "criticality": 1,
        "name": "Brownout Status",
        "description": "This indicates that device is reset due to fluctuations in supply voltage of communication board.",
        "resolution": "Verify the supply voltage to the transmitter and the loop resistance. Check for any noise source near the transmitter wiring. Power cycle the device and if the problem persists replace the communication module."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 0,
        "name": "Program Flow Failure",
        "description": "This fault will get set if any of the critical parts of the communication firmware code is not executed within the expected duration.",
        "resolution": "Check if any other faults are reported, like DAC faults. If DAC faults are reported, replace the communication board.\n\nIf there are no DAC faults, check the connection between the communication and sensor modules. Power cycle the device and if the problem persists, replace the communication module."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 0,
        "name": "ROM Failure",
        "description": "The communication module is reporting corruption in the Read-only Memory (ROM).",
        "resolution": "Power cycle the device. If the problem still persists after power cycle then ROM might be damaged so need to replace the communication module.\n\nBefore replacing the communication module ensure that the device is operating in environment which falls under the specifications as mentioned in GWR User's manual 34-SL-25-11."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 0,
        "name": "RAM Failure",
        "description": "The communication module is reporting corruption in the Random Access Memory (RAM).",
        "resolution": "Power cycle the device. If the problem still persists after power cycle then RAM might be damaged so need to replace the communication module.\n\nBefore replacing the communication module ensure that the device is operating in environment which falls under the specifications as mentioned in GWR User's manual 34-SL-25-11."
    },

    {
        "byte": 2,
        "bit": 5,
        "criticality": 1,
        "name": "Surface Signal Quality Bad",
        "description": "The shape of the reflection off the product surface does not match the configured model very well.",
        "resolution": "Read the echo curve and configure the reflection models and DC parameters accordingly."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 1,
        "name": "Surface Signal Strength Bad",
        "description": "The amplitude of the reflection off the product surface is weak.",
        "resolution": "Read the echo curve and configure the reflection models and DC parameters accordingly."
    },
    {
        "byte": 2,
        "bit": 3,
        "criticality": 1,
        "name": "Sensor Electronics Over Temperature",
        "description": "The sensor module temperature exceeded the operating temperature range specification which is -40 to +85°C.",
        "resolution": "Ensure that the process temperature is within the operating temperature range of the transmitter (-40 to +85°C). If it is certain that the reading is in error, the Sensor housing may need to be replaced."
    },
    {
        "byte": 2,
        "bit": 2,
        "criticality": 0,
        "name": "Primary Variable Bad",
        "description": "The distance to the surface of the product or the distance to the interface between two products used to derive the Primary variable is in critical status. The reason may be that variations in the echo curve, possibly caused by turbulence or obstacles, are interfering with the actual reflections from the product.",
        "resolution": "Read the echo curve and check if valid reflections are detected by the device, and stem markers show valid peak positions. If not, configure the correlation algorithm model parameters for each reflection. Ensure the Dielectric Constants configured are valid for the vapor and products in the tank."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "Interface Signal Quality Bad",
        "description": "The shape of the reflection off the interface between the upper and lower products does not match the configured model very well.",
        "resolution": "Read the echo curve and configure the reflection models and DC parameters accordingly."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 1,
        "name": "Interface Signal Strength Bad",
        "description": "The amplitude of the reflection off the interface between the upper and lower products is weak.",
        "resolution": "Read the echo curve and configure the reflection models and DC parameters accordingly."
    },

    {
        "byte": 3,
        "bit": 7,
        "criticality": 0,
        "name": "Database Version Mismatch",
        "description": "The communication module firmware is not compatible with the sensor module firmware.",
        "resolution": "Update the communication module and sensor module with compatible versions of firmware.\nCheck with Honeywell service person/support team for version details."
    },
    {
        "byte": 3,
        "bit": 6,
        "criticality": 0,
        "name": "Database Data Mismatch",
        "description": "The communication module and sensor module database parameters are inconsistent.",
        "resolution": "Check that the cable between the communication module and sensor module is properly connected.\n\nPower cycle the device.\n\nIf the problem still persists, try loading the correct communication and sensor firmware versions."
    },
    {
        "byte": 3,
        "bit": 5,
        "criticality": 0,
        "name": "Sensor Parameter Write Failure",
        "description": "Sensor module does not have the latest parameter configuration.",
        "resolution": "Power cycle the device and perform device configuration again.\nIf the problem still persists, load the correct communication and sensor firmware versions."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 0,
        "name": "Linearization Table Error",
        "description": "This condition indicates that one of the Dynamic Variables is mapped to a Device Variable that is derived from a linearized level measurement and the corrected level calculation failed. This condition is considered non-critical for the SV, TV and QV Dynamic Variables. However, if this condition is true for the PV Dynamic Variable, then the critical Measurement Failure condition will also be set.",
        "resolution": "Ensure that the level linearization table has at least two entries, that the entries in both measured and corrected level columns are in ascending or descending order, and that all of the entries are within the range specified by the measurement configuration parameters."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 1,
        "name": "Strapping/Linearization Table Absent",
        "description": "If linearization table is not available in communication board NVM when level linearization is enabled,\nOR\nIf strapping table is not available when volume calculation type is Strapping Table.",
        "resolution": "Configure the linearization table before level linearization is enabled. Configure the strapping table to select volume calculation type Strapping Table."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 1,
        "name": "Display NVM Corrupt",
        "description": "Display configuration data has been corrupted.",
        "resolution": "Power cycle the device.\nDevice will try to restore last saved configuration. If CRC still doesn't match with last configuration, the NVM block will be defaulted. In that case, reconfigure display general configurations like screen rotation, language and password.\nIf individual views are corrupted then reconfigure the corresponding view that is defaulted."
    },
    {
        "byte": 3,
        "bit": 1,
        "criticality": 0,
        "name": "Sensor Characterization Data Failure",
        "description": "Sensor Characterization Data CRC corruption.",
        "resolution": "Re-load the sensor firmware, and if problem still persists, replace the sensor housing."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "Local Display Timeout",
        "description": "Display module not connected properly to communication module, or the interface connector or hardware of either communication module or display module is damaged.",
        "resolution": "Power cycle the device and check the local display connectivity. Secure Display connections and recheck. Refer to Maintenance in User manual 34-SL-25-11 for more details about how to connect the display module with Communication module.\nIf problem still persists, replace the display module."
    },

    {
        "byte": 4,
        "bit": 7,
        "criticality": 0,
        "name": "Power Accumulator Fault",
        "description": "Power Accumulator board is damaged or malfunctioning.",
        "resolution": "Power-cycle the device and if problem persists, replace the sensor module. If problem still persists, replace the terminal block assembly."
    },
    {
        "byte": 4,
        "bit": 5,
        "criticality": 0,
        "name": "Sensor Power Supply 3.3V Fault",
        "description": "Power Accumulator board is damaged or malfunctioning.",
        "resolution": "Power-cycle the device and if problem persists, replace the sensor module. If problem still persists, replace the terminal block assembly."
    },
    {
        "byte": 4,
        "bit": 4,
        "criticality": 0,
        "name": "Sensor Power Supply 2.5V Fault",
        "description": "Power Accumulator board is damaged or malfunctioning.",
        "resolution": "Power-cycle the device and if problem persists, replace the sensor housing. If problem still persists, replace the terminal block assembly."
    },
    {
        "byte": 4,
        "bit": 3,
        "criticality": 0,
        "name": "Sensor Power Supply 2.5V OSC Fault",
        "description": "Power Accumulator board is damaged or malfunctioning.",
        "resolution": "Power-cycle the device and if problem persists, replace the sensor housing. If problem still persists, replace the terminal block assembly."
    },
    {
        "byte": 4,
        "bit": 2,
        "criticality": 0,
        "name": "Sensor Flash CRC Failure",
        "description": "Sensor board firmware has been corrupted.",
        "resolution": "Attempt to reload the sensor firmware. If problem persists after flashing sensor firmware, replacement of the sensor module is required."
    },
    {
        "byte": 4,
        "bit": 1,
        "criticality": 0,
        "name": "Sensor External Ram Failure",
        "description": "Sensor board external RAM corruption detected",
        "resolution": "Power-cycle the device and see if the condition re-occurs. If so, replacement of the sensor module is required."
    },
    {
        "byte": 4,
        "bit": 0,
        "criticality": 0,
        "name": "Sensor Internal Ram Failure",
        "description": "Sensor board internal RAM corruption detected.",
        "resolution": "Power-cycle the device and see if the condition re-occurs. If so, replacement of the sensor module is required."
    },

    {
        "byte": 5,
        "bit": 5,
        "criticality": 0,
        "name": "Sensor NVRAM Corruption",
        "description": "The non-volatile memory located in the sensor has become corrupt and cannot be used.",
        "resolution": "In this case the sensor NVRAM must be reverted back to default values. This action can be initiated from the Services page."
    },
    {
        "byte": 5,
        "bit": 4,
        "criticality": 0,
        "name": "Reference Reflection Not Located",
        "description": "The sensor was unable to locate the Reference Reflection used to determine the location of the Reference Plane within the time sampled data. This reflection must be found in order for any other reflections to be located correctly.",
        "resolution": "Read the echo curve with the Show Behind Reference Plane option selected. Load the echo curve into the Correlation Algorithm page and verify the model parameters for the Reference Reflection."
    },
    {
        "byte": 5,
        "bit": 3,
        "criticality": 0,
        "name": "Sensor in Low Power Mode",
        "description": "The sensor is in Low Power Mode designed to handle low range burnout.",
        "resolution": "The fault is set if any critical faults are set and device burnout direction (fail-safe) is set to low (3.5 mA). Please perform a hard or soft reset."
    },
    {
        "byte": 5,
        "bit": 2,
        "criticality": 0,
        "name": "Sensor in Factory Mode",
        "description": "Unit is in factory/test mode.",
        "resolution": "Power-cycle the device and check if problem re-occurs."
    },
    {
        "byte": 5,
        "bit": 1,
        "criticality": 0,
        "name": "Sensor Board Oscillator Failure",
        "description": "If no power accumulator faults are detected, then most likely the sensor board is damaged/defective.",
        "resolution": "Power cycle the device. If problem persists, replace the sensor housing."
    },
    {
        "byte": 5,
        "bit": 0,
        "criticality": 0,
        "name": "Sensor Code Flow Fault",
        "description": "The sensor is detecting that the time between measurements has exceeded the allowed limit.",
        "resolution": "Ensure configurations are correct and restart the sensor. If problem persists, replacement of the sensor housing may be necessary."
    },

    {
        "byte": 6,
        "bit": 7,
        "criticality": 0,
        "name": "Sensor Configuration DB Corrupt",
        "description": "NVM copy of sensor configuration database block is corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 6,
        "criticality": 1,
        "name": "Display Common Config DB Corrupt",
        "description": "NVM copy of Display General Parameters database block found corrupt.",
        "resolution": "Power cycle the device and re-configure display configuration if required."
    },
    {
        "byte": 6,
        "bit": 5,
        "criticality": 1,
        "name": "Display View Config DB Corrupt",
        "description": "NVM copy of Display View/Screen Configuration database block found corrupt.",
        "resolution": "Power cycle the device and re-configure display configuration if required."
    },
    {
        "byte": 6,
        "bit": 4,
        "criticality": 1,
        "name": "Advanced Diagnostics DB Corrupt",
        "description": "Communication board RAM copy of Advanced diagnostics database block found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists then RAM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 3,
        "criticality": 1,
        "name": "Configuration Change DB Corrupt",
        "description": "NVM copy of Configuration history database block found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists then the NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 2,
        "criticality": 0,
        "name": "General Configuration DB Corrupt",
        "description": "NVM copy of General Configuration database found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 1,
        "criticality": 0,
        "name": "Vital Configuration DB Corrupt",
        "description": "NVM copy of Vital Configuration database block found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 0,
        "criticality": 0,
        "name": "Common DB Corrupt",
        "description": "NVM copy of common database block found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists then NVM might be damaged so need to replace the communication module."
    },

    {
        "byte": 7,
        "bit": 7,
        "criticality": 0,
        "name": "Comm Board Model Info DB Corrupt",
        "description": "NVM copy of Model Info database block found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module. "
    },
    {
        "byte": 7,
        "bit": 6,
        "criticality": 0,
        "name": "Calibration DB Corrupt",
        "description": "NVM copy of Calibration database block found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 7,
        "bit": 5,
        "criticality": 0,
        "name": "Volume Strapping Table 2 Config DB Corrupt",
        "description": "NVM copy of Volume Strapping Table 2 database block is corrupt.\nThis non critical alarm can occur regardless of the volume calculation type setting.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module.\nEven if problem does not persist, user may be required to configure the strapping table again for their application (as the device parameters part of this block will be restored to default values)."
    },
    {
        "byte": 7,
        "bit": 4,
        "criticality": 0,
        "name": "Volume Strapping Table 1 Config DB Corrupt",
        "description": "NVM copy of Volume Strapping Table 1 database block is corrupt.\nThis non critical alarm can occur regardless of the volume calculation type setting.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module.\nEven if problem does not persist, user may be required to configure the strapping table again for their application (as the device parameters part of this block will be restored to default values)."
    },
    {
        "byte": 7,
        "bit": 3,
        "criticality": 0,
        "name": "Linearization Table 2 Config DB Corrupt",
        "description": "NVM copy of Level Linearization Table 2 database block is corrupt.\nThis non critical alarm can occur even if linearization is disabled.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module.\nEven if problem does not persist, user may be required to configure the linearization table again for their application (as the device parameters part of this block will be restored to default values)."
    },
    {
        "byte": 7,
        "bit": 2,
        "criticality": 0,
        "name": "Linearization Table 1 Config DB Corrupt",
        "description": "NVM copy of Level Linearization Table 1 database block is corrupt.\nThis non critical alarm can occur even if linearization is disabled.",
        "resolution": "Power cycle the device.\nIf the problem still persists, then NVM might be damaged so need to replace the communication module.\nEven if problem does not persist, user may be required to configure the linearization table again for their application (as the device parameters part of this block will be restored to default values)."
    },
    {
        "byte": 7,
        "bit": 1,
        "criticality": 0,
        "name": "Algorithm Configuration DB Corrupt",
        "description": "NVM copy of sensor algorithm database block found corrupt.",
        "resolution": "Power cycle the device.\nIf the problem still persists then NVM might be damaged so need to replace the communication board.\nUser may need to configure the correlation algorithm parameters if device does not make valid level measurements."
    },
    {
        "byte": 7,
        "bit": 0,
        "criticality": 0,
        "name": "Model Number Mismatch",
        "description": "The Model Key or Model Number stored in the Sensor Board does not match the corresponding fields in the Communication Board of the SLG700 transmitter.",
        "resolution": "This condition will occur if either the Rook Assembly containing the Sensor Board or the Communication Board in the transmitter are replaced with a replacement that was previously used in another transmitter. This condition causes the transmitter to go into a failure mode until the replacemnt board is identified and has its Model Number updated. This can be done from the Model Number Mismatch section of the Services page."
    },

    {
        "byte": 8,
        "bit": 7,
        "criticality": 1,
        "name": "Sensor Configuration DB RAM Corrupt",
        "description": "RAM copy of Sensor Configuration database block found corrupt.",
        "resolution": "Power cycle the device."
    },
    {
        "byte": 8,
        "bit": 6,
        "criticality": 1,
        "name": "Display Common DB RAM Corrupt",
        "description": "RAM copy of Display General Parameters database block found corrupt.",
        "resolution": "Power cycle the device and re-configure display configuration if required."
    },
    {
        "byte": 8,
        "bit": 5,
        "criticality": 1,
        "name": "Display View DB RAM Corrupt",
        "description": "RAM copy of Display View/Screen Configuration database block found corrupt.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 8,
        "bit": 4,
        "criticality": 1,
        "name": "Advanced Diagnostics DB RAM Corrupt",
        "description": "",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 8,
        "bit": 3,
        "criticality": 1,
        "name": "Configuration Change DB RAM Corrupt",
        "description": "RAM copy of Configuration History database block found corrupt.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 8,
        "bit": 2,
        "criticality": 1,
        "name": "General Configuration DB RAM Corrupt",
        "description": "RAM copy of General Configuration database block found corrupt.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 8,
        "bit": 1,
        "criticality": 1,
        "name": "Vital Configuration DB RAM Corrupt",
        "description": "RAM copy of Vital Configuration database block found corrupt.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 8,
        "bit": 0,
        "criticality": 1,
        "name": "Common DB RAM Corrupt",
        "description": "RAM copy of Common database block found corrupt.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },

    {
        "byte": 9,
        "bit": 6,
        "criticality": 1,
        "name": "Calibration DB RAM Corrupt",
        "description": "RAM copy of Calibration database block found corrupt.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 9,
        "bit": 5,
        "criticality": 1,
        "name": "Volume Strapping Table 2 Config DB RAM Corrupt",
        "description": "RAM copy of Volume Strapping Table 2 database block found corrupt.\nThis non critical alarm can occur even if volume calculation type is None.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 9,
        "bit": 4,
        "criticality": 1,
        "name": "Volume Strapping Table 1 Config DB RAM Corrupt",
        "description": "RAM copy of Volume Strapping Table 1 database block found corrupt.\nThis non critical alarm can occur even if volume calculation type is None.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 9,
        "bit": 3,
        "criticality": 1,
        "name": "Linearization Table 2 Config DB RAM Corrupt",
        "description": "Level Linearization Table 2 RAM copy of database block found corrupt.\nThis non critical alarm can occur even if linearization is disabled.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 9,
        "bit": 2,
        "criticality": 1,
        "name": "Linearization Table 1 Config DB RAM Corrupt",
        "description": "Level Linearization Table 1 RAM copy of database block found corrupt.\nThis non critical alarm can occur even if linearization is disabled.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },
    {
        "byte": 9,
        "bit": 1,
        "criticality": 1,
        "name": "Sensor Algorithm Config DB RAM Corrupt",
        "description": "RAM copy of Sensor Algorithm database block found corrupt.",
        "resolution": "Power cycle the device and if problem still persists, replace the communication module."
    },

    {
        "byte": 10,
        "bit": 7,
        "criticality": 1,
        "name": "Display View 8 Corrupt",
        "description": "Display View 8 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 8."
    },
    {
        "byte": 10,
        "bit": 6,
        "criticality": 1,
        "name": "Display View 7 Corrupt",
        "description": "Display View 7 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 7."
    },
    {
        "byte": 10,
        "bit": 5,
        "criticality": 1,
        "name": "Display View 6 Corrupt",
        "description": "Display View 6 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 6."
    },
    {
        "byte": 10,
        "bit": 4,
        "criticality": 1,
        "name": "Display View 5 Corrupt",
        "description": "Display View 5 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 5."
    },
    {
        "byte": 10,
        "bit": 3,
        "criticality": 1,
        "name": "Display View 4 Corrupt",
        "description": "Display View 4 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 4."
    },
    {
        "byte": 10,
        "bit": 2,
        "criticality": 1,
        "name": "Display View 3 Corrupt",
        "description": "Display View 3 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 3."
    },
    {
        "byte": 10,
        "bit": 1,
        "criticality": 1,
        "name": "Display View 2 Corrupt",
        "description": "Display View 2 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 2."
    },
    {
        "byte": 10,
        "bit": 0,
        "criticality": 1,
        "name": "Display View 1 Corrupt",
        "description": "Display View 1 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 1."
    }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SLG700_ProcessConnectorTypes = { Type: }
///
/// This enumeration contains the valid definitions for the different process connector models supported in the SLG700 transmitter. This enumeration
/// is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     Transmitter_model  variable as read through device specific command 149
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SLG700_ProcessConnectorTypes =
{
    0: { Type: "SLG720: Standard Temperature, Standard Pressure" },
    1: { Type: "SLG722: Standard Temperature, High Strength" },
    2: { Type: "SLG724: Standard Temperature, High Pressure" },
    3: { Type: "SLG726: High Temperature, High Pressure" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SLG700_ProbeTypes = { Type: }
///
/// This enumeration contains the valid definitions for the different probe types supported in the SLG700 transmitter. This enumeration is
/// based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     probe_type  variable as read through device specific command 201
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SLG700_ProbeTypes =
{
    0: { Type: "Custom" },
    1: { Type: "Rod" },
    2: { Type: "Wire" },
    3: { Type: "Coax" },
    4: { Type: "Twin Rod" },
    5: { Type: "Twin Wire" },
    6: { Type: "Multi-Twist Wire" },
    7: { Type: "PTFE Coated Rod" },
    8: { Type: "PTFE Coated Wire" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SLG700_ProbeEndTypes = { Type: }
///
/// This enumeration contains the valid definitions for the different probe end types supported in the SLG700 transmitter. This enumeration is
/// based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     probe_end_type  variable as read through device specific command 201
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SLG700_ProbeEndTypes =
{
    0: { Type: "None" },
    1: { Type: "Clamp" },
    2: { Type: "Weight" },
    3: { Type: "Loop" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SLG700_DeviceVariables
///
/// This array contains the names corresponding to the 15 possible device variables that the transmitter supports.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SLG700_DeviceVariables =
[
    { code: 0,  classification: "Length",     name: "Product Level" },
    { code: 1,  classification: "Analytical", name: "Product Level %" },
    { code: 2,  classification: "Length",     name: "Distance To Product" },
    { code: 3,  classification: "Velocity",   name: "Product Level Rate" },
    { code: 4,  classification: "Volume",     name: "Product Volume" },
    { code: 5,  classification: "Length",     name: "Vapour Thickness" },
    { code: 6,  classification: "Analytical", name: "Vapour Thickness %" },
    { code: 7,  classification: "Volume",     name: "Vapour Volume" },
    { code: 8,  classification: "Length",     name: "Interface Level" },
    { code: 9,  classification: "Length",     name: "Interface Level %" },
    { code: 10, classification: "Length",     name: "Distance To Interface" },
    { code: 11, classification: "Velocity",   name: "Interface Level Rate" },
    { code: 12, classification: "Length",     name: "Upper Product Thickness" },
    { code: 13, classification: "Volume",     name: "Lower Product Volume" },
    { code: 14, classification: "Volume",     name: "Upper Product Volume" }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SLG700 Device Model
///
/// Initialize the global SL_Transmitter object to represent a SLG700 transmitter. This global device model will be accessed by numerous global
/// functions to implement functionality that is common to all SmartLine transmitter types. It also provides storage for those display elements
/// that are not bound directly to a point parameter database item but rather made up of a number of components that can be updated asyncronously.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SL_Device.expectedModel = "SLG700";

SL_Device.deviceVariables = SLG700_DeviceVariables;

SL_Device.analogPV.name = "";
SL_Device.analogPV.visibility = "visible";
SL_Device.analogPV.ignoreStatus = "false";

SL_Device.dynamicVariables.PV.name = "";
SL_Device.dynamicVariables.PV.visibility = "visible";
SL_Device.dynamicVariables.PV.ignoreStatus = "false";

SL_Device.dynamicVariables.SV.name = "";
SL_Device.dynamicVariables.SV.visibility = "visible";
SL_Device.dynamicVariables.SV.ignoreStatus = "false";

SL_Device.dynamicVariables.TV.name = "";
SL_Device.dynamicVariables.TV.visibility = "visible";
SL_Device.dynamicVariables.TV.ignoreStatus = "false";

SL_Device.dynamicVariables.QV.name = "";
SL_Device.dynamicVariables.QV.visibility = "visible";
SL_Device.dynamicVariables.QV.ignoreStatus = "false";

SL_Device.deviceRevisions = SLG700_DeviceRevisions;

SL_Device.command48.numberOfBytes = SLG700_DeviceRevisions[1].command48StatusBytes;
SL_Device.command48.statusBits = SLG700_Cmd48StatusBits;

SL_Device.command220.numberOfBytes = SLG700_DeviceRevisions[1].command220StatusBytes;
SL_Device.command220.statusBits = SLG700_Cmd220StatusBits;

SL_Device.command220.statusBytes.length = SL_Device.command220.numberOfBytes;

SL_Device.modelNumberParts.push(new SL_ScriptData("table_I"));
SL_Device.modelNumberParts.push(new SL_ScriptData("table_II"));
