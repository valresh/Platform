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
/// \file  SMV800.js
///
/// \brief Java script file to contain enumerations and functions that are specific to the SMV800 transmitter.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// History:
///
/// EXP510.1-36     June   4, 2018      Developer Label D_EXP510.1-36.0-02
/// [RLEAPFI-509]   June   4, 2018      Added support for Device Revision 2
/// EXP510.1-35     May   27, 2018      Developer Label D_EXP510.1-35.0-06
/// EXP510.1-34     May   21, 2018      Developer Label D_EXP510.1-34.0-12
/// [RLEAPFI-642]   April 30, 2018      Add support for building a SMV800 specific device model.
/// EXP510.1-31     April 24, 2018      Initial Experion R510 testbed release
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_NumberOfStatusBytes = [{ Revision: , Cmd48: , Cmd220: }]
///
/// This array indicates the number of status bytes that have been defined for the array of device specific status bytes that are read through
/// the HART Universal Command 48 and the SmartLine Device Specific Command 220 for each of the released revisions of the transmitter firmware.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_NumberOfStatusBytes =
[
    { "Revision": 1, "Cmd48": 4, "Cmd220": 13 },
    { "Revision": 2, "Cmd48": 5, "Cmd220": 13 }
];

var SMV800_DeviceRevisions =
{
    1: new SL_DeviceRevision(1, 4, 13),
    2: new SL_DeviceRevision(2, 5, 13)
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_Cmd48StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the SMV800 transmitter
/// in the response to HART Universal Command 48. The location within the returned status byte array is defined by the combination of the byte
/// number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_Cmd48StatusBits =
[
    {
        "byte": 0,
        "bit": 4,
        "criticality": 0,
        "name": "Comm VCC Fault",
        "description": "This bit is set when the Communication module supply voltage goes outside the 2.8 to 3.2 V operating range.",
        "resolution": "The Communications module is not receiving the proper supply voltage from the Sensor module.  If this problem persists, try replacing the sensor module/Terminal Block assembly."
    },
    {
        "byte": 0,
        "bit": 3,
        "criticality": 0,
        "name": "Sensor Critical Failure",
        "description": "Failure in sensing section of Pressure or Temperature sensor module/Terminal Block Assembly.",
        "resolution": "This is a roll-up status bit that is set when any of the following critical status conditions are present:\nCorrupt NVM, ROM or RAM\nCommunication timeout\nCalibration or characterization data corrupt\nTemperature sensor input open, CJ issue, Corrupt Database,\nFlow Calculation failure due to bad Pressure, Temperature values, or invalid flow configurations.\nReset the device. If problem still persists, check if the environment is within spec. It it is, check if Temperature input wiring is correct and input is secured. Check if Flow configuration parameters set correctly. If all the conditions are ok, and problem still persists, replace the temperature sensor module\Terminal Block Assembly followed by Meter body."
    },
    {
        "byte": 0,
        "bit": 2,
        "criticality": 0,
        "name": "SIL Diagnostic Failure",
        "description": "SIL diagnostic failure reported in the communication module. This could be due to:\n\n  - RAM Failure\n  - ROM Failure\n  - Program Flow Failure",
        "resolution": "Power cycle the device. If the problem persists replace the Communication module."
    },
    {
        "byte": 0,
        "bit": 1,
        "criticality": 0,
        "name": "Configuration Data Corrupt",
        "description": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n\n  - Common DB Corrupt\n  - Vital DB Corrupt\n  - General DB Corrupt\n  - Totalizer Config DB Corrupt",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication Module."
    },
    {
        "byte": 0,
        "bit": 0,
        "criticality": 0,
        "name": "Communication Module DAC Failure",
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
        "description": "Factory calibration for the pressure sensor module or temperature sensor module/Terminal Block Assembly is missing. Accuracy will be compromised.",
        "resolution": "Replace the temprature sensor module/Terminal Block Assembly first. If status still persists, replace the pressure Meter body or return the device for factory calibration."
    },
    {
        "byte": 1,
        "bit": 5,
        "criticality": 1,
        "name": "Primary Variable Out of Range",
        "description": "The process input mapped as Primary Variable (PV) is outside of the specified range for that variable (PV LRV to PV URV).",
        "resolution": "Check that the configured range matches the expected process range and reconfigure the range if applicable.\n\nFor Pressure as Primary Variable, if the pressure reading is correct, the transmitter may need to be replaced with one that has a more suitable range. If not, the  Meter Body may have been damaged. Check the transmitter for accuracy and linearity. Replace MeterBody and recalibrate if needed.\n\nFor Temperature as Primary variable, Check the sensor input wiring. Check the LRV, URV and, if required, change to a sensor type with a greater temperature range suitable to the process conditions and perform user corrections if needed.\n\nFor Flow as Primary Variable, if the Pressure and Temperature ranges and inputs are all correct, check the Flow LRV and URV and set the ranges suitable for the Flow process."
    },
    {
        "byte": 1,
        "bit": 4,
        "criticality": 1,
        "name": "Fixed Current Mode",
        "description": "Output current is fixed and not varying as per the process input. Loop current mode is disabled or Loop Test mode is active.",
        "resolution": "Enable loop current mode if it is disabled or exit the Loop Test mode if active. Alternatively power cycle the device to exit from Loop Test mode."
    },
    {
        "byte": 1,
        "bit": 3,
        "criticality": 1,
        "name": "CJ Temperature Out of Limits",
        "description": "The temperature at the Cold Junction for a Thermocouple input sesnor is too high.",
        "resolution": " If the temperature is > 85°C and if cold junction compensation is Internal (range is -40°C to 85°C), accuracy and life span may decrease if internal temperature remains overrange.\n\nIf cold junction compensation is Fixed (range is -50°C to 85°C) and input exceeds this limit, PV value will be bad. As long as transmitter is isolated from the temperature source, there will not be any degradation. Take steps to isolate the device from the temperature source. If the environmental conditions are met, and problem still persists, then replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 1,
        "bit": 2,
        "criticality": 1,
        "name": "Sensor Non-critical Failure",
        "description": "Failure in pressure sensor module or temperature sensor module/Terminal Block Assembly.",
        "resolution": "Thispersists, replace the temperatuer sensor module/Terminal Block assembly."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "Comm Module Non-critical Failure",
        "description": "Communication Module communication section failure.",
        "resolution": "This is a roll-up status bit that is set when any of the following non-critical status conditions are present.\nAdvanced Diagnostic database Corrupt or database associated with the Configuration Changes is Corrupt.\nEnvironmental temperature above 100°C.\nPower cycle the device. If the problem still persists, verify the environment is within spec. If it is, then replace the Communication Module."
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
        "criticality": 0,
        "name": "Low Supply Voltage",
        "description": "The supply voltage to the transmitter terminals is too low (<14 volts), or the supply voltage to the Temperature Sensor Module/Terminal Block Assembly or to the Pressure Sensor Module is too low.",
        "resolution": "Check that the power supply and loop resistance are within specification. If possible, try to increase the voltage level of the supply. If supply voltage and loop resistance are adequate and the problem persists, replace the Communication Module. If the problem still persists, replace the Temperature Sensor Module/Terminal Block Assembly followed by Meter body."
    },
    {
        "byte": 2,
        "bit": 6,
        "criticality": 1,
        "name": "No DAC Calibration",
        "description": "No DAC calibration has been performed on the device.",
        "resolution": "If higher accuracy is desired, return the device to the factory for DAC calibration."
    },
    {
        "byte": 2,
        "bit": 5,
        "criticality": 0,
        "name": "Tamper Alarm",
        "description": "The Tamper Alarm is set if more than a specified number of attempts or actual configuration changes are made, when Tamper Alarm is enabled (Device can be either Write Protected or not). The warning stays active until the specified Tamper latency period has elapsed.",
        "resolution": "Make sure configuration changes are made by the authorized users. If needed, set the Tamper attempt to maximum value (10) or disable the Tamper alarm during setup stages of the transmitter to avoid alarm being setup frequently."
    },
    {
        "byte": 2,
        "bit": 4,
        "criticality": 0,
        "name": "Unreliable Sensor Communication",
        "description": "Internal communication quality between the Communication Module and Temperature Sensor Module is degrading or internal communication quality between the Communication Module and Pressure Sensor Module is degrading.",
        "resolution": "Transmitter might be installed in a noisy environment.\n1.Check proper grounding of the transmitter\n2.Secure the connection between Temperature Sensor Module/Terminal Block Assembly and the connector on the transmitter housing, and also between Communication Module and the connector on the housing.\n3.Check the connection between Communication Module and Meter body. Check especially for any bent pins in the connectors. When the connections are secured, power cycle the device. If problem persists replace the Communication Module followed by the Temperature Sensor Module/Terminal Block Assembly followed by Meter body."
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
        "resolution": "Verify that the LRV and URV values are configured properly for the desired process range for the variable that is mapped to analog output."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "URV Set Error - Span Config Button",
        "description": "Set operation using external Span button was rejected.",
        "resolution": "Please check whether the inputs are valid for the intended URV."
    },
    {
        "byte": 2,
        "bit": 0,
        "criticality": 1,
        "name": "LRV Set Error - Zero Config Button",
        "description": "Set operation using external Zero button was rejected.",
        "resolution": "Please check whether the inputs are valid for the intended LRV."
    },

    {
        "byte": 3,
        "bit": 7,
        "criticality": 1,
        "name": "Flow Calculation Error",
        "description": "During setup and configuration of the flow algorithm parameters, invalid parameter values may have been entered which are causing:\n1.divide by zero and/or Square root of a negative math error in the flow calculation. or 2.Flow calculation resulting in negative flow indicating reverse flow in the primary element.",
        "resolution": "For condition 1, carefully review the flow algorithm parameter values. Correct any errors.\nFor condition 2, set the Reverse Flow bit ON if Reverse Flow is expected.\nWhen the flow is showing a good value and this status is cleared, reset the device to clear any Critical Status that may have been generated due to any of the causes listed."
    },
    {
        "byte": 3,
        "bit": 6,
        "criticality": 1,
        "name": "Simulation Mode",
        "description": "Simulation mode(sim) is enabled for the Differential Pressure, Static Pressure, Process Temperature Input or Flow.",
        "resolution": "Simulation mode simplifies testing of flow calculations prior to online operation. While conducting testing, the status indicates that simulated input values are being used. When testing is completed, clear the sim mode for the inputs to return to true process measurement."
    },
    {
        "byte": 3,
        "bit": 5,
        "criticality": 1,
        "name": "Temperature Input Out of Range",
        "description": "Temperature Sensor input under or over range. Process input is outside the Temperature Sensor range limits (LRL and URL)",
        "resolution": "Check the input wiring and the Process temperature.  Check that the selected Temperature Sensor type has a range consistent with the process temperature range\n. If a different range is required, choose a different sensor type to measure the temperature."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 1,
        "name": "Low Power Mode",
        "description": "Indicates that Pressure or Temperature Sensor Module is operating in low power mode. This indicates that the transmitter is operating in a reduced accuracy mode due to a critical failure.",
        "resolution": "Identify the Critical status that is causing the sensor(s) to go to low power mode and follow the resolution provided."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 1,
        "name": "Temperature Sensor Input Open",
        "description": "Temperature Sensor Input Open.",
        "resolution": "Check the Temperature Sensor input wiring. Make sure input is connected and secure."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 1,
        "name": "Sensor Over Temperature",
        "description": "Specifies that the Temperature Sensor Module internal CPU temperature is going out of limits (Valid Range -40°C to 85°C) or Pressure Meter Body temperature is too high. Accuracy and life span may decrease if it remains high.",
        "resolution": "Make sure the environment is within specification. Take steps to insulate the device from temperature source. If the environment is within spec, replace the Temperature Sensor Module/Terminal Block Assembly followed by Meter body."
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "Totalizer at maximum value",
        "description": "The Totalizer count has reached the user configured Maximum Totalizer Value.",
        "resolution": "This status will stay set for the configured Totalizer Status Latency period, at which time it will be cleared."
    },

    {
        "byte": 4,
        "bit": 1,
        "criticality": 1,
        "name": "No Flow Output",
        "description": "The Flow Algorithm has been configured for “No Flow Output.",
        "resolution": "Verify that the Flow Algorithm Configuration is as expected."
    },
    {
        "byte": 4,
        "bit": 0,
        "criticality": 1,
        "name": "Totalizer stopped",
        "description": "The Flow Totalizer is mapped to PV and is controlling the Analog Output. The Totalization has been stopped and the Analog Output is not updating.",
        "resolution": "Verify all inputs to the Flow Totalizer algorithm."
    }

 ];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_Cmd220StatusBits = [{ Byte: , Bit: , Criticality: , Name: , Description: , Resolution: }]
///
/// This array indicates the position, name, criticality, cause and resolution information for each status bit defined for the SMV800 transmitter
/// in the response to SmartLine specific Command 220. The location within the returned status byte array is defined by the combination of the
/// byte number and bit number, both of which are indexed from 0.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_Cmd220StatusBits =
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
        "description": "The DAC or micro-controller chip on the Communication module may be damaged",
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
        "byte": 1,
        "bit": 5,
        "criticality": 1,
        "name": "Low Transmitter Supply",
        "description": "Supply voltage is low at transmitter terminals or supply to Temperature Sensor Module/Terminal Block Assembly or Communication Module is low.",
        "resolution": "This is a roll-up status bit that is set when any of the following critical status conditions are present.\n1.Low supply voltage.\n2.Low sensor supply voltage.\nFor condition 1, check the power supply and loop resistance are within spec. If they are, then replace the Communication Module.\nCondition 2 can occur with low supply voltage from the Communication Module or by loading by the Temperature Sensor Module/Terminal Block Assembly or Communication Module. Try to increase the voltage level of supply. If problem persists, try replacing the Communication Module. If the problem still persists, replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 1,
        "bit": 4,
        "criticality": 1,
        "name": "DAC Control Word Write Fault",
        "description": "An attempt to write to the DAC controlling the loop current has failed. The DAC or micro-controller chip on the communication board may be damaged.",
        "resolution": "Power cycle the device. If the problem persists, replace the communication module."
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
        "description": "SIL diagnostic failure reported in the Communication Module.",
        "resolution": "Reset the device. If problem persist, replace in the Communication Module."
    },
    {
        "byte": 1,
        "bit": 1,
        "criticality": 1,
        "name": "ROM Failure",
        "description": "SIL diagnostic failure reported in the Communication Module.",
        "resolution": "Reset the device. If problem persist, replace in the Communication Module."
    },
    {
        "byte": 1,
        "bit": 0,
        "criticality": 0,
        "name": "RAM Failure",
        "description": "SIL diagnostic failure reported in the Communication Module.",
        "resolution": "Reset the device. If problem persist, replace in the Communication Module."
    },

    {
        "byte": 2,
        "bit": 2,
        "criticality": 1,
        "name": "Display: Config Data Corrupt",
        "description": "Display configuration data has been corrupted.",
        "resolution": "Reset the device. Device will try to restore last saved configuration. If the user configuration is not restored, user will need to reconfigure display Screens."
    },
    {
        "byte": 2,
        "bit": 1,
        "criticality": 1,
        "name": "Display: Communication Timeout",
        "description": "Communication between the Communication Module and Display has failed.",
        "resolution": "Make sure display is securely connected to the Communication Module. If it is, then power cycle the device and recheck. If the problem persists, replace the Display. If problem still persists, replace the Communication Module."
    },

    {
        "byte": 3,
        "bit": 5,
        "criticality": 1,
        "name": "Temperature: Sensor Comm. Timeout",
        "description": "There is no communication between Communication Module and Temperature Sensor Module, or invalid data is encountered on communication line due to noise.",
        "resolution": "User can verify the connection from the Communication Module to the housing and from the Temperature Sensor Module/Terminal Block Assembly Mto the housing.  Especially check for any bent pins in the connectors. If cable/connector is not the problem, try replacing the Communication Module.\nIf it does not fix the problem, replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 3,
        "bit": 4,
        "criticality": 1,
        "name": "Temperature: Calibration Corrupt",
        "description": "Temperature Sensor Module calibration data is corrupted",
        "resolution": "Reset the device, replace the Temperature Sensor Module/Terminal Block Assembly if issue persists."
    },
    {
        "byte": 3,
        "bit": 3,
        "criticality": 1,
        "name": "Temperature: Sensing Failure",
        "description": "Failure in Temperature Sensor Module",
        "resolution": "Reset the device, replace the Temperature Sensor Module/Terminal Block Assembly if issue persists."
    },
    {
        "byte": 3,
        "bit": 2,
        "criticality": 1,
        "name": "Pressure: Sensor Comm. Timeout",
        "description": "There is no communication between Communication Module and Meter body.",
        "resolution": "User can verify the connection from the Communication Module to the Meter body. Especially check for any bent pins in the connectors. If cable/connector is not the problem, try replacing the Communication Module. If it does not fix the problem, replace the Meter body."
    },
    {
        "byte": 3,
        "bit": 1,
        "criticality": 1,
        "name": "Pressure: NVM Corrupt",
        "description": "Pressure sensor NVM corrupted.",
        "resolution": "Replace the Meter Body"
    },
    {
        "byte": 3,
        "bit": 0,
        "criticality": 1,
        "name": "Pressure: Sensing Failure",
        "description": "Failure in Pressure Sensor Module.",
        "resolution": "Reset the devive, replace the meter body if issue persists."
    },

    {
        "byte": 4,
        "bit": 6,
        "criticality": 1,
        "name": "Temperature: Sensor Overheated",
        "description": "Specifies that the temperature sensor internal CPU temperature is going out of limits. Valid Range (-40°C to 85°C).",
        "resolution": "Make sure the environment is within spec. If it is, replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 5,
        "criticality": 1,
        "name": "Temperature: No Factory Calibration",
        "description": "Temperature Sensor Module factory calibration is missing. Accuracy will be compromised.",
        "resolution": "Replace the Temperature Sensor Module/Terminal Block Assembly or return the device or Temperature Sensor Module/Terminal Block Assembly for factory calibration."
    },
    {
        "byte": 4,
        "bit": 4,
        "criticality": 1,
        "name": "Temperature: Unreliable Communication",
        "description": "Unreliable temperature sensor communication.",
        "resolution": " Either the transmitter is installed in a noisy environment or internal communication quality between the Communication Module and Temperature Sensor Module/Terminal Block Assembly is degrading. Check proper grounding of the transmitter. Secure the connection between Temperature Sensor Module/Terminal Block Assembly and the connector on the transmitter housing, and also between Communication Module and the connector on the housing.  Check especially for any bent pins in the connectors. When the connections are secured, power cycle the device. If problem persists replace the Communication Module followed by the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 3,
        "criticality": 1,
        "name": "Temperature: ADC Reference Fault",
        "description": "One of the temperature sensor's internal ADC reference voltages is bad.",
        "resolution": "Power cycle the device. If the problem persisits replace the temperature sensor module/Terminal Block Assembly."
    },
    {
        "byte": 4,
        "bit": 2,
        "criticality": 1,
        "name": "Temperature: ADC1 Range Fault",
        "description": "Input value supplied to the ADC1 is > ADC Range",
        "resolution": "Reset the device, replace the temperature sensor module/Terminal Block Assembly if issue persists."
    },
    {
        "byte": 4,
        "bit": 1,
        "criticality": 1,
        "name": "Temperature: ADC0 Range Fault",
        "description": "Input value supplied to the ADC0 is > ADC Range",
        "resolution": "Reset the device, replace the temperature sensor module/Terminal Block Assembly if issue persists."
    },
    {
        "byte": 4,
        "bit": 0,
        "criticality": 1,
        "name": "Temperature: CJ/CT Delta Warning",
        "description": "The internal Cold Junction (CJ) temperature and the Processor Core (CT) temperature difference in the temperature module is > 10°C.",
        "resolution": "Verify environment is within spec. If it is, and if the problem persists, then replace the Temperature Sensor Module/Terminal Block Assembly."
    },

    {
        "byte": 5,
        "bit": 7,
        "criticality": 1,
        "name": "Temperature: Program Flow Failure",
        "description": "SIL diagnostic failure reported in the Temperature Sensor Module.",
        "resolution": "Reset the device. If problem persist, replace in the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 6,
        "criticality": 1,
        "name": "Temperature: ROM Failure",
        "description": "SIL diagnostic failure reported in the Temperature Sensor Module.",
        "resolution": "Reset the device. If problem persist, replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 5,
        "criticality": 1,
        "name": "Temperature: RAM Failure",
        "description": "SIL diagnostic failure reported in the Temperature Sensor Module.",
        "resolution": "Reset the device. If problem persist, replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 4,
        "criticality": 1,
        "name": "Temperature: Suspect Input",
        "description": "Reported temperature input data greatly exceeds measurable range and may be invalid. Could be a process problem, but it could also be a Temperature Sensor Module problem: ADC Voltages may be out of range or CJ out of limit.",
        "resolution": "Power cycle the device. Put transmitter in output mode and check transmitter status. Check other diagnostic conditions like ADC voltages, CJ out of limits etc. are present. If no other diagnostic conditions are present, condition is most likely Temperature Sensor Module related. Check installation and power cycle the device. If the problem still persists, replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 3,
        "criticality": 1,
        "name": "Temperature: Process or CJ Temperature Bad",
        "description": "Temperature sensor input or Cold Junction value is bad",
        "resolution": "Check if the temperature input connected is as expected. If it is, check the sensor ranges configured are right for the process. Check if the CJ valus is within range or not. If the values are within range, power cycle the device. If the problem persists, then replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 2,
        "criticality": 1,
        "name": "Temperature: Characterization Table Fault",
        "description": "Temperature sensor characterization is corrupt or there is a Sensor firmware failure.",
        "resolution": "If following a power cycle the failure recurs, then replace the temperature sensor module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 1,
        "criticality": 1,
        "name": "Temperature: NVM corrupt",
        "description": "Temperature sensor NVM database is corrupted.",
        "resolution": "If following a power cycle the failure recurs, then replace the temperature sensor module/Terminal Block Assembly."
    },
    {
        "byte": 5,
        "bit": 0,
        "criticality": 1,
        "name": "Temperature: Low Sensor Supply",
        "description": "Temperature Sensor Module voltages may be outside the valid limits.",
        "resolution": "If possible try to increase the voltage level of supply. Power cycle the device and recheck. If problem persists, replace the Temperature Sensor Module/Terminal Block Assembly."
    },

    {
        "byte": 6,
        "bit": 6,
        "criticality": 1,
        "name": "Temperature: Input Fault",
        "description": "Temperature sensor input reading is bad due to input open/improper wiring, faulty sensor or out of range conditions.",
        "resolution": "Check the temperature sensor input wiring. If connections are ok, check the sensor range configured is right for the process. If it is, power cycle the device. If the issue persists, then replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 6,
        "bit": 5,
        "criticality": 1,
        "name": "Temperature: Cold Junction Fault",
        "description": "Temperature Sensor Module Cold Junction value is bad.",
        "resolution": "Verify the reported Cold Junction temperature against the actual value. If value is outside the CJ range, power cycle the device. If the issue persists, then replace the Temperature Sensor Module/Terminal Block Assembly."
    },
    {
        "byte": 6,
        "bit": 3,
        "criticality": 1,
        "name": "Temperature: Input Value Bad",
        "description": "Input value is bad.",
        "resolution": "Check if the sensor connection to input is as expected. If it is, check the sensor range configured is right for the process. If it is, power cycle the device. If the issue persists, then replace the sensor module/Terminal Block Assembly."
    },
    {
        "byte": 6,
        "bit": 2,
        "criticality": 1,
        "name": "Temperature: Characterization Error",
        "description": "Temperature Sensor Module characterization is corrupt or there is a Sensor firmware failure.",
        "resolution": "If following a power cycle the failure recurs, then replace the Temperature Sensor Module/Terminal Block Assembly"
    },
    {
        "byte": 6,
        "bit": 0,
        "criticality": 1,
        "name": "Temperature: Excess Correction",
        "description": "This status indicates that the difference between the device measured value to the temperature calibration point (Low CAL point or High CAL point) is within the +/- 1.5 to 3% of span during user corrects.",
        "resolution": "To clear this status, perform a Reset Corrects for temperature Sensor. Use the source such that the difference between the device measured value to the temperature calibration point (Low CAL point or High CAL point) is less than +/- 1.5% of span."
    },

    {
        "byte": 7,
        "bit": 7,
        "criticality": 1,
        "name": "Pressure: Program Flow Failure",
        "description": "SIL diagnostic failure reported in the Pressure Sensor Module.",
        "resolution": "Reset the device. If problem persist, replace in the Meter body."
    },
    {
        "byte": 7,
        "bit": 6,
        "criticality": 1,
        "name": "Pressure: ROM Failure",
        "description": "SIL diagnostic failure reported in the Pressure Sensor Module.",
        "resolution": "Reset the device. If problem persist, replace the Meter body."
    },
    {
        "byte": 7,
        "bit": 5,
        "criticality": 1,
        "name": "Pressure: RAM Failure",
        "description": "Sensor module is reporting corruption in the Random Access Memory (RAM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 7,
        "bit": 4,
        "criticality": 1,
        "name": "Pressure: Suspect Input",
        "description": "Reported Differential pressure, Static pressure input or Meter body temperature data greatly exceeds measurable range and may be invalid. Could be a process problem, but it could also be a Meter body problem.",
        "resolution": "Power cycle the device. Put transmitter in output mode and check transmitter status. If the problem still persists, replace the Meter body."
    },
    {
        "byte": 7,
        "bit": 3,
        "criticality": 1,
        "name": "DP/MBT/SP/PT/Flow Bad",
        "description": "One of the process inputs to the device and/or the flow calculation has failed.",
        "resolution": "If Bad DP/MBT/SP/PT statuses set, that could be the reason for Bad Flow. Check those statuses and follow the resolutions provided.\nIf the other process mesurements are ok, Bad Flow may be a result of an invalid algorithm configuration. Correct the configuration parameters, power cycle the device to clear any latched statuses, and recheck the calculated flow. If a Flow Control Fault is set, reset the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 7,
        "bit": 2,
        "criticality": 1,
        "name": "Pressure: Characterization Table Fault",
        "description": "Pressure Sensor Module characterization is corrupt or there is a Sensor firmware failure.",
        "resolution": "If following a power cycle the failure recurs, then replace the Meter body."
    },
    {
        "byte": 7,
        "bit": 1,
        "criticality": 1,
        "name": "Pressure: Meter Body Failure",
        "description": "Pressure Sensor Module is reporting a critical failure of the pressure sensing measurement within the Meter Body, which may be caused by one of the following:\n1.Meter body failure\2.Sensor communication timeout\n3.Sensor firmware flow failure",
        "resolution": "Reset the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 7,
        "bit": 0,
        "criticality": 1,
        "name": "Pressure: Low Sensor Supply",
        "description": "Pressure sensor Module voltage may be outside the valid limits.",
        "resolution": "If possible try to increase the voltage level of supply. Power cycle the device and recheck. If problem persists, replace the Meter body."
    },

    {
        "byte": 8,
        "bit": 7,
        "criticality": 0,
        "name": "Pressure: Sensor Overheated",
        "description": "The Meter Body temperature is too high. Accuracy and life span may decrease if it remains high.",
        "resolution": "Verify the environmental temperature is within specification. Take steps to insulate the Meter body from the temperature source."
    },
    {
        "byte": 8,
        "bit": 6,
        "criticality": 0,
        "name": "Pressure: Ureliable Communication",
        "description": "Unreliable communication from pressure sensor to the Communication Module.",
        "resolution": " Either the transmitter is installed in a noisy environment or internal communication quality between the Communication Module and Pressure Sensor is degrading. Check proper grounding of the transmitter. Secure the connection between pressure sensor and the Communication Module. Check especially for any bent pins in the connectors. When the connections are secured, power cycle the device. If problem persists replace the Communication Module followed by the Meter body."
    },
    {
        "byte": 8,
        "bit": 5,
        "criticality": 0,
        "name": "Pressure: No Factory Calibration",
        "description": "Pressure Sensor factory calibration is missing. Accuracy will be compromised.",
        "resolution": "Replace the Meter body or return the device or Meter body for factory calibration."
    },
    {
        "byte": 8,
        "bit": 4,
        "criticality": 0,
        "name": "Pressure: RAM Database Fault",
        "description": "Pressure Sensor RAM database corrupt.",
        "resolution": "Power cycle the device. If the problem persists replace the Meter body."
    },
    {
        "byte": 8,
        "bit": 3,
        "criticality": 0,
        "name": "Pressure: Sensor Overload",
        "description": "The Meter Body is sensing Differential or Static pressure > the spec limit of Differential or Static pressure Upper Range Limit(URL).",
        "resolution": "Check that the process inputs are within spec for the Diff and Static Pressure for this device input range. Correct the excessive pressure input. If higher pressures are required, a higher range device type may be required. Meter Body may have been damaged."
    },
    {
        "byte": 8,
        "bit": 2,
        "criticality": 1,
        "name": "Pressure: Characterization Calc Error",
        "description": "Pressure Sensor characterization is corrupt or there is a Sensor firmware failure.",
        "resolution": "If following a power cycle the failure recurs, then replace the Meter body."
    },
    {
        "byte": 8,
        "bit": 1,
        "criticality": 0,
        "name": "Pressure: Excess Span Correction",
        "description": "URV correct is excessive for given input.",
        "resolution": "Perform reset corrects on DP, SP. Repeat pressure calibration at the proper calibration points (DP LRV, DP URV, SP LRV, SP URV) if needed."
    },
    {
        "byte": 8,
        "bit": 0,
        "criticality": 1,
        "name": "Pressure: Excess Zero Correction",
        "description": "Zero calibration, LRV correct is excessive for given input.",
        "resolution": "Perform reset corrects on DP, SP. Repeat pressure calibration at the proper calibration points (DP LRV, DP URV, SP LRV, SP URV)  if needed."
    },

    {
        "byte": 9,
        "bit": 5,
        "criticality": 1,
        "name": "Pressure: Bad Totalizer Flow",
        "description": "The Flow Totalizer calculation has failed. Possible causes are:\n1.Bad DP/SP/MBT/PT input.\n2.Invalid flow algorithm configuration.\n3.Firmware flow control fault.",
        "resolution": "If Bad DP/MBT/SP/PT statuses set, that could be the reason for Bad Flow. Check those statuses and follow the resolutions provided.\nIf the other process measurements are ok, Bad Flow may be a result of an invalid algorithm configuration. Correct the configuration parameters, power cycle the device to clear any latched stauses, and recheck the calculated flow. If a Flow Control Fault is set, reset the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 9,
        "bit": 4,
        "criticality": 1,
        "name": "Pressure: Bad Flow",
        "description": "The Flow calculation has failed. Possible causes are:\n1.Bad DP/SP/MBT/PT input.\n2.Invalid flow algorithm configuration.\n3.Firmware flow control fault.",
        "resolution": "If Bad DP/MBT/SP/PT statuses set, that could be the reason for Bad Flow. Check those statuses and follow the resolutions provided.\nIf the other process measurements are ok, Bad Flow may be a result of an invalid algorithm configuration. Correct the configuration parameters, power cycle the device to clear any latched stauses, and recheck the calculated flow. If a Flow Control Fault is set, reset the device. If the problem persists, replace the Meter Body."
    },
    {
        "byte": 9,
        "bit": 3,
        "criticality": 1,
        "name": "Pressure: Bad Process Temperature",
        "description": "The Process Temperature input measurement is far outside the specified range. The Temperature Sensor Module may be damaged.",
        "resolution": "Reset the device, replace the Temperature Sensor Module/Terminal Block Assembly if issue persists."
    },
    {
        "byte": 9,
        "bit": 2,
        "criticality": 1,
        "name": "Pressure: Bad Static Pressure",
        "description": "The Static Pressure input measurement is far outside the specified range. The meter body may be damaged.",
        "resolution": "Reset the device, replace the Meter Body if issue persists."
    },
    {
        "byte": 9,
        "bit": 1,
        "criticality": 1,
        "name": "Pressure: Bad Meter Body Temperature",
        "description": "The Meter body Temperature measurement is far outside the specified range. The meter body may be damaged.",
        "resolution": "Reset the device, replace the Meter Body if issue persists."
    },
    {
        "byte": 9,
        "bit": 0,
        "criticality": 1,
        "name": "Pressure: Bad Differential Pressure",
        "description": "The Differential Pressure input measurement is far outside the specified range. The meter body may be damaged.",
        "resolution": "Reset the device, replace the Meter Body if issue persists."
    },

    {
        "byte": 10,
        "bit": 6,
        "criticality": 0,
        "name": "Display Common Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the Display common configuration portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 10,
        "bit": 5,
        "criticality": 0,
        "name": "Display Screen Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the Display screen configuration portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 10,
        "bit": 4,
        "criticality": 0,
        "name": "Adv Diag DB Corrupt",
        "description": "The Communication module is reporting corruption in the advanced diagnostic portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 10,
        "bit": 3,
        "criticality": 0,
        "name": "Config Change DB Corrupt",
        "description": "The Communication module is reporting corruption in the configuration change parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 10,
        "bit": 2,
        "criticality": 0,
        "name": "General Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the general configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module,"
    },
    {
        "byte": 10,
        "bit": 1,
        "criticality": 0,
        "name": "Vital Config DB Corrupt",
        "description": "The Communication module is reporting corruption in the vital configuration parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },
    {
        "byte": 10,
        "bit": 0,
        "criticality": 0,
        "name": "Common DB Corrupt",
        "description": "The Communication module is reporting corruption in the common parameters portion of the database in the Non-Volatile Memory (NVM).",
        "resolution": "Power cycle the device. If the problem persists, replace the Communication module."
    },

    {
        "byte": 11,
        "bit": 7,
        "criticality": 0,
        "name": "Display Screen 8 Corrupt",
        "description": "Display View 8 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 8."
    },
    {
        "byte": 11,
        "bit": 6,
        "criticality": 0,
        "name": "Display Screen 7 Corrupt",
        "description": "Display View 7 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 7."
    },
    {
        "byte": 11,
        "bit": 5,
        "criticality": 0,
        "name": "Display Screen 6 Corrupt",
        "description": "Display View 6 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 6."
    },
    {
        "byte": 11,
        "bit": 4,
        "criticality": 0,
        "name": "Display Screen 5 Corrupt",
        "description": "Display View 5 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 5."
    },
    {
        "byte": 11,
        "bit": 3,
        "criticality": 0,
        "name": "Display Screen 4 Corrupt",
        "description": "Display View 4 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 4."
    },
    {
        "byte": 11,
        "bit": 2,
        "criticality": 0,
        "name": "Display Screen 3 Corrupt",
        "description": "Display View 3 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 3."
    },
    {
        "byte": 11,
        "bit": 1,
        "criticality": 0,
        "name": "Display Screen 2 Corrupt",
        "description": "Display View 1 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 2."
    },
    {
        "byte": 11,
        "bit": 0,
        "criticality": 0,
        "name": "Display Screen 1 Corrupt",
        "description": "Display View 1 database block found corrupt",
        "resolution": "Power cycle the device. User will need to reconfigure Display View 1."
    },

    {
        "byte": 12,
        "bit": 7,
        "criticality": 0,
        "name": "Flow: Simulating Flow",
        "description": "Simulation mode is enabled for the Process Flow. Simulation mode simplifies testing of flow calculations prior to online operation.",
        "resolution": "While conducting testing, the status indicates that simulation is being used. When testing is completed, clear the simulation mode for the inputs to return to true process measurement."
    },
    {
        "byte": 12,
        "bit": 6,
        "criticality": 0,
        "name": "Flow: Simulating Process Temperature",
        "description": "Simulation mode is enabled for the Process Temperature input. Simulation mode simplifies testing of flow calculations prior to online operation.",
        "resolution": "While conducting testing, the status indicates that simulation is being used. When testing is completed, clear the simulation mode for the inputs to return to true process measurement."
    },
    {
        "byte": 12,
        "bit": 5,
        "criticality": 0,
        "name": "Flow: Simulating Static Pressure",
        "description": "Simulation mode is enabled for the Static Pressure input. Simulation mode simplifies testing of flow calculations prior to online operation.",
        "resolution": "While conducting testing, the status indicates that simulation is being used. When testing is completed, clear the simulation mode for the inputs to return to true process measurement."
    },
    {
        "byte": 12,
        "bit": 4,
        "criticality": 0,
        "name": "Flow: Simulating Differential Pressure",
        "description": "Simulation mode is enabled for the Differential Pressure input. Simulation mode simplifies testing of flow calculations prior to online operation.",
        "resolution": "While conducting testing, the status indicates that simulation is being used. When testing is completed, clear the simulation mode for the inputs to return to true process measurement."
    },
    {
        "byte": 12,
        "bit": 3,
        "criticality": 0,
        "name": "Flow: Bad SP/PT Compensation",
        "description": "Static Pressure and/or Process Temperature input has failed such that these inputs to the flow calculation are undetermined.\nWith AP/PT Compensation setting configured ON and AP/PT Failsafe setting configured OFF, the status flag indicates that flow calculation is using configured nominal or design value for the failed input/s. With AP/PT Compensation setting configured ON and AP/PT failsafe setting configured ON, the flow calculation will fail when Static Pressure and/or Temperature input fails, and a Critical Status will be generated. This flag is applicable only for ASME 1981 algorithms, Standard equations.",
        "resolution": "This is a status to indicate that AP/PT Compensation settings are ON, Static Pressure and or Temperature input/s failed and alternate values are being used for flow calculation. Check which of the inputs failed and follow the suggested resolutions on those input failures. If it is desired that alternate values be used for any special process conditions, then no actions required."
    },
    {
        "byte": 12,
        "bit": 2,
        "criticality": 0,
        "name": "Flow: Reverse Flow",
        "description": "The flow calculation is producing a negative flow value indicating that the flow is reversed in the element.",
        "resolution": "This is just an indication of Reverse flow and the status will clear automatically when the flow goes in the forward direction. If reverse flow is expected, the Reverse Flow Calculation option must be selected in the Flow Setup, otherwise any reverse flow detected will produce a flow value of zero."
    },
    {
        "byte": 12,
        "bit": 1,
        "criticality": 0,
        "name": "Flow: Square Root of Negative Number",
        "description": "During setup and configuration of the flow algorithm parameters, invalid parameter values have been entered which are causing a square root of negative math error in the flow calculation.",
        "resolution": "Carefully review the flow algorithm parameter values. Correct any errors."
    },
    {
        "byte": 12,
        "bit": 0,
        "criticality": 0,
        "name": "Flow: Divide by Zero",
        "description": "During setup and configuration of the flow algorithm parameters, invalid parameter values have been entered which are causing a division by zero math error in the flow calculation.",
        "resolution": "Carefully review the flow algorithm parameter values. Correct any errors."
    }
];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_InputSensorTypes = { Type: }
///
/// This enumeration contains the valid definitions for the style of temperatue sensing elements that can be connected to the SMV800 Multivariable
/// transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     sensor1_type_code variable as read through device specific command 136
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_InputSensorTypes =
{
    0: { Type: "milliVolts" },
    1: { Type: "Thermocouple" },
    2: { Type: "RTD" },
    3: { Type: "Ohms" },
    4: { Type: "None" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_InputSensorIDs = { Type: }
///
/// This enumeration contains the valid definitions for the identification of the specific construction details of the temperatue sensing elements
/// that can be connected to the SMV800 Multivariable transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is
/// expected to be applied to:
///     sensor1_id variable as read through device specific command 136
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_InputSensorIDs =
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
    10: { Type: "RTD", Model: "Pt 25, a=385" },
    11: { Type: "RTD", Model: "Pt 100, a=385" },
    12: { Type: "RTD", Model: "Pt 200, a=385" },
    13: { Type: "RTD", Model: "Pt 500, a=385" },
    14: { Type: "RTD", Model: "Pt 1000, a=385" },
    15: { Type: "Ohms", Model: "500 Ω" },
    16: { Type: "Ohms", Model: "2K Ω" },
    17: { Type: "Ohms", Model: "3K Ω" },
    31: { Type: "None", Model: "None" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_InputSensorWires = { Wires: , Description: }
///
/// This enumeration contains the valid definitions for the number of wires associated with the RTD and Resistance style of temperatue sensing
/// elements that can be connected to the SMV800 Multivariable transmitter. This enumeration is based on Device Revision 4 of the HART sensor
/// firmware and is expected to be applied to:
///     rtd1_type  variable as read through device specific command 136
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_InputSensorWires =
{
    0: { Wires: 2, Description: "2 wire" },
    1: { Wires: 3, Description: "3 wire" },
    2: { Wires: 4, Description: "4 wire" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_ColdJunctionTemperatureSources = { Source: }
///
/// This enumeration contains the valid definitions for the source of the cold junction temperature input that can be used by the SMV800
/// Multivariable transmitter. This enumeration is based on Device Revision 4 of the HART sensor firmware and is expected to be applied to:
///     cj_selection variable as read through device specific command 135
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_ColdJunctionTemperatureSources =
{
    0: { Source: "Internal" },
    1: { Source: "External" },
    2: { Source: "Fixed Value" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_BreakDetectionModes = { Mode: }
///
/// This enumeration contains the valid definitions for the different modes of the Break Detection function that can be used by the SMV800
/// Multivariable transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     break_detect_mode variable as read through device specific command 135
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_BreakDetectionModes =
{
    0: { Mode: "Disabled" },
    1: { Mode: "Enabled" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_StaticPressureTypes = { Type: }
///
/// This enumeration contains the valid definitions for the different static pressure type options available when ordering the SMV800 multivariable
/// transmitter. This enumeration is based on Device Revision 14 of the HART sensor firmware and is expected to be
/// applied to:
///     Third character of key_number variable as read through device specific command 139
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_StaticPressureTypes =
{
    "A": { Type: "Absolute (psia)" },
    "G": { Type: "Gauge (psig)" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_LocalDisplayTypes = { Mode: }
///
/// This enumeration contains the valid definitions for the different local display types that can be connected to the SMV800 Multivariable
/// transmitter. This enumeration is based on Device Revision 1 of the HART sensor firmware and is expected to be applied to:
///     display_type_code  variable as read through device specific command 134
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_LocalDisplayTypes =
{
    0: { Mode: "None" },
    1: { Mode: "Advanced" },
    2: { Mode: "Basic" },
    3: { Mode: "None" }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800_DeviceVariables
///
/// This array contains the names corresponding to the 5 possible device variables that the transmitter supports.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var SMV800_DeviceVariables =
[
    { code: 0, classification: "Pressure",        name: "Differential Pressure" },
    { code: 1, classification: "Pressure",        name: "Static Pressure" },
    { code: 2, classification: "Temperature",     name: "Process Temperature" },
    { code: 3, classification: "Volumetric Flow", name: "Flow Rate" },
    { code: 4, classification: "Temperature",     name: "Meter Body Temperature" },
    { code: 5, classification: "Volumetric Flow", name: "Totalizer Flow" }
];



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SMV800 Device Model
///
/// Initialize the global SL_Transmitter object to represent a SMV800 transmitter. This global device model will be accessed by numerous global
/// functions to implement functionality that is common to all SmartLine transmitter types. It also provides storage for those display elements
/// that are not bound directly to a point parameter database item but rather made up of a number of components that can be updated asyncronously.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SL_Device.expectedModel = "SMV800";
SL_Device.deviceVariables = SMV800_DeviceVariables;

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

SL_Device.deviceRevisions = SMV800_DeviceRevisions;

SL_Device.command48.numberOfBytes = SMV800_DeviceRevisions[1].command48StatusBytes;
SL_Device.command48.statusBits = SMV800_Cmd48StatusBits;

SL_Device.command220.numberOfBytes = SMV800_DeviceRevisions[1].command220StatusBytes;
SL_Device.command220.statusBits = SMV800_Cmd220StatusBits;

SL_Device.command220.statusBytes.length = SL_Device.command220.numberOfBytes;

SL_Device.modelNumberParts.push(new SL_ScriptData("key_number"));
SL_Device.modelNumberParts.push(new SL_ScriptData("table_I"));
SL_Device.modelNumberParts.push(new SL_ScriptData("table_II"));
SL_Device.modelNumberParts.push(new SL_ScriptData("table_III"));
SL_Device.modelNumberParts.push(new SL_ScriptData("table_IV"));

SMV800_MutivariableModule = new SL_MultivariableModule();
SMV800_MutivariableModule.staticPressureTypes = SMV800_StaticPressureTypes;
SMV800_MutivariableModule.ColdJunctionTemperatureSources = SMV800_ColdJunctionTemperatureSources;
SMV800_MutivariableModule.inputSensorTypes = SMV800_InputSensorTypes;
SMV800_MutivariableModule.inputSensorIDs = SMV800_InputSensorIDs;
SMV800_MutivariableModule.inputSensorWires = SMV800_InputSensorWires;

SL_Device.module = SMV800_MutivariableModule;