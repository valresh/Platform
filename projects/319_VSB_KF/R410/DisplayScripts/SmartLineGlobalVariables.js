//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \copyright
/// COPYRIGHT (c) 2017
/// HONEYWELL INTERNATIONAL INC.
/// ALL RIGHTS RESERVED
///
/// Legal rights of Honeywell International Inc. in this software is distinct from ownership of any medium in which the software is embodied.
/// Copyright notices must be reproduced in any copies authorized by Honeywell International Inc.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file  SmartLineGlobalVariables.js
///
/// \brief
/// Java script file to contain variables that are shared between the Faceplate and Point Detail displays for all of the Honeywell
/// SmartLine family of HART transmitters. To avoid name conflicts, all variable names in this file should use the following identifiers:
///     "SL_"       generic function that applies to both the Point Detail and Faceplate displays for all SmartLine families
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// History:
///
/// EXP510.1-36     June   4, 2018      Developer Label D_EXP510.1-36.0-02
/// EXP510.1-36     June   4, 2018      Added the enumeration for the HART Variables Scan Configuration. Removed commented out code.
/// [RLEAPFI-520]   June   4, 2018      Modified the symbols for the units containing "metric tons".
/// EXP510.1-35		May   27, 2018		Developer Label D_EXP510.1-35.0-06
/// EXP510.1-34		May   21, 2018		Developer Label D_EXP510.1-34.0-12
/// EXP510.1-31		April 24, 2018		Initial Experion R510 testbed release
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

var SL_LogDebugInformation = 0;
var SL_DebugLevel = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  Global enumerations for HART 7 devices
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// HART_DeviceVariableFamilies
///
/// <summary>
///	This object maps the standard HART Device Variable Family codes, which are compatible with HART 7 implementations, to the corresponding
/// physical property or functionality. These codes are defined in Table 20 Device Variable Family Codes of the HART Common
/// Tables Specification HCF_SPEEC-183, Revision 23.0 dated 29 May 2013.
///
/// The enumeraation is coded as an object with child objects with the following properties:
///     code:           The numeric value (decimal) assigned to the family by the HART foundation.
///     family:         A text sting identifying the physical property or functionality of the family.
/// </summary>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var HART_DeviceVariableFamilies =
{
    4:   { code: 4,   family: "Temperature" },
    5:   { code: 5,   family: "Pressure" },
    6:   { code: 6,   family: "Valve or Actuator" },
    7:   { code: 7,   family: "Simple PID Control" },
    8:   { code: 8,   family: "pH" },
    9:   { code: 9,   family: "Conductivity" },
    10:  { code: 10,  family: "Totalizer" },
    11:  { code: 11,  family: "Level" },
    12:  { code: 12,  family: "Vortex Flow" },
    13:  { code: 13,  family: "Mag Flow" },
    14:  { code: 14,  family: "Coriolis Flow" },
    250: { code: 250, family: "Not Used" }
};
  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// HART_DeviceVariableClassifications
///
/// <summary>
///	This array maps the standard HART Device Variable Classification codes, which are compatible with HART 7 implementations, to the
/// corresponding physical property that can be used to index the expanded unit codes table. These codes are defined in Table 21 Device
/// Variable Classification Codes of the HART Common Tables Specification HCF_SPEEC-183, Revision 23.0 dated 29 May 2013.
///
/// The enumeraation is coded as an object with child objects with the following properties:
///     code:           The numeric value (decimal) assigned to the classification by the HART foundation.
///     property:       A text sting identifying the physical property of the classification.
/// </summary>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var HART_DeviceVariableClassifications =
{
    0:   { code: 0,   classification: "Not Classified" },
    64:  { code: 64,  classification: "Temperature" },
    65:  { code: 65,  classification: "Pressure" },
    66:  { code: 66,  classification: "Volumetric Flow" },
    67:  { code: 67,  classification: "Velocity" },
    68:  { code: 68,  classification: "Volume" },
    69:  { code: 69,  classification: "Length" },
    70:  { code: 70,  classification: "Time" },
    71:  { code: 71,  classification: "Mass" },
    72:  { code: 72,  classification: "Mass Flow" },
    73:  { code: 73,  classification: "Mass per Volume" },
    74:  { code: 74,  classification: "Viscosity" },
    75:  { code: 75,  classification: "Angular Velocity" },
    76:  { code: 76,  classification: "Area" },
    77:  { code: 77,  classification: "Energy" },
    78:  { code: 78,  classification: "Force" },
    79:  { code: 79,  classification: "Power" },
    80:  { code: 80,  classification: "Frequency" },
    81:  { code: 81,  classification: "Analytical" },
    82:  { code: 82,  classification: "Capacitance" },
    83:  { code: 83,  classification: "Voltage" },
    84:  { code: 84,  classification: "Current" },
    85:  { code: 85,  classification: "Resistance" },
    86:  { code: 86,  classification: "Angle" },
    87:  { code: 87,  classification: "Conductance" },
    88:  { code: 88,  classification: "Volume per Volume" },
    89:  { code: 89,  classification: "Volume per Mass" },
    90:  { code: 90,  classification: "Concentration" },
    91:  { code: 91,  classification: "Reserved" },
    92:  { code: 92,  classification: "Reserved" },
    93:  { code: 93,  classification: "Reserved" },
    94:  { code: 94,  classificationv: "Reserved" },
    95:  { code: 95,  classification: "Reserved" },
    96:  { code: 96,  classification: "Acceleration" },
    97:  { code: 97,  classification: "Turbidity" },
    98:  { code: 98,  classification: "Temperature Difference" },
    99:  { code: 99,  classification: "Volumetric Gas Flow per Second" },
    100: { code: 100, classification: "Volumetric Gas Flow per Minute" },
    101: { code: 101, classification: "Volumetric Gas Flow per Hour" },
    102: { code: 102, classification: "Volumetric Gas Flow per Day" },
    103: { code: 103, classification: "Volumetric Liquid Flow per Second" },
    104: { code: 104, classification: "Volumetric Liquid Flow per Minute" },
    105: { code: 105, classification: "Volumetric Liquid Flow per Hour" },
    106: { code: 106, classification: "Volumetric Liquid Flow per Day" },
    107: { code: 107, classification: "Thermal Expansion" },
    108: { code: 108, classification: "Volumetric Energy Density" }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// HART_StandardUnitsCodes
///
/// <summary>
///	This array maps the standard HART unit codes, which are compatible with HART 5 implementations, to the corresponding
/// physical property, name and symbol. These codes are defined in Table 2 Engineering Unit Codes of the HART Common
/// Tables Specification HCF_SPEEC-183, Revision 23.0 dated 29 May 2013.
///
/// The enumeraation is coded as an array of objects with the following properties:
///     code:           The numeric value (decimal) assigned to the unit by the HART foundation.
///     property:       A text sting identifying the physical property that the unit applies to; such as Pressure.
///     name:           The full description of the unit without any abreviations or symbols.
///     symbol:         The full unit symbol including any qualifying elements such as Standard Conditions or Country.
///     shortSymbol:    Shorten version of the full symbol without the qualifying elements such as Standard Conditions or Country.
/// </summary>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var HART_StandardUnitCodes =
[
    { code: 1,   classification: "Pressure",          name: "inches of water at 68 degrees Fahrenheit",      symbol: "inH2O @ 68°F",       shortSymbol: "inH2O" },
    { code: 2,   classification: "Pressure",          name: "inches of mercury at 0 degrees Celsius",        symbol: "inHg @ 0°C",         shortSymbol: "inHg" },
    { code: 3,   classification: "Pressure",          name: "feet of water at 68 degrees Fahrenheit",        symbol: "ftH2O @ 68°F",       shortSymbol: "ftH2O" },
    { code: 4,   classification: "Pressure",          name: "millimeters of water at 68 degrees Fahrenheit", symbol: "mmH2O @ 68°F",       shortSymbol: "mmH2O" },
    { code: 5,   classification: "Pressure",          name: "millimeters of mercury at 0 degrees Celsius",   symbol: "mmHg @ 0°C",         shortSymbol: "mmHg" },
    { code: 6,   classification: "Pressure",          name: "pounds per square inch",                        symbol: "psi",                shortSymbol: "" },
    { code: 7,   classification: "Pressure",          name: "bars",                                          symbol: "bar",                shortSymbol: "" },
    { code: 8,   classification: "Pressure",          name: "millibars",                                     symbol: "mbar",               shortSymbol: "" },
    { code: 9,   classification: "Pressure",          name: "grams per square centimeter",                   symbol: "g/cm²",              shortSymbol: "" },
    { code: 10,  classification: "Pressure",          name: "kilograms per square centimeter",               symbol: "kg/cm²",             shortSymbol: "" },
    { code: 11,  classification: "Pressure",          name: "pascals",                                       symbol: "Pa",                 shortSymbol: "" },
    { code: 12,  classification: "Pressure",          name: "kilopascals",                                   symbol: "kPa",                shortSymbol: "" },
    { code: 13,  classification: "Pressure",          name: "torrs",                                         symbol: "torr",               shortSymbol: "" },
    { code: 14,  classification: "Pressure",          name: "atmospheres",                                   symbol: "atm",                shortSymbol: "" },
    { code: 15,  classification: "Volumetric Flow",   name: "cubic feet per minute",                         symbol: "ft³/min",            shortSymbol: "" },
    { code: 16,  classification: "Volumetric Flow",   name: "US liquid gallons per minute",                  symbol: "gal (US liq)/min",   shortSymbol: "gal/min" },
    { code: 17,  classification: "Volumetric Flow",   name: "liters per minute",                             symbol: "l/min",              shortSymbol: "" },
    { code: 18,  classification: "Volumetric Flow",   name: "Imperial gallons per minute",                   symbol: "gal (imp)/min",      shortSymbol: "gal/min" },
    { code: 19,  classification: "Volumetric Flow",   name: "cubic meters per hour",                         symbol: "m³/h",               shortSymbol: ""  },
    { code: 20,  classification: "Velocity",          name: "feet per second",                               symbol: "ft/s",               shortSymbol: "" },
    { code: 21,  classification: "Velocity",          name: "meters per second",                             symbol: "m/s",                shortSymbol: "" },
    { code: 22,  classification: "Volumetric Flow",   name: "US liquid gallons per second",                  symbol: "gal (US liq)/s",     shortSymbol: "gal/s" },
    { code: 23,  classification: "Volumetric Flow",   name: "million US liquid gallons per day",             symbol: "10⁶ gal (US liq)/d", shortSymbol: "10⁶ gal/d" },
    { code: 24,  classification: "Volumetric Flow",   name: "liters per second",                             symbol: "l/s",                shortSymbol: "" },
    { code: 25,  classification: "Volumetric Flow",   name: "megaliters per day",                            symbol: "Ml/d",               shortSymbol: "" },
    { code: 26,  classification: "Volumetric Flow",   name: "cubic feet per second",                         symbol: "ft³/s",              shortSymbol: "" },
    { code: 27,  classification: "Volumetric Flow",   name: "cubic feet per day",                            symbol: "ft³/d",              shortSymbol: "" },
    { code: 28,  classification: "Volumetric Flow",   name: "cubic meters per second",                       symbol: "m³/s",               shortSymbol: "" },
    { code: 29,  classification: "Volumetric Flow",   name: "cubic meters per day",                          symbol: "m³/d",               shortSymbol: "" },
    { code: 30,  classification: "Volumetric Flow",   name: "Imperial gallons per hour",                     symbol: "gal (imp)/h",        shortSymbol: "gal/h" },
    { code: 31,  classification: "Volumetric Flow",   name: "Imperial gallons per day",                      symbol: "gal (imp)/d",        shortSymbol: "gal/d" },
    { code: 32,  classification: "Temperature",       name: "degrees Celsius",                               symbol: "°C",                 shortSymbol: "" },
    { code: 33,  classification: "Temperature",       name: "degrees Fahrenheit",                            symbol: "°F",                 shortSymbol: "" },
    { code: 34,  classification: "Temperature",       name: "degrees Rankine",                               symbol: "°R",                 shortSymbol: "" },
    { code: 35,  classification: "Temperature",       name: "Kelvin",                                        symbol: "K",                  shortSymbol: "" },
    { code: 36,  classification: "Voltage",           name: "millivolts",                                    symbol: "mV",                 shortSymbol: "" },
    { code: 37,  classification: "Resistance",        name: "ohms",                                          symbol: "Ω",                  shortSymbol: "" },
    { code: 38,  classification: "Frequency",         name: "hertz",                                         symbol: "Hz",                 shortSymbol: "" },
    { code: 39,  classification: "Current",           name: "milliamperes",                                  symbol: "mA",                 shortSymbol: "" },
    { code: 40,  classification: "Volume",            name: "US gallons",                                    symbol: "gal (US)",           shortSymbol: "gal" },
    { code: 41,  classification: "Volume",            name: "liters",                                        symbol: "l",                  shortSymbol: "" },
    { code: 42,  classification: "Volume",            name: "Imperial gallons",                              symbol: "gal (imp)",          shortSymbol: "gal" },
    { code: 43,  classification: "Volume",            name: "cubic meters",                                  symbol: "m³",                 shortSymbol: "" },
    { code: 44,  classification: "Length",            name: "feet",                                          symbol: "ft",                 shortSymbol: "" },
    { code: 45,  classification: "Length",            name: "meters",                                        symbol: "m",                  shortSymbol: "" },
    { code: 46,  classification: "Volume",            name: "US oil barrels",                                symbol: "bbl (US oil)",       shortSymbol: "bbl" },
    { code: 47,  classification: "Length",            name: "inches",                                        symbol: "in",                 shortSymbol: "" },
    { code: 48,  classification: "Length",            name: "centimeters",                                   symbol: "cm",                 shortSymbol: "" },
    { code: 49,  classification: "Length",            name: "millimeters",                                   symbol: "mm",                 shortSymbol: "" },
    { code: 50,  classification: "Time",              name: "minutes",                                       symbol: "min",                shortSymbol: "" },
    { code: 51,  classification: "Time",              name: "seconds",                                       symbol: "s",                  shortSymbol: "" },
    { code: 52,  classification: "Time",              name: "hours",                                         symbol: "h",                  shortSymbol: "" },
    { code: 53,  classification: "Time",              name: "days",                                          symbol: "d",                  shortSymbol: "" },
    { code: 54,  classification: "Viscosity",         name: "centistokes",                                   symbol: "cSt",                shortSymbol: "" },
    { code: 55,  classification: "Viscosity",         name: "centipoise",                                    symbol: "cP",                 shortSymbol: "" },
    { code: 56,  classification: "Conductance",       name: "microsiemens",                                  symbol: "µS",                 shortSymbol: "" },
    { code: 57,  classification: "Analytical",        name: "percent",                                       symbol: "%",                  shortSymbol: "" },
    { code: 58,  classification: "Voltage",           name: "volts",                                         symbol: "V",                  shortSymbol: "" },
    { code: 59,  classification: "Analytical",        name: "potential of hydrogen",                         symbol: "pH",                 shortSymbol: "" },
    { code: 60,  classification: "Mass",              name: "grams",                                         symbol: "g",                  shortSymbol: "" },
    { code: 61,  classification: "Mass",              name: "kilograms",                                     symbol: "kg",                 shortSymbol: "" },
    { code: 62,  classification: "Mass",              name: "metric tons",                                   symbol: "t",                  shortSymbol: "" },
    { code: 63,  classification: "Mass",              name: "pounds",                                        symbol: "lb",                 shortSymbol: "" },
    { code: 64,  classification: "Mass",              name: "short tons",                                    symbol: "t (sht)",            shortSymbol: "t" },
    { code: 65,  classification: "Mass",              name: "long tons",                                     symbol: "t (lng)",            shortSymbol: "t" },
    { code: 66,  classification: "Conductance",       name: "millisiemens per centimeter",                   symbol: "mS/cm",              shortSymbol: "" },
    { code: 67,  classification: "Conductance",       name: "microsiemens per centimeter",                   symbol: "µS/cm",              shortSymbol: "" },
    { code: 68,  classification: "Force",             name: "newtons",                                       symbol: "N",                  shortSymbol: "" },
    { code: 69,  classification: "Energy",            name: "joules",                                        symbol: "J",                  shortSymbol: "" },
    { code: 70,  classification: "Mass Flow",         name: "grams per second",                              symbol: "g/s",                shortSymbol: "" },
    { code: 71,  classification: "Mass Flow",         name: "grams per minute",                              symbol: "g/min",              shortSymbol: "" },
    { code: 72,  classification: "Mass Flow",         name: "grams per hour",                                symbol: "g/h",                shortSymbol: "" },
    { code: 73,  classification: "Mass Flow",         name: "kilograms per second",                          symbol: "kg/s",               shortSymbol: "" },
    { code: 74,  classification: "Mass Flow",         name: "kilograms per minute",                          symbol: "kg/min",             shortSymbol: "" },
    { code: 75,  classification: "Mass Flow",         name: "kilograms per hour",                            symbol: "kg/h",               shortSymbol: "" },
    { code: 76,  classification: "Mass Flow",         name: "kilograms per day",                             symbol: "kg/d",               shortSymbol: "" },
    { code: 77,  classification: "Mass Flow",         name: "metric tons per minute",                        symbol: "t (metric)/min",     shortSymbol: "t/min" },
    { code: 78,  classification: "Mass Flow",         name: "metric tons per hour",                          symbol: "t (metric)/h",       shortSymbol: "t/h" },
    { code: 79,  classification: "Mass Flow",         name: "metric tons per day",                           symbol: "t (metric)/d",       shortSymbol: "t/d" },
    { code: 80,  classification: "Mass Flow",         name: "pounds per second",                             symbol: "lb/s",               shortSymbol: "" },
    { code: 81,  classification: "Mass Flow",         name: "pounds per minute",                             symbol: "lb/min",             shortSymbol: "" },
    { code: 82,  classification: "Mass Flow",         name: "pounds per hour",                               symbol: "lb/h",               shortSymbol: "" },
    { code: 83,  classification: "Mass Flow",         name: "pounds per day",                                symbol: "lb/d",               shortSymbol: "" },
    { code: 84,  classification: "Mass Flow",         name: "short tons per minute",                         symbol: "t (sht)/min",        shortSymbol: "t/min" },
    { code: 85,  classification: "Mass Flow",         name: "short tons per hour",                           symbol: "t (sht)/h",          shortSymbol: "t/h" },
    { code: 86,  classification: "Mass Flow",         name: "short tons per day",                            symbol: "t (sht)/d",          shortSymbol: "t/d" },
    { code: 87,  classification: "Mass Flow",         name: "long tons per hour",                            symbol: "t (lng)/h",          shortSymbol: "t/h" },
    { code: 88,  classification: "Mass Flow",         name: "long tons per day",                             symbol: "t (lng)/d",          shortSymbol: "t/d" },
    { code: 89,  classification: "Energy",            name: "deka therm",                                    symbol: "dth",                shortSymbol: "" },
    { code: 90,  classification: "Mass Per Volume",   name: "specific gravity units",                        symbol: "SG",                 shortSymbol: "" },
    { code: 91,  classification: "Mass Per Volume",   name: "grams per cubic centimeter",                    symbol: "g/cm³",              shortSymbol: "" },
    { code: 92,  classification: "Mass Per Volume",   name: "kilograms per cubic meter",                     symbol: "kg/m³",              shortSymbol: "" },
    { code: 93,  classification: "Mass Per Volume",   name: "pounds per US gallon",                          symbol: "lb/gal (US)",        shortSymbol: "lb/gal" },
    { code: 94,  classification: "Mass Per Volume",   name: "pounds per cubic foot",                         symbol: "lb/ft³",             shortSymbol: "" },
    { code: 95,  classification: "Mass Per Volume",   name: "grams per milliliter",                          symbol: "g/ml",               shortSymbol: "" },
    { code: 96,  classification: "Mass Per Volume",   name: "kilograms per liter",                           symbol: "kg/l",               shortSymbol: "" },
    { code: 97,  classification: "Mass Per Volume",   name: "grams per liter",                               symbol: "g/l",                shortSymbol: "" },
    { code: 98,  classification: "Mass Per Volume",   name: "pounds per cubic inch",                         symbol: "lb/in³",             shortSymbol: "" },
    { code: 99,  classification: "Mass Per Volume",   name: "short tons per cubic yd",                       symbol: "t (sht)/yd³",        shortSymbol: "t/yd³" },
    { code: 100, classification: "Mass Per Volume",   name: "degrees twaddell",                              symbol: "°Tw",                shortSymbol: "" },
    { code: 101, classification: "Concentration",     name: "degrees brix",                                  symbol: "°Bx",                shortSymbol: "" },
    { code: 102, classification: "Mass Per Volume",   name: "degrees baumé heavy",                           symbol: "°Bé (heavy)",        shortSymbol: "°Bé" },
    { code: 103, classification: "Mass Per Volume",   name: "degrees baumé light",                           symbol: "°Bé (light)",        shortSymbol: "°Bé" },
    { code: 104, classification: "Mass Per Volume",   name: "degrees API",                                   symbol: "°API",               shortSymbol: "" },
    { code: 105, classification: "Concentration",     name: "percent solids per weight",                     symbol: "%",                  shortSymbol: "" },
    { code: 106, classification: "Concentration",     name: "percent solids per volume",                     symbol: "%",                  shortSymbol: "" },
    { code: 107, classification: "Volume Per Mass",   name: "degrees balling",                               symbol: "°B",                 shortSymbol: "" },
    { code: 108, classification: "Concentration",     name: "proof per volume",                              symbol: "ABV",                shortSymbol: "" },
    { code: 109, classification: "Concentration",     name: "proof per mass",                                symbol: "ABW",                shortSymbol: "" },
    { code: 110, classification: "Volume",            name: "US bushels",                                    symbol: "bsh (US)",           shortSymbol: "bsh" },
    { code: 111, classification: "Volume",            name: "cubic yards",                                   symbol: "yd³",                shortSymbol: "" },
    { code: 112, classification: "Volume",            name: "cubic feet",                                    symbol: "ft³",                shortSymbol: "" },
    { code: 113, classification: "Volume",            name: "cubic inches",                                  symbol: "in³",                shortSymbol: "" },
    { code: 114, classification: "Velocity",          name: "inches per second",                             symbol: "in/s",               shortSymbol: "" },
    { code: 115, classification: "Velocity",          name: "inches per minute",                             symbol: "in/min",             shortSymbol: "" },
    { code: 116, classification: "Velocity",          name: "feet per minute",                               symbol: "ft/min",             shortSymbol: "" },
    { code: 117, classification: "Angular Velocity",  name: "degrees per second",                            symbol: "°/s",                shortSymbol: "" },
    { code: 118, classification: "Angular Velocity",  name: "revolutions per second",                        symbol: "rps",                shortSymbol: "" },
    { code: 119, classification: "Angular Velocity",  name: "revolutions per minute",                        symbol: "rpm",                shortSymbol: "" },
    { code: 120, classification: "Velocity",          name: "meters per hour",                               symbol: "m/h",                shortSymbol: "" },
    { code: 121, classification: "Volumetric Flow",   name: "normal cubic meters per hour",                  symbol: "m³/h @ 0°C, 1 atm",  shortSymbol: "m³/h" },
    { code: 122, classification: "Volumetric Flow",   name: "normal liters per hour",                        symbol: "l/h  @ 0°C, 1 atm",  shortSymbol: "l/h" },
    { code: 123, classification: "Volumetric Flow",   name: "standard cubic feet per minute",                symbol: "scf/min",            shortSymbol: "" },
    { code: 124, classification: "Volume",            name: "US liquid barrels",                             symbol: "bbl (US liq)",       shortSymbol: "bbl" },
    { code: 125, classification: "Mass",              name: "ounce",                                         symbol: "oz",                 shortSymbol: "" },
    { code: 126, classification: "Energy",            name: "foot pound force",                              symbol: "ft·lbf",             shortSymbol: "" },
    { code: 127, classification: "Power",             name: "kilowatt",                                      symbol: "kW",                 shortSymbol: "" },
    { code: 128, classification: "Energy",            name: "kilowatt hour",                                 symbol: "kW·h",               shortSymbol: "" },
    { code: 129, classification: "Power",             name: "horsepower",                                    symbol: "hp",                 shortSymbol: "" },
    { code: 130, classification: "Volumetric Flow",   name: "cubic feet per hour",                           symbol: "ft³/h",              shortSymbol: "" },
    { code: 131, classification: "Volumetric Flow",   name: "cubic meters per minute",                       symbol: "m³/min",             shortSymbol: "n" },
    { code: 132, classification: "Volumetric Flow",   name: "US oil barrels per second",                     symbol: "bbl (US oil)/s",     shortSymbol: "bbl/s" },
    { code: 133, classification: "Volumetric Flow",   name: "US oil barrels per minute",                     symbol: "bbl (US oil)/min",   shortSymbol: "bbl/min" },
    { code: 134, classification: "Volumetric Flow",   name: "US oil barrels per hour",                       symbol: "bbl (US oil)/h",     shortSymbol: "bbl/h" },
    { code: 135, classification: "Volumetric Flow",   name: "US oil barrels per day",                        symbol: "bbl (US oil)/d",     shortSymbol: "bbl/d" },
    { code: 136, classification: "Volumetric Flow",   name: "US gallons per hour",                           symbol: "gal (US)/h",         shortSymbol: "gal/h" },
    { code: 137, classification: "Volumetric Flow",   name: "Imperial gallons per second",                   symbol: "gal (imp)/s",        shortSymbol: "gal/s" },
    { code: 138, classification: "Volumetric Flow",   name: "liters per hour",                               symbol: "l/h",                shortSymbol: "" },
    { code: 139, classification: "Concentration",     name: "parts per million",                             symbol: "ppm",                shortSymbol: "" },
    { code: 140, classification: "Power",             name: "megacalories per hour",                         symbol: "Mcal/h",             shortSymbol: "" },
    { code: 141, classification: "Power",             name: "megajoules per hour",                           symbol: "MJ/h",               shortSymbol: "" },
    { code: 142, classification: "Power",             name: "British thermal units per hour",                symbol: "Btu/h",              shortSymbol: "" },
    { code: 143, classification: "Angle",             name: "degrees",                                       symbol: "°",                  shortSymbol: "" },
    { code: 144, classification: "Angle",             name: "radians",                                       symbol: "rad",                shortSymbol: "" },
    { code: 145, classification: "Pressure",          name: "inches of water at 60 degrees Fahrenheit",      symbol: "inH2O @ 60°F",       shortSymbol: "inH2O" },
    { code: 146, classification: "Mass Per Volume",   name: "micrograms per liter",                          symbol: "µg/l",               shortSymbol: "" },
    { code: 147, classification: "Mass Per Volume",   name: "micrograms per cubic meter",                    symbol: "µg/m³",              shortSymbol: "" },
    { code: 148, classification: "Analytical",        name: "percent consistency",                           symbol: "%",                  shortSymbol: "" },
    { code: 149, classification: "Volume Per Volume", name: "volume percent",                                symbol: "%",                  shortSymbol: "" },
    { code: 150, classification: "Analytical",        name: "percent steam quality",                         symbol: "%",                  shortSymbol: "" },
    { code: 151, classification: "Length",            name: "feet inch sixteenths",                          symbol: "ft·in·¹/₁₆",         shortSymbol: "" },
    { code: 152, classification: "Volume Per Mass",   name: "cubic feet per pound",                          symbol: "ft³/lb",             shortSymbol: "" },
    { code: 153, classification: "Capacitance",       name: "picofarads",                                    symbol: "pF",                 shortSymbol: "" },
    { code: 154, classification: "Volume Per Volume", name: "milliliters per liter",                         symbol: "ml/l",               shortSymbol: "" },
    { code: 155, classification: "Volume Per Volume", name: "microliters per liter",                         symbol: "µl/l",               shortSymbol: "" },
    { code: 156, classification: "Analytical",        name: "decibels",                                      symbol: "dB",                 shortSymbol: "" },
    { code: 160, classification: "Analytical",        name: "percent plato",                                 symbol: "°P",                 shortSymbol: "" },
    { code: 161, classification: "Analytical",        name: "percent lower explosion level",                 symbol: "%LEL",               shortSymbol: "" },
    { code: 162, classification: "Energy",            name: "megacalories",                                  symbol: "Mcal",               shortSymbol: "" },
    { code: 163, classification: "Resistance",        name: "kilohms",                                       symbol: "KΩ",                 shortSymbol: "" },
    { code: 164, classification: "Energy",            name: "megajoules",                                    symbol: "MJ",                 shortSymbol: "" },
    { code: 165, classification: "Energy",            name: "British thermal units",                         symbol: "Btu",                shortSymbol: "" },
    { code: 166, classification: "Volume",            name: "normal cubic meter",                            symbol: "m³ @ 0°C, 1 atm",    shortSymbol: "m³" },
    { code: 167, classification: "Volume",            name: "normal liter",                                  symbol: "l @ 0°C, 1 atm",     shortSymbol: "l" },
    { code: 168, classification: "Volume",            name: "standard cubic feet",                           symbol: "scf",                shortSymbol: "" },
    { code: 169, classification: "Concentration",     name: "parts per billion",                             symbol: "ppb",                shortSymbol: "" },
    { code: 235, classification: "Volumetric Flow",   name: "US gallons per day",                            symbol: "gal (US)/d",         shortSymbol: "gal/d" },
    { code: 236, classification: "Volume",            name: "hectoliter",                                    symbol: "hl",                 shortSymbol: "" },
    { code: 237, classification: "Pressure",          name: "megapascals",                                   symbol: "MPa",                shortSymbol: "" },
    { code: 238, classification: "Pressure",          name: "inches of water at 4 degrees Celsius",          symbol: "inH2O @ 4°C",        shortSymbol: "inH2O" },
    { code: 239, classification: "Pressure",          name: "millimeters of water at 4 degrees Celsius",     symbol: "mmH2O @ 4°C",        shortSymbol: "mmH2O" }
];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// HART_ExpansionUnitCodes
///
/// <summary>
///	This array maps the extended HART unit codes (170-219) and physical property, which are compatible with HART 6 implementations, to the
/// corresponding name and symbol. These codes are defined in Tables 2.64 to 2.108 Engineering Unit Code Expansions of the HART Common
/// Tables Specification HCF_SPEEC-183, Revision 23.0 dated 29 May 2013.
///
/// The enumeraation is coded as an array of objects with the following properties:
///     code:           The numeric value (decimal) assigned to the unit by hte HART foundation.
///     property:       A text sting identifying the physical property that the unit applies to; such as Pressure.
///     name:           The full description of the unit without any abreviations or symbols.
///     symbol:         The full unit symbol including any qualifying elements such as Standard Conditions or Country.
///     shortSymbol:    Shorten version of the full symbol without the qualifying elements such as Standard Conditions or Country.
/// </summary>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var HART_ExpansionUnitCodes =
[
    { classification: "Pressure",                          code: 170, name: "centimeters of water at 4 degrees Celsius",      symbol: "cmH2O @ 4°C",          shortSymbol: "cmH2O" },
    { classification: "Pressure",                          code: 171, name: "meters of water at 4 degrees Celsius",           symbol: "mH2O @ 4°C",           shortSymbol: "mH2O" },
    { classification: "Pressure",                          code: 172, name: "centimeters of mercury at 0 degrees Celsius",    symbol: "cmHg @ 0°C",           shortSymbol: "cmHg" },
    { classification: "Pressure",                          code: 173, name: "pounds per square foot",                         symbol: "lb/ft²",               shortSymbol: "" },
    { classification: "Pressure",                          code: 174, name: "hectopascals",                                   symbol: "hPa",                  shortSymbol: "" },
    { classification: "Pressure",                          code: 175, name: "pounds per square inch absolute",                symbol: "psi (abs)",            shortSymbol: "psi" },
    { classification: "Pressure",                          code: 176, name: "kilograms per square meter",                     symbol: "kg/m²",                shortSymbol: "" },
    { classification: "Pressure",                          code: 177, name: "feet of water at 4 degrees Celsius",             symbol: "ftH2O @ 4°C",          shortSymbol: "ftH2O" },
    { classification: "Pressure",                          code: 178, name: "feet of water at 60 degrees Fahrenheit",         symbol: "ftH2O @ 60°F",         shortSymbol: "ftH2O" },
    { classification: "Pressure",                          code: 179, name: "meters of mercury at 0 degrees Celsius",         symbol: "mHg @ 0°C",            shortSymbol: "mHg" },
    { classification: "Pressure",                          code: 180, name: "million pounds per square inch",                 symbol: "10⁶ psi",              shortSymbol: "" },
    { classification: "Volumetric Flow",                   code: 170, name: "US beer barrels per second",                     symbol: "bbl (US beer)/s",      shortSymbol: "bbl/s" },
    { classification: "Volumetric Flow",                   code: 171, name: "beer barrels per minute",                        symbol: "bbl (beer)/min",       shortSymbol: "bbl/min" },
    { classification: "Volumetric Flow",                   code: 172, name: "beer barrels per hour",                          symbol: "bbl (beer)/h",         shortSymbol: "bbl/h" },
    { classification: "Volumetric Flow",                   code: 173, name: "beer barrels per day",                           symbol: "bbl (beer)/d",         shortSymbol: "bbl/d" },
    { classification: "Volumetric Flow",                   code: 174, name: "normal liters per day",                          symbol: "l/d @ 0°C, 1 atm",     shortSymbol: "l/d" },
    { classification: "Volumetric Flow",                   code: 175, name: "normal liters per minute",                       symbol: "l/min @ 0°C, 1 atm",   shortSymbol: "l/min" },
    { classification: "Volumetric Flow",                   code: 176, name: "normal liters per second",                       symbol: "l/s @ 0°C, 1 atm",     shortSymbol: "l/s" },
    { classification: "Volumetric Flow",                   code: 177, name: "standard liters per day",                        symbol: "l/d @ 20°C, 1 atm",    shortSymbol: "l/d" },
    { classification: "Volumetric Flow",                   code: 178, name: "standard liters per hour",                       symbol: "l/h @ 20°C, 1 atm",    shortSymbol: "l/h" },
    { classification: "Volumetric Flow",                   code: 179, name: "standard liters per minute",                     symbol: "l/min @ 20°C, 1 atm",  shortSymbol: "l/min" },
    { classification: "Volumetric Flow",                   code: 180, name: "standard liters per second",                     symbol: "l/s @ 20°C, 1 atm",    shortSymbol: "l/s" },
    { classification: "Volumetric Flow",                   code: 181, name: "normal cubic meters per day",                    symbol: "m³/d @ 0°C, 1 atm",    shortSymbol: "m³/d" },
    { classification: "Volumetric Flow",                   code: 182, name: "normal cubic meters per minute",                 symbol: "m³/min @ 0°C, 1 atm",  shortSymbol: "m³/min" },
    { classification: "Volumetric Flow",                   code: 183, name: "normal cubic meters per second",                 symbol: "m³/s @ 0°C, 1 atm",    shortSymbol: "m³/s" },
    { classification: "Volumetric Flow",                   code: 184, name: "standard cubic feet per day",                    symbol: "ft³/d @ 32°F, 1 atm",  shortSymbol: "ft³/d" },
    { classification: "Volumetric Flow",                   code: 185, name: "standard cubic feet per hour",                   symbol: "ft³/h @ 32°F, 1 atm",  shortSymbol: "ft³/h" },
    { classification: "Volumetric Flow",                   code: 186, name: "standard cubic feet per second",                 symbol: "ft³/s @ 32°F, 1 atm",  shortSymbol: "ft³/s" },
    { classification: "Volumetric Flow",                   code: 187, name: "standard cubic meters per day",                  symbol: "m³/d @ 20°C, 1 atm",   shortSymbol: "m³/d" },
    { classification: "Volumetric Flow",                   code: 188, name: "standard cubic meters per hour",                 symbol: "m³/h @ 20°C, 1 atm",   shortSymbol: "m³/h" },
    { classification: "Volumetric Flow",                   code: 189, name: "standard cubic meters per minute",               symbol: "m³/min @ 20°C, 1 atm", shortSymbol: "m³/min" },
    { classification: "Volumetric Flow",                   code: 190, name: "standard cubic meters per second",               symbol: "m³/s @ 20°C, 1 atm",   shortSymbol: "m³/s" },
    { classification: "Volume",                            code: 170, name: "beer barrels",                                   symbol: "bbl (beer)",           shortSymbol: "bbl" },
    { classification: "Volume",                            code: 171, name: "standard liters",                                symbol: "l @ 20°C, 1 atm",      shortSymbol: "l" },
    { classification: "Volume",                            code: 172, name: "standard cubic meters",                          symbol: "m³ @ 20°C, 1 atm",     shortSymbol: "m³" },
    { classification: "Length",                            code: 170, name: "microns",                                        symbol: "µm",                   shortSymbol: "" },
    { classification: "Length",                            code: 171, name: "microinchs",                                     symbol: "µin",                  shortSymbol: "" },
    { classification: "Time",                              code: 170, name: "milliseconds",                                   symbol: "ms",                   shortSymbol: "" },
    { classification: "Time",                              code: 171, name: "microseconds",                                   symbol: "µs",                   shortSymbol: "" },
    { classification: "Time",                              code: 172, name: "nanoseconds",                                    symbol: "ns",                   shortSymbol: "" },
    { classification: "Mass Per Volume",                   code: 170, name: "milligrams per liter",                           symbol: "mg/l",                 shortSymbol: "" },
    { classification: "Mass Per Volume",                   code: 171, name: "milligrams per cubic meter",                     symbol: "mg/m³",                shortSymbol: "" },
    { classification: "Viscosity",                         code: 170, name: "pascal seconds",                                 symbol: "Pa·s",                 shortSymbol: "" },
    { classification: "Force",                             code: 170, name: "kilonewtons",                                    symbol: "kN",                   shortSymbol: "" },
    { classification: "Power",                             code: 170, name: "megawatt",                                       symbol: "MW",                   shortSymbol: "" },
    { classification: "Power",                             code: 171, name: "megajoules per day",                             symbol: "MJ/d",                 shortSymbol: "" },
    { classification: "Power",                             code: 172, name: "million British thermal units per second",       symbol: "10⁶ Btu/s",            shortSymbol: "" },
    { classification: "Power",                             code: 173, name: "million British thermal units per hour",         symbol: "10⁶ Btu/h",            shortSymbol: "" },
    { classification: "Power",                             code: 174, name: "million British thermal units per day",          symbol: "10⁶ Btu/d",            shortSymbol: "" },
    { classification: "Current",                           code: 170, name: "nanoamperes",                                    symbol: "nA",                   shortSymbol: "" },
    { classification: "Current",                           code: 171, name: "microamperes",                                   symbol: "µA",                   shortSymbol: "" },
    { classification: "Resistance",                        code: 170, name: "megaohms",                                       symbol: "MΩ",                   shortSymbol: "" },
    { classification: "Resistance",                        code: 171, name: "ohm centimeters",                                symbol: "Ω·cm",                 shortSymbol: "" },
    { classification: "Resistance",                        code: 172, name: "kilo ohm centimeters",                           symbol: "kΩ·cm",                shortSymbol: "" },
    { classification: "Resistance",                        code: 173, name: "megaohm centimeters",                            symbol: "MΩ·cm",                shortSymbol: "" },
    { classification: "Resistance",                        code: 174, name: "milliohms",                                      symbol: "mΩ",                   shortSymbol: "" },
    { classification: "Concentration",                     code: 170, name: "parts per thousand",                             symbol: "ppth",                 shortSymbol: "" },
    { classification: "Acceleration",                      code: 170, name: "gravitational acceleration",                     symbol: "g",                    shortSymbol: "" },
    { classification: "Acceleration",                      code: 171, name: "feet per second squared",                        symbol: "ft/s²",                shortSymbol: "" },
    { classification: "Acceleration",                      code: 172, name: "meters per second squared",                      symbol: "m/s²",                 shortSymbol: "" },
    { classification: "Turbidity",                         code: 170, name: "Formazin Nephelometric Units",                   symbol: "FNU",                  shortSymbol: "" },
    { classification: "Turbidity",                         code: 171, name: "Formazin Turbidity Units",                       symbol: "FTU",                  shortSymbol: "" },
    { classification: "Turbidity",                         code: 172, name: "Nephelometric Turbidity Units",                  symbol: "NTU",                  shortSymbol: "" },
    { classification: "Volumetric Gas Flow per Second",    code: 176, name: "normal liters per second",                       symbol: "l/s @ 0°C, 1 atm",     shortSymbol: "l/s" },
    { classification: "Volumetric Gas Flow per Second",    code: 180, name: "standard liters per second",                     symbol: "l/s @ 20°C, 1 atm",    shortSymbol: "l/s" },
    { classification: "Volumetric Gas Flow per Second",    code: 183, name: "normal cubic meters per second",                 symbol: "m³/s @ 0°C, 1 atm",    shortSymbol: "m³/s" },
    { classification: "Volumetric Gas Flow per Second",    code: 186, name: "standard cubic feet per second",                 symbol: "ft³/s @ 32°F, 1 atm",  shortSymbol: "ft³/s" },
    { classification: "Volumetric Gas Flow per Second",    code: 190, name: "standard cubic meters per second",               symbol: "m³/s @ 20°C, 1 atm",   shortSymbol: "m³/s" },
    { classification: "Volumetric Gas Flow per Minute",    code: 175, name: "normal liters per minute",                       symbol: "l/min @ 0°C, 1 atm",   shortSymbol: "l/min" },
    { classification: "Volumetric Gas Flow per Minute",    code: 179, name: "standard liters per minute",                     symbol: "l/min @ 20°C, 1 atm",  shortSymbol: "l/min" },
    { classification: "Volumetric Gas Flow per Minute",    code: 182, name: "normal cubic meters per minute",                 symbol: "m³/min @ 0°C, 1 atm",  shortSymbol: "m³/min" },
    { classification: "Volumetric Gas Flow per Minute",    code: 189, name: "standard cubic meters per minute",               symbol: "m³/min @ 20°C, 1 atm", shortSymbol: "m³/min" },
    { classification: "Volumetric Gas Flow per Hour",      code: 178, name: "standard liters per hour",                       symbol: "l/h @ 20°C, 1 atm",    shortSymbol: "l/h" },
    { classification: "Volumetric Gas Flow per Hour",      code: 185, name: "standard cubic feet per hour",                   symbol: "ft³/h @ 32°F, 1 atm",  shortSymbol: "ft³/h" },
    { classification: "Volumetric Gas Flow per Hour",      code: 188, name: "standard cubic meters per hour",                 symbol: "m³/h @ 20°C, 1 atm",   shortSymbol: "m³/h" },
    { classification: "Volumetric Gas Flow per Day",       code: 174, name: "normal liters per day",                          symbol: "l/d @ 0°C, 1 atm",     shortSymbol: "l/d" },
    { classification: "Volumetric Gas Flow per Day",       code: 177, name: "standard liters per day",                        symbol: "l/d @ 20°C, 1 atm",    shortSymbol: "l/d" },
    { classification: "Volumetric Gas Flow per Day",       code: 181, name: "normal cubic meters per day",                    symbol: "m³/d @ 0°C, 1 atm",    shortSymbol: "m³/d" },
    { classification: "Volumetric Gas Flow per Day",       code: 184, name: "standard cubic feet per day",                    symbol: "ft³/d @ 32°F, 1 atm",  shortSymbol: "ft³/d" },
    { classification: "Volumetric Gas Flow per Day",       code: 187, name: "standard cubic meters per day",                  symbol: "m³/d @ 20°C, 1 atm",   shortSymbol: "m³/d" },
    { classification: "Volumetric Liquid Flow per Second", code: 170, name: "US beer barrels per second",                     symbol: "bbl (US beer)/s",      shortSymbol: "bbl/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 174, name: "Imperial fluid ounces per second",               symbol: "oz (imp)/s",           shortSymbol: "oz/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 175, name: "US fluid ounces per second",                     symbol: "oz (US)/s",            shortSymbol: "oz/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 176, name: "milliliters (cc) per second",                    symbol: "ml/s",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Second", code: 178, name: "hectoliters per second",                         symbol: "hl/s",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Second", code: 180, name: "megaliters per second",                          symbol: "Ml/s",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Second", code: 182, name: "US thousand gallons per second",                 symbol: "10³ gal (US)/s",       shortSymbol: "10³ gal/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 183, name: "US million gallons per second",                  symbol: "10⁶ gal (US)/s",       shortSymbol: "10⁶ gal/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 185, name: "Imperial million gallons per second",            symbol: "10⁶ gal (imp)/s",      shortSymbol: "10⁶ gal/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 187, name: "acre foot per second",                           symbol: "ac·ft/s",              shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Second", code: 188, pname: "US drum per second",                            symbol: "drum (US)/s",          shortSymbol: "drum/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 189, name: "US liquid barrels per second",                   symbol: "bbl (US liq)/s",       shortSymbol: "bbl/s" },
    { classification: "Volumetric Liquid Flow per Second", code: 192, name: "Imperial beer barrels per second",               symbol: "bbl (imp beer)/s",     shortSymbol: "bbl/s" },
    { classification: "Volumetric Liquid Flow per Minute", code: 171, name: "US beer barrels per minute",                     symbol: "bbl (US beer)/min",    shortSymbol: "bbl/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 174, name: "Imperial fluid ounces per minute",               symbol: "oz (imp)/min",         shortSymbol: "oz/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 175, name: "US fluid ounces per minute",                     symbol: "oz (US)/min",          shortSymbol: "oz/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 176, name: "milliliters (cc) per minute",                    symbol: "ml/min",               shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Minute", code: 178, name: "hectoliters per minute",                         symbol: "hl/min",               shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Minute", code: 180, name: "megaliters per minute",                          symbol: "Ml/min",               shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Minute", code: 182, name: "US thousand gallons per minute",                 symbol: "10³ gal (US)/min",     shortSymbol: "10³ gal/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 183, name: "US million gallons per minute",                  symbol: "10⁶ gal (US)/min",     shortSymbol: "10⁶ gal/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 185, name: "Imperial million gallons per minute",            symbol: "10⁶ gal (imp)/min",    shortSymbol: "10⁶ gal/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 187, name: "acre foot per minute",                           symbol: "ac·ft/min",            shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Minute", code: 188, name: "US drum per minute",                             symbol: "drum (US)/min",        shortSymbol: "drum/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 189, name: "US liquid barrels per minute",                   symbol: "bbl (US liq)/min",     shortSymbol: "bbl/min" },
    { classification: "Volumetric Liquid Flow per Minute", code: 192, name: "Imperial beer barrels per minute",               symbol: "bbl (imp beer)/min",   shortSymbol: "bbl/min" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 172, name: "US beer barrels per hour",                       symbol: "bbl (US beer)/h",      shortSymbol: "bbl/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 174, name: "Imperial fluid ounces per hour",                 symbol: "oz (imp)/h",           shortSymbol: "oz/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 175, name: "US fluid ounces per hour",                       symbol: "oz (US)/h",            shortSymbol: "oz/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 176, name: "milliliters (cc) per hour",                      symbol: "ml/h",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 178, name: "hectoliters per hour",                           symbol: "hl/h",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 180, name: "megaliters per hour",                            symbol: "Ml/h",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 182, name: "US thousand gallons per hour",                   symbol: "10³ gal (US)/h",       shortSymbol: "10³ gal/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 183, name: "US million gallons per hour",                    symbol: "10⁶ gal (US)/h",       shortSymbol: "10⁶ gal/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 185, name: "Imperial million gallons per hour",              symbol: "10⁶ gal (imp)/h",      shortSymbol: "10⁶ gal/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 187, name: "acre foot per hour",                             symbol: "ac·ft/h",              shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 188, name: "US drum per hour",                               symbol: "drum (US)/h",          shortSymbol: "drum/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 189, name: "US liquid barrels per hour",                     symbol: "bbl (US liq)/h",       shortSymbol: "bbl/h" },
    { classification: "Volumetric Liquid Flow per Hour",   code: 192, name: "Imperial beer barrels per hour",                 symbol: "bbl (imp beer)/h",     shortSymbol: "bbl/h" },
    { classification: "Volumetric Liquid Flow per Day",    code: 173, name: "US beer barrels per day",                        symbol: "bbl (US beer)/d",      shortSymbol: "bbl/d" },
    { classification: "Volumetric Liquid Flow per Day",    code: 174, name: "Imperial fluid ounces per day",                  symbol: "oz (imp)/d",           shortSymbol: "oz/d" },
    { classification: "Volumetric Liquid Flow per Day",    code: 175, name: "US fluid ounces per day",                        symbol: "oz (US)/d",            shortSymbol: "oz/d" },
    { classification: "Volumetric Liquid Flow per Day",    code: 176, name: "milliliters (cc) per day",                       symbol: "ml/d",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Day",    code: 178, name: "hectoliters per day",                            symbol: "hl/d",                 shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Day",    code: 182, name: "US thousand gallons per day",                    symbol: "10³ gal (US)/d",       shortSymbol: "10³ gal/d" },
    { classification: "Volumetric Liquid Flow per Day",    code: 185, name: "Imperial million gallons per day",               symbol: "10⁶ gal (imp)/d",      shortSymbol: "10⁶ gal/d" },
    { classification: "Volumetric Liquid Flow per Day",    code: 187, name: "acre foot per day",                              symbol: "ac·ft/d",              shortSymbol: "" },
    { classification: "Volumetric Liquid Flow per Day",    code: 188, name: "US drum per day",                                symbol: "drum (US)/d",          shortSymbol: "drum/d" },
    { classification: "Volumetric Liquid Flow per Day",    code: 189, name: "US liquid barrels per day",                      symbol: "bbl (US liq)/d",       shortSymbol: "bbl/d" },
    { classification: "Volumetric Liquid Flow per Day",    code: 192, name: "Imperial beer barrels per day",                  symbol: "bbl (imp beer)/d",     shortSymbol: "bbl/d" },
    { classification: "Thermal Expansion",                 code: 170, name: "per degree Celsius",                             symbol: "/°C",                  shortSymbol: "" },
    { classification: "Thermal Expansion",                 code: 171, name: "per degree Fahrenheit",                          symbol: "/°F",                  shortSymbol: "" },
    { classification: "Volumetric Energy Density",         code: 170, name: "kilojoules per liter",                           symbol: "kJ/l",                 shortSymbol: "" },
    { classification: "Volumetric Energy Density",         code: 170, name: "British thermal Unit per cubic foot",            symbol: "Btu/ft³",              shortSymbol: "" }
];
 


//  HART_SmartLineDeviceTypeCodes = [{ code: , manufacturer: , ID: , type: , description: }]
//
//	This array maps the HART Expanded Device Type Codes registered by the HART foundation to the manufacturer, ID and description for
//  the Honeywell SmartLine family of transmitters.
//  These codes would normally be defined in Table 1 Expanded Device Type Codes of the HART Common Tables Specification HCF_SPEEC-183, but
//  some of them were assigned after the latest revision of this specification.

var SL_SmartLineDeviceTypes =
[
    { typeCode: 5920, manufacturer: "Honeywell", model: "ST800",  family: "Pressure",      description: "SmartLine Pressure" },
    { typeCode: 5921, manufacturer: "Honeywell", model: "ST700",  family: "Pressure",      description: "SmartLine Pressure" },
    { typeCode: 5922, manufacturer: "Honeywell", model: "STT850", family: "Temperature",   description: "SmartLine Temperature" },
    { typeCode: 5924, manufacturer: "Honeywell", model: "SMV800", family: "Multivariable", description: "SmartLine Multivariable" },
    { typeCode: 5925, manufacturer: "Honeywell", model: "SLG800", family: "Level",         description: "SmartLine GWR Level" },
    { typeCode: 5926, manufacturer: "Honeywell", model: "SLG700", family: "Level",         description: "SmartLine GWR Level" },
    { typeCode: 5927, manufacturer: "Honeywell", model: "SLN800", family: "Level",         description: "SmartLine NCR Level" },
    { typeCode: 5928, manufacturer: "Honeywell", model: "SLN700", family: "Level",         description: "SmartLine NCR Level" },
    { typeCode: 5930, manufacturer: "Honeywell", model: "STT750", family: "Temperature",   description: "SmartLine Temperature" },
    { typeCode: 5931, manufacturer: "Honeywell", model: "STT700", family: "Temperature",   description: "SmartLine Temperature" }
];


var transformCharModeArray =
[
    { code: 0,  mode: "Linear" },
    { code: 1,  mode: "Square Root" },
    { code: 26, mode: "Linear" },
    { code: 27, mode: "Linear" },
    { code: 28, mode: "Square Root" }
];


var SL_LocalDisplayTypes =
{
    0: { type: "None" },
    1: { type: "Advanced" },
    2: { type: "Basic" },
    3: { type: "Standard" }
};


var deviceStatuses =
{
	0: { Parameter: "HPVMISM",  Description: "Range mismatch",                 Color: "#FF0000" },
	1: { Parameter: "HDEVIDFL", Description: "Device ID mismatch",             Color: "#FF0000" },
	2: { Parameter: "HREVMISM", Description: "Revision mismatch",              Color: "#FF0000" },
	3: { Parameter: "HDEVMISM", Description: "Device type mismatch",           Color: "#FF0000" },
	4: { Parameter: "HDEVMALF", Description: "Device Malfunction",             Color: "#FF0000" },
	5: { Parameter: "HCOMERFL", Description: "HART communication status",      Color: "#FF0000" },
	6: { Parameter: "HPVOUTLM", Description: "Primary Variable Out Of Limits", Color: "#FF0000" },
	7: { Parameter: "HVOUTLM",  Description: "Device Variable Out Of Limits",  Color: "#FFCC66" },
	8: { Parameter: "HCURRSAT", Description: "Loop Current Saturated",         Color: "#FFCC66" }
};


var statusDisplayScheme =
[
  { type:  "normal",  color: "gray",   fillColor: "#E0E0E0", lineColor: "#E0E0E0", textColor: "#000000", textColorDarkBG: "#FFFFFF" },
  { type:  "info",    color: "white",  fillColor: "#FFFFFF", lineColor: "#606060", textColor: "#000000", textColorDarkBG: "#FFFFFF" },
  { type:  "warning", color: "yellow", fillColor: "#FFC627", lineColor: "#FFC627", textColor: "#FFC627", textColorDarkBG: "#FFC627" },
  { type:  "error",   color: "red",    fillColor: "#EE3124", lineColor: "#EE3124", textColor: "#EE3124", textColorDarkBG: "#EE3124" }
];


var analogPVStatuses =
[
  { code: 0, statusString: "Bad",       type:  "error" },
  { code: 1, statusString: "Uncertain", type:  "warning" },
  { code: 2, statusString: "Normal",    type:  "normal" },
  { code: 3, statusString: "Manual",    type:  "info" }
];

var HART_DeviceVariableStatuses =
[
  { lowIndex: 0,    highIndex: 0,    processDataStatus: "Unknown",  limitStatus: "",             moreStatusAvailable: "",     type: "error" },
  { lowIndex: 1,    highIndex: 7,    processDataStatus: "Bad",      limitStatus: "Not Limited",  moreStatusAvailable: "",     type: "error" },
  { lowIndex: 8,    highIndex: 15,   processDataStatus: "Bad",      limitStatus: "Not Limited",  moreStatusAvailable: "More", type: "error" },
  { lowIndex: 16,   highIndex: 23,   processDataStatus: "Bad",      limitStatus: "Low Limited",  moreStatusAvailable: "",     type: "error" },
  { lowIndex: 24,   highIndex: 31,   processDataStatus: "Bad",      limitStatus: "Low Limited",  moreStatusAvailable: "More", type: "error" },
  { lowIndex: 32,   highIndex: 39,   processDataStatus: "Bad",      limitStatus: "High Limited", moreStatusAvailable: "",     type: "error" },
  { lowIndex: 40,   highIndex: 47,   processDataStatus: "Bad",      limitStatus: "High Limited", moreStatusAvailable: "More", type: "error" },
  { lowIndex: 48,   highIndex: 55,   processDataStatus: "Bad",      limitStatus: "Constant",     moreStatusAvailable: "",     type: "error" },
  { lowIndex: 56,   highIndex: 63,   processDataStatus: "Bad",      limitStatus: "Constant",     moreStatusAvailable: "More", type: "error" },
  { lowIndex: 64,   highIndex: 71,   processDataStatus: "Poor",     limitStatus: "Not Limited",  moreStatusAvailable: "",     type: "warning" },
  { lowIndex: 72,   highIndex: 79,   processDataStatus: "Poor",     limitStatus: "Not Limited",  moreStatusAvailable: "More", type: "warning" },
  { lowIndex: 80,   highIndex: 87,   processDataStatus: "Poor",     limitStatus: "Low Limited",  moreStatusAvailable: "",     type: "warning" },
  { lowIndex: 88,   highIndex: 95,   processDataStatus: "Poor",     limitStatus: "Low Limited",  moreStatusAvailable: "More", type: "warning" },
  { lowIndex: 96,   highIndex: 103,  processDataStatus: "Poor",     limitStatus: "High Limited", moreStatusAvailable: "",     type: "warning" },
  { lowIndex: 104,  highIndex: 111,  processDataStatus: "Poor",     limitStatus: "High Limited", moreStatusAvailable: "More", type: "warning" },
  { lowIndex: 112,  highIndex: 119,  processDataStatus: "Poor",     limitStatus: "Constant",     moreStatusAvailable: "",     type: "warning" },
  { lowIndex: 120,  highIndex: 127,  processDataStatus: "Poor",     limitStatus: "Constant",     moreStatusAvailable: "More", type: "warning" },
  { lowIndex: 128,  highIndex: 135,  processDataStatus: "Manual",   limitStatus: "Not Limited",  moreStatusAvailable: "",     type: "info" },
  { lowIndex: 136,  highIndex: 143,  processDataStatus: "Manual",   limitStatus: "Not Limited",  moreStatusAvailable: "More", type: "info" },
  { lowIndex: 144,  highIndex: 151,  processDataStatus: "Manual",   limitStatus: "Low Limited",  moreStatusAvailable: "",     type: "info" },
  { lowIndex: 152,  highIndex: 159,  processDataStatus: "Manual",   limitStatus: "Low Limited",  moreStatusAvailable: "More", type: "info" },
  { lowIndex: 160,  highIndex: 167,  processDataStatus: "Manual",   limitStatus: "High Limited", moreStatusAvailable: "",     type: "info" },
  { lowIndex: 168,  highIndex: 175,  processDataStatus: "Manual",   limitStatus: "High Limited", moreStatusAvailable: "More", type: "info" },
  { lowIndex: 176,  highIndex: 183,  processDataStatus: "Manual",   limitStatus: "Constant",     moreStatusAvailable: "",     type: "info" },
  { lowIndex: 184,  highIndex: 191,  processDataStatus: "Manual",   limitStatus: "Constant",     moreStatusAvailable: "More", type: "info" },
  { lowIndex: 192,  highIndex: 199,  processDataStatus: "Good",     limitStatus: "Not Limited",  moreStatusAvailable: "",     type: "normal" },
  { lowIndex: 200,  highIndex: 207,  processDataStatus: "Good",     limitStatus: "Not Limited",  moreStatusAvailable: "More", type: "normal" },
  { lowIndex: 208,  highIndex: 215,  processDataStatus: "Good",     limitStatus: "Low Limited",  moreStatusAvailable: "",     type: "normal" },
  { lowIndex: 216,  highIndex: 223,  processDataStatus: "Good",     limitStatus: "Low Limited",  moreStatusAvailable: "More", type: "normal" },
  { lowIndex: 224,  highIndex: 231,  processDataStatus: "Good",     limitStatus: "High Limited", moreStatusAvailable: "",     type: "normal" },
  { lowIndex: 232,  highIndex: 239,  processDataStatus: "Good",     limitStatus: "High Limited", moreStatusAvailable: "More", type: "normal" },
  { lowIndex: 240,  highIndex: 247,  processDataStatus: "Good",     limitStatus: "Constant",     moreStatusAvailable: "",     type: "normal" },
  { lowIndex: 248,  highIndex: 255,  processDataStatus: "Good",     limitStatus: "Constant",     moreStatusAvailable: "More", type: "normal" }
];


var IOM_ScanConfigurations =
{
    1:  { value: 1,  dynamicVariables: false, deviceVariables: true,  rate: 1 },
    2:  { value: 2,  dynamicVariables: true,  deviceVariables: false, rate: 1 },
    3:  { value: 3,  dynamicVariables: false, deviceVariables: true,  rate: 2 },
    4:  { value: 4,  dynamicVariables: true,  deviceVariables: false, rate: 2 },
    5:  { value: 5,  dynamicVariables: true,  deviceVariables: true,  rate: 2 },
    6:  { value: 6,  dynamicVariables: false, deviceVariables: true,  rate: 4 },
    7:  { value: 7,  dynamicVariables: true,  deviceVariables: false, rate: 4 },
    8:  { value: 8,  dynamicVariables: true,  deviceVariables: true,  rate: 4 },
    9:  { value: 9,  dynamicVariables: false, deviceVariables: true,  rate: 8 },
    10: { value: 10, dynamicVariables: true,  deviceVariables: false, rate: 8 },
    11: { value: 11, dynamicVariables: true,  deviceVariables: true,  rate: 8 },
    12: { value: 12, dynamicVariables: false, deviceVariables: true,  rate: 16 },
    13: { value: 13, dynamicVariables: true,  deviceVariables: false, rate: 16 },
    14: { value: 14, dynamicVariables: true,  deviceVariables: true,  rate: 16 },
    15: { value: 15, dynamicVariables: false, deviceVariables: false, rate: 0 },
    16: { value: 16, dynamicVariables: false, deviceVariables: true,  rate: -1 },
    17: { value: 17, dynamicVariables: true,  deviceVariables: false, rate: -1 },
    18: { value: 18, dynamicVariables: true,  deviceVariables: true,  rate: -1 }
};

var IOM_DeviceStatuses =
[
  {
    parameter: "HCOMERFL",
    criticality: 0,
    name: "HART Communication Failure",
    description: "The HART Enabled I/O Module is not able to communicate with the HART device configured for this Analog Input Channel.",
    resolution: "The communication failure may be caused by faulty wiring between the I/O Module and HART device, noises on the wires, or problems with the I/O Module or HART device. Check all components in the communication pathway to determine the root cause."
  },
  {
    parameter: "HDEVMISM",
    criticality: 1,
    name: "Device Type Mismatch",
    description: "The HART Device Type configured in the Analog I/O Channel properties is different from the type reported by the transmitter connected to the channel.\n\nThis may happen:\n     - During commissioning if the Device Type was not pre-configured\n     - When the device is replaced with a newer model\n     - If the Experion configuration is changed or corrupted",
    resolution: "Click on the Device Details menu item above to view the Device Type configured for the Analog I/O channel and the Device Type reported by the transmitter.\n\nGenerally the transmitter type will be correct and the I/O channel configuration should be corrected using the Control Builder application. A new diagnostic template should also be created to allow the correct alarms to be generated within Experion.\n\nIf the device type of the transmitter is incorrect, check for recent wiring or hardware modifications."
  },
  {
    parameter: "HREVMISM",
    criticality: 1,
    name: "Device Revision Mismatch",
    description: "The HART Device Revision configured in the Analog I/O Channel properties is different from the Device Revision of the firmware reported by the transmitter connected to the channel.\n\nThis may happen:\n     - During commissioning if the Device Revision was not pre-configured\n     - When the device is replaced with a newer model\n     - If the Experion configuration is changed or corrupted",
    resolution: "Click on the Device Details menu item above to view the Device Revision configured for the Analog I/O Channel and the Device Revision reported by the transmitter.\n\nGenerally the transmitter revision will be correct and the I/O channel configuration should be corrected using the Control Builder application. A new diagnostic template should also be created to allow the correct alarms to be generated within Experion.\n\nIf the Device Revision of the transmitter is incorrect, check for recent wiring or hardware modifications."
  },
  {
    parameter: "HDEVIDFL",
    criticality: 1,
    name: "Device ID Mismatch",
    description: "The HART Device ID configured in the Analog I/O Channel properties is different from the unique ID reported by the transmitter connected to the channel.\n\nThis may happen:\n     - During commissioning if the Device ID was not pre-configured\n     - When the device is replaced with a newer model\n     - If the Experion configuration is changed or corrupted",
    resolution: "Click on the Device Details menu item above to view the Device ID configured for the Analog I/O Channel and the Device ID reported by the transmitter.\n\nThe transmitter ID is unique and cannot be changed so the only resolution is to update the I/O Module configuration. This can be done by clicking on the link displayed on the Device Details tab.\n\nIf not commissioning a new transmitter and the transmitter was not replaced, other I/O configuration properties should be checked for possible corruption."
  },
  {
    parameter: "HPVMISM",
    criticality: 1,
    name: "Device PV Range Mismatch",
    description: "The Range configuration in the Analog I/O Channel properties is different from the Range configuration reported by the transmitter connected to the channel. The 4 range parameters are used by the I/O module to convert the analog loop current into the Process Variable in engineering units.\n\nA mismatch will cause the control system to use a PV different than what the transmitter measured, with undesirable results. This may be noticed on the faceplate with the analog and digital PV values not being close.",
    resolution: "Click on the Range menu item above to view the configuration for the I/O Channel and the Transmitter. The 4 range parameters are shown on both the Transmitter and I/O Channel sides of the display and are connected by horizontal lines. Mismatched parameters will be connected by red lines. Also check to make sure that the transformation (linear versus square root) is correct on both sides.\n\nIncorrect values must be changed using the Control Builder application as they must be downloaded to the I/O Module."
  },
  {
    parameter: "HSMSTRFL",
    criticality: 1,
    name: "Secondary Master Present",
    description: "The parent HART Enabled I/O Module has detected that another communication device configured as a Secondary Master has been connected to the transmitter and is sending messages to the transmitter. This may slow down the periodic communication between the I/O Module and the transmitter.",
    resolution: "This status condition is presented mainly for informational purposes as it is often necessary for maintenance personnel to connect to the transmitter with a hand-held terminal or other PC-based application to view and modify advanced configuration parameters.\n\nCheck with personnel if unauthorized access to the transmitter is suspected."
  }
];



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  Pseudo Class Object Definitions
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_DeviceRevision
///
/// <summary>
/// Function and prototype to create a pseudo class for the variable information associated with a Device Revision of the SmartLine firmware:
/// </summary>
///
/// <returns>
/// New SL_DeviceRevision object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_DeviceRevision(myRevision, myCommand48StatusBytes, myCommand220StatusBytes)
{
    this.revision = myRevision;
    this.command48StatusBytes = myCommand48StatusBytes;
    this.command220StatusBytes = myCommand220StatusBytes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_ScriptData
///
/// <summary>
/// Function and prototype to create a pseudo class for a point parameter accessed through HMIWeb ScriptData:
/// </summary>
///
/// <returns>
/// New SL_ScriptData object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_ScriptData(myParameter)
{
    this.parameter = myParameter;
    this.exists = false;
    this.changed = false;
    this.valid = false;
    this.value = undefined;
}

SL_ScriptData.prototype.Fetch = function (dataProvider, pointName)
{
    try
    {
        var tag = pointName + "." + this.parameter;

        this.changed = false;
        this.valid = false;
        this.value = undefined;
        this.exists = dataProvider.DataExists(tag);

        if (this.exists)
        {
            this.value = dataProvider.DataValue(tag);

            if (this.value != undefined)
            {
                this.changed = dataProvider.DataChanged(tag);
                this.valid = dataProvider.QualityGood(tag);
            }
        }

        SL_LogDebugLevelMessage(3, "SL_ScriptData.Fetch", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + this.parameter + ", exists = " + this.exists + ", changed = " + this.changed + ", valid = " + this.valid + ", value = " + this.value);
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_ScriptData.Fetch", "Could not fetch ScriptData: " + "dataProvider = " + dataProvider + ", point = " + pointName + ", parameter = " + this.name, error);
        this.exists = false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_DisplayItem
///
/// <summary>
/// Function and prototype to create a pseudo class for an item that is displayed on the point detail displays with:
///   Properties:
///     labelUserControl
///     valueUserControl
///   Methods:
///     SetLabel(string)
///     SetVisibility(boolean)
///     Hide()
///     Show()
/// </summary>
///
/// <returns>
/// New SL_DisplayItem object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_DisplayItem(myLabelUserControl, myValueUserControl)
{
    this.labelUserControl = myLabelUserControl;
    this.valueUserControl = myValueUserControl;
}

SL_DisplayItem.prototype.SetLabel = function (label)
{
    try
    {
        this.labelUserControl.Value = label;
    }
    catch (error)
    {
        if (this.labelUserControl != undefined)
            SL_LogErrorMessage("SL_DisplayItem.SetLabel", "Could not set label text: " + "label control = " + this.labelUserControl.id + ", text = " + label, error);
        else
            SL_LogErrorMessage("SL_DisplayItem.SetLabel", "User control is undefined: text = " + label, error);
    }
}

SL_DisplayItem.prototype.SetVisibility = function (isVisible)
{
    try
    {
        SL_SetVisibility(this.labelUserControl, isVisible);
        SL_SetVisibility(this.valueUserControl, isVisible);
    }
    catch (error)
    {
        if ((this.labelUserControl != undefined) && (this.valueUserControl != undefined))
            SL_LogErrorMessage("SL_DisplayItem.SetVisibility", "Could not set visibility: label control = " + this.labelUserControl.id + ", value control = " + this.valueUserControl.id + ", visibility = " + isVisible, error);
        else
            SL_LogErrorMessage("SL_DisplayItem.SetVisibility", "User control is undefined: label control = " + this.labelUserControl + ", value control = " + this.valueUserControl + ", visibility = " + isVisible, error);
    }
}

SL_DisplayItem.prototype.Hide = function ()
{
    try
    {
        SL_SetVisibility(this.labelUserControl, false);
        SL_SetVisibility(this.valueUserControl, false);
    }
    catch (error)
    {
        if ((this.labelUserControl != undefined) && (this.valueUserControl != undefined))
            SL_LogErrorMessage("SL_DisplayItem.Hide", "Could not hide controls: label control = " + this.labelUserControl.id + ", value control = " + this.valueUserControl.id, error);
        else
            SL_LogErrorMessage("SL_DisplayItem.Hide", "User control is undefined: label control = " + this.labelUserControl + ", value control = " + this.valueUserControl, error);
    }
}

SL_DisplayItem.prototype.Show = function ()
{
    try
    {
        SL_SetVisibility(this.labelUserControl, true);
        SL_SetVisibility(this.valueUserControl, true);
    }
    catch (error)
    {
        if ((this.labelUserControl != undefined) && (this.valueUserControl != undefined))
            SL_LogErrorMessage("SL_DisplayItem.Show", "Could not show controls: label control = " + this.labelUserControl.id + ", value control = " + this.valueUserControl.id, error);
        else
            SL_LogErrorMessage("SL_DisplayItem.Show", "User control is undefined: label control = " + this.labelUserControl + ", value control = " + this.valueUserControl, error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_ModelNumberComponent
///
/// <summary>
/// Function and prototype to create a pseudo class for a individual component string that makes up part of the overall model number:
/// </summary>
///
/// <returns>
/// New SL_ModelNumberComponent object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_ModelNumberComponent(myParameterName)
{
    this.parameterName = myParameterName;
    this.changedFlag = false;
    this.qualityGood = false;
    this.value = "";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_PressureModule
///
/// <summary>
/// Function and prototype to create a pseudo class for a SmartLine pressure sensor to contain properties for:
/// </summary>
///
/// <returns>
/// New SL_PressureModule object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_PressureModule()
{
    this.type;
    this.moduleTypes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_Transmitter
///
/// <summary>
/// Function and prototype to create a pseudo class for a HART SmartLine transmitter to contain properties for:
///     expectedType        reference to a SL_SmartLineDeviceType object indicating the expected transmitter type
///     type                reference to a SL_SmartLineDeviceType object indicating the actual transmitter type
///     modelKey            string identifying the subclass of the major type, such as SLG726
///     modelNumberParts    array of substrings that make up the full model number
///     deviceRevision      integer indicating the revision level of the firmware running in the transmitter
///     pointID             name of the analog channel that is wired to the transmitter
///     deviceVariables     array of strings holding the names of all of the device variables supported by the device type
///     dynamicVariables    array of objects holding the properties of the possible 4 HART dynamic variables, PV, SV, TV and QV
///     analogPV            object to hold the properties of the analog PV that the I/O module calculates from the loop current
///     command220          object to hold the properties of the status bytes read through the device specific Command 220
///     command48           object to hold the properties of the status bytes read through the HART Universal Command 48
///     criticalFaults      array containing the properties of all of the critical fault conditions active in the transmitter
///     nonCriticalFaults   array containing the properties of all of the non-critical fault conditions active in the transmitter
///     allFaults           array containing the properties of all fault conditions active in the transmitter
///     module              object to hold model specific properties of the transmitter, such as probe type
/// and methods to:
///     getDescription()    returns a single string containing all of the properties
/// </summary>
///
/// <returns>
/// New SL_Transmitter object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_Transmitter()
{
    this.expectedModel = "";
    this.type;
    this.modelKey = new SL_ScriptData("key_number");
    this.modelNumberParts = [];
    this.modelNumber = "";
    this.deviceRevisions;
    this.deviceRevision = 0;
    this.pointID = "";
    this.deviceVariables;
    this.dynamicVariables =
    {
        PV: { symbol: "PV", visibility: "visible", name: "", classification: "", units: "", ignoreStatus: true, index: 1, tooltip: "HART Primary Variable" },
        SV: { symbol: "SV", visibility: "visible", name: "", classification: "", units: "", ignoreStatus: true, index: 2, tooltip: "HART Secondary Variable" },
        TV: { symbol: "TV", visibility: "hidden",  name: "", classification: "", units: "", ignoreStatus: true, index: 3, tooltip: "HART Tertiary Variable" },
        QV: { symbol: "QV", visibility: "hidden",  name: "", classification: "", units: "", ignoreStatus: true, index: 4, tooltip: "HART Quaternary Variable" }
    };
    this.analogPV = { symbol: "PV", visibility: "visible", name: "", classification: "", units: "", ignoreStatus: false, index: 1 };
    this.command220 = { numberOfBytes: 0, statusBits: [], statusBytes: [] };
    this.command48 = { numberOfBytes: 0, statusBits: [] };
    this.criticalFaults = [];
    this.nonCriticalFaults = [];
    this.allFaults = [];
    this.module;
}



var SL_Device = new SL_Transmitter();

