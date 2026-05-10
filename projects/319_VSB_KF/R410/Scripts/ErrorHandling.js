//***************************************************************************
// Copyright (C) 2006 Honeywell Limited
// $Archive: /System Displays/Web Application Displays/Scripts/ErrorHandling.js $								
// $Workfile: ErrorHandling.js $												
// $Modtime: 8/14/07 7:31p $											
// $Revision: 5 $
// $Author: Stickell $														
//***************************************************************************
// This file contains usful functions for handling errors in client-side
// scripts. It depends on the functions in Logging.js
//***************************************************************************
// If you reference this file then you also need to reference:
//  Logging.js (InitializeLogger, LogErrorMessage)
//***************************************************************************

// --------------------------------------------------------------------------------------
// HANDLE_ERROR
// --------------------------------------------------------------------------------------
// Handles logging of all errors that may occur. This function can take any number
// of arguments, which will be concatenated in the log message raised for this error.
// --------------------------------------------------------------------------------------
function HANDLE_ERROR()
{
	var src = "SCRIPT ERROR: <" + document.location.pathname + ">";
	var msg = "";
	
	for( var i = 0; i<arguments.length; i++ )
	{
		msg += arguments[i] + ", ";
	}

	if (typeof(InitializeLogger) == "function" && InitializeLogger())
	{
		// We can just log this message.
		LogErrorMessage(src, msg);	
		//	RaiseScriptError(src, msg);
	}
	else
	{
		// There is no way to log the message, so alert the user instead.
		alert(src + "\n" + msg + "\nInitializeLogger type = '" + typeof(InitializeLogger) + "'");
	}
}

// --------------------------------------------------------------------------------------
// ExceptionString
// --------------------------------------------------------------------------------------
// This function constructs a useful string from the fields of a caught exception.
// --------------------------------------------------------------------------------------
function ExceptionString(e)
{
	if (typeof(e) == "object" &&
		typeof(e.number) != "undefined" &&
		typeof(e.description) != "undefined")
	{
		return "(" + e.number + ") " + e.description;
	}
	else
	{
		return e.toString();
	}
}

// --------------------------------------------------------------------------------------
// ScriptError
// --------------------------------------------------------------------------------------
// This function logs details about javascript exceptions.
// --------------------------------------------------------------------------------------
function ScriptError(funct, e)
{
	var msg = "<function '" + funct + "'>";
    if (e != null)
    {
        msg += "<error '" + ExceptionString(e)  + "'>";
    }

    HANDLE_ERROR(msg);
}

// --------------------------------------------------------------------------------------
// ShowNotImplemented()
// --------------------------------------------------------------------------------------
//	Displays a 'not implemented' message.
// --------------------------------------------------------------------------------------
function ShowNotImplemented()
{
	alert("Function not yet implemented.");
}

