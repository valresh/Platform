// ----------------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------------

// Some key-codes used in key-press handling

var VK_ESC				= 27;
var VK_ENTER			= 13;

var PARAM_DELIM			= "$$PARAMDELIM$$";
var VALUE_DELIM			= "$$VALUEDELIM$$";

// ----------------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------------

var g_strI18NText		= "";
var g_strFieldName		= "";
var g_strFilterValue	= "";
var g_strFilterState	= "";
var g_strFilterDesc		= "";

// ----------------------------------------------------------------------------------
// DoKeyPress
//
//	Invoked by the 'onkeypress' event on the body element. This is used to provide
//	keyboard support for such keys as 'ESC' and 'ENTER'
// ----------------------------------------------------------------------------------

function DoKeyPress()
{
	var key = window.event.keyCode;

	switch(key)
	{
	case VK_ESC:
		window.close();
		break;

	case VK_ENTER:
		// ENTER on a button element
		if( window.event.srcElement.tagName != "BUTTON" )
			DoUnload();
		break;

	default:
		break;
	}
}

// ----------------------------------------------------------------------------------
// DoLoad
//
//	Invoked by the 'onload' event of the body element. It is responsible for 
//	retrieving and restoring the previous state of the dialog.
// ----------------------------------------------------------------------------------

function DoLoad()
{
	var item = window.dialogArguments;
	var strParam = item.Param;
	var strValue = item.Value;

	// Set the cancel value to 'true' in case we terminate early
	window.returnValue = true;

	// Split the param at the PARAM_DELIM charater. This should return an array
	// containing two strings - the first being the table type, and the
	// second being the field name.
	
	var rgParams = strParam.split(PARAM_DELIM);

	g_strI18NText = rgParams[0];
	g_strFieldName = rgParams[1];	

	// Split the value at the VALUE_DELIM separator. This should return an array
	// containing three strings - the first being the filter value string, 
	// the second the persisted state string for the dialog, and the third being
	// the filter description.

	if (strValue == null)
	{
		g_strFilterValue = "";
		g_strFilterState = null;
		g_strFilterDesc = "";
	}
	else
	{
		var rgValue = strValue.split(VALUE_DELIM);
		g_strFilterValue = rgValue[0];
		g_strFilterState = rgValue.length > 1 ? rgValue[1] : null;
		g_strFilterDesc = rgValue.length > 2 ? rgValue[2] : null;
	}

	// Load 
	LoadDialog();
}

// ----------------------------------------------------------------------------------
// DoUnload
//
//	Invoked by the user clicking the 'OK' button to apply a filter. 
// ----------------------------------------------------------------------------------

function DoUnload()
{
	var item = window.dialogArguments;	

	// Set the new value
	item.Value = GetValue() + VALUE_DELIM + GetState() + VALUE_DELIM + GetDescription();
	
	// Unload
	UnloadDialog();
	
	// Set the cancel value to 'false' since we're setting a new value
	window.returnValue = false;
	
	window.close();
}

  