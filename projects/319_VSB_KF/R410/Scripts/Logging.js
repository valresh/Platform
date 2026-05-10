//***************************************************************************
// Copyright (C) 2006 Honeywell Limited
// $Archive: /System Displays/Web Application Displays/Scripts/Logging.js $								
// $Workfile: Logging.js $												
// $Modtime: 8/14/07 7:52p $											
// $Revision: 5 $
// $Author: Stickell $														
//***************************************************************************
// This file contains the definitions and functions required to access the
// HMI-Web client logger from script.
//***************************************************************************
var APPID_SCRIPTLOGGER	= "{DF71FC53-3A57-4081-B17B-0FADC9321884}";
var GRP_SCRIPTLOGGER	= 2;

var L_ERROR				= 10;
var L_LOG				= 15;
var L_WARNING			= 15;
var L_RANGE				= 20;
var L_PARAM				= 30;
var L_INFO				= 35;
var L_PROFILE			= 35;
var L_RETURN			= 40;
var L_PRINT				= 100;

var E_FAIL				= 0x80004005;

var g_oScriptLogger		= null;

// --------------------------------------------------------------------------------------
// InitializeLogger
// --------------------------------------------------------------------------------------
// Calling this function makes certain that the script logger has been initialized,
// returning 'true' if initialization has been successful, or false otherwise.
// --------------------------------------------------------------------------------------
function InitializeLogger()
{
	if (g_oScriptLogger == null)
	{
		try
		{
			g_oScriptLogger = new ActiveXObject("HW_LoggingClient.HW_Logger");
			g_oScriptLogger.Initialize( APPID_SCRIPTLOGGER, -1, "" );
			window.attachEvent("onunload", TerminateLogger);
		}
		catch( err )
		{
			g_oScriptLogger = null;
		}
	}
	return (g_oScriptLogger != null);
}

// --------------------------------------------------------------------------------------
// LogMessageEx
// --------------------------------------------------------------------------------------
// Logs messages for a particular paranoid group
// --------------------------------------------------------------------------------------
function LogMessageEx(grp, par, src, msg)
{
	if( !InitializeLogger())
		return;
	
	try
	{
		if( g_oScriptLogger.MessagePassesFilter(grp, par) )
		{
			g_oScriptLogger.LogMessageEx(grp, par, src, msg);
		}	
	}
	catch( err )
	{
		// ignore
	}
}

// --------------------------------------------------------------------------------------
// LogMessage
// --------------------------------------------------------------------------------------
// Logs messages using the default paranoid group
// --------------------------------------------------------------------------------------
function LogMessage(par, src, msg)
{
	LogMessageEx(GRP_SCRIPTLOGGER, par, src, msg);
}

// --------------------------------------------------------------------------------------
// LogErrorMessage
// --------------------------------------------------------------------------------------
// Logs messages using the default paranoid group and L_ERROR
// --------------------------------------------------------------------------------------
function LogErrorMessage(src, msg)
{
	LogMessageEx(GRP_SCRIPTLOGGER, L_ERROR, src, msg);
}

// --------------------------------------------------------------------------------------
// ScriptLogger
// --------------------------------------------------------------------------------------
// Used by the Configurable UI display components.
// Logs messages using the default paranoid group and L_INFO.
// This tries to use the CurrentFileName function defined in HtmlUtils, but will
// still work if that function is not defined.
// --------------------------------------------------------------------------------------
function ScriptLogger(msg)
{
	var src = "";
	if (typeof(CurrentFileName) == "function")
	{
		src = CurrentFileName();
	}
	else
	{
		src = window.location.pathname;
	}
	LogMessageEx(GRP_SCRIPTLOGGER, L_INFO, src, msg);
} 

// --------------------------------------------------------------------------------------
// RaiseScriptError
// --------------------------------------------------------------------------------------
// Raises an error message
// --------------------------------------------------------------------------------------
function RaiseScriptError(src, msg)
{
	if(!InitializeLogger())
		return;
		
	try
	{
		g_oScriptLogger.RaiseError(E_FAIL, "", 0, src, msg);
	}
	catch( err )
	{
		// Ignore
	}
}

// --------------------------------------------------------------------------------------
// TerminateLogger
// --------------------------------------------------------------------------------------
// Terminates the script logger
// --------------------------------------------------------------------------------------
function TerminateLogger()
{
	if( g_oScriptLogger == null )
		return;

	try
	{
		g_oScriptLogger.Uninitialize();
	}
	catch( err )
	{
		// ignore
	}

	g_oScriptLogger = null;
}
