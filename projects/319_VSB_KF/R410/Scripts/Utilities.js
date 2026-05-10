//***************************************************************************
// Copyright (C) 2006 Honeywell Limited
// $Archive: /System Displays/_Master/Common/Scripts/Utilities.js $								
// $Workfile: Utilities.js $												
// $Modtime: 3/10/09 6:41p $											
// $Revision: 40 $
// $Author: Cirving $														
//***************************************************************************
// This file contains useful utility script functions for use by any display
// which needs them.
//***************************************************************************
// If you reference this script file then you also need to reference:
// Logging.js       (LogErrorMessage)
// ErrorHandling.js (ExceptionString, ScriptError)
// JSExtensions.js  (JsType*, String.trim)
// HtmlUtils.js     (StripPath, InsertScriptBlock)
//***************************************************************************

//----------------------------------------------------------
// LoadXMLDocument
//----------------------------------------------------------
// This function is called to create an XML Parser object,
// and to load it with the specified document or string.
//----------------------------------------------------------
function LoadXMLDocument(strXML, bIsURL)
{
	var xmlDoc = null;
	try
	{
		xmlDoc = new ActiveXObject("MSXML2.FreeThreadedDOMDocument.6.0");
		xmlDoc.async = false;
		xmlDoc.setProperty("SelectionLanguage", "XPath");
		
		if (bIsURL)
		{
			xmlDoc.load(strXML);
		}
		else
		{
			xmlDoc.loadXML(strXML);
		}
	
		if (xmlDoc.parseError.errorCode != 0)
		{
			ScriptError("LoadXMLDocument", "Error " + xmlDoc.parseError.errorCode + " parsing XML document '" +  xmlDoc.parseError.url + "': " + xmlDoc.parseError.reason);
			xmlDoc = null;
		}
	}
	catch(e)
	{
		ScriptError("LoadXMLDocument", "Exception creating or loading XMl Document: " + e.description);
		xmlDoc = null;
	}
	return xmlDoc;
}

//------------------------------------------------------------------------
// GetStationApp
//------------------------------------------------------------------------
// This function returns a reference to the Station Application object
// if it can be found, or 'null' otherwise.
//------------------------------------------------------------------------
function GetStationApp()
{
	try
	{
		var objExternal = window.external.Application;
		if (objExternal != null && typeof(objExternal) == JsTypeObject)
		{
			if (typeof(objExternal.StationNumber) == JsTypeNumber)
			{
				// The external object has the StationNumber property and can be reasonably assumed to be the Station Application object.
				return objExternal;
			}
			else if (typeof(window.dialogArguments) != JsTypeUndefined &&
					 window.dialogArguments != null &&
					 typeof(window.dialogArguments.StationApp) != JsTypeUndefined)
			{
				return window.dialogArguments.StationApp
			}
		}
	}
	catch(e)
	{
		throw "exception testing type of window.external.Application: " + ExceptionString(e);
	}
	return null;	// we have not found the Station Application object.
}

var SECLEVEL_MNGR = 5;
var SECLEVEL_ENGR = 4;
var SECLEVEL_SUPV = 3;
var SECLEVEL_OPER = 2;
var SECLEVEL_LVL2 = 1;
var SECLEVEL_LVL1 = 0;

//------------------------------------------------------------------------
// StationSecurityLevel
//------------------------------------------------------------------------
// This function returns a reference to the current Station Security
// Level, defaulting to 'OPER' if the Station Application object is
// not actually available.
//------------------------------------------------------------------------
function StationSecurityLevel()
{
	// Retrieve the Station Application
	var objStationApp = GetStationApp();
	if (objStationApp == null)
	{
		// Use the default security level.
		return SECLEVEL_OPER;
	}
	else
	{
		// Retrieve the actual security level.
		return objStationApp.SecurityLevel;
	}
}

//------------------------------------------------------------------------
// RefreshCurrentPage
//------------------------------------------------------------------------
// This function refreshes the current Station display and any associated
// data, with the option to preserve any 'referrer' hedaer on the original
// page or not.
//------------------------------------------------------------------------
function RefreshCurrentPage(preserveReferrer)
{
    if (preserveReferrer)
    {
        window.top.location.reload();
    }
    else
    {
        var stationApp = GetStationApp();
        if (stationApp != null)
        {
            stationApp.Refresh();
        }
        else
        {
            window.top.navigate(window.top.location.href);
        }
    }
}

//------------------------------------------------------------------------
// OperatorChangeRefresh
//------------------------------------------------------------------------
// This function will refresh the current page if there is actually an operator
// logged in.
//------------------------------------------------------------------------
function OperatorChangeRefresh()
{
    // If an operator is actually logged in, refresh the page.
    // If no operator is logged in the OperatorID is a single space
    // character.
    if (GetStationApp().OperatorID != " ")
    {
        RefreshCurrentPage(false);
    }
}
			
//------------------------------------------------------------------------
// PersistData
//------------------------------------------------------------------------
// Called to cache a given value or object for the duration of the
// current browser session.
//------------------------------------------------------------------------
function PersistData(strKey, varValue)
{
	var objStationApp = GetStationApp();
	if (objStationApp != null)
	{
		//alert("Station application object is available!");
		
		//.......................................................................
		// Use the Station dictionary to perform the persistance.
		//.......................................................................
		objStationApp.Dictionary.Remove(strKey);	
		objStationApp.Dictionary.Add(strKey, varValue);
	}
	else
	{
		//.......................................................................
		// Use the #default#userData behavior (which should be on the body
		// of the current document) to store data.
		//.......................................................................
		//alert("Persisting data in the userData behavior on the document body.");
		document.body.setAttribute(strKey, varValue);
	}
}
							
//------------------------------------------------------------------------
// PersistedData
//------------------------------------------------------------------------
// Called to retrieve a persisted value or object. If a persisted value or
// object is not found, then the default is returned.
//------------------------------------------------------------------------
function PersistedData(strKey, varDefault)
{
	var result = null;
	try
	{
		var objStationApp = GetStationApp();
		if (objStationApp != null)
		{
			//.......................................................................
			// We are using the Station dictionary for persistance purposes.
			//.......................................................................
			//alert("Retrieving persisted data from the Station Dictionary.");
			result = objStationApp.dictionary.item(strKey).value;
		}
		else
		{
			//.......................................................................
			// Use the #default#userData behavior (which should be on the body
			// of the current document) to store data.
			//.......................................................................
			//alert("Retrieving persisted data from the UserData store.");
			document.body.Load(strKey);
			result = document.body.getAttribute(strKey);
		}
	}
	catch(e)
	{
		//alert("exception retrieving persisted data for key '" + strKey + "': " + e.description);
		result = null;
	}

	//.......................................................................
	// Check if we need to return the default value instead.
	//.......................................................................
	if (typeof(result) == JsTypeUndefined || result == null)
	{
		result = varDefault;
	}
	
	//alert("PersistedData returning [" + result + "]");
	return result;
}


//-----------------------------------------------------------------------
// Translate
//-----------------------------------------------------------------------
// This function performs a translation for the given source-text and
// returns the translated text.
//
// For complete functionality, there must be a function called 
// "DoAppTranslation" defined: this is responsible for performing the 
// actual translation of the input string.
//-----------------------------------------------------------------------
var g_xmlTranslations = null;

function Translate(strText)
{
	//.......................................................................
	// Strip the I18N markers from the string.
	//.......................................................................
	if (strText.indexOf("I18N('") >= 0 || strText.indexOf("Translate('") >= 0)
	{
		strText = strText.replace(/I18N\('/gi, "");	
		strText = strText.replace(/Translate\('/gi, "");	
		strText = strText.replace(/'\)/gi, "");
	}
	
	var strKey = escape(strText);
	var strValue = strText;
	
	//.......................................................................
	// Check whether we can do anything more.
	//.......................................................................
	if (typeof(DoAppTranslation) == JsTypeFunction ||
	    typeof(DoAppTranslation) == JsTypeUnknown)
	{
		try
		{
			//.......................................................................
			// Make sure our translation cache is available, and also accessible
			// (for some reason, sometimes you can't access the cached one).
			//.......................................................................
			if (g_xmlTranslations == null && typeof(PersistedData) == JsTypeFunction)
			{
				// Attempt to restore the object from its persisted state.
				g_xmlTranslations = PersistedData("Translations", null)
				if (g_xmlTranslations != null)
				{
				    // Check that the cached object is still usable.
				    try
				    {
				        var base = g_xmlTranslations.documentElement;
				    }
				    catch(e)
				    {
				        // The cached object is not usable, so ditch it.
				        g_xmlTranslations = null;
				    }
				}
			}
				
			if (g_xmlTranslations == null)
			{
				//.......................................................................
				// Create a new translation cache.
				//.......................................................................
				g_xmlTranslations = LoadXMLDocument("<Translations></Translations>", false);
			}
			
			//.......................................................................
			// See if we have a cached translation.
			//.......................................................................
			var xmlTranslation = g_xmlTranslations.selectSingleNode("//Translation[@Key = '" + strKey + "']");
			if (xmlTranslation != null)
			{
				//alert("Cached translation found for English string '" + strText + "'");
				strValue = xmlTranslation.text;
			}
			else
			{
				//alert("There is no cached translation for English string '" + strText + "'");
				
				//.......................................................................
				// Query the server for the translation.
				//.......................................................................
				strValue = DoAppTranslation(strText);
								
				//.......................................................................
				// Cache the newly-retrieved translation.
				//.......................................................................
				xmlTranslation = g_xmlTranslations.createElement("Translation");
				xmlTranslation.setAttribute("Key", strKey);
				xmlTranslation.text = strValue;
				g_xmlTranslations.documentElement.appendChild(xmlTranslation);
				if (window.PersistData)
				{
					PersistData("Translations", g_xmlTranslations);
				}
			}
			
			// Make sure we're not returning an empty translation.
			if (strValue == "")
			{
				LogErrorMessage("Utilities::Translate", "Missing translation for '" + strText + "'");
				strValue = strText;
			}
		}
		catch(e)
		{
			ScriptError("Utilities::Translate", e);
			strValue = strText;
		}
	}
	return strValue;
}

///------------------------------------------------------------------------
/// ParameterXml
///------------------------------------------------------------------------
/// <summary>
/// This recursive function turns a set of key-value pairs, in which
/// the value may itself be an array into element-basedXML suitable for
/// use as an argument to a SOAP method call.
/// </summary>
/// <param name="param">
/// The array of parameters you wish to construct the XML representation of.
/// </param>
/// <remarks>
/// the parameter type is 'unknown' if it comes through as a vbscript array.
/// </remarks>
///------------------------------------------------------------------------
function ParameterXml(param)
{
	var ArrayXML = "";

	if (typeof(param) == JsTypeUndefined || param == null || (typeof(param) == JsTypeObject && param.length == 0))
	{
	    // We have no parameters at all!
	    return "";
	}
	else if (typeof(param) != JsTypeObject && typeof(param) != JsTypeUnknown)
	{
		// We have a single non-array value, and don't need to turn it into XML.
		return param;
    }
	else if (typeof(param[0]) == JsTypeString && param.length > 1)
    {
        // Assume that we have been given a name-value pair, with the name of the
        // parameter in the first element and the value in the second.
        var value = "";
        if (typeof(param[1]) == JsTypeObject)
        {
            // Assume the parameter value is an array of primitive types.
            var arrType = typeof(param[1][0]);
            for (var j = 0; j < param[1].length; j++)
            { 
                value = value + "<" + arrType + ">" + param[1][j] + "</" + arrType + ">";
            }
        }
        else
        {
            value = param[1];
        }
        return "<" + param[0] + ">" + value + "</" + param[0] + ">";
    }
    else
    {
        // We have been given an array of parameters. Concatenate the XML for each one.
        var arrayXml = "";
        for (var i = 0; i < param.length; i++)
        {
            // Get the XML for this particlar parameter.
            arrayXml = arrayXml + ParameterXml(param[i]);            
        }
        return arrayXml;
    }
}

///------------------------------------------------------------------------
/// OperatorMessage
///------------------------------------------------------------------------
/// <summary>
/// Display a message to the current user.
/// </summary>
/// <param name="m">
/// The localized message to be displayed to the current user.
/// </param>
///------------------------------------------------------------------------
function OperatorMessage(m)
{
	var objStationApp = GetStationApp();
	if (objStationApp != null)
	{
		// Display the message in the Station Message Zone.
		objStationApp.TemporaryMessageZoneText = m;
	}
	else
	{
		// Display the message in an alert box.
		alert(m);
	}
}

///------------------------------------------------------------------------
/// OperatorPromptAsync
///------------------------------------------------------------------------
/// <summary>
/// Prompts the operator for a response using the appropriate method, and
/// calls the passed-in functions when the response is made or the prompt
/// is cancelled.
/// </summary>
/// <param name="message">
/// The localized prompt message for the user.
/// </param>
/// <param name="promptType">
///	0 => you expect a free-text response from the user. This is the default.
/// 1 => you expect a yes / no answer from the user.
/// </param>
/// <param name="callbackFunction">
/// A script function which takes a single argument (the users's response as a string) 
/// which will be called when the user makes their response. The argument value will 
/// be "Y" if they answer yes or OK, "N" if they answer no, or null if the user
/// makes no response, or cancels the prompt.
/// </param>
/// <param name="responseTimeout">
/// The amount of time (in Milliseconds) for which the prompt should remain 
/// available to the user.
/// </param>
///------------------------------------------------------------------------
function OperatorPromptAsync(message, promptType, callbackFunction, responseTimeout)
{
    try
    {	
		var response = null;	// Default response value.
		
		var objStationApp = GetStationApp();
		if (objStationApp == null)
		{
			// Use the built-in window prompts.
			if (promptType == 1)
			{
				// We want a y/n response.
				if (window.confirm(message))
				{
					response = "Y";
				}
				else
				{
					response = "N";
				}
			}
			else
			{
				// The response is freeform.
				response = window.prompt(message, "");
			}
		}
		else
		{
			// Use the Station prompting mechanism. Note that we need to call
			// out to a VBScript function for this because the Station Application object 
			// relies on an inout parameter to pass the response value back. 
			// JavaScript has no support for inout (or any reference) parameters.
			
			// Make sure the VBScript function exists.
			if (typeof(GetStationOperatorResponse) == JsTypeUndefined)
			{
				// Create a VBScript block for the required function in the header.
				var vbscript = "\n" +
					"Function GetStationOperatorResponse(objStationApp, promptMessage, promptType)\n" +
					"On Error Resume Next\n" +
					"	Dim responseSoFar, responseState\n" +
					"	responseState = objStationApp.GetResponse3(promptMessage, promptType, responseSoFar)\n" +
					"	If (responseState  = 0) Then\n" +
					"		' The response has been cancelled.\n" +
					"		GetStationOperatorResponse = null\n" +
					"	ElseIf IsNull(responseSoFar) Or responseSoFar = \"\" Then\n" +
					"		' The response is still pending.\n" +
					"		GetStationOperatorResponse = \"\"\n" +
					"	Else\n" +
					"		' The response is available.\n" +
					"		GetStationOperatorResponse = responseSoFar\n" +
					"	End If\n" +
					"End Function\n";
				InsertScriptBlock("VBScript", vbscript);
			}
			
			// Work out if the user still has time to respond.
			var defaultTimeout = 10000;	// 10s
			var timeRemaining = typeof(responseTimeout) == JsTypeNumber ? parseInt(responseTimeout, 10) : defaultTimeout;			
			if (timeRemaining > 0)
			{		
				// Execute the VBScript callout function.
				response = GetStationOperatorResponse(objStationApp, message, promptType)
				if (response != null && response == "")
				{
					// The user hasn't responded yet. Check again in a little while.				
					var pollDelay = 50;			
					window.setTimeout( function() { OperatorPromptAsync(message, promptType, callbackFunction, timeRemaining - pollDelay); }, pollDelay);
					return;	// Don't make any callbacks just yet.
				}
			}
			else
			{
				// The user has run out of time to respond. Make sure the prompt is no
				// longer displayed and treat this as a 'cancel'.
				objStationApp.MessageZoneText = "";
				response = null;	// => cancel.
			}
		}	

		// OK - once we got here, we either have a response, or the response has been ignored / cancelled.
		// execute the callback function from a different thread so that whatever it does can't interrupt 
		// the thread that first called this function if the response was immediate.
		if (callbackFunction != null && (typeof(callbackFunction) == JsTypeFunction || typeof(callbackFunction) == JsTypeObject))
		{
			window.setTimeout(function(){ callbackFunction(typeof(response) == JsTypeString && promptType == 1 ? response.toUpperCase() : response); }, 10);
		}
    }
    catch(e) 
    { 
        ScriptError('OperatorPromptAsync: ', e); 
    }
    return response;
}

///------------------------------------------------------------------------
/// OperatorPromptAsyncEx
///------------------------------------------------------------------------
/// <summary>
/// Prompts the operator for a response using the appropriate method, 
/// translates any response into a known set of values and then calls the
/// passed-in callback function when a response is received.
/// </summary>
/// <param name="message">
/// The localized prompt message for the user.
/// </param>
/// <param name="promptType">
/// The type of prompt you wish to show and get a response for. 
/// Prompts may be boolean, which the user can answer 'Y', 'N' or ignore, or 
/// they can be free-form, where the user responds with any string, or can choose
/// to cancel or ignore the prompt.
/// The responses will be interpreted differently depending on the value of this parameter:
/// 0 or "USER_INPUT": freeform prompt, any user input text returned.       
/// 1 or "YES_NO": boolean prompt, 'Y' => "YES", 'N' or ignored => "NO"
/// 2 or "YES_NO_CANCEL": boolean prompt, 'Y' => "YES", 'N' => "NO", ignored => "CANCEL"
/// 3 or "OK_CANCEL": boolean prompt, 'Y' => "OK", 'N' or ignored => "CANCEL"
/// 4 or "OK_ONLY": operator message, returns "OK" when the user has acknowledged the message.
/// </param>
/// <param name="callbackFunction">
/// A script function which takes a single argument (the users's response as a string) 
/// which will be called when the user makes their response. The argument value will 
/// be "Y" if they answer yes or OK, "N" if they answer no, or null if the user
/// makes no response, or cancels the prompt.
/// </param>
/// <param name="responseTimeout">
/// Optional. The amount of time (in Milliseconds) for which the prompt should 
/// remain available to the user.
/// </param>
///------------------------------------------------------------------------
function OperatorPromptAsyncEx(message, promptType, callbackFunction, responseTimeout)
{
    // Confirm that the callback function is really available.
    if (callbackFunction == null || typeof(callbackFunction) != JsTypeFunction)
	{
	    throw "Callback function is a required parameter";	
	}
		
	// Call the standard OperatorPromptAsync function with an appropriate set of arguments.	
	switch(promptType)
	{
		case "YES_NO":
		case "1":
		case 1:
		{
			OperatorPromptAsync(message, 1, function(responseValue) { callbackFunction(responseValue == "Y" ? "YES" : "NO"); }, responseTimeout);
			break;
		}
		case "YES_NO_CANCEL":
		case "2":
		case 2:
		{
			OperatorPromptAsync(message, 1, function(responseValue) { callbackFunction(responseValue == null ? "CANCEL" : (responseValue == "Y" ? "YES" : "NO")); }, responseTimeout);
			break;
		}
		case "OK_CANCEL":
		case "3":
		case 3:
		{
			OperatorPromptAsync(message, 1, function(responseValue) { callbackFunction(responseValue == "Y" ? "OK" : "CANCEL"); }, responseTimeout);
			break;
		}
		case "OK_ONLY":
		case "4":
		case 4:
		{	
			// This should actually be an operator message!
			// Show the message then call the callback function in a different thread.
			OperatorMessage(message);
			window.setTimeout( function() { callbackFunction("OK"); }, 10);
		}
		default:	//USER_INPUT or 0
		{
			// If none of the boolean prompts are specified, assume we are prompting for user input.
			// We do not need to parse the reponse value into anything else.
			OperatorPromptAsync(message, 0, callbackFunction, responseTimeout);
			break;
		}
	}              
}


// --------------------------------------------------------------------------
// CallupPage
// --------------------------------------------------------------------------
// <summary>
// This function is used to call up a new display in one of several different 
// ways, including standard navigation, popup navigation and various types
// of task request.
// </summary>
// <param name="url">
// The new thing you are trying to call up.
// </param>
// <param name="targetWindow">
// The method or location that should be used to call up the new page.
// </param>
// --------------------------------------------------------------------------
function CallupPage(url, targetWindow, windowFeatures, popupLeft, popupTop)
{
	// Initialize the return value
	var calledUp = false;
	
	// Retrieve the Station Application
	var objStationApp = GetStationApp();
	try
	{
		switch(targetWindow.toLowerCase())
		{
		    case "_stationmenu":
		    {
				if (objStationApp != null)
				{
				    objStationApp.InvokeMenu2(url);
				}
				else
				{
					throw "Unable to invoke Station menu: Station Application is not available.";
				}
				break;
		    }
		    case "_stationtask":
		    {
				if (objStationApp != null)
				{
				    var script = "objStationApp.RequestTask(" + url + ");"
				    eval(script);
				}
				else
				{
					throw "Unable to request task: Station Application is not available.";
				}
				break;
		    }
			case "_stationdetail":
			{
				if (objStationApp != null)
				{
					var nLRN = 21 //+ parseInt(objStationApp.StationNumber, 10) - 1; Calculated LRNs not valid for Station numbers > 20: 21 should always work.
					objStationApp.RequestTask(nLRN, 73, 0, 0, 0, url);
				}
				else
				{
					throw "Unable to callup point detail: Station Application is not available.";
				}
				break;
			}
			case "_cmsdetail":
			{
				if (objStationApp != null)
				{
					var nLRN = 21 //+ parseInt(objStationApp.StationNumber, 10) - 1; Calculated LRNs not valid for Station numbers > 20: 21 should always work.
					objStationApp.RequestTask(nLRN, 78, 0, 0, 0, url);
				}
				else
				{
					throw "Unable to callup cms detail: Station Application is not available.";
				}
				break;
			}
			case "_stationpopup":
			{
				// Locate the popup behavior.
				var objBehavior = window.document.getElementById("HDXPopupBehavior");
				if (objBehavior != null)
				{
					objBehavior.CreatePopupWindow(url, popupLeft, popupTop, 0);
				}
				else
				{
					throw "Unable to create HMIWeb popup: popup behavior is not available.";
				}
				break;	
			}		
			case "_dhtmlpopup":
			{
			    window.showModelessDialog(url, null, windowFeatures);
			    break;
			}
			case "_newhelpwindow":
			{
			    var HelpWin = window.open(window.external.WebServerURI + url, null, 'width=900,height=880,status=no,resizable=yes,scrollbars=yes,menubar=yes,toolbar=yes,location=no,directories=no');
			    HelpWin.MoveTo(100, 100);
			    break;
			}
			case "_newwindow":
			{
			    window.open(window.external.WebServerURI + url, null, 'width=1200,height=900,status=yes,resizable=yes,scrollbars=yes,location=no,directories=no');
			    break;
			}
case "_stationdisplay":
    {
        if (objStationApp != null) 
	{
            var pat = url.toLowerCase().substring(0, 7);
            var newstr
            switch (pat) {

                case "wuri:\\":
                    {
                        newstr = window.external.webserveruri;
                        url = url.replace("wuri:\\", newstr);
                        break;
                    }
                case "wuri://":
                    {
                        newstr = window.external.webserveruri;
                        url = url.replace("wuri://", newstr);
                        break;
                    }
                default:
                    {
                         break;
                    }
            }

            objStationApp.CurrentPage = url;
            break;
        }

        if (url.toLowerCase().indexOf("://") < 0 && url.toLowerCase().indexOf(":\\\\") < 0) 
	{
            // There is no protocol in the url to be called up: nothing else we can do.
            throw "Unable to callup Station Display: Station Application is not available.";
        }
        // Else no Station but protocol is provided: fall through to
        // the generic window callup function with a target of the top window.
        targetWindow = "_top";
    }
			default:
			{
				// Just open the specified URL in the specified window.			
				window.open(url, targetWindow, windowFeatures, false);
				break;	
			}
		}
		calledUp = true;	// Because we made it this far with no exceptions.
	}
	catch(e)
	{
		ScriptError("CallupPage with target '" + targetWindow + "' and argument '" + url + "':", e);
	}
	return calledUp;	
}

// --------------------------------------------------------------------------
// CurrentTopDisplay
// --------------------------------------------------------------------------
// This function is used to retrieve the name of the top-most display
// currently called up in Station or the browser.
// --------------------------------------------------------------------------
function CurrentTopDisplay()
{
	// Retrieve the Station Application
	var displayURL = "";
	try
	{
	    displayURL = window.top.location.href;
	}
	catch(e)
	{
	    // This exception occurs if the current window is
	    // hosted inside a page from a different domain. 
	    // Under normal IE security, permission to read the 
	    // location object properties is denied in this case.
	    var objStationApp = GetStationApp();
	    if (objStationApp != null)
	    {
		    // Station presumably knows what the top level display is...
		    displayURL = objStationApp.CurrentPageLocation; 
	    }
	    else
	    {
	        // Use the URL for the current window, since we can't get to the 
	        // URL for the topmost window.
	        displayURL = window.location.href;
	    }
	}

    if (displayURL.toLowerCase().indexOf("sysdsp.htm") >= 0)
    {
        // We actually want the DSP display that is hosted inside sysDSP.htm
        // It is specified in the 'HSCPage' URL parameter.
        var hscPageKey = "hscpage"; // the lower-case version of the name of the parameter we are looking for.
        var parameters = GetURLParameters(displayURL, true);
        if (parameters != null && typeof(parameters[hscPageKey]) != JsTypeUndefined)
        {
            displayURL = parameters[hscPageKey];
        }
        
        // Check that the display is in the full 'sysNNN.dsp' format
        // rather than just being a numeric display.
		var nonNumeric = displayURL.replace(/\d/g, "");
        if (nonNumeric.length == 0)
        {
            // We have a display number instead of a file name.
            displayURL = "sys" + displayURL.pad(3, "0", true) + ".dsp";
        }
    }
	
	return StripPath(displayURL);
}

//-----------------------------------------------------------------------
// TranslateElementText
//-----------------------------------------------------------------------
// This function translates and updates the text of the supplied
// element as required.
// Note that 'option' children of 'select' elements cannot have
// extensible attributes, hence all the checking.
//-----------------------------------------------------------------------
function TranslateElementText(objSrcElem)
{
	try
	{
	    var tagName = objSrcElem.tagName.toLowerCase();
	    var isNotOption = (tagName != "option");
	    
		//.......................................................................
		// Make sure the element is not already being translated.
		//.......................................................................
		if (isNotOption && parseBoolean(objSrcElem.getAttribute("translating"), false) == true)
		{
			return;
		}
		
		//.......................................................................
		// Translate the element's text value
		//.......................................................................
		if (isNotOption)
		{
		    objSrcElem.setAttribute("translating", true, 0);
		}			
		var strInput = (isNotOption ? objSrcElem.getAttribute("I18NValue").trim() : "");
		switch(tagName)
		{
			case "input":
				// The value property should be translated.
				if (strInput.length == 0)
				{
					strInput = objSrcElem.value;
				}
				objSrcElem.value = Translate(strInput);
				break;
				
			case "option":
				// The text property should be translated.
				if (strInput.length == 0)
				{
					strInput = objSrcElem.text.trim();
				}
				objSrcElem.text = Translate(strInput);
				break;
				
			case "select":
				// All the options should be translated.
				var objOption;
				for (var i = 0; i < objSrcElem.options.length; i++)
				{
					TranslateElementText(objSrcElem.options.item(i));
				}
				break;
				
			case "title":
				// The document title should be translated.
				if (strInput.length == 0)
				{
					strInput = objSrcElem.id;
				}
				if (strInput.length == 0)
				{
					strInput = objSrcElem.ownerDocument.title.trim();
				}		
				objSrcElem.ownerDocument.title = Translate(strInput);
				break;
				
			default:
				// See if the developer has specified a property to be translated...
				var strProperty = objSrcElem.getAttribute("L10NProperty");
				if (strProperty == "")
				{
					// No developer property specified. Translate the innerText of the element.
					if (strInput.length == 0)
					{
						strInput = objSrcElem.innerText.trim();
					}			
					objSrcElem.innerText = Translate(strInput);
				}
				else
				{
					// Translate the specified property.
					if (strInput.length == 0)
					{
						strInput = objSrcElem.getAttribute(strProperty);
					}			
					objSrcElem.setAttribute(strProperty, Translate(strInput), 0);
				}
				break;
		}
	}
	catch(e)
	{
		ScriptError("Utilities::TranslateElementText", e);
	}
	
	try
	{
	    if (isNotOption)
	    {
		    objSrcElem.setAttribute("translating", false, 0);
		}
	}
	catch(e)
	{
		// Ignore this error - the attribute does get set correctly, and I have no idea why this was thrown.
	}	
}

//------------------------------------------------------------------------
// ApplyMenuText
//------------------------------------------------------------------------
// <summary>
// This function extracts context information for the current display from
// the specified Navigation Menu element and displays it in 
// appropriate places in the specified top-left-title element.
// </summary>
// <returns>
// true if the MenuText was applied successfuly, false otherwise.
// </returns>
//------------------------------------------------------------------------
function ApplyMenuText(menuElement, titleElement, iconImg)
{
    var applied = false;
    try
    { 
        if (typeof(menuElement.NavMenu) == JsTypeObject)
        {
            titleElement.innerHTML = menuElement.MenuTitle;
            var iconSrc = menuElement.MenuIcon
            iconImg.src = iconSrc;
            if (iconSrc != "" && iconImg.currentStyle.display == "none")
            {
                iconImg.style.display = "inline";
            }
            applied = true;
        }
    }
    catch(e)
    {
        ScriptError("ApplyMenuText", e);
    } 
    return applied;        
}

//------------------------------------------------------------------------
// ToggleDropDownMenu
//------------------------------------------------------------------------
// <summary>
// This function toggles the visibility of the specified menu element,and
// adjusts the indicator on the associated togglebutton to match.
// </summary>
//------------------------------------------------------------------------
function ToggleDropDownMenu(menuElement, buttonElement)
{
    try
    { 
        if (menuElement.currentStyle.display.toLowerCase() == "none")
        {
            RemoveClassFromElement(buttonElement, "DropDown");
            menuElement.style.display = "block";
            IncludeClassOnElement(buttonElement, "CloseUp");
        }
        else
        {
            RemoveClassFromElement(buttonElement, "CloseUp");
            menuElement.style.display = "none";
            IncludeClassOnElement(buttonElement, "DropDown");
        }
    }
    catch(e)
    {
        ScriptError("ToggleDropDownMenu", e);
    }
}

//---------------------------------------------------------------
// Function ValidateNumericInput
//---------------------------------------------------------------
// This function can be used as the event sink for an input field
// that requires a numeric value. It causes the 'blur' (and thus
// 'change') event to fire when 'enter' is pressed, and otherwise
// makes sure that only numbers are entered.
//---------------------------------------------------------------
function ValidateNumericInput()
{
	if (window.event.keyCode == 13)
	{
		if (isNaN(parseFloat(window.event.srcElement.value)))
		{
			OperatorMessage(Translate("Please enter a numeric value"));
			return false;
	    }
	    else
	    {
			window.event.srcElement.blur();
		}
        return true;
	}
}