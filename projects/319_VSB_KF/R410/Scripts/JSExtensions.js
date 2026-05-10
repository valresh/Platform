//***************************************************************************
// Copyright (C) 2006 Honeywell Limited
// $Archive: /System Displays/Web Application Displays/Scripts/JSExtensions.js $								
// $Workfile: JSExtensions.js $												
// $Modtime: 11/08/07 12:46p $											
// $Revision: 7 $
// $Author: Stickell $														
//***************************************************************************
// The following functions extend the intrinsic Javascript object classes.
//***************************************************************************

//***************************************************************************
// Useful constants
//***************************************************************************
//---------------------------------------------------------------
// Possible values returned by the typeof operator.
//---------------------------------------------------------------
var JsTypeBoolean = "boolean";
var JsTypeFunction = "function";
var JsTypeNumber = "number";
var JsTypeObject = "object";
var JsTypeString = "string";
var JsTypeUndefined = "undefined";
var JsTypeUnknown = "unknown";  // Usually means a VBScript object that cannot be directly converted to a JS type.

//***************************************************************************
// Extensions to the String object.
//***************************************************************************
//---------------------------------------------------------------
// trim
//---------------------------------------------------------------
// Add the the VB-like Trim functionality.
//---------------------------------------------------------------
String.prototype.trim = function() 
{ 
    var str = this.toString();
    return str.replace(/(\s+$)|(^\s+)/g, '');
}

//---------------------------------------------------------------
// remove
//---------------------------------------------------------------
// Strip a substring from the string.
//---------------------------------------------------------------
String.prototype.remove = function(stringToRemove) 
{ 
    var re = new RegExp(stringToRemove,"ig");
    return this.toString().replace(re, '').trim();
}

//----------------------------------------------------------
// string
//----------------------------------------------------------
// VB-like string
//----------------------------------------------------------
String.prototype.string = function(l) 
{ 
    var s = '', i = 0; 
    while (i++ < l) 
    { 
        s += this; 
    } 
    return s; 
}

//----------------------------------------------------------
// zf
//----------------------------------------------------------
// The string format for Zero-Fill
//----------------------------------------------------------
String.prototype.zf = function(len) 
{ 
    return '0'.string(len - this.length) + this; 
}

//----------------------------------------------------------
// pad
//----------------------------------------------------------
// Pads the string representation of the specified value 
// with the specified character if its length is less than 
// that desired.
//----------------------------------------------------------
String.prototype.pad = function(numChars, character, bPadLeft)
{
    var padded = this;
	while( padded.length < numChars )
	{
		if( bPadLeft )
			padded = character + padded;
		else
			padded += character;
	}
	return padded;
}

//----------------------------------------------------------
// htmlEncode
//----------------------------------------------------------
// This function was lifted from a script-sample at 
// http://www.ericphelps.com/scripting/samples/Decode.htm
// The author has explicitly stated that the page-code is in the public 
// domain and may be re-used. That page contains decode and encode functions 
// for several more encoding types - including printable text, rot13, hex,
// base 64 if we ever need one of those.
//----------------------------------------------------------
String.prototype.htmlEncode = function() 
{
	if (!isNaN(parseFloat(this)))
	{
		return this;
	}
		
	var strBuffer = new String();
	for (var i = 0; i < this.length; i++)
	{
		strBuffer = strBuffer.concat("&#" + this.charCodeAt(i) + ";");
	}
	return strBuffer;
}

//***************************************************************************
// Extensions to the Number object.
//***************************************************************************
//---------------------------------------------------------------
// zf
//---------------------------------------------------------------
// The Number format for Zero-Fill
//---------------------------------------------------------------
Number.prototype.zf = function(len)
{ 
    return this.toString().zf(len); 
}

//***************************************************************************
// Extensions to the Boolean object.
//***************************************************************************
//---------------------------------------------------------------
// parseBoolean
//---------------------------------------------------------------
// This function attempts to parse a boolean value from its input 
// argument, and accepts a default value for use if the parse 
// was not possible.
//---------------------------------------------------------------
function parseBoolean(varInput, bDefault)
{
	var inputType = typeof(varInput);
	if (varInput == null || 
	    inputType == JsTypeUndefined ||
	    inputType == JsTypeObject)
	{
	    return bDefault;
	}
	if (inputType == JsTypeBoolean)
	{
		return varInput;
	}
	else if (inputType == JsTypeNumber || (inputType == JsTypeString && !isNaN(parseInt(varInput, 10))))
	{
		return new Boolean(parseInt(varInput, 10));
	}
	else if (inputType == JsTypeString && varInput.toLowerCase() == new String("" + true))
	{
		return true;
	}
	else if (inputType == JsTypeString && varInput.toLowerCase() == new String("" + false))
	{
		return false;
	}
	else
	{
		return new Boolean(bDefault);
	}
}

//***************************************************************************
// Extensions to the Array object.
//***************************************************************************
//-----------------------------------------------------------------------
// TwoDArray
//-----------------------------------------------------------------------
// This constructor creates a 2-dimensional array of the specified size,
// and initializes each entry in it to a specified default value.
// Note that this cannot be a new constructor for Array because that
// object alreadyhas a constructor which can accept any number of arguments.
//-----------------------------------------------------------------------
function TwoDArray(rowCount, columnCount, defaultValue)
{
    var arr = new Array(rowCount);
    for (var i = 0; i < rowCount; i++)
    {
        arr[i] = new Array(columnCount);
        for (var j = 0; j < columnCount; j++)
        {
            arr[i][j] = defaultValue;
        }
    }
    return arr;
}




