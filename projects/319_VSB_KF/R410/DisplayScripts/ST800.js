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
/// \file  ST800.js
///
/// \brief Java script file to contain variables and functions that are specific to the ST800 transmitter or the SmartLine Pressure family.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// History:
///
/// EXP510.1-36     June   4, 2018      Developer Label D_EXP510.1-36.0-02
/// [RLEAPFI-475]   May   28, 2018      Added the Loop Voltage Low status bit.
/// EXP510.1-35		May   27, 2018		Developer Label D_EXP510.1-35.0-06
/// EXP510.1-34		May   21, 2018		Developer Label D_EXP510.1-34.0-12
/// [RLEAPFI-642]	April 30, 2018		Add support for building a ST800 specific device model.
/// EXP510.1-31		April 24, 2018		Initial Experion R510 testbed release
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ST800_NumberOfStatusBytes = [{ Revision: , Cmd48: , Cmd220: }]
///
/// This array indicates the number of status bytes that have been defined for the array of device specific status bytes that are read through
/// the HART Universal Command 48 and the SmartLine Device Specific Command 220 for each of the released revisions of the transmitter firmware.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var ST800_NumberOfStatusBytes =
[
    { "Revision": 1, "Cmd48": 3, "Cmd220": 8 },
    { "Revision": 2, "Cmd48": 3, "Cmd220": 8 },
    { "Revision": 3, "Cmd48": 3, "Cmd220": 8 },
    { "Revision": 4, "Cmd48": 3, "Cmd220": 8 },
    { "Revision": 5, "Cmd48": 4, "Cmd220": 8 }
];

var ST800_DeviceRevisions =
{
    1: new SL_DeviceRevision(1, 3, 8),
    2: new SL_DeviceRevision(2, 3, 8),
    3: new SL_DeviceRevision(3, 3, 8),
    4: new SL_DeviceRevision(4, 3, 8),
    5: new SL_DeviceRevision(5, 4, 8)
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ST800_Cmd48StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the ST800 transmitter
/// in the response to HART Universal Command 48. The location within the returned status byte array is defined by the combination of the byte
/// number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var ST800_Cmd48StatusBits =
[
    {
        "byte": 0,
        "bit": 5,
        "criticality": 0,
        "name": "Sensor Communication Timeout",
        "description": "There is no communication between communication board and sensor board, or invalid data is encountered on communication line due to noise.",
        "resolution": "Verify that the cable/connector between communication and sensor board is secure and that it is not damaged.\n\nIf cable/connector is not the problem, replace the communication module. If the problem persists, replace the meterbody."
    },
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "Meter Body Critical Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n  - Meter Body Failure\n  - Char Table CRC Fault\n  - PV,SV,TV Bad\n  - Sensor Program Flow Failure\n  - Suspect Input\n  - Sensor RAM Failure\n  - Sensor ROM Failure\n  - Sensor RAM Database Fault",
        "resolution": "Refer to the resolution for each condition present."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "Electronic Module Diag Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n\n  - Comm. RAM Failure\n  - Comm. ROM Failure\n  - Comm. Program Flow Failure",
        "resolution": "Refer to the resolution for each condition present."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 0,
        "name": "Config Data Corrupt",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n\n  - Common DB Corrupt\n  - Vital DB Corrupt\n  - General DB Corrupt",
        "resolution": "Refer to the resolution for each condition present."
    },
    {
        "byte": 0,
        "bit": 1,
        "criticality": 0,
        "name": "Meter Body NVM Corrupt",
        "description": "Critical NVM failure reported at the Sensor level",
        "resolution": "Power cycle the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 0,
        "bit": 0,
        "criticality": 0,
        "name": "Electronic Module DAC Failure",
        "description": "This is a roll-up status bit that is set when any of the following DAC critical status conditions are present.\n\n  - SPI Interface Failure\n  - Packet Error\n  - Over Current\n  - Under Current\n  - Temperature Above 140°C\n  - Control Word Write Fault",
        "resolution": "Refer to the resolution for each condition present."
    },

    {
        "byte": 1,
        "bit": 7,
        "criticality": 1,
        "name": "No DAC Compensation",
        "description": "Communication board DAC compensation has not been performed. Loop accuracy may be slightly compromised. Effect will be minor degradation of ambient temperature influence specifications.",
        "resolution": "Replace the communication module to achieve the maximum current loop accuracy or return the device to factory for DAC compensation."
    },
    {
        "byte": 1,
        "bit": 6,
        "criticality": 1,
        "name": "No Factory Calibration",
        "description": "Factory calibration is missing. Accuracy will be compromised.",
        "resolution": "Replace the Meter Body or return the device for factory calibration."
    },
    {
        "byte": 1,
        "bit": 5,
        "criticality": 1,
        "name": "Primary Variable Out of Range",
        "description": "This is a roll-up status bit that is set any of the following non-critical conditions are present\n  - Sensor Overload or Fault\n  - Redundant Characterization Calculation Error",
        "resolution": "Check range and, if required, replace transmitter with one that has a wider range.\n\nMeter Body may have been damaged. Check the transmitter for accuracy and linearity. Replace the Meter Body and recalibrate if needed."
    },
    {
        "byte": 1,
        "bit": 4,
        "criticality": 1,
        "name": "Fixed Current Mode",
        "description": "Output current is fixed and not varying as per input because, Loop current mode is disabled or Loop Test is active.",
        "resolution": "Enable loop current mode if it is disabled or exit the Loop Test mode if active."
    },
    {
        "byte": 1,
        "bit": 3,
        "criticality": 1,
        "name": "Sensor Over Temperature",
        "description": "Meter Body temperature is too high. Accuracy and life span may decrease if it remains high.",
        "resolution": "Verify the environment is within spec. Take steps to insulate Meter Body from temperature source."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 1,
        "name": "Meter Body Excess Correct",
        "description": "This is a roll-up status bit that is set when any of the following non-critical status conditions are present\n\n  - Excess zero correct\n  - Excess Span correct\n\nIt is likely that the process pressure was incorrect or transmitter was in output mode during a CORRECT procedure.",
        "resolution": "Check input pressure and be sure that it matches calibrated range values (LRV and URV). Check Meter Body. Perform an LRV or ZERO CORRECT procedure followed by a URV CORRECT procedure."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "Electronic Module Comm Failure",
        "description": "This is a roll-up status bit that is set when any of the following non-critical status conditions are present.\n\n  - Advanced Diagnostic database Corrupt\n  - Configuration Changes database corrupt\n  - Environmental temperature above 100°C",
        "resolution": "Power cycle the device. If the problem still persists, verify the environment is within spec. If it is, then replace the Electronics Module."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 1,
        "name": "Local Display Failure",
        "description": "Display not detected or configuration data has been corrupted",
        "resolution": "Secure Display connections and recheck. If problem persists, reset the device. If the problem still persists, replace the Display."
    },

    {
        "byte": 2,
        "bit": 7,
        "criticality": 1,
        "name": "Low Supply Voltage",
        "description": "The supply voltage to the transmitter terminals is too low or the supply voltage to the Sensor board is too low.",
        "resolution": "Check that the power supply and loop resistance are within specification. If possible, try to increase the voltage level of the supply. If supply voltage and loop resistance are adequate and the problem persists, replace the Electronics Module. If the problem still persists, replace the Meter Body."
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
        "description": "The Tamper Alarm is set if more than a specified number of attempted or actual configuration changes are made, when Tamper Alarm is enabled (Device can be either Write Protected or not). The warning stays active until the specified latency period has elapsed.",
        "resolution": "See the User's Manual 34-ST-25-35 for Tamper Alarm configuration."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 1,
        "name": "Meter Body Unreliable Communication",
        "description": "Either the transmitter is installed in a noisy environment or internal communication quality between the Electronics Module and Sensor is degrading.",
        "resolution": "Call service person."
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
        "description": "Calculated analog output is either above or below the specified loop current limits (LRV and URV). The transmitter input is not in specified range.",
        "resolution": "Verify that the LRV and URV values are correct and that the process conditions are within range."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "URV Set Error - Span Config Button",
        "description": "Set operation using external Span button was rejected.",
        "resolution": "Verify that the inputs are valid for the intended operation."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 1,
        "name": "LRV Set Error - Zero Config Button",
        "description": "Set operation using external Zero button was rejected.",
        "resolution": "Verify that the inputs are valid for the intended operation."
    },

    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "PILD Blockage Detected",
        "description": "One of the impulse lines is blocked. Possible causes are:\n\n  - The transmitter is not installed to a process or the process is not in operation\n  - One of the impulse lines of DP is blocked and the other side is open to atmosphere or has a constant pressure\n  - Both sides of the impulse lines are blocked\n  - Noise in process is too low",
        "resolution": "For case 1, just ignore the alarm. It's suggested that PILD be enabled only after the transmitter is installed and the process is running.\n\nFor case 2 & 3, check and clear the impulse line(s). Then disable and enable PILD when the process and transmitter is in normal operation.\n\nFor case 4, PILD is not applicable to this specific situation. It�s suggested to disable PILD."
    }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ST800_Cmd220StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the ST800 transmitter
/// in the response to SmartLine specific Command 220. The location within the returned status byte array is defined by the combination of the
/// byte number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var ST800_Cmd220StatusBits =
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
        "description": "The DAC or micro-controller chip on the communication module may be damaged.",
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
        "resolution": "Check that the power supply and loop resistance are within spec. If they are, replace the communication module."
    },
    {
        "byte": 0,
        "bit": 0,
        "criticality": 1,
        "name": "Loop Voltage Low",
        "description": "Incorrect supply voltage.",
        "resolution": "Check the power supply and loop resistance to confirm if they are within specification. If they are, replace the communication module."
    },

    {
        "byte": 1,
        "bit": 2,
        "criticality": 0,
        "name": "Comm. Program Flow Failure",
        "description": "This fault will get set if any of the critical parts of the communication firmware code is not executed within the expected duration.",
        "resolution": "Check if any other faults are reported, like DAC faults. If DAC faults are reported, replace the communication module.\n\nIf there are no DAC faults, check the connection between the communication and sensor modules. Power cycle the device and if the problem persists, replace the communication module."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "Comm. Module ROM Failure",
        "description": "The communication module is reporting corruption in the Read-only Memory (ROM).",
        "resolution": "Power cycle the device. If the problem still persists after power cycle then ROM might be damaged so need to replace the communication module.\n\nBefore replacing the communication module ensure that the device is operating in environment which falls under the specifications as mentioned in the User's Manual 34-ST-25-35."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 1,
        "name": "Comm. Module RAM Failure",
        "description": "The communication module is reporting corruption in the Random Access Memory (RAM).",
        "resolution": "Power cycle the device. If the problem still persists after power cycle then RAM might be damaged so need to replace the communication module.\n\nBefore replacing the communication module ensure that the device is operating in environment which falls under the specifications as mentioned in the User's Manual 34-ST-25-35."
    },

    {
        "byte": 2,
        "bit": 2,
        "criticality": 1,
        "name": "Display Config Data Corrupt",
        "description": "Display configuration data has been corrupted.",
        "resolution": "Power cycle the device. Device will try to restore last saved configuration. If CRC still doesn't match with last configuration, the NVM block will be defaulted. User will need to reconfigure display Screens."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "Local Display Timeout",
        "description": "Display module not connected properly to communication module, or the interface connector or hardware of either communication module or display module is damaged.",
        "resolution": "Power cycle the device and check the local display connectivity. Secure Display connections and recheck. Refer to Maintenance in the User's Manual 34-ST-25-35 for more details about how to connect the display module with Communication module.\nIf problem still persists, replace the display module."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 1,
        "name": "Advanced Diagnostic NVM Corrupt",
        "description": "Advanced Diagnostic NVM data has been corrupted.",
        "resolution": "Power cycle the device. Device will try to restore last saved block. If it fails, the NVM block will be defaulted and corresponding default flag will be set. The default flag will be reset by device reset."
    },

    {
        "byte": 3,
        "bit": 1,
        "criticality": 1,
        "name": "Low Transmitter supply",
        "description": "Incorrect supply voltage.",
        "resolution": "Check the power supply and loop resistance are within spec. If they are, then replace the communication module."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "DAC Control Word Write Fault",
        "description": "An attempt to write to the DAC controlling the loop current has failed. The DAC or micro-controller chip on the communication module may be damaged.",
        "resolution": "Power cycle the device. If the problem persists, replace the communication module."
    },

    {
        "byte": 4,
        "bit": 7,
        "criticality": 0,
        "name": "Sensor Program Flow Failure",
        "description": "Sensor module is reporting corruption in the program code flow.",
        "resolution": "Power cycle the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 4,
        "bit": 6,
        "criticality": 0,
        "name": "Sensor ROM Failure",
        "description": "Sensor module is reporting corruption in the Read-only Memory (ROM)",
        "resolution": "Power cycle the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 4,
        "bit": 5,
        "criticality": 1,
        "name": "Sensor RAM Failure",
        "description": "Sensor module is reporting corruption in the Random Access Memory (RAM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 4,
        "bit": 4,
        "criticality": 1,
        "name": "Suspect Input",
        "description": "Reported pressure input data greatly exceeds measurable range and may be invalid. Could be a process problem, but it could also be a Meter Body problem.",
        "resolution": "Power cycle the device. Put transmitter in output mode and check transmitter status. Diagnostic messages should identify where problem is. If no other diagnostic message is given, condition is most likely meter body related. Check installation and replace the Meter Body."
    },
    {
        "byte": 4,
        "bit": 3,
        "criticality": 1,
        "name": "Device Variable Invalid",
        "description": "Pressure, Static Pressue or MeterBody Temperature measurement invalid.",
        "resolution": "If following a power cycle the failure recurs, then replace the Meter Body."
    },
    {
        "byte": 4,
        "bit": 2,
        "criticality": 1,
        "name": "Characterization Table CRC Fault",
        "description": "Sensor characterization is corrupt or there is a Sensor firmware failure.",
        "resolution": "Power cycle the device. If problem persists, then replace the Meter Body."
    },
    {
        "byte": 4,
        "bit": 1,
        "criticality": 1,
        "name": "Meter Body Failure",
        "description": "This status bit is set when any of the following conditions are present.\n\n  - Meter body failure\n  - Sensor communication timeout\n  - Sensor firmware flow failure",
        "resolution": "Power cycle the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 4,
        "bit": 0,
        "criticality": 1,
        "name": "Low Sensor Supply",
        "description": "This can be caused by low supply voltage from the communication module to the sensor module or by excessive loading by either module.",
        "resolution": "If possible, try to increase the voltage level of supply.Try replacing the communication module. If that does not fix the problem replace the Meter Body."
    },

    {
        "byte": 5,
        "bit": 7,
        "criticality": 1,
        "name": "Sensor RAM Database Fault",
        "description": "Sensor module is reporting corruption in the Random Access Memory (RAM).",
        "resolution": "Reset the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 5,
        "bit": 6,
        "criticality": 1,
        "name": "PV Out of Range",
        "description": "Calculated pressure is above Upper Transducer Limit (UTL).",
        "resolution": "Check Range and, if required, replace transmitter with one that has wider range.\n\n  - Meter Body may have been damaged. \n  - Check the transmitter for accuracy and linearity. Replace Meter Body and recalibrate if needed."
    },
    {
        "byte": 5,
        "bit": 5,
        "criticality": 1,
        "name": "TV Bad",
        "description": "The MeterBody Temperature measurement is far outside the specified range. The meter body may be damaged.",
        "resolution": "Power cycle the device. If problem persists, replace the Meterbody."
    },
    {
        "byte": 5,
        "bit": 4,
        "criticality": 1,
        "name": "SV Bad",
        "description": "The Static Pressure input measurement is far outside the specified range. The meter body may be damaged.",
        "resolution": "Power cycle the device. If problem persists, replace the Meterbody."
    },
    {
        "byte": 5,
        "bit": 3,
        "criticality": 0,
        "name": "PV Bad",
        "description": "The Differential Pressure input measurement is far outside the specified range. The meter body may be damaged.",
        "resolution": "Power cycle the device. If problem persists, replace the Meterbody."
    },
    {
        "byte": 5,
        "bit": 2,
        "criticality": 1,
        "name": "Characterization Calc Error",
        "description": "The redundant integrity check on the pressure calculation indicates a failure.",
        "resolution": "Power cycle the device. If the problem persists, replace the Meter Body.."
    },
    {
        "byte": 5,
        "bit": 1,
        "criticality": 1,
        "name": "Excess Span Correction",
        "description": "The SPAN correction factor is outside acceptable limits. INPUT may have been incorrect during calibration or transmitter was in output mode during a URV CORRECT procedure.",
        "resolution": "Check input pressure and be sure that it matches calibrated range values (LRV and URV). Check Meter Body. Perform an LRV or ZERO CORRECT procedure followed by a URV or SPAN CORRECT procedure."
    },
    {
        "byte": 5,
        "bit": 0,
        "criticality": 1,
        "name": "Excess Zero Correction",
        "description": "The ZERO correction factor is outside acceptable limits. INPUT may have been incorrect during calibration or transmitter was in output mode during an LRV or ZERO CORRECT procedure.",
        "resolution": "Check input pressure and be sure that it matches calibrated range values (LRV and URV). Check Meter Body. Perform an LRV or ZERO CORRECT procedure followed by a URV or SPAN CORRECT procedure."
    },

    {
        "byte": 6,
        "bit": 6,
        "criticality": 1,
        "name": "Display Common Config DB Corrupt",
        "description": "The communication module is reporting corruption in the Display common configuration portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem still persists then RAM might be damaged so need to replace the communication module.."
    },
    {
        "byte": 6,
        "bit": 5,
        "criticality": 1,
        "name": "Display View Config DB Corrupt",
        "description": "The communication module is reporting corruption in the Display screen cpnfiguration portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem still persists then RAM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 4,
        "criticality": 1,
        "name": "Advanced Diagnostics DB Corrupt",
        "description": "The communication module is reporting corruption in the advanced diagnostic portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem still persists then RAM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 3,
        "criticality": 1,
        "name": "Configuration Change DB Corrupt",
        "description": "The communication module is reporting corruption in the portion of the database in the Non-Volatile Memory (NVM) associated with the configuration changes.",
        "resolution": "Power cycle the device. If the problem still persists then the NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 2,
        "criticality": 0,
        "name": "General Configuration DB Corrupt",
        "description": "The communication module is reporting corruption in the general configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem still persists, then NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 1,
        "criticality": 0,
        "name": "Vital Configuration DB Corrupt",
        "description": "The communication module is reporting corruption in the vital configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem still persists, then NVM might be damaged so need to replace the communication module."
    },
    {
        "byte": 6,
        "bit": 0,
        "criticality": 0,
        "name": "Common DB Corrupt",
        "description": "The Electronics module is reporting corruption in the common parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem still persists then NVM might be damaged so need to replace the communication module."
    },

    {
        "byte": 7,
        "bit": 7,
        "criticality": 1,
        "name": "Display View 8 Corrupt",
        "description": "Display View 8 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 8."
    },
    {
        "byte": 7,
        "bit": 6,
        "criticality": 1,
        "name": "Display View 7 Corrupt",
        "description": "Display View 7 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 7."
    },
    {
        "byte": 7,
        "bit": 5,
        "criticality": 1,
        "name": "Display View 6 Corrupt",
        "description": "Display View 6 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 6."
    },
    {
        "byte": 7,
        "bit": 4,
        "criticality": 1,
        "name": "Display View 5 Corrupt",
        "description": "Display View 5 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 5."
    },
    {
        "byte": 7,
        "bit": 3,
        "criticality": 1,
        "name": "Display View 4 Corrupt",
        "description": "Display View 4 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 4."
    },
    {
        "byte": 7,
        "bit": 2,
        "criticality": 1,
        "name": "Display View 3 Corrupt",
        "description": "Display View 3 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 3."
    },
    {
        "byte": 7,
        "bit": 1,
        "criticality": 1,
        "name": "Display View 2 Corrupt",
        "description": "Display View 2 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 2."
    },
    {
        "byte": 7,
        "bit": 0,
        "criticality": 1,
        "name": "Display View 1 Corrupt",
        "description": "Display View 1 database block found corrupt.",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 1."
    }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ST800_ModuleTypes = { Name: , Abbreviation: }
///
/// This enum defines the name and abbreviation associated with each of the valid values of the ModuleType enumeration.
/// This enumeration contains the valid definitions for the different pressure module types in terms of the type of pressure measured. This
/// enumeration is based on Device Revision 5 of the HART sensor firmware and is expected to be applied to:
///     module_type_code  variable as read through device specific command 134
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var ST800_ModuleTypes =
{
    0: { name: "Differential Pressure", abbreviation: "DP" },
    1: { name: "Absolute Pressure",     abbreviation: "AP" },
    2: { name: "Gauge Pressure",        abbreviation: "GP" }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ST800_DeviceVariables
///
/// This array contains the names corresponding to the 3 possible device variables that the transmitter supports.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var ST800_DeviceVariables =
[
    { code: 0, classification: "Pressure",    name: "Pressure" },
    { code: 1, classification: "Temperature", name: "Meter Body Temperature" },
    { code: 2, classification: "Pressure",    name: "Static Pressure" }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ST800 Device Model
///
/// Initialize the global SL_Transmitter object to represent a ST800 transmitter. This global device model will be accessed by numerous global
/// functions to implement functionality that is common to all SmartLine transmitter types. It also provides storage for those display elements
/// that are not bound directly to a point parameter database item but rather made up of a number of components that can be updated asyncronously.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SL_Device.expectedModel = "ST800";
SL_Device.deviceVariables = ST800_DeviceVariables;

SL_Device.analogPV.classification = "Pressure";
SL_Device.analogPV.name = "Process Pressure";
SL_Device.analogPV.visibility = "visible";
SL_Device.analogPV.ignoreStatus = "false";

SL_Device.dynamicVariables.PV.classification = "Pressure";
SL_Device.dynamicVariables.PV.name = "Process Pressure";
SL_Device.dynamicVariables.PV.visibility = "visible";
SL_Device.dynamicVariables.PV.ignoreStatus = "true";

SL_Device.dynamicVariables.SV.classification = "Temperature";
SL_Device.dynamicVariables.SV.name = "Meter Body Temperature";
SL_Device.dynamicVariables.SV.visibility = "visible";
SL_Device.dynamicVariables.SV.ignoreStatus = "true";

SL_Device.dynamicVariables.TV.classification = "Pressure";
SL_Device.dynamicVariables.TV.name = "";
SL_Device.dynamicVariables.TV.visibility = "hidden";
SL_Device.dynamicVariables.TV.ignoreStatus = "true";

SL_Device.dynamicVariables.QV.name = "";
SL_Device.dynamicVariables.QV.visibility = "hidden";
SL_Device.dynamicVariables.QV.ignoreStatus = "true";

SL_Device.deviceRevisions = ST800_DeviceRevisions;

SL_Device.command48.numberOfBytes = ST800_DeviceRevisions[1].command48StatusBytes;
SL_Device.command48.statusBits = ST800_Cmd48StatusBits;

SL_Device.command220.numberOfBytes = ST800_DeviceRevisions[1].command220StatusBytes;
SL_Device.command220.statusBits = ST800_Cmd220StatusBits;

SL_Device.command220.statusBytes.length = SL_Device.command220.numberOfBytes;

ST800_PressureModule = new SL_PressureModule();
ST800_PressureModule.moduleTypes = ST800_ModuleTypes;

SL_Device.module = ST800_PressureModule;