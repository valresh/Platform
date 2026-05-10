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
/// \file  SmartLineGlobalFunctions.js
///
/// \brief
/// Java script file to contain functions that are shared between the Faceplate and Point Detail displays for all of the Honeywell
/// SmartLine family of HART transmitters. To avoid name conflicts, all function names in this file should use the following identifiers:
///     "SL_"       generic function that applies to both the Point Detail and Faceplate displays for all SmartLine families
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// History:
///
/// EXP510.1-36     June   4, 2018      Developer Label D_EXP510.1-36.0-02
/// [RLEAPFI-475]   May   28, 2018      Corrected a case sensitivity bug - replaced "ByteIndex" with "byteIndex".
/// EXP510.1-35		May   27, 2018		Developer Label D_EXP510.1-35.0-06
/// EXP510.1-34		May   21, 2018		Developer Label D_EXP510.1-34.0-12
/// [RLEAPFI-642]	April 30, 2018		Create separate functions for updating the device type and device revision.
/// [RLEAPFI-639]	April 25, 2018		Add logic to check the quality of the value and status before updating the PV, SV, TV and QV controls.
/// [RLEAPFI-443]	April 24, 2018		Consolidate local display type and firmware handler into SmartLineGlobalFunctions.js
/// EXP510.1-31		April 24, 2018		Initial Experion R510 testbed release
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Generic Functions - Faceplate and Point Detail displays
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Add the trim() method to the String data type if it is not supported natively by the browser.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if (!String.prototype.trim)
{
    String.prototype.trim = function ()
    {
        return this.replace(/^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g, '');
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_LogMessage
///
/// <summary>
/// Formats the specified message text with header information and adds it to the HMIWeb log file.
/// </summary>
///
/// <param name="messageType">
/// A short text string identifying the type of the message to be logged; such as "ERROR" or "Debug".
/// </param>
///
/// <param name="source">
/// A string containing the name of the function that generated the message.
/// </param>
///
/// <param name="message">
/// A string containing the main content of the message to be logged.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_LogMessage(messageType, source, message)
{
    var header = "SMARTLINE"

    if ((messageType != null) && (messageType.length > 0))
        header += " " + messageType;
    header += ": ";

    if ((SL_Device != undefined) && (SL_Device.type != undefined))
    {
        if ((SL_Device.pointID != undefined) && (SL_Device.pointID != null) && (SL_Device.pointID.length > 0))
            header += "CurrentPoint: " + SL_Device.pointID + " ";

        if ((SL_Device.type.model != null) && (SL_Device.type.model.length > 0))
            header += "Model: " + SL_Device.type.model + " ";
    }

    if ((source != null) & (source.length > 0))
        header += "Source: " + source + " ";

    window.external.LogMessage(header + "Message: " + message);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_LogErrorgMessage
///
/// <summary>
/// Formats the specified message text with header information and adds it to the HMIWeb log file with an implied message type of "ERROR".
/// </summary>
///
/// <param name="source">
/// A string containing the name of the function that generated the error message.
/// </param>
///
/// <param name="message">
/// A string containing the main content of the error message to be logged.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_LogErrorgMessage(currentPoint, source, message)
{
    SL_LogMessage("ERROR", currentPoint, source, message);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_LogErrorMessage
///
/// <summary>
/// Formats the specified message text with header information and adds it to the HMIWeb log file with an implied message type of "ERROR".
/// </summary>
///
/// <param name="source">
/// A string containing the name of the function that generated the error message.
/// </param>
///
/// <param name="message">
/// A string containing the main content of the error message to be logged.
/// </param>
///
/// <param name="error">
/// A reference to the error condition caught by the try-catch block.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_LogErrorMessage(source, message, error)
{
    if ((error != undefined) && (error != null))
    {
        SL_LogMessage("ERROR", source, message + ", Error details follow on next line.");
        window.external.LogMessage(error);
    }
    else
        SL_LogMessage("ERROR", source, message);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_LogDebugLevelMessage
///
/// <summary>
/// Formats the specified message text with header information and adds it to the HMIWeb log file with an implied message type of "Debug",
/// provided that the global SL_DebugLevel flag has been set to the appropriate level. This use of a debug flag allows the code to be
/// written with trace information that can be tweaked at run time to aid in detecting problems without overloading the log file during
/// normal conditions.
///</summary>
///
/// <remarks>
/// While the interpretation of the debug levels is not strictly defined, it is expected that debug level 1 will just track event firing
/// and level 2 will track function entry and exit. 
/// </remarks>
///
/// <param name="requiredDebugLevel">
/// An integer value that is compared against the global SL_DebugLevel setting to determine if the message ahould be logged.
/// </param>
///
/// <param name="source">
/// A string containing the name of the function that generated the message.
/// </param>
///
/// <param name="message">
/// A string containing the main content of the message to be logged.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_LogDebugLevelMessage(requiredDebugLevel, source, message)
{
    if (SL_DebugLevel >= requiredDebugLevel)
        SL_LogMessage("Debug", source, message);
}

function SL_LogDebugMessage(currentPoint, source, message)
{
    if (SL_DebugLevel > 0)
        SL_LogMessage("Debug", source, message);
}

function SL_LogFunctionEntry(currentPoint, source)
{
    SL_LogDebugLevelMessage(1, source, "Entering Function");
}

function SL_LogFunctionExit(currentPoint, source)
{
    SL_LogDebugLevelMessage(1, source, "Leaving Function");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_LogEvent
///
/// <summary>
/// Formats the specified function name and parameter info text strings with header information and adds it to the HMIWeb log file with an
/// implied message type of "Debug", provided that the global SL_DebugLevel flag has been set to the appropriate level. 
/// </summary>
///
/// <param name="requiredDebugLevel">
/// An integer value that is compared against the global SL_DebugLevel setting to determine if the message ahould be logged.
/// </param>
///
/// <param name="object">
/// A string containing the name of the function that generated the message.
/// </param>
///
/// <param name="event">
/// A string containing the names and values of the parameters provided to the function.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_LogEvent(object, event)
{
    SL_LogDebugLevelMessage(1, object.id, "Event = " + event);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_LogFunctionParameters
///
/// <summary>
/// Formats the specified function name and parameter info text strings with header information and adds it to the HMIWeb log file with an
/// implied message type of "Debug", provided that the global SL_DebugLevel flag has been set to the appropriate level. 
/// </summary>
///
/// <param name="requiredDebugLevel">
/// An integer value that is compared against the global SL_DebugLevel setting to determine if the message ahould be logged.
/// </param>
///
/// <param name="source">
/// A string containing the name of the function that generated the message.
/// </param>
///
/// <param name="parameters">
/// A string containing the names and values of the parameters provided to the function.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_LogFunctionParameters(requiredDebugLevel, source, parameters)
{
    SL_LogDebugLevelMessage(requiredDebugLevel, source, "Parameters: " + parameters);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_FetchScriptData
///
/// <summary>
/// Returns the status and value infomation associated with a specific point.parameter identified in the ScriptData list of an HMIWeb
/// UI element.
/// </summary>
///
/// <param name="dataProvider">
/// Reference to the UI element that contains the desired parameter in its ScriptData list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point that contains the desired parameter. 
/// Note that this name may be an alias name that is dynamically reassigned through the use of custom properites.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the desired parameter within the context of the above point.
/// Note that this name may be an alias name that is dynamically reassigned through the use of custom properites.
/// </param>
///
/// <returns>
/// A new SL_ScriptData object containing the status and value information for the scripted parameter.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_FetchScriptData(dataProvider, pointName, parameterName)
{
    var scriptData = new SL_ScriptData(parameterName);

    try
    {
        scriptData.Fetch(dataProvider, pointName);
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_FetchScriptData", "Could not fetch ScriptData: " + "dataProvider = " + dataProvider + ", point = " + pointName + ", parameter = " + parameterName, error);
        scriptData.exists = false;
    }

    return scriptData;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_CheckScriptDataQuality
///
/// <summary>
/// Combines a check of the DataChanged and QualityGood properties of the specified ScriptData parameter and returns TRUE if both properties
/// are set to TRUE.
/// </summary>
///
/// <param name="dataProvider">
/// Reference to the UI element that contains the desired parameter in its ScriptData list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point that contains the parameter as it is identified in the ScriptData list of the dataProvider.
/// Note that this name may be an alias name that dynamically reassigned through the use of custom properites.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the desired parameter within the context of the above point.
/// </param>
///
/// <returns>
/// TRUE if the parameter has changed and the quality is good; FALSE otherwise.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_CheckScriptDataQuality(dataProvider, pointName, parameterName)
{
    var dataOK = false;

    try
    {
        var scriptData = SL_FetchScriptData(dataProvider, pointName, parameterName);
        dataOK = (scriptData.exists && scriptData.changed && scriptData.valid);
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_CheckScriptDataQuality", "Could not verify ScriptData: " + "dataProvider = " + dataProvider.id + ", pointName = " + pointName + ", parameterName = " + parameterName, error);
    }

    return dataOK;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_SetVisibility
///
/// <summary>
/// Utility function to allow the Visibility property of a user control to be set through a boolean rather than the traditional "visible"
/// and "hidden" text strings.
/// </summary>
///
/// <param name="userControl">
/// Reference to the UI element whose Visibilty property is to be set.
/// </param>
///
/// <param name="makeVisible">
/// Boolean indicating if the specified user control should be made visible.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_SetVisibility(userControl, makeVisible)
{
    try
    {
        if (makeVisible)
            userControl.style.visibility = "visible";
        else
            userControl.style.visibility = "hidden";
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_SetVisibility", "Could not update visibility of user control: " + "userControl = " + userControl.id + " to " + makeVisible, error);
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_SetReverseVideo
///
/// <summary>
/// Consolidates the logic to set a user control to reverse video style.
/// </summary>
///
/// <param name="userControl">
/// Reference to the UI element whose style class is to be set.
/// </param>
///
/// <param name="leftAlign">
/// A numeric value set to 1 if the user control is to be left aligned.
/// </param>
///
/// <param name="controlType">
/// A string indicating the content of the user control such as "Numeric", "Text" or "Label" which is part of hte complet estyle name.
/// </param>
///
/// <returns>
/// None
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_SetReverseVideo(userControl, leftAlign, controlType)
{
    try
    {
        var styleClass = "SL_" + controlType;

        if (leftAlign)
            styleClass += "_Left";

        userControl.StyleClass = styleClass + "_BadQuality";
    }
    catch (error)
    {
        var userControlID;
        if ((userControl != undefined) || (userControl != null))
            userControlID = userControl.id;

        SL_LogErrorMessage("SL_SetVisibility", "Could not set reverse video: userControl = " + userControlID + ", leftAlign = " + leftAlign + ", controlType = " + controltype, error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_UpdateEnumeratedParameter
///
/// <summary>
/// Consolidates the logic to update a user control bound to a point parameter that has an Enumeration data type. The logic includes
/// checking the DataChanged and DataQuality flags of the bound parameter before trying to obtain the string corresponding to the DataValue
/// property of the parameter. If a valid string is obtained from the parameter value then the style of the user control is set to the
/// normal presentation of background and text colors, otherwise the user control style is set to the corresponding reverse video effect.
/// </summary>
///
/// <param name="dataProvider">
/// Reference to the UI element that contains the desired parameter in its ScriptData list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point that contains the parameter as it is identified in the ScriptData list of the dataProvider.
/// Note that this name may be an alias name that dynamically reassigned through the use of custom properites.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the desired parameter within the context of the above point.
/// </param>
///
/// <param name="enumeration">
/// A string containing the name of the JavaScript object that contains the value-string pairs that make up the enumeration data type.
/// </param>
///
/// <param name="property">
/// A string containing the name of the property of above enumeration object that contains the string to display for the corresponding key value.
/// </param>
///
/// <param name="indicatorStyle">
/// A string identifying the overall style of the user control such as "Text" or "Label". This will be combined with the "SL_" header and
/// either "_Normal" or "_BadQuality" to determine the style to apply to the user control.
/// </param>
///
/// <returns>
/// A string containing the text written to the control, which will be an empty string if the user control was not updated.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_UpdateEnumeratedParameter(dataProvider, pointName, parameterName, enumeration, property, indicatorStyle)
{
    var enumerationEntry;
    var enumeratedString;
    var dataValue;

    try
    {
        SL_LogFunctionParameters(4, "SL_UpdateEnumeratedParameter", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName + ", enumeration = " + enumeration + ", property = " + property + ", style = " + indicatorStyle);

        var scriptData = SL_FetchScriptData(dataProvider, pointName, parameterName);
        if (scriptData.exists && scriptData.changed)
        {
            if (scriptData.valid)
            {
                dataValue = scriptData.value;
                enumerationEntry = this[enumeration][scriptData.value];

                if ((enumerationEntry != undefined) && (enumerationEntry != null))
                {
                    enumeratedString = enumerationEntry[property];

                    if ((enumeratedString != undefined) && (enumeratedString != null))
                    {
                        dataProvider.value = enumeratedString;
                        dataProvider.StyleClass = "SL_" + indicatorStyle + "_Normal";
                    }
                }

                SL_LogDebugLevelMessage(5, "SL_UpdateEnumeratedParameter", parameterName + " = " + scriptData.value + " : " + enumeratedString);
            }
            else
                dataProvider.StyleClass = "SL_" + indicatorStyle + "_BadQuality";
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_UpdateEnumeratedParameter", "Could not decode enumeration: " + "dataProvider = " + dataProvider + ", point = " + pointName + ", parameter = " + parameterName + ", enumeration = " + enumeration + ", property = " + property + ", style = " + indicatorStyle + ", value = " + dataValue + ", string = " + enumeratedString, error);
    }

    return enumeratedString;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetHARTStandardUnit
///
/// <summary>
/// Returns the unit code object from the HART Standard Unit Codes table corresponding to the specified code.
/// </summary>
///
/// <param name="unitCode">
/// Numeric value which should correspond to one of the valid HART standard unit codes.
/// </param>
///
/// <returns>
/// A object from the HART_StandardUnitCodes array with the property, name, symbol and short symbol for the specified units, or NULL if
/// the specified numeric code could not be found in the table.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetHARTStandardUnit(unitCode)
{
    for (var index = 0; index < HART_StandardUnitCodes.length; index++)
    {
        if (HART_StandardUnitCodes[index].code == unitCode)
        {
            return HART_StandardUnitCodes[index];
        }
    }

    return null;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetHARTExpansionUnit
///
/// <summary>
/// Returns the unit code object from the HART Expansion Unit Codes tables corresponding to the specified classification code and unit code.
/// </summary>
///
/// <param name="classification">
/// String indicating which device variable classification should be used to index into the expansion code tables.
/// </param>
///
/// <param name="unitCode">
/// Numeric value which should correspond to one of the valid HART expansion unit codes for the above specified classification.
/// </param>
///
/// <returns>
/// A object from the HART_ExpansionUnitCodes array with the property, name, symbol and short symbol for the requested units, or NULL if
/// the specified numeric codes could not be found in the tables.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetHARTExpansionUnit(classification, unitCode)
{
    try
    {
        // Determine if the Standard Unit Table or the Expansion Unit Tables must be used.
        if ((unitCode >= 170) && (unitCode <= 219))
        {
            for (var index = 0; index < HART_ExpansionUnitCodes.length; index++)
            {
                if ((HART_ExpansionUnitCodes[index].classification == classification) &&
                    (HART_ExpansionUnitCodes[index].code == unitCode))
                {
                    
                    return HART_ExpansionUnitCodes[index];
                }
            }
        }
        else
            return SL_GetHARTStandardUnit(unitCode);
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_GetHARTExpansionUnit", "Could not get HART units: " + "classification = " + classification + ", unitCode = " + unitCode, error);
    }

    return null;
}


function HART_GetIndexFromStandardUnitCode(unitCode)
{
    var index;

    for (index = 0; index < HART_StandardUnitCodes.length; index++)
    {
        if (unitCode == HART_StandardUnitCodes[index].code)
        {
            return index;
        }
    }

    return -1;
}

    function HART_GetNameFromStandardUnitCode(unitCode)
    {
        var index = HART_GetIndexFromStandardUnitCode(unitCode);

        if (index >= 0)
        {
            return HART_StandardUnitCodes[index].name;
        }

        return "";
    }

    function HART_GetPropertyFromStandardUnitCode(unitCode)
    {
        var index = HART_GetIndexFromStandardUnitCode(unitCode);

        if (index >= 0)
        {
            return HART_StandardUnitCodes[index].property;
        }

        return "";
    }

    function HART_GetSymbolFromStandardUnitCode(unitCode)
    {
        var index = HART_GetIndexFromStandardUnitCode(unitCode);

        if (index >= 0)
        {
            return HART_StandardUnitCodes[index].symbol;
        }

        return "";
    }

    function HART_GetShortSymbolFromStandardUnitCode(unitCode)
    {
        var index = HART_GetIndexFromStandardUnitCode(unitCode);

        if (index >= 0)
        {
            if (HART_StandardUnitCodes[index].shortSymbol != "")
            {
                return HART_StandardUnitCodes[index].shortSymbol;
            }
            else
            {
                return HART_StandardUnitCodes[index].symbol;
            }
        }

        return "";
    }

    function SL_GetNumDecimals(value)
    {
        var absValue = Math.abs(value);
        var numDecimals;

        if (absValue == 0)
        {numDecimals = 0;}
        else if (absValue < 0.01)
        {numDecimals = 4;}
        else if (absValue < 1.0)
        {numDecimals = 3;}
        else if (absValue < 100.0)
        {numDecimals = 2;}
        else if (absValue < 1000.0)
        {numDecimals = 1;}
        else {numDecimals = 0;}

        return numDecimals;
    }


    function SL_GetTransformCharType(code)
    {
        var i;

        for (i = 0; i < transformCharModeArray.length; i++)
        {
            if (code == transformCharModeArray[i].code)
            {
                return transformCharModeArray[i].mode;
            }
        }
        return "";
    }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Faceplate Functions
///
/// Functions in this section are designed to implement common functionality that relate to the faceplate UI elements of all SmartLine
/// transmitters.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_DynamicVariable_Initialize
///
/// <summary>
/// This function provides the dual functionality of updating the name and symbol of one of the HART Dynamic Variables, either
/// PV, SV, TV or QV, as well as hiding the full shape containing the dynamic variable if the selected variable is not valid for the
/// current configuration. The validity of the dynamic variables can chnaged dpending on the transmitter module type, such as the single
/// and dual input temperature transmitter module types. For those transmitter families that have more than 4 device variables, such as
/// level and multivariable, different variables can be mapped to the 4 dynamic variables.
///
/// To accomodate these complexities, the data required to populate the shape correctly is contained within the global device model
/// object. This data is read by the shape functions but it is the responsibility of others to ensure that the data is correct based
/// on configuartion parameters read from the transmitter.
/// </summary>
///
/// <param name="shape">
/// A reference to the HMIWeb display object of type SL_DynamicVariable shape for which the name and visibility should be updated.
/// </param>
///
/// <returns>
/// None
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_DynamicVariable_Initialize(shape)
{
    try
    {
        SL_LogFunctionParameters(2, "SL_DynamicVariable_Initialize", "shape.id = " + shape.id);

        // Determine which of the 4 HART Dynamic Variables this shape is configured for and select the
        // corresponding variable definition. It is expected that the variable definitions have already
        // been updated with the correct symbol, name and visibility properties appropriate for the
        // selected SmartLine transmitter.
        var variableDefinition;
        var variableType = shape.GetCustomProperty("Value", "VariableType");

        switch (variableType)
        {
            case "PV": variableDefinition = SL_Device.dynamicVariables.PV; break;
            case "SV": variableDefinition = SL_Device.dynamicVariables.SV; break;
            case "TV": variableDefinition = SL_Device.dynamicVariables.TV; break;
            case "QV": variableDefinition = SL_Device.dynamicVariables.QV; break;

            default: break;
        }

        // Update the shape object with the properties from the variable definition.
        if (variableDefinition != undefined)
        {
            shape.style.visibility = variableDefinition.visibility;

            if (shape.style.visibility != "hidden")
            {
                shape.objects("textboxVariableSymbol").Value = variableDefinition.symbol;
                shape.objects("textboxVariableSymbol").title = variableDefinition.tooltip;
                shape.objects("alphaVariableName").Value = variableDefinition.name;
            }
        }

        SL_LogDebugLevelMessage(4, "SL_DynamicVariable_Initialize", "Type = " + variableDefinition.symbol + ", Visibility = " + variableDefinition.visibility + ", Name = " + variableDefinition.name + ", Tooltip = " + variableDefinition.tooltip);
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_DynamicVariable_Initialize", "Could not initialize Dynamic Variable: " + "shape = " + shape.id + ", variableType = " + variableType + ", variableDefinition = " + variableDefinition.symbol, error);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_DynamicVariable_SetValue
///
/// <summary>
/// This function provides the functionality of updating the Value of one of the HART dynamic variables (PV, SV, TV and QV) on the faceplate.
/// </summary>
///
/// <param name="shape">
/// A reference to the HMIWeb display object of type SL_DynamicVariable shape for which the value should be updated.
/// </param>
///
/// <returns>
/// None
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_DynamicVariable_SetValue(shape)
{
    var variableValue = new SL_ScriptData("ValueParameter");
    var variableStatus = new SL_ScriptData("StatusParameter");

    try
    {
        SL_LogFunctionParameters(2, "SL_DynamicVariable_SetValue", "shape.id = " + shape.id);

        // Get a reference to the alphanumeric user control that will display the value of the dynamic variable.
        var alphaVariableValue = shape.objects("alphaVariableValue");

        // Check to see if the point parameter associated with the dynamic variable's value has a good value.
        // If not, do not update any of the user controls
        variableValue.Fetch(alphaVariableValue, "SLAnalogPoint");
        if (variableValue.changed && variableValue.valid)
        {
            // Determine if a fixed number of decimal places should be used or if it should be based on the value.
            var useNumDecimals = shape.GetCustomProperty("Value", "UseNumDecimals");
            var numDecimals = shape.GetCustomProperty("Value", "NumDecimals");
            if (useNumDecimals == 0)
                numDecimals = SL_GetNumDecimals(variableValue.value);

            // Update the value in the user control.
            alphaVariableValue.value = variableValue.value.toFixed(numDecimals);

            // Check to see if there is a valid Status parameter for this dynamic variable.
            // Generally this will depend on which command the HART I/O module will use to read the dynamic variables; 3 or 9.
            var ignoreStatus = shape.parentElement.parentElement.GetCustomProperty("Value", "IgnoreStatus");
            var textColor = statusDisplayScheme[0].textColorDarkBG;

            if (ignoreStatus == 0)
            {
                variableStatus.Fetch(alphaVariableValue, "SLAnalogPoint");
                if (variableStatus.changed && variableStatus.valid)
                    textColor = SL_GetHARTDeviceVariableStatus(variableStatus.value).textColorDarkBG;
            }

            // Set the text color matching the Status for both the dynamic variable value and the corresponding units.
            alphaVariableValue.textColor = textColor;
            shape.objects("alphaUnit").textColor = textColor;
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_DynamicVariable_SetValue", "Could not set dynamic variable value: " + "value = " + variableValue.value + ", statusCode = " + variableStatus.value, error);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetHARTDeviceVariableStatus
///
/// <summary>
/// Utility function to decode the numeric code for the status of a dynamic variable into a color scheme and tooltip for the user controls.
/// </summary>
///
/// <param name="statusCode">
/// The numeric value of the status code to be decoded.
/// </param>
///
/// <returns>
/// Object with the text, font color, line color and fill color corresponding to the numeric status code.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetHARTDeviceVariableStatus(statusCode)
{
    var i;
    var j;

    for (i = 0; i < HART_DeviceVariableStatuses.length; i++)
    {
        if ((statusCode >= HART_DeviceVariableStatuses[i].lowIndex) && (statusCode <= HART_DeviceVariableStatuses[i].highIndex))
        {
            for (j = 0; j < statusDisplayScheme.length; j++)
            {
                if (HART_DeviceVariableStatuses[i].type == statusDisplayScheme[j].type)
                {
                    return {
                        processDataStatus: HART_DeviceVariableStatuses[i].processDataStatus,
                        limitStatus: HART_DeviceVariableStatuses[i].limitStatus,
                        moreStatusAvailable: HART_DeviceVariableStatuses[i].moreStatusAvailable,
                        fillColor: statusDisplayScheme[j].fillColor,
                        lineColor: statusDisplayScheme[j].lineColor,
                        textColorDarkBG: statusDisplayScheme[j].textColorDarkBG
                    };
                }
            }
        }
    }

    return "";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_AnalogPV_Initialize
///
/// <summary>
/// This function provides the dual functionality of updating the name and symbol of the analog Process Variable (PV) calculated from the
/// physical loop current by the Experion C300 I/O module. This variable is always available so there is no need to hide the user controls
/// associated with this process variable, although the name of the property being measured can be variable in some SmartLine transmitters.
/// </summary>
///
/// <param name="shape">
/// A reference to the HMIWeb display object of type SL_AnalogPV shape for which the name and visibility should be updated.
/// </param>
///
/// <returns>
/// None
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_AnalogPV_Initialize(shape)
{
    var variableDefinition = SL_Device.analogPV;

    try
    {
        SL_LogFunctionParameters(2, "SL_AnalogPV_Initialize", "shape = " + shape.id);

        if (variableDefinition != undefined)
        {
            shape.style.visibility = variableDefinition.visibility;

            if (shape.style.visibility != "hidden")
            {
                shape.objects("textboxVariableSymbol").Value = variableDefinition.symbol;
                shape.objects("alphaVariableName").Value = variableDefinition.name;
            }
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_AnalogPV_Initialize", "Could not initialize Analog PV " + "shape = " + shape + ", variable definition = " + variableDefinition.symbol, error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_AnalogPV_SetValue
///
/// <summary>
/// This function provides the functionality of updating the Value of the analog PV on the faceplate.
/// </summary>
///
/// <param name="shape">
/// A reference to the HMIWeb display object of type SL_AnalogPV shape for which the value should be updated.
/// </param>
///
/// <returns>
/// None
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_AnalogPV_SetValue(shape)
{
    var variableValue = new SL_ScriptData("PV");
    var variableStatus = new SL_ScriptData("PVSTS");

    try
    {
        SL_LogFunctionParameters(2, "SL_AnalogPV_SetValue", "shape.id = " + shape.id);

        // Get a reference to the alphanumeric user control that will display the value of the dynamic variable.
        var alphaVariableValue = shape.objects("alphaVariableValue");

        // Check to see if the point parameter associated with the dynamic variable's value has a good value.
        // If not, do not update any of the user controls
        variableValue.Fetch(alphaVariableValue, "SLAnalogPoint");
        if (variableValue.changed && variableValue.valid)
        {
            // Determine if a fixed number of decimal places should be used or if it should be based on the value.
            var useNumDecimals = shape.GetCustomProperty("Value", "UseNumDecimals");
            var numDecimals = shape.GetCustomProperty("Value", "NumDecimals");
            if (useNumDecimals == 0)
                numDecimals = SL_GetNumDecimals(variableValue.value);

            // Update the value in the user control.
            alphaVariableValue.value = variableValue.value.toFixed(numDecimals);

            // Check to see if there is a valid Status parameter for this dynamic variable.
            var ignoreStatus = shape.GetCustomProperty("Value", "IgnoreStatus");
            var textColor = statusDisplayScheme[0].textColorDarkBG;

            if (ignoreStatus == 0)
            {
                variableStatus.Fetch(alphaVariableValue, "SLAnalogPoint");
                if (variableStatus.changed && variableStatus.valid)
                    textColor = SL_GetAnalogPVStatus(variableStatus.value).textColorDarkBG;
            }

            // Set the text color matching the Status for both the dynamic variable value and the corresponding units.
            alphaVariableValue.textColor = textColor;
            shape.objects("alphaUnit").textColor = textColor;
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_AnalogPV_SetValue", "Could not set analog PV value: " + "value = " + variableValue.value + ", statusCode = " + variableStatus.value, error);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetAnalogPVStatus
///
/// <summary>
/// Utility function to decode the numeric code for the status of the analog PV into a color scheme and tooltip for the user controls.
/// </summary>
///
/// <param name="statusCode">
/// The numeric value of the status code to be decoded.
/// </param>
///
/// <returns>
/// Object with the text, font color, line color and fill color corresponding to the numeric status code.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetAnalogPVStatus(statusCode)
{
    var i;
    var j;

    for (i = 0; i < analogPVStatuses.length; i++)
    {
        if (statusCode == analogPVStatuses[i].code)
        {
            for (j = 0; j < statusDisplayScheme.length; j++)
            {
                if (analogPVStatuses[i].type == statusDisplayScheme[j].type)
                {
                    return {
                        statusString: analogPVStatuses[i].statusString,
                        fillColor: statusDisplayScheme[j].fillColor,
                        lineColor: statusDisplayScheme[j].lineColor,
                        textColorDarkBG: statusDisplayScheme[j].textColorDarkBG
                    };
                }
            }
        }
    }

    return "";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_SetProcessVariableUnits
///
/// <summary>
/// This function decodes the unit code read for one of the process variables and sets the unit string field to the short symbol for
/// that unit and the tooltip to the long symbol for that unit.
/// </summary>
///
/// <param name="unitsIndicator">
/// A reference to the HMIWeb display object of type SL_DynamicVariable.sha or SL_AnalogPV.sha for which the units symbol and tooltip
/// should be updated.
/// </param>
///
/// <returns>
/// None
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_SetProcessVariableUnits(unitsIndicator)
{
    var shape = unitsIndicator.parentElement.parentElement;
    var unitCode = new SL_ScriptData("UnitParameter");
    var classification = "";
    var unit = null;
    var symbol = "";
    var tooltip = "";

    try
    {
        SL_LogFunctionParameters(2, "SL_SetProcessVariableUnits", "shape = " + shape.id);

        // Determine the parameter name needed to fetch the script data. This is different for the analog PV and the Dynamic variables.
        unitCode.parameter = shape.GetCustomProperty("Parameter", "UnitParameter");
        unitCode.Fetch(unitsIndicator, "SLAnalogPoint");

        if (unitCode.changed && unitCode.valid)
        {
            // Determine if the Standard Unit Table or the Expansion Unit Tables must be used.
            if ((unitCode.value >= 170) && (unitCode.value <= 219))
            {
                // Determine which of the 4 HART Dynamic Variables this shape is configured for and select the
                // corresponding variable definition. It is expected that the variable definitions have already
                // been updated with the correct classification.
                switch (unitCode.parameter)
                {
                    case "HEU":       classification = SL_Device.analogPV.classification; break;
                    case "HDYNEU[1]": classification = SL_Device.dynamicVariables.PV.classification; break;
                    case "HDYNEU[2]": classification = SL_Device.dynamicVariables.SV.classification; break;
                    case "HDYNEU[3]": classification = SL_Device.dynamicVariables.TV.classification; break;
                    case "HDYNEU[4]": classification = SL_Device.dynamicVariables.QV.classification; break;

                    default: break;
                }
                unit = SL_GetHARTExpansionUnit(classification, unitCode.value);
            }
            else
            {
                unit = SL_GetHARTStandardUnit(unitCode.value);
            }

            if (unit != null)
            {
                tooltip = unit.symbol;
                if (unit.shortSymbol.length > 0)
                    symbol = unit.shortSymbol;
                else
                    symbol = tooltip;
            }

            unitsIndicator.value = symbol;
            unitsIndicator.title = tooltip;

            SL_LogDebugLevelMessage(4, "SL_SetProcessVariableUnits", "shape = " + shape.id + ", classification = " + classification + ", unitCode = " + unitCode.value + ", symbol = " + symbol + ", tooltip = " + tooltip);
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_SetProcessVariableUnits", "Could not set PV units " + "shape = " + shape.id + ", unitParameter = " + unitCode.parameter + ", classification = " + classification + ", unitCode = " + unitCode.value, error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetDynamicVariableNames
///
/// <summary>
/// </summary>
///
/// <param name="dataProvider">
/// </param>
///
/// <returns>
/// None
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetDynamicVariableNames(dataProvider)
{
    SL_GetDynamicVariableName(dataProvider, SL_Device.dynamicVariables.PV);
    SL_GetDynamicVariableName(dataProvider, SL_Device.dynamicVariables.SV);
    SL_GetDynamicVariableName(dataProvider, SL_Device.dynamicVariables.TV);
    SL_GetDynamicVariableName(dataProvider, SL_Device.dynamicVariables.QV);

    SL_Device.analogPV.name = SL_Device.dynamicVariables.PV.name;
    SL_Device.analogPV.classification = SL_Device.dynamicVariables.PV.classification;
};


function SL_GetDynamicVariableName(dataProvider, variableDefinition)
{
    try
    {
        SL_LogFunctionParameters(2, "SL_GetDynamicVariableName", "dataProvider = " + dataProvider.id + ", dynamicVariable = " + variableDefinition.symbol);

        var deviceVariableCode = new SL_ScriptData("HDYNDVC[" + variableDefinition.index + "]");
        
        // Get the numeric value of the HART device variable code from the script data list.
        deviceVariableCode.Fetch(dataProvider, "CurrentPoint");

        if (deviceVariableCode.changed && deviceVariableCode.valid)
        {
            variableDefinition.name = SL_Device.deviceVariables[deviceVariableCode.value].name;
            variableDefinition.classification = SL_Device.deviceVariables[deviceVariableCode.value].classification;
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_GetDynamicVariableName", "Could not get dynamic variable name: " + "dataProvider = " + dataProvider + ", variableDefinition = " + variableDefinition, error);
    }
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Faults and Resolutions Functions
///
/// Functions in this section are designed to implement common functionality that relate to obtaining and displaying the fault conditions
/// that are currently active in a tranmsiiter and the associated possible resolution steps.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function SL_GetFaultInfo(cmdType, byteIndex, bitIndex)
{
    var faultInfoArray;

    switch (cmdType)
    {
        case 48:
            faultInfoArray = SL_Device.command48.statusBits;
            break;

        case 220:
            faultInfoArray = SL_Device.command220.statusBits;
            break;

        default:
            break;
    }


    if ((faultInfoArray != undefined) && (faultInfoArray.length > 0))
    {
        for (i = 0; i < faultInfoArray.length; i++)
        {
            var faultInfo = faultInfoArray[i];

            if ((faultInfo.byte == byteIndex) && (faultInfo.bit == bitIndex))
            {
                SL_LogDebugLevelMessage(5, "SL_GetFaultInfo", "Command = " + cmdType.toString() + ", Byte = " + faultInfo.byte + ", Bit = " + faultInfo.bit + ", name = " + faultInfo.name);
                return faultInfo;
            }
        }

        SL_LogDebugLevelMessage(3, "SL_GetFaultInfo", "Could not find fault info for Command = " + cmdType.toString() + ", Byte = " + byteIndex + ", Bit = " + bitIndex);
        return { criticality: 1, name: "Command " + cmdType.toString() + " Byte " + byteIndex + " Bit " + bitIndex, description: "Not available. Check device user's manual.", resolution: "Not available. Check device user's manual." };
    }

    return null;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_UpdateFaults
///
/// <summary>
/// R
/// </summary>
///
/// <param name="dataProvider">
/// </param>
///
/// <param name="pointName">
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_UpdateFaults(dataProvider, pointName)
{
    SL_LogFunctionParameters(2, "SL_UpdateFaults", "dataProvider = " + dataProvider.id + ", pointName = " + pointName);

    SL_Device.criticalFaults.length = 0;
    SL_Device.nonCriticalFaults.length = 0;
    SL_Device.allFaults.length = 0;

    if ((SL_Device.expectedModel != undefined) && (SL_Device.command48.numberOfBytes > 0) && (SL_Device.command220.numberOfBytes > 0))
    {
        try
        {
            var communicationFault = dataProvider.DataValue(pointName + ".HCOMERFL");

            //Read the device status bits specified by the global variable IOM_DeviceStatuses
            var i;
            var statusName;
            var faultInfo;

            for (i = 0; i < IOM_DeviceStatuses.length; i++)
            {
                var statusName = pointName + "." + IOM_DeviceStatuses[i].parameter;

                if (dataProvider.DataValue(statusName) == 1)
                {
                    faultInfo =
                    {
                        criticality: IOM_DeviceStatuses[i].criticality,
                        name: IOM_DeviceStatuses[i].name,
                        description: IOM_DeviceStatuses[i].description,
                        resolution: IOM_DeviceStatuses[i].resolution
                    };

                    switch (faultInfo.criticality)
                    {
                        case 0:
                            SL_LogDebugLevelMessage(4, "SL_UpdateFaults", "I/O Module - Critical: name = " + faultInfo.name);
                            SL_Device.criticalFaults.push(faultInfo);
                            break;

                        case 1:
                        default:
                            SL_LogDebugLevelMessage(4, "SL_UpdateFaults", "I/O Module - Non-Critical: name = " + faultInfo.name);
                            SL_Device.nonCriticalFaults.push(faultInfo);
                            break;
                    }
                }
            }

            if (communicationFault != 1)
            {
                //Read the command 48 status info on a per bit manner from the HCMD48BT array to gather all active status bits
                var j;
                var byteIndex;
                var bitIndex;

                for (i = 0; i < (8 * SL_Device.command48.numberOfBytes) ; i++)
                {
                    j = i + 1;
                    statusName = pointName + ".HCMD48BT[" + j.toString() + "]";

                    if (dataProvider.DataValue(statusName) == 1)
                    {
                        byteIndex = Math.floor(i / 8);
                        bitIndex = ((i / 8) - Math.floor(i / 8)) * 8;
                        
                        faultInfo = SL_GetFaultInfo(48, byteIndex, bitIndex);

                        if (faultInfo != null)
                        {
                            switch (faultInfo.criticality)
                            {
                                case 0:
                                    SL_LogDebugLevelMessage(4, "SL_UpdateFaults", "Command 48 - Critical: name = " + faultInfo.name);
                                    SL_Device.criticalFaults.push(faultInfo);
                                    break;

                                case 1:
                                default:
                                    SL_LogDebugLevelMessage(4, "SL_UpdateFaults", "Command 48 - Non-Critical: name = " + faultInfo.name);
                                    SL_Device.nonCriticalFaults.push(faultInfo);
                                    break;
                            }
                        }
                        else
                            SL_LogDebugLevelMessage(3, "SL_UpdateFaults", "Could not get definition for Command 48 Fault: Byte = " + byteIndex + ", Bit = " + bitIndex);
                    }
                }

                //Read the command 220 status info on a per byte manner to gather all active status bits
                var statusByte = 0;

                for (i = 0; i < SL_Device.command220.numberOfBytes; i++)
                {
                    statusName = pointName + ".detailed_status_" + i.toString();
                    statusByte = dataProvider.DataValue(statusName);

                    for (j = 0; j < 8; j++)
                    {
                        if ((statusByte & Math.pow(2, j)) > 0)
                        {
                            byteIndex = i;
                            bitIndex = j;

                            faultInfo = SL_GetFaultInfo(220, byteIndex, bitIndex);

                            if (faultInfo != null)
                            {
                                switch (faultInfo.criticality)
                                {
                                    case 0:
                                        SL_LogDebugLevelMessage(4, "SL_UpdateFaults", "Command 220 - Critical: name = " + faultInfo.name);
                                        SL_Device.criticalFaults.push(faultInfo);
                                        break;

                                    case 1:
                                    default:
                                        SL_LogDebugLevelMessage(4, "SL_UpdateFaults", "Command 220 - Non-Critical: name = " + faultInfo.name);
                                        SL_Device.nonCriticalFaults.push(faultInfo);
                                        break;
                                }
                            }
                            else
                                SL_LogDebugLevelMessage(3, "SL_UpdateFaults", "Could not get definition for Command 220 Fault: Byte = " + byteIndex + ", Bit = " + bitIndex);
                        }
                    }
                }
            }

            SL_Device.allFaults = SL_Device.criticalFaults.concat(SL_Device.nonCriticalFaults);
        }
        catch (error)
        {
            SL_LogErrorMessage("SL_UpdateFaults", "Could not update fault info: " + "dataProvider = " + dataProvider.id + ", pointName = " + pointName, error);
        }
    }
    else
        SL_LogDebugLevelMessage(2, "SL_UpdateFaults", "Device Type not set!!" );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Device Type Functions
///
/// Functions in this section are designed to implement common functionality that relate to getting information about the device type.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetIndexOfDeviceInfo
///
/// <summary>
/// Returns the index of the element in the SL_SmartLineDeviceTypes array that matches the specified numeric type code.
/// </summary>
///
/// <param name="typeCode">
/// An integer value that identifies the desired SmartLine transmitter type, such as 5926 for the SLG700 GWR Level family.
/// </param>
///
/// <returns>
/// The index of the object containing the device info for the specified type code; or -1 if the speified type code was not found.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetIndexOfDeviceInfo(typeCode)
{
    var index;

    for (index = 0; index < SL_SmartLineDeviceTypes.length; index++)
    {
        if (typeCode == SL_SmartLineDeviceTypes[index].typeCode)
            return index;
    }

    return -1;
}


function SL_UpdateDeviceType(dataProvider, pointName)
{
    // Assume failure unless proven otherwise!
    var deviceType = new SL_ScriptData("HDEVTYPE7");
    var deviceTypeStatus = false;

    try
    {
        SL_LogFunctionParameters(2, "SL_UpdateDeviceType", "dataProvider = " + dataProvider.id + ", pointName = " + pointName);

        deviceType.Fetch(dataProvider, pointName);
        if (deviceType.changed && deviceType.valid)
        {
            if ((SL_Device.type == undefined) || (SL_Device.type.typeCode != deviceType.value))
            {
                var index = SL_GetIndexOfDeviceInfo(deviceType.value);
                if (index >= 0)
                {
                    SL_Device.type = SL_SmartLineDeviceTypes[index];
                    deviceTypeStatus = true;
                    SL_LogDebugLevelMessage(3, "SL_UpdateDeviceType", "typeCode = " + SL_Device.type.typeCode + ", model = " + SL_Device.type.model);
                }
                else
                    SL_LogDebugLevelMessage(3, "SL_UpdateDeviceType", "HDEVTYPE7 parameter not valid!!");
            }
        }
        else
            SL_LogDebugLevelMessage(2, "SL_UpdateDeviceType", "HDEVTYPE7 parameter not valid!!");
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_UpdateDeviceType", "Could not update device type: " + "dataProvider = " + dataProvider + ", pointName = " + pointName + ", typeCode = " + deviceType.value, error);
    }

    return deviceTypeStatus;
}

function SL_UpdateDeviceRevision(dataProvider, pointName)
{
    // Assume failure unless proven otherwise!
    var deviceRevision = new SL_ScriptData("HDEVREV");
    var deviceRevisionStatus = false;

    try
    {
        SL_LogFunctionParameters(2, "SL_UpdateDeviceRevision", "dataProvider = " + dataProvider.id + ", pointName = " + pointName);

        deviceRevision.Fetch(dataProvider, pointName);
        if (deviceRevision.changed)
        {
            if (deviceRevision.valid)
            {
                var revisionInfo = SL_Device.deviceRevisions[deviceRevision.value];
                if (revisionInfo != null)
                {
                    SL_Device.deviceRevision = revisionInfo.revision;

                    SL_Device.command48.numberOfBytes = revisionInfo.command48StatusBytes;

                    SL_Device.command220.numberOfBytes = revisionInfo.command220StatusBytes;
                    SL_Device.command220.statusBytes.length = SL_Device.command220.numberOfBytes;

                    deviceRevisionStatus = true;
                    SL_LogDebugLevelMessage(3, "SL_UpdateDeviceRevision", "revision = " + SL_Device.deviceRevision + ", Command 48 Bytes = " + SL_Device.command48.numberOfBytes + ", Command 220 Bytes = " + SL_Device.command220.numberOfBytes);
                }

                SL_Device.allFaults.length = 0;
                SL_Device.criticalFaults.length = 0;
                SL_Device.nonCriticalFaults.length = 0;
            }
            else
                SL_LogDebugLevelMessage(2, "SL_UpdateDeviceRevision", "HDEVREV parameter not valid!!");
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_UpdateDeviceRevision", "Could not update device revision: " + "dataProvider = " + dataProvider + ", pointName = " + pointName + ", revision = " + deviceRevision.value, error);
    }

    return deviceRevisionStatus;
}



function SL_UpdateDeviceInfo(dataProvider, pointName)
{
    var deviceTypeStatus = SL_UpdateDeviceType(dataProvider, pointName);
    var deviceRevisionStatus = SL_UpdateDeviceRevision(dataProvider, pointName);

    return (deviceTypeStatus && deviceRevisionStatus);
}

function SL_GetDeviceManufacturer(typeCode)
{
    var index = SL_GetIndexOfDeviceInfo(typeCode);

    if (index >= 0)
        return SL_SmartLineDeviceTypes[index].manufacturer;

    return "";
}


function SL_GetDeviceID(typeCode)
{
    var index = SL_GetIndexOfDeviceInfo(typeCode);

    if (index >= 0)
        return SL_SmartLineDeviceTypes[index].model;

    return "";
}


function SL_GetDeviceType(typeCode)
{
    var index = SL_GetIndexOfDeviceInfo(typeCode);

    if (index >= 0)
        return SL_SmartLineDeviceTypes[index].family;

    return "";
}

function SL_GetDeviceFamily(typeCode)
{
    var index = SL_GetIndexOfDeviceInfo(typeCode);

    if (index >= 0) {
        return SL_SmartLineDeviceTypes[index].family;
    }

    return "";
}

function SL_GetDeviceDescription(typeCode)
{
    var index = SL_GetIndexOfDeviceInfo(typeCode);

    if (index >= 0)
        return SL_SmartLineDeviceTypes[index].description;

    return "";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Device Details Page Functions
///
/// Functions in this section are designed to implement common functionality that relate to user interface elements on the Device Details tab.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetLongDeviceTypeString
///
/// <summary>
/// Generates a string that includes the device type code as a numeric, in both decimal and hexadecimal format, and for SmartLine devices, a
/// string containing the manufacturer, model identifier and the family, such as Pressure or Multivariable.
/// </summary>
///
/// <remarks>
/// This function is intended as an event handler for the user control used to display the Device Type parameter in both the
/// SL_IdentificationDetails and SL_IOChannelDetails shapes. Since the functionality is shared between these shapes, it is provided as a
/// global function. As an event handler it requires a reference to the user control so that it can update its value.
/// </remarks>
///
/// <param name="dataProvider">
/// A reference to the user control that contains the point parameter to be decoded as a device type code in its script data list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point as identified in the script data list of the above user control.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the parameter containing the type code as identified in the script data list of the above user control.
/// </param>
///
/// <returns>
/// None. Upon successful decoding of the type code, the string is copied directly to the specified user control.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetLongDeviceTypeString(dataProvider, pointName, parameterName)
{
	try
	{
	    SL_LogFunctionParameters(1, "SL_GetLongDeviceTypeString", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName);

	    if (SL_CheckScriptDataQuality(dataProvider, pointName, parameterName))
        {
	        var typeCode = dataProvider.DataValue(pointName + "." + parameterName);
            var typeString = "(" + typeCode + "  0x" + typeCode.toString(16).toUpperCase() + ")  ";

            if (typeCode == 251)
            {
                typeString += "Generic HART Device";
            }
            else
            {
                var typeIndex = SL_GetIndexOfDeviceInfo(typeCode);
                if (typeIndex >= 0)
                {
                    var type = SL_SmartLineDeviceTypes[typeIndex];
                    typeString += "Honeywell " + type.model + " " + type.description;
                }
                else
                {
                    typeString += "Unknown Device Type";
                }
            }

            dataProvider.Value = typeString;
        }
    }
	catch (error)
    {
	    SL_LogErrorMessage("SL_GetLongDeviceTypeString", "Could not expand device type: typeCode = " + typeCode + ", typeString = " + typeString, error);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_LocalDisplayTypeHandler
///
/// <summary>
/// Updates the type string for the SmartLine Local Display Type parameter and either hides the user controls associated with the Local Display
/// Firmware or makes them visible depending on whether or not the local display type contains firmware.
/// </summary>
///
/// <remarks>
/// This function is intended as an event handler for the user control used to display the Local Display Type and Local Display Firmware
/// parameters. Since this functionality is common to all SmartLine transmitter types, it is provided as a global function. As an event handler,
/// it requires a reference to the user control so that it can update its value.
/// </remarks>
///
/// <param name="dataProvider">
/// A reference to the user control that contains the point parameter to be decoded as a local display type code in its script data list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point as identified in the script data list of the above user control.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the parameter containing the type code as identified in the script data list of the above user control.
/// </param>
///
/// <returns>
/// None. Upon successful decoding of the type code, the string is copied directly to the specified user control.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_LocalDisplayTypeHandler(dataProvider, pointName, parameterName)
{
    // Initialize variables.
    var typeCode = new SL_ScriptData(parameterName);
    var typeString = "";
    var firmwareVisibility = "";

    try
    {
        SL_LogFunctionParameters(2, "SL_LocalDisplayTypeHandler", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName);

        // Check that the display type parameter has been changed.
        typeCode.Fetch(dataProvider, pointName);
        if (typeCode.changed)
        {
            if (typeCode.valid)
            {
                // Read the display type code and set the user control to the matching string.
                typeString = SL_LocalDisplayTypes[typeCode.value].type;

                dataProvider.StyleClass = "SL_Text_Normal";
                dataProvider.Value = typeString;

                // Only the Advanced and Basic display types have an associated firmware revision number.
                // Check the disply type string and set the visibility of the Local Display Firmware user controls accordingly.
                if ((typeString == "Advanced") || (typeString == "Basic"))
                    firmwareVisibility = "visible";
                else
                    firmwareVisibility = "hidden";

                dataProvider.document.getElementById("alphaLocalDisplayFirmware").style.visibility = firmwareVisibility;
                dataProvider.document.getElementById("textboxLocalDisplayFirmware").style.visibility = firmwareVisibility;

                SL_LogDebugLevelMessage(4, "SL_LocalDisplayTypeHandler", "code = " + typeCode.value + ", string = " + typeString + ", firmware = " + firmwareVisibility);
            }
            else
            {
                dataProvider.StyleClass = "SL_Text_BadQuality";
                dataProvider.Value = typeString;
            }
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_LocalDisplayTypeHandler", "Could not update the Local Display type user controls:  code = " + typeCode.value + ", string = " + typeString + ", firmwareVisibility = " + firmwareVisibility, error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_UpdateModelNumber
///
/// <summary>
/// Checks the ScriptData objects for all component parts of the model number, records the results in the global SL_Device object for the
/// transmitter and updates the user control if the model number is complete. Since the parameters that make up the full model number
/// can be updated in separate update messages and in random order, it is necessary to save any valid parts in the global structure until
/// all components are valid.
/// </summary>
///
/// <remarks>
/// This function is intended as an event handler for the user control used to display the Model Number on the Device Details page. Since
/// this functionality is common to all SmartLine transmitter types, it is provided as a global function. As an event handler, it
/// requires a reference to the user control so that it can update its value.
/// </remarks>
///
/// <param name="dataProvider">
/// A reference to the user control that contains the model number. This user control must have all of the point parameters that make up
/// the full model number included in its ScriptData list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point as identified in the script data list of the above user control.
/// </param>
///
/// <returns>
/// None. Upon successful decoding the model number is copied directly to the specified user control.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_UpdateModelNumber(dataProvider, pointName)
{
    try
    {
        var validModelNumber = true;
        for (var i = 0; i < SL_Device.modelNumberParts.length; i++)
        {
            SL_Device.modelNumberParts[i].Fetch(dataProvider, pointName);
            if (!SL_Device.modelNumberParts[i].valid)
                validModelNumber = false;
        }

        if (validModelNumber)
        {
            SL_Device.modelNumber = "";
            for (var i = 0; i < SL_Device.modelNumberParts.length; i++)
                SL_Device.modelNumber += SL_Device.modelNumberParts[i].value;
            SL_Device.modelNumber = SL_Device.modelNumber.trim();

            SL_LogDebugLevelMessage(4, "SL_UpdateModelNumber", "model number = " + SL_Device.modelNumber);
            dataProvider.value = SL_Device.modelNumber;
        }
        else
            SL_LogDebugLevelMessage(4, "SL_UpdateModelNumber", "Incomplete model number");
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_UpdateModelNumber", "Could not update model number: ", error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Temperature Functions
///
/// Functions in this section are designed to implement common functionality that relate to the UI elements of all SmartLine
/// Temperature transmitters.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_TemperatureSensor
///
/// <summary>
/// Function and prototype to create a pseudo class for a SmartLine temperature sensor to contain properties for:
///     number          either 1 or 2 for the first and second sensors
///     type            such as RTD or Thermocouple
///     style           such as Pt 1000, a=385 or Type J
///     numberOfWires   either 2, 3 or 4 when the type is an RTD
/// and a method:
///     getDescription()    returns a single string containing all of the properties
/// </summary>
///
/// <returns>
/// New SL_TemperatureSensor object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_TemperatureSensor(myNumber)
{
    this.number = myNumber;
    this.typeCode = new SL_ScriptData("sensor" + myNumber + "_type_code");
    this.id = new SL_ScriptData("sensor" + myNumber + "_id");
    this.rtdType = new SL_ScriptData("rtd" + myNumber + "_type");
    this.type = "";
    this.model = "";
    this.numberOfWires = "";
    this.description = "";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_TemperatureModule
///
/// <summary>
/// Function and prototype to create a pseudo class for a SmartLine temperature sensor to contain properties for:
///     type                 such as Single Input or Dual Input
///     hasThermocouple      boolean indicating if one of the temperature sesnors is configured as a Thermocouple, hence needing a Cold Junction
///     coldJunctionSource   source for the Cold Junction Temperature, such as External, Internal or Fixed
///     PvCalculationSources enumeration containing the valid PV calculation source inputs
///     PvCalculation        such as Sensor 1, Sensor 2 or Sensors 1 & 2
///     numberOfSensors      the number of sensor inputs available, 1 or 2
///
/// </summary>
///
/// <returns>
/// New SL_TemperatureSensor object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_TemperatureModule()
{
    this.type;
    this.hasThermocouple = false;
    this.coldJunctionSource;
    this.PvCalculationSources;
    this.PvCalculation;
    this.numberOfSensors = 1;
    this.inputSensorTypes;
    this.inputSensorIDs;
    this.inputSensorWires;
    this.sensors =
    [
        new SL_TemperatureSensor(1),
        new SL_TemperatureSensor(2)
    ];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetTemperatureModuleType
///
/// <summary>
/// Checks the ScriptData properties for the parameter indicating the temperature module type, either single or dual input, and updates the
/// corresponding fields in the global SL_Device variable for the transmitter.
/// </summary>
///
/// <param name="dataProvider">
/// A reference to the user control that contains the point parameter to be decoded as a temperature module type code in its script data list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point as identified in the script data list of the above user control.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the parameter containing the type code as identified in the script data list of the above user control.
/// </param>
///
/// <returns>
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetTemperatureModuleType(dataProvider, pointName, parameterName)
{
    var scriptData = new SL_ScriptData(parameterName);
    var moduleType;

    try
    {
        SL_LogFunctionParameters(2, "SL_GetTemperatureModuleType", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName);

        scriptData.Fetch(dataProvider, pointName);
        if (scriptData.changed && scriptData.valid)
        {
            moduleType = SL_Device.module.PvCalculationSources[scriptData.value].Source;

            if (moduleType == "Single Input")
            {
                SL_Device.module.type = "Single Input";
                SL_Device.module.numberOfSensors = 1;

                SL_Device.dynamicVariables.TV.name = "Sensor Temperature";
                SL_Device.dynamicVariables.TV.visibility = "visible";

                SL_Device.dynamicVariables.QV.visibility = "hidden";
            }
            else
            {
                SL_Device.module.type = "Dual Input";
                SL_Device.module.numberOfSensors = 2;

                SL_Device.dynamicVariables.TV.name = "Sensor 1 Temperature";
                SL_Device.dynamicVariables.TV.visibility = "visible";

                SL_Device.dynamicVariables.QV.name = "Sensor 2 Temperature";
                SL_Device.dynamicVariables.QV.visibility = "visible";
            }

            SL_Device.analogPV.name = "Process Temperature";

            SL_Device.dynamicVariables.PV.name = "Process Temperature";
            SL_Device.dynamicVariables.PV.visibility = "visible";

            SL_Device.dynamicVariables.SV.name = "Cold Junction Temperature";
            SL_Device.dynamicVariables.SV.visibility = "visible";

            SL_LogDebugLevelMessage(5, "SL_GetTemperatureModuleType", "module Type = " + SL_Device.module.type);
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_GetTemperatureModuleType", "Could not decode module type: dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName + ", pv calculation = " + scriptData.value + ", module type = " + SL_Device.module.type, error);
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_UpdateTemperatureSensorDescription
///
/// <summary>
/// Checks the ScriptData objects for all component parts that make up the complete description of a temperature sensor connected to the
/// transmitter, records the results in the global SL_Device object for the transmitter and updates the user control if the description
/// is complete. Since the parameters that make up the full  description can be updated in separate update messages and in random order,
/// it is necessary to save any valid parts in the global structure until all components are valid.
/// </summary>
///
/// <param name="dataProvider">
/// A reference to the user control that contains the description. This user control must have all of the point parameters that make up
/// the full description included in its ScriptData list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point for which the parameters should be checked, as it is encoded in the ScriptData list.
/// </param>
///
/// <param name="sensorObject">
/// A reference to the SL_TemperatureSensor object to update; either 1 or 2.
/// </param>
///
/// <returns>
/// None. The description string is updated in the sensor object if successful.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_UpdateTemperatureSensorObject(dataProvider, pointName, sensorObject)
{
    try
    {
        SL_LogFunctionParameters(2, "SL_UpdateTemperatureSensorObject", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", sensorNumber = " + sensorObject.number);

        sensorObject.description = "";

        // If the sensor type code is new, try to decode it. If it is a valid value, check to see if it is a Thermocouple.
        sensorObject.typeCode.Fetch(dataProvider, pointName);
        if (sensorObject.typeCode.changed)
        {
            if (sensorObject.typeCode.valid)
            {
                sensorObject.type = SL_Device.module.inputSensorTypes[sensorObject.typeCode.value].Type;

                if (sensorObject.type == "Thermocouple")
                    SL_Device.module.hasThermocouple = true;
            }
            else
                sensorObject.type = "";
        }

        // If the sensor ID code is new, try to decode it.
        sensorObject.id.Fetch(dataProvider, pointName);
        if (sensorObject.id.changed)
        {
            if (sensorObject.id.valid)
            {
                sensorObject.type  = SL_Device.module.inputSensorIDs[sensorObject.id.value].Type;
                sensorObject.model = SL_Device.module.inputSensorIDs[sensorObject.id.value].Model;

                if (sensorObject.type == "Thermocouple")
                    SL_Device.module.hasThermocouple = true;
            }
            else
                sensorObject.model = "";
        }

        // Third check the number of RTD wires parameter.
        sensorObject.rtdType.Fetch(dataProvider, pointName);
        if (sensorObject.rtdType.changed)
        {
            if (sensorObject.rtdType.valid)
                sensorObject.numberOfWires = SL_Device.module.inputSensorWires[sensorObject.rtdType.value].Description;
            else
                sensorObject.numberOfWires = "";
        }

        SL_LogDebugLevelMessage(5, "SL_UpdateTemperatureSensorObject", "type = " + sensorObject.type + ", model = " + sensorObject.model + ", numberOfWires = " + sensorObject.numberOfWires);

        // Update the full description string
        if ((sensorObject.type.length > 0) && (sensorObject.model.length > 0) && (sensorObject.numberOfWires.length > 0))
        {
            if (sensorObject.type == "RTD")
                sensorObject.description = sensorObject.type + " - " + sensorObject.model + ", " + sensorObject.numberOfWires;
            else if (sensorObject.type == "None")
                sensorObject.description = sensorObject.type;
            else
                sensorObject.description = sensorObject.type + " - " + sensorObject.model;
        }

        dataProvider.value = sensorObject.description;
        if (sensorObject.description.length > 0)
            dataProvider.StyleClass = "SL_Text_Normal";
        else
            dataProvider.StyleClass = "SL_Text_BadQuality";

    }
    catch (error)
    {
        SL_LogErrorMessage("SL_UpdateTemperatureSensorObject", "Could not decode temperature sensor type: ", error);
    }

    return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_UpdateTemperatureModuleType
///
/// <summary>
/// Checks the parameter indicating the temperature module type, either single or dual input, and updates the module type on the display.
/// The text label and visibility of the UI elements related to the two individual temperature sensors are updated based on whether it
/// is a dual or single input model.
/// </summary>
///
/// <param name="dataProvider">
/// A reference to the user control that contains the point parameter to be decoded as a temperature module type code in its script data list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point as identified in the script data list of the above user control.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the parameter containing the type code as identified in the script data list of the above user control.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_UpdateTemperatureModuleType(dataProvider, pointName, parameterName)
{
    var dualInputDevice;
    var sensor1Label;

    try
    {
        SL_LogFunctionParameters(2, "SL_UpdateTemperatureSensorVisibility", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName);

        SL_GetTemperatureModuleType(dataProvider, pointName, parameterName);
        if ((SL_Device.module.type != undefined) && (SL_Device.module.type.length > 0))
        {
            // Update the text in the Module Type alphanumeric control.
            dataProvider.value = SL_Device.module.type;
            dataProvider.styleClass = "SL_Text_Normal";

            // Determine the required label and visibility
            if (SL_Device.module.type == "Dual Input")
            {
                dualInputDevice = true;
                sensor1Label = "Temperature Sensor 1";
            }
            else
            {
                dualInputDevice = false;
                sensor1Label = "Temperature Sensor";
            }

            // Update the text used for the label for the Sensor 1 description and installation date.
            dataProvider.document.getElementById("textboxSensor1").value = sensor1Label;
            dataProvider.document.getElementById("textboxSensor1Installation").value = sensor1Label;

            // Update the visibility of the label and value for the Sensor 2 description.
            SL_SetVisibility(dataProvider.document.getElementById("textboxSensor2"), dualInputDevice);
            SL_SetVisibility(dataProvider.document.getElementById("alphaSensor2"), dualInputDevice);

            // Update the visibility of the label and value for the Sensor 2 installation date.
            SL_SetVisibility(dataProvider.document.getElementById("textboxSensor2Installation"), dualInputDevice);
            SL_SetVisibility(dataProvider.document.getElementById("shapeSensor2Installation"), dualInputDevice);

            // Update the visibility of the label and value for the PV calculation mode which only applies to a dual input transmitter.
            SL_SetVisibility(dataProvider.document.getElementById("textboxPVCalculationMode"), dualInputDevice);
            SL_SetVisibility(dataProvider.document.getElementById("alphaPVCalculationMode"), dualInputDevice);
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_UpdateTemperatureSensorVisibility", "Could not update temperature sensor controls. module type = " + SL_Device.module.type, error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Pressure Functions
///
/// Functions in this section are designed to implement common functionality that relate to the UI elements of all SmartLine
/// Pressure transmitters.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_GetPressureModuleType
///
/// <summary>
/// </summary>
///
/// <remarks>
/// </remarks>
///
/// <param name="dataProvider">
/// A reference to the user control that contains the point parameter to be decoded as a pressure module type code in its script data list.
/// </param>
///
/// <param name="pointName">
/// A string containing the name of the point as identified in the script data list of the above user control.
/// </param>
///
/// <param name="parameterName">
/// A string containing the name of the parameter containing the type code as identified in the script data list of the above user control.
/// </param>
///
/// <returns>
/// None.
/// </returns>
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_GetPressureModuleType(dataProvider, pointName, parameterName)
{
    var scriptData = new SL_ScriptData(parameterName);
    var moduleType;
    var moduleTypeName;

    try
    {
        SL_LogFunctionParameters(4, "SL_GetPressureModuleType", "dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName);

        scriptData.Fetch(dataProvider, pointName);
        if (scriptData.changed && scriptData.valid)
        {
            moduleType = SL_Device.module.moduleTypes[scriptData.value];
            moduleTypeName = moduleType.name;

            switch (moduleType.name)
            {
                case "Differential Pressure":
                    SL_Device.analogPV.name = moduleType.name;

                    SL_Device.dynamicVariables.PV.name = moduleType.name;
                    SL_Device.dynamicVariables.TV.name = "Static Pressure";
                    SL_Device.dynamicVariables.TV.visibility = "visible";
                    break;

                case "Absolute Pressure":
                case "Gauge Pressure":
                    SL_Device.analogPV.name = moduleType.name;

                    SL_Device.dynamicVariables.PV.name = moduleType.name;
                    SL_Device.dynamicVariables.TV.name = "";
                    SL_Device.dynamicVariables.TV.visibility = "hidden";
                    break;

                default:
                    SL_Device.analogPV.name = "Process Pressure";

                    SL_Device.dynamicVariables.PV.name = "Process Pressure";
                    SL_Device.dynamicVariables.TV.name = "";
                    SL_Device.dynamicVariables.TV.visibility = "hidden";
                    break;
            }

            SL_Device.dynamicVariables.SV.name = "Meter Body Temperature";
            SL_Device.dynamicVariables.SV.visibility = "visible";

            SL_Device.module.type = moduleType;
        }
    }
    catch (error)
    {
        SL_LogErrorMessage("SL_GetPressureModuleType", "Could not decode module type: dataProvider = " + dataProvider.id + ", point = " + pointName + ", parameter = " + parameterName + ", value = " + scriptData.value + ", moduleType = " + moduleType + ", name = " + moduleTypeName, error);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Multivariable Functions
///
/// Functions in this section are designed to implement common functionality that relate to the UI elements of all SmartLine
/// Multivariable transmitters.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SL_MultivariableModule
///
/// <summary>
/// Function and prototype to create a pseudo class for a SmartLine multivariable module to contain properties for:
///     type            such as Differential, Absolute or Gauge
///     moduleTypes     enumeration of possible types
/// </summary>
///
/// <returns>
/// New SL_TemperatureSensor object.
/// </returns>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function SL_MultivariableModule()
{
    this.staticPressureTypes;
    this.staticPressureType;
    this.hasThermocouple = false;
    this.coldJunctionTemperatureSources;
    this.coldJunctionSource;
    this.inputSensorTypes;
    this.inputSensorIDs;
    this.inputSensorWires;
    this.sensor = new SL_TemperatureSensor(1);
}