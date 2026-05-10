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
/// \file  STT700.js
///
/// \brief Java script file to contain enumerations and functions that are specific to the STT700 transmitter.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// History:
///
/// EXP510.1-35		May   27, 2018		Developer Label D_EXP510.1-35.0-06
/// EXP510.1-34		May   21, 2018		Developer Label D_EXP510.1-34.0-12
/// [RLEAPFI-642]	April 30, 2018		Add support for building a STT700 specific device model.
/// EXP510.1-31		April 24, 2018		Initial Experion R510 testbed release
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_NumberOfStatusBytes = [{ Revision: , Cmd48: , Cmd220: }]
///
/// This array indicates the number of status bytes that have been defined for the array of device specific status bytes that are read through
/// the HART Universal Command 48 and the SmartLine Device Specific Command 220 for each of the released revisions of the transmitter firmware.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_NumberOfStatusBytes =
[
    { "Revision": 1, "Cmd48": 5, "Cmd220": 7 }
];

var STT700_DeviceRevisions =
{
    1: new SL_DeviceRevision(1, 5, 7)
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_Cmd48StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the STT700 transmitter
/// in the response to HART Universal Command 48. The location within the returned status byte array is defined by the combination of the byte
/// number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_Cmd48StatusBits =
[
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "Config Data Corrupt",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present:\n\n  - Common DB Corrupt\n  - Vital DB Corrupt\n  - General DB Corrupt",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "Char./Cal. Data Corrupt",
        "description": "Factory calibration data is corrupted or missing or factory data block is corrupted.",
        "resolution": "Reset the device. If the problem persists, replace the device."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 0,
        "name": "Sensor Input Failure",
        "description": "This is a roll-up status bit that is set when any of the following conditions are present:\n\n  - Common DB Corrupt\n  - Input 1 Fault\n  - Input 2 Fault\n  - Suspect Input\n  - ADC Reference voltage out of range\n  - Cold Junction temperature out of range\n  - Sensor measurement outside of Transducer Limits",
        "resolution": "For conditions 1 to 6, check sensor and connections. If the connections are ok, reset the device. If problem still persists, replace the device.\nFor condition 7, try to bring the process within the input limits, or use sensors that match the process ranges."
    },
    {
        "byte": 0,
        "bit": 1,
        "criticality": 0,
        "name": "DAC Failure",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present:\n\n  - DAC SPI Interface Failure\n  - DAC Packet Error\n  - DAC Over Current\n  - DAC Under Current\n  - DAC Temperature Above 140°C\n  - DAC Control Word Write Fault",
        "resolution": "Refer to the resolution for each condition present."
    },
    {
        "byte": 0,
        "bit": 0,
        "criticality": 0,
        "name": "SIL Diagnostic Failure",
        "description": "SIL Diagnostics failures have been detected. This is a roll-up status bit that is set when any of the following critical status conditions are present:\n\n  - RAM Failure\n  - ROM Failure\n  - Program Flow Failure",
        "resolution": "Reset the device. If the problem persists replace the device."
    },

    {
        "byte": 1,
        "bit": 6,
        "criticality": 1,
        "name": "Fixed Current Mode",
        "description": "Output current is fixed and not varying as per the process input. Loop current mode is disabled or Loop Test is active.",
        "resolution": "Enable loop current mode if it is disabled or exit the Loop Test mode if active."
    },
    {
        "byte": 1,
        "bit": 5,
        "criticality": 1,
        "name": "Sensor 1 Excess URV Correction",
        "description": "The difference between the device measured value and the HIGH CAL point for sensor 1 is within the +/- 1.5 to 3% of span range during user corrects.",
        "resolution": "To clear this status, perform a Reset Corrects for Sensor 1. Use the source such that the difference between the device measured value and the HIGH CAL point is less than +/- 1.5% of span."
    },
    {
        "byte": 1,
        "bit": 4,
        "criticality": 1,
        "name": "Sensor 1 Excess LRV Correction",
        "description": "The difference between the device measured value and the LOW CAL point for sensor 1 is within the +/- 1.5 to 3% of span range during user corrects.",
        "resolution": "To clear this status, perform a Reset Corrects for Sensor 1. Use the source such that the difference between the device measured value and the LOW CAL point is less than +/- 1.5% of span."
    },
    {
        "byte": 1,
        "bit": 3,
        "criticality": 1,
        "name": "CJ Temperature Out of Limits",
        "description": "The transmitter's Cold Junction temperature is outside the range of -40 to 85°C. Accuracy and life span may decrease if it remains out of limits.",
        "resolution": "Take steps to isolate the device from the temperature source. If the environmental conditions are met, and problem still persists, then replace the device."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 1,
        "name": "Primary Variable Out of Range",
        "description": "Loop PV is outside the configured span for the process (LRV to URV).",
        "resolution": "Check the sensor input wiring. Check the LRV, URV and, if required, change to a sensor type with a greater temperature range suitable to the process conditions and perform user corrections if needed."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "No Factory Calibration",
        "description": "Factory calibration is missing. Device will function with degraded accuracy.",
        "resolution": "If higher accuracy is desired, return the device for factory calibration."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 1,
        "name": "Core Temperature Out of Limits",
        "description": "The transmitter's core temperature is outside the range of -40°C to 125°C. Accuracy and life span may decrease if it remains out of limits.",
        "resolution": "Take steps to isolate the device from the temperature source. If the environmental conditions are met, and problem still persists, then replace the device."
    },

    {
        "byte": 2,
        "bit": 6,
        "criticality": 1,
        "name": "Analog Output Saturated",
        "description": "The calculated analog output is outside the 0 to 100% range based on the configured span (LRV to URV).",
        "resolution": "Verify that the LRV and URV values are configured properly for the desired process range."
    },
    {
        "byte": 2,
        "bit": 5,
        "criticality": 1,
        "name": "Input 2 Fault",
        "description": "Temperature Input 2 reading is bad.",
        "resolution": "Check that the temperature sensor is connected to the Input 2 terminals correctly. Check the wiring for any breaks. Check if the temperature sensor connected to Input 2 matches the configuration. If it does, check that the configured sensor range is right for the process. If it is, power cycle the device. If the issue persists, then replace the transmitter."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 1,
        "name": "Input 1 Fault",
        "description": "Temperature Input 1 reading is bad.",
        "resolution": "Check that the temperature sensor is connected to the Input 1 terminals correctly. Check the wiring for any breaks.. Check if the temperature sensor connected to Input 1 matches the configuration. If it does, check that the configured sensor range is right for the process. If it is, power cycle the device. If the issue persists, then replace the transmitter."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "No DAC Compensation",
        "description": "Device DAC compensation has not been performed. Loop accuracy may be slightly compromised. The effect will be minor degradation of ambient temperature influence specifications.",
        "resolution": "Replace the transmitter or return the device to factory for DAC compensation."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 1,
        "name": "Suspect Input",
        "description": "Reported temperature input data greatly exceeds measurable range and may be invalid. Could be a process problem, but it could also be a sensor or termination problem. ADC Voltages may be out of range or the Cold Junction temperature may be out of limit.",
        "resolution": "Power cycle the device. Put transmitter in output mode and check transmitter status. Check other diagnostic conditions like ADC voltages, Cold Junction temperature out of limits, etc. are present. If no other diagnostic conditions are present, condition is most likely sensor related. Check installation and power cycle the device. If the problem still persists, replace the device."
     },

     {
        "byte": 3,
        "bit": 7,
        "criticality": 1,
        "name": "Supply Voltage Fault",
        "description": "This bit is set when one of the supply voltages, DAC Loop or MCU, in the device is outside its specification limits.",
        "resolution": "Check that the power supply and loop resistance are within specification. If possible, try to increase the voltage level of the supply. If supply voltage and loop resistance are adequate and the problem persists, replace the device."
    },
    {
        "byte": 3,
        "bit": 6,
        "criticality": 1,
        "name": "Watchdog Reset",
        "description": "Watchdog reset. Device reset occured due to malfunction.",
        "resolution": "Reset the device. If issue still persists, repalce the device."
    },
    {
        "byte": 3,
        "bit": 5,
        "criticality": 1,
        "name": "Sensor 2 Input Out of Range",
        "description": "Sensor 2 input is under or over range. Process input is outside the configured Sensor 2 LRV and URV",
        "resolution": "Check the input 2 wiring. Check if LRV, URV are set based on the expected process. Also, check the process to see if it is within the expected range."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 1,
        "name": "Sensor 1 Input Out of Range",
        "description": "Sensor 1 input is under or over range. Process input is outside the configured Sensor 1 LRV and URV",
        "resolution": "Check the input 1 wiring. Check if LRV, URV are set based on the expected process. Also, check the process to see if it is within the expected range."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 1,
        "name": "Sensor 2 Excess URV Correction",
        "description": "The difference between the device measured value and the HIGH CAL point for sensor 2 is within the +/- 1.5 to 3% of span range during user corrects.",
        "resolution": "To clear this status, perform a Reset Corrects for Sensor 2. Use the source such that the difference between the device measured value and the HIGH CAL point is less than +/- 1.5% of span."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 1,
        "name": "Sensor 2 Excess LRV Correction",
        "description": "The difference between the device measured value and the LOW CAL point for sensor 2 is within the +/- 1.5 to 3% of span range during user corrects.",
        "resolution": "To clear this status, perform a Reset Corrects for Sensor 2. Use the source such that the difference between the device measured value and the LOW CAL point is less than +/- 1.5% of span."
    },
    {
        "byte": 3,
        "bit": 1,
        "criticality": 1,
        "name": "ADC Fault",
        "description": "Analog to Digital Converter reference voltages may be out of range.",
        "resolution": " Power cycle the device. If the problem still persists replace the device."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "Excess Delta Detected",
        "description": "When the Excess Delta Alarm is enabled and the device is using both input sensors to calculate the process temperature, this status indicates that the difference in Sensor 1 and Sensor 2 values exceeds the configured Delta Limit.",
        "resolution": "Make sure desired limit value for the Delta Temperature is correct. If the limit is correct, check Input 1 and Input 2 readings."
    },

    {
        "byte": 4,
        "bit": 0,
        "criticality": 1,
        "name": "SIL Diagnostics",
        "description": "SIL Diagnostics failures.",
        "resolution": "Reset the device. If issue persists, replace the device."
    }
 ];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_Cmd220StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the STT700 transmitter
/// in the response to SmartLine specific Command 220. The location within the returned status byte array is defined by the combination of the
/// byte number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_Cmd220StatusBits =
[
    {
        "byte": 0,
        "bit": 7,
        "criticality": 2,
        "name": "DAC SPI Interface Failure",
        "description": "Failure of the DAC SPI Interface.",
        "resolution": "Power cycle the device. Check that the power supply and loop resistance are within specifications. If they are, then replace the device."
    },
    {
        "byte": 0,
        "bit": 6,
        "criticality": 0,
        "name": "DAC Packet Error",
        "description": "The DAC or micro-controller chip on the device may be damaged",
        "resolution": "Power cycle the device. Check that the power supply and loop resistance are within specifications. If they are, then replace the device."
    },
    {
        "byte": 0,
        "bit": 5,
        "criticality": 0,
        "name": "DAC Over Current",
        "description": "The loop current value is greater than actual requested value. This might be due to some damaged component taking more current than expected.",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "DAC Under Current",
        "description": "The loop current value is below the actual requested value.",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "DAC Temperature Above 140°C",
        "description": "The internal temperature is dangerously high - above 140°C.",
        "resolution": "Verify that the environment is within the operating specification of the transmitter. If it is, then replace the device."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 1,
        "name": "DAC Temperature Above 100°C",
        "description": "The internal temperature is high - above 100°C.",
        "resolution": "Check the ambient temperature. If the ambient temperature is well below 100°C, check if there are other DAC faults reported by the device. Power cycle the device and if the problem persists replace the device."
    },
    {
        "byte": 0,
        "bit": 1,
        "criticality": 0,
        "name": "DAC Write Failure",
        "description": "DAC or micro-controller chip on device may be damaged.",
        "resolution": "Power cycle the device. If the problem persists replace the device."
    },

    {
        "byte": 1,
        "bit": 7,
        "criticality": 0,
        "name": "Calibration Table Fault",
        "description": "Factory calibration data is corrupted or missing.",
        "resolution": "Reset the device. If the problem persists replace the device."
    },
    {
        "byte": 1,
        "bit": 6,
        "criticality": 0,
        "name": "Characterization Table Fault",
        "description": "Factory characterization data block is corrupted.",
        "resolution": "Reset the device. If the problem persists replace the device."
    },
    {
        "byte": 1,
        "bit": 5,
        "criticality": 0,
        "name": "EEPROM Block Corrupt",
        "description": "EEPROM Block Corrupted.",
        "resolution": "Reset the device. If the problem persists replace the device."
    },

    {
        "byte": 1,
        "bit": 4,
        "criticality": 0,
        "name": "Stack Overflow",
        "description": "Stack Overflow detected in the device,",
        "resolution": "Reset the device. If the problem persists replace the device."
    },
    {
        "byte": 1,
        "bit": 3,
        "criticality": 0,
        "name": "Flow Control Fault",
        "description": "SIL diagnostic failure reported in the device.",
        "resolution": "Reset the device. If the problem persists replace the device."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 0,
        "name": "Flash/ROM Memory Corrupt",
        "description": "SIL diagnostic monitoring detected that the Flash memory or ROM is corrupted.",
        "resolution": "Reset the device. If the problem persists replace the device."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "RAM Database Corrupt",
        "description": "SIL diagnostic monitoring detected that the RAM is corrupted.",
        "resolution": "Reset the device. If the problem persists replace the device."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 0,
        "name": "RAM Corrupt",
        "description": "SIL diagnostic monitoring detected that the RAM is corrupted.",
        "resolution": "Reset the device. If the problem persists replace the device."
    },

    {
        "byte": 2,
        "bit": 7,
        "criticality": 0,
        "name": "Input 2 Short (RTD)",
        "description": "Input 2 terminals shorted. This is only applicable to RTD temperature sensors.",
        "resolution": "Check the sensor input 2 wiring; making sure it is connected to the right terminals and is secured."
    },
    {
        "byte": 2,
        "bit": 6,
        "criticality": 0,
        "name": "Input 2 Open",
        "description": "Input 2 terminals have an open circuit.",
        "resolution": "Check the sensor input 2 wiring; making sure it is connected to the right terminals and is secured."
    },
    {
        "byte": 2,
        "bit": 5,
        "criticality": 0,
        "name": "Input 1 Short (RTD)",
        "description": "Input 1 terminals shorted. This is only applicable to RTD temperature sensors.",
        "resolution": "Check the sensor input 1 wiring; making sure it is connected to the right terminals and is secured."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 0,
        "name": "Input 1 Open",
        "description": "Input 1 terminals have an open circuit.",
        "resolution": "Check the sensor input 1 wiring; making sure it is connected to the right terminals and is secured."
    },
    {
        "byte": 2,
        "bit": 3,
        "criticality": 0,
        "name": "Input 2 Under Range",
        "description": "Input 2 reading is under specified Range",
        "resolution": "Check if sensor input 2 ranges are correct for the process."
    },
    {
        "byte": 2,
        "bit": 2,
        "criticality": 0,
        "name": "Input 2 Over Range",
        "description": "Input 2 reading is over specified Range",
        "resolution": "Check if sensor input 2 ranges are correct for the process."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 0,
        "name": "Input 1 Under Range",
        "description": "Input 1 reading is under specified Range",
        "resolution": "Check if sensor input 1 ranges are correct for the process."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 0,
        "name": "Input 1 Over Range",
        "description": "Input 1 reading is over specified Range",
        "resolution": "Check if sensor input 1 ranges are correct for the process."
    },

    {
        "byte": 3,
        "bit": 6,
        "criticality": 1,
        "name": "Input 2 User Correct Active",
        "description": "User performed user corrections on Sensor 2.",
        "resolution": "This status indicates device user corrects are performed on the sensor 2. Power cycle will clear this status."
    },
    {
        "byte": 3,
        "bit": 5,
        "criticality": 1,
        "name": "Input 1 User Correct Active",
        "description": "User performed user corrections on sensor 1.",
        "resolution": "This status indicates device user corrects are performed on the sensor 1. Power cycle will clear this status."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 1,
        "name": "DAC Loop Voltage Failure",
        "description": "This bit is set when the DAC Loop supply voltage in the device is outside its specification limits.",
        "resolution": "Check that the power supply and loop resistance are within specification. If possible, try to increase the voltage level of the supply. If supply voltage and loop resistance are adequate and the problem persists, replace the device."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 1,
        "name": "MCU Transmitter Supply",
        "description": "This bit is set when the MCU supply voltage in the device is outside its specification limits.",
        "resolution": "Check that the power supply and loop resistance are within specification. If possible, try to increase the voltage level of the supply. If supply voltage and loop resistance are adequate and the problem persists, replace the device."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 0,
        "name": "CJ CT Delta Warning",
        "description": "The internal Cold Junction (CJ) and Processor Core (CT) temperatures differ by more than 10°C.",
        "resolution": "Verify that the environment is within specification. If it is, and if the problem persists, then replace the device."
    },
    {
        "byte": 3,
        "bit": 1,
        "criticality": 0,
        "name": "ADC Reference Fault",
        "description": "One of the sensor internal ADC reference voltages is bad.",
        "resolution": "Power cycle the device. If the problem persisits replace the device."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 0,
        "name": "ADC Range Fault",
        "description": "Input value supplied to the ADC is > ADC Range",
        "resolution": "Reset the device, replace the device if issue persists."
    },

    {
        "byte": 4,
        "bit": 2,
        "criticality": 0,
        "name": "User Correct Active",
        "description": "User performed user corrections on Sensor 1 or Sensor 2",
        "resolution": "This status indicates device user corrects are performed on the sensors. Power cycle will clear this status."
    },
    {
        "byte": 4,
        "bit": 1,
        "criticality": 0,
        "name": "Break Detected",
        "description": "When enabled, when Input open condition occurs (based on Loop control mode), loop current will be set to 21.5 mA. When disabled, when Input open condition occurs, loop current will be set to 20.8 mA.",
        "resolution": "To clear this status or if it is not desired to set the critical status on open inputs, set the Break Detect to Disabled."
    },
    {
        "byte": 4,
        "bit": 0,
        "criticality": 0,
        "name": "Latching Alarm Latched",
        "description": "This bit will get set when Latching Alarm Enabled. This will latch critical faults related to sensor inputs until power cycled.",
        "resolution": "To clear this status or if it is not desired to latch the sensor inputs related critical status, set the Latching Alarm to Disabled."
    },

    {
        "byte": 5,
        "bit": 7,
        "criticality": 0,
        "name": "Adv. Diag. DB Corrupt",
        "description": "The device is reporting corruption in the Advanced diagnostics portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 5,
        "bit": 6,
        "criticality": 0,
        "name": "Sensor 2 DB Corrupt",
        "description": "The device is reporting corruption in the Sensor 2 portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 5,
        "bit": 5,
        "criticality": 0,
        "name": "Sensor 1 DB Corrupt",
        "description": "The device is reporting corruption in the Sensor 1 portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 5,
        "bit": 4,
        "criticality": 0,
        "name": "Sensor common DB Corrupt",
        "description": "The device is reporting corruption in the Sensor common configuration portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 5,
        "bit": 3,
        "criticality": 0,
        "name": "Config Change DB Corrupt",
        "description": "The device is reporting corruption in the configuration change parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 5,
        "bit": 2,
        "criticality": 0,
        "name": "General Config DB Corrupt",
        "description": "The device is reporting corruption in the general configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device,"
    },
    {
        "byte": 5,
        "bit": 1,
        "criticality": 0,
        "name": "Vital Config DB Corrupt",
        "description": "The device is reporting corruption in the vital configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 5,
        "bit": 0,
        "criticality": 0,
        "name": "Common DB Corrupt",
        "description": "The device is reporting corruption in the common parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },

    {
        "byte": 6,
        "bit": 7,
        "criticality": 0,
        "name": "Adv. Diag. DB Write Failure",
        "description": "The device is reporting write failure in the Advanced diagnostics portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 6,
        "bit": 6,
        "criticality": 0,
        "name": "Sensor 2 DB Write Failure",
        "description": "The device is reporting write failure in the Sensor 2 portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 6,
        "bit": 5,
        "criticality": 0,
        "name": "Sensor 1 DB Write Failure",
        "description": "The device is reporting write failure in the Sensor 1  portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 6,
        "bit": 4,
        "criticality": 0,
        "name": "Sensor common DB Write Failure",
        "description": "The device is reporting write failure in the sensor common portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 6,
        "bit": 3,
        "criticality": 0,
        "name": "Config Change DB Write Failure",
        "description": "The device is reporting write failure in the configuration change parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 6,
        "bit": 2,
        "criticality": 0,
        "name": "General Config Write Failure",
        "description": "The device is reporting write failure in the general configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device,"
    },
    {
        "byte": 6,
        "bit": 1,
        "criticality": 0,
        "name": "Vital Config DB Write Failure",
        "description": "The device is reporting write failure in the vital configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    },
    {
        "byte": 6,
        "bit": 0,
        "criticality": 0,
        "name": "Common DB Write Failure",
        "description": "The device is reporting write failure in common parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the device."
    }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_InputSensorTypes = { Type: }
///
/// This enumeration contains the valid definitions for the style of temperatue sensing elements that can be connected to the STT700 temperature
/// transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     sensor1_type_code variable as read through device specific command 138
///     sensor2_type_code variable as read through device specific command 139
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_InputSensorTypes =
{
    0: { Type: "milliVolts" },
    1: { Type: "Thermocouple" },
    2: { Type: "RTD" },
    3: { Type: "Ohms" },
    4: { Type: "None" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_InputSensorIDs = { Type: }
///
/// This enumeration contains the valid definitions for the identification of the specific construction details of the temperatue sensing elements
/// that can be connected to the STT700 temperature transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is
/// expected to be applied to:
///     sensor1_id variable as read through device specific command 138
///     sensor2_id variable as read through device specific command 139
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_InputSensorIDs =
{
    0:    { Type: "milliVolts", Model: "22 mV" },
    1:    { Type: "milliVolts", Model: "125 mV" },
    2:    { Type: "milliVolts", Model: "1200 mV" },
    256:  { Type: "Thermocouple", Model: "Type E" },
    257:  { Type: "Thermocouple", Model: "Type J" },
    258:  { Type: "Thermocouple", Model: "Type K" },
    259:  { Type: "Thermocouple", Model: "Type N" },
    260:  { Type: "Thermocouple", Model: "Type T" },
    261:  { Type: "Thermocouple", Model: "Type S" },
    262:  { Type: "Thermocouple", Model: "Type R" },
    263:  { Type: "Thermocouple", Model: "Type B" },
    264:  { Type: "Thermocouple", Model: "Type C (W5W26)" },
    265:  { Type: "Thermocouple", Model: "Type L GOST" },
    512:  { Type: "Thermocouple", Model: "Pt 25, a=385" },
    513:  { Type: "RTD", Model: "Pt 100, a=385" },
    514:  { Type: "RTD", Model: "Pt 200, a=385" },
    515:  { Type: "RTD", Model: "Pt 500, a=385" },
    516:  { Type: "RTD", Model: "Pt 1000, a=385" },
    517:  { Type: "RTD", Model: "Pt 50, a=391" },
    518:  { Type: "RTD", Model: "Pt 100, a=391" },
    519:  { Type: "RTD", Model: "Ni 120, a=672" },
    520:  { Type: "RTD", Model: "Cu 10, a=427" },
    521:  { Type: "RTD", Model: "Cu 50, a=426" },
    522:  { Type: "RTD", Model: "Cu 50, a=428" },
    523:  { Type: "RTD", Model: "Cu 100, a=426" },
    524:  { Type: "RTD", Model: "Cu 100, a=428" },
    525:  { Type: "RTD", Model: "Cu 100, Roxspur" },
    768:  { Type: "Ohms", Model: "500 Ω" },
    769:  { Type: "Ohms", Model: "2K Ω" },
    770:  { Type: "Ohms", Model: "3K Ω" },
    1024: { Type: "None", Model: "None" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_InputSensorWires = { Wires: , Description: }
///
/// This enumeration contains the valid definitions for the number of wires associated with the RTD and Resistance style of temperatue sensing
/// elements that can be connected to the STT700 temperature transmitter. This enumeration is based on Device Revision 1 of the HART sensor
/// firmware and is expected to be applied to:
///     rtd1_type  variable as read through device specific command 138
///     rtd2_type  variable as read through device specific command 139
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_InputSensorWires =
{
    0: { Wires: 2, Description: "2 wire" },
    1: { Wires: 3, Description: "3 wire" },
    2: { Wires: 4, Description: "4 wire" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_ColdJunctionTemperatureSources = { Source: }
///
/// This enumeration contains the valid definitions for the source of the cold junction temperature input that can be used by the STT700
/// temperature transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     cj_selection variable as read through device specific command 146
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_ColdJunctionTemperatureSources =
{
    0: { Source: "Internal" },
    1: { Source: "External" },
    2: { Source: "Fixed Value" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_BreakDetectionModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of the Break Detection function that can be used by the STT700
/// temperature transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     break_detect_mode variable as read through device specific command 146
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_BreakDetectionModes =
{
    0: { Mode: "Disabled" },
    1: { Mode: "Enabled" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_PVCalculationModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of calculating the output PV Temperature in the STT700
/// temperature transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     loop_control_choice variable as read through device specific command 188
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_PVCalculationModes =
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
/// STT700_PVCalculationSources = { Source: }
///
/// This enumeration contains the valid definitions for the different input sensors that can be used as the source for the PV calculaition routine
/// in the STT700 temperature transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be
/// applied to:
///     contolling_var variable as read through device specific command 188
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_PVCalculationSources =
{
    0: { Source: "Sensors 1 & 2" },
    1: { Source: "Sensor 1" },
    2: { Source: "Sensor 2" },
    4: { Source: "Single Input" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_DeviceVariants = { Type: }
///
/// This enumeration contains the valid definitions for the different module types of single or dual inputs. This enumeration is based on
/// Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     device_variant variable as read through device specific command 188
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_DeviceVariants =
{
    1: { Type: "Single Input" },
    2: { Type: "Dual Input" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_ExcessDeltaDetectionModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of the Excess Delta temperature detection function that can be used by
/// the STT700 temperature transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     xs_delta_detection variable as read through device specific command 144
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_ExcessDeltaDetectionModes =
{
    0: { Mode: "Disabled" },
    1: { Mode: "Enabled" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700_DeviceVariables
///
/// This array contains the names corresponding to the 4 possible device variables that the transmitter supports.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var STT700_DeviceVariables =
[
    { code: 0, classification: "Temperature", name: "Process Temperature" },
    { code: 1, classification: "Temperature", name: "Cold Junction Temperature" },
    { code: 2, classification: "Temperature", name: "Sensor 1 Temperature" },
	{ code: 3, classification: "Temperature", name: "Sensor 2 Temperature" }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// STT700 Device Model
///
/// Initialize the global SL_Transmitter object to represent a STT700 transmitter. This global device model will be accessed by numerous global
/// functions to implement functionality that is common to all SmartLine transmitter types. It also provides storage for those display elements
/// that are not bound directly to a point parameter database item but rather made up of a number of components that can be updated asyncronously.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SL_Device.expectedModel = "STT700";
SL_Device.deviceVariables = STT700_DeviceVariables;

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

SL_Device.deviceRevisions = STT700_DeviceRevisions;

SL_Device.command48.numberOfBytes = STT700_DeviceRevisions[1].command48StatusBytes;
SL_Device.command48.statusBits = STT700_Cmd48StatusBits;

SL_Device.command220.numberOfBytes = STT700_DeviceRevisions[1].command220StatusBytes;
SL_Device.command220.statusBits = STT700_Cmd220StatusBits;

SL_Device.command220.statusBytes.length = SL_Device.command220.numberOfBytes;

STT700_TemperatureModule = new SL_TemperatureModule();
STT700_TemperatureModule.PvCalculationSources = STT700_PVCalculationSources;
STT700_TemperatureModule.inputSensorTypes = STT700_InputSensorTypes;
STT700_TemperatureModule.inputSensorIDs = STT700_InputSensorIDs;
STT700_TemperatureModule.inputSensorWires = STT700_InputSensorWires;

SL_Device.module = STT700_TemperatureModule;
