// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// opapps_common.js

var hscDummyPointName = "!!!";
var hscGlobalPipelinePoint = "";
var hscGlobalRoutePoint = "";
var hscGlobalSegmentPoint = "";
var hscGlobalCompressorPoint = "";

var _DISPLAYTYPE_PipelinePressureProfile = "PipelinePressureProfile";
var _DISPLAYTYPE_RouteProfile = "RouteProfile";
var _DISPLAYTYPE_PipelineInferredFlowProfile = "PipelineInferredFlowProfile";
var _DISPLAYTYPE_SegmentInferredFlow = "SegmentInferredFlow";
var _DISPLAYTYPE_SegmentMassBalance = "SegmentMassBalance";
var _DISPLAYTYPE_CompressorMap = "CompressorMap";
var _DISPLAYTYPE_DifferentialPressure = "DifferentialPressure";
var _RESPONSETYPE_CompressorMapDefinition = "CompressorMapDefinition";
var _RESPONSETYPE_CompressorMapHistory = "CompressorMapHistory";

var _VIEWNAME_SegmentInferredFlow = "FlowTrend";

var _MAX_SPACING_ITERATIONS = 1000;

function onSilverlightError(sender, args) {
	var appSource = "";
	if (sender != null && sender != 0) {
		appSource = sender.getHost().Source;
	}

	var errorType = args.ErrorType;
	var iErrorCode = args.ErrorCode;

	if (errorType == "ImageError" || errorType == "MediaError") {
		return;
	}

	var errMsg = "Unhandled Error in Silverlight Application " + appSource + "\n";

	errMsg += "Code: " + iErrorCode + "    \n";
	errMsg += "Category: " + errorType + "       \n";
	errMsg += "Message: " + args.ErrorMessage + "     \n";

	if (errorType == "ParserError") {
		errMsg += "File: " + args.xamlFile + "     \n";
		errMsg += "Line: " + args.lineNumber + "     \n";
		errMsg += "Position: " + args.charPosition + "     \n";
	}
	else if (errorType == "RuntimeError") {
		if (args.lineNumber != 0) {
			errMsg += "Line: " + args.lineNumber + "     \n";
			errMsg += "Position: " + args.charPosition + "     \n";
		}
		errMsg += "MethodName: " + args.methodName + "     \n";
	}

	throw new Error(errMsg);
}

String.prototype.visualWidth = function () {
	var ruler = document.getElementById("ruler");
	ruler.innerHTML = this;
	var returnVal = ruler.offsetWidth;
	ruler.innerHTML = "";
	return returnVal;
}

String.prototype.visualHeight = function () {
	var ruler = document.getElementById("ruler");
	ruler.innerHTML = this;
	var returnVal = ruler.offsetHeight;
	ruler.innerHTML = "";
	return returnVal;
}

String.prototype.trimToPx = function (length) {
	var tmp = this;
	var trimmed = this;
	if (tmp.visualWidth() > length) {
		trimmed += "...";
		while (trimmed.visualWidth() > length) {
			tmp = tmp.substring(0, tmp.length - 1);
			trimmed = tmp + "...";
		}
	}

	return trimmed;
}

function windowWidth(margin) {
	var w = $(window).width() - 20;

	return ((w - margin.left - margin.right - 20) < 0) ? margin.left + margin.right + 2 : w;
}

function windowWidthCompressorMap(margin) {
	// This function is used when margin is 
	var w = $(window).width() - margin.left - margin.right;

	return w > 0 ? w : 20;
}


function windowHeight(margin) {
	var h = $(window).height() - 20;

	return ((h - margin.top - margin.bottom - 20 < 0) ?
		margin.top + margin.bottom + 2 : h) - $("#OpAppsHeader").outerHeight() - $("#OpAppsFooter").outerHeight();
}

function windowHeightCompressorMap(margin) {
	var h = $(window).height() - margin.top - margin.bottom - $("#OpAppsHeader").outerHeight() - $("#OpAppsFooter").outerHeight();

	return h > 0 ? h : 20;
}

function urldecode(str) {
	return decodeURIComponent((str + '').replace(/\+/g, '%20'));
}

function GetURLParam(name) {
	name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
	var regexS = "[\\?&]" + name + "=([^&#]*)";
	var regex = new RegExp(regexS);
	var results = regex.exec(urldecode(window.location.href));
	if (results == null)
		return "";
	else
		return results[1];
}

function SpaceValuesOut(theData, scaleFn, minTickLabelSize) {
	var minGap;					// Return the minimum gap on the last run through
	var singlePixelDataSize = 1 / (scaleFn(1) - scaleFn(0)); // Determine how much 1 pixel is in terms of scaled data values
	
	for (var iter = 0; iter < _MAX_SPACING_ITERATIONS; iter++) {
		var moved = false;		// Has a value moved significantly this iteration?
		minGap = Infinity;
		for (var i = 1; i < theData.length - 1; i++) {
			// Calculate gaps in pixel sizes
			var leftGap = scaleFn(theData[i]) - scaleFn(theData[i - 1]);
			var rightGap = scaleFn(theData[i + 1]) - scaleFn(theData[i]);

			if (leftGap < minTickLabelSize && rightGap < minTickLabelSize) {
				// Not enough space on either side, so move to the middle
				var old_pos = theData[i];
				var new_pos = (theData[i - 1] + theData[i + 1]) / 2;
				theData[i] = new_pos;

				if (Math.abs(old_pos - new_pos) > (singlePixelDataSize * 0.5)) {
					// Only consider the move significant if it caused more than a pixel's difference
					moved = true;
				}
			}
			else if (leftGap < minTickLabelSize) {
				// Move 1 pixel to the right
				theData[i] += singlePixelDataSize;
				moved = true;
			}
			else if (rightGap < minTickLabelSize) {
				// Move 1 pixel to the left
				theData[i] -= singlePixelDataSize;
				moved = true;
			}

			// Update the minimum gap
			minGap = Math.min(minGap, scaleFn(theData[i]) - scaleFn(theData[i - 1]), scaleFn(theData[i + 1]) - scaleFn(theData[i]));
		}

		if (moved == false)
			// Nothing has moved significantly so stop
			break;
	}

	return minGap;
}

function AdjustProfileValues(theData, adjustment, firstElement) {

	// for each profile value
	var adjustmentPerElement = adjustment / (theData.length-1);
	
	theData[0] = firstElement;
	var old_pos = theData[0];
	
	for (var i = 1; i < theData.length; i++) {		
		var new_pos = old_pos + adjustmentPerElement;
		old_pos = new_pos;
		theData[i] = new_pos;
	}
}


function interpolateProfileReadings(interval, x1, x2, y1, y2) {
    var profileReadings = [];

    var adjustment = (x2 - x1) / (interval - 1);
    var adjustmentSource = x1;
    var profileValue = y1;

    for (var i = 0; i < interval; i++) {
        var profileSrc = {};

        // get the interpolated position
        profileSrc["Position"] = adjustmentSource;
        profileSrc["Value"] = interpolateSingleReading(x1, x2, y1, y2, adjustmentSource);

        adjustmentSource = adjustmentSource + adjustment;
        profileReadings.push(profileSrc);
    }

    return profileReadings;
}

function interpolateSingleReading(x1, x2, y1, y2, xPosition)
{
    var yValue = 0;

    // Linear equation to calculate profile readings
    // y = ax + b
    var xDelta = x2-x1;
    if (xDelta == 0 )
    {
        xDelta=1;
    }

    var a = (y2 - y1) / xDelta;
    var b = y1 - (a * x1);
       
    // get the interpolated value
    yValue = a * xPosition + b;
    
    return yValue;
}

function sliceDataArray(pData, slicePos)
{
	var cData = new Array( new Array() );
	
	for( var i=0; i<pData.length; i++ )
	{		
 	 	for( var j=0; j < pData[i].length; j++ )
  		{
			if(arguments.length == 3){
				cData[i][j]= pData[i][j].slice(slicePos, arguments[2]);
			
			}
			else{
				cData[i][j]= pData[i][j].slice(slicePos);	
			}
  		}
	}

  return cData;
}

function snapToNode(xBisector, pData, xMouseCoord, tolerance)
{
	//xBisector is used to get x-axid coord from mouse position
	var index = xBisector(pData, xMouseCoord);
	var cData = null;
	
	// handle dataset boundaries, without compromising null
	if(index == 0)
	{
		cData = pData[0];
	}
	
	else if(index >= pData.length)
	{
		cData = null;
	}
	
	else
	{
		var vBefore = pData[index-1];
		var vAfter = pData[index];		
		var cBefore = xMouseCoord - vBefore[0];
		var cAfter = vAfter[0] - xMouseCoord;
		
		// if data is within tolerance
		if(cBefore < tolerance || cAfter < tolerance)
		{
			if( cBefore	<	cAfter )
			{
				cData	=	vBefore;
			}
			else 
			{				
				cData	=	vAfter			
			}
		}
	}
	
	return cData;
}

function snapToSegment(xBisector, pData, xMouseCoord, tolerance)
{
	//xBisector is used to get x-axid coord from mouse position
	var index = xBisector(pData, xMouseCoord);
	var cData = pData[index];
	
	if(index >= pData.length)
	{
		cData = pData[pData.length -1];
		index = 0;
	}
	
	if(index > 0)
	{
		var vBefore = pData[index-1];
		var vAfter = pData[index];		
		var cBefore = xMouseCoord - vBefore[0];
		var cAfter = vAfter[0] - xMouseCoord;
		
		if(cBefore < tolerance || cAfter < tolerance)
		{
			if(	cBefore	<	cAfter)
			{
				cData = vBefore;
			}
			else
			{
				cData = vAfter;
			}
		}
	}
	
	return cData;
}

var colorScheme = {
	LegendBackground: "#edeeef",
	LegendBorder: "#515c66",
	Pressure: "#2ecc71",
	ElevatedPressure: "#9b59b6",
	LeakPressure: "#ee3124",
	Temperature: "#3498db",
	OperatingRange: "rgb(230,230,230)",
	Selection: "#caced1",
	HoverOver: "lightgrey",
	RegularText: "black",
	ReferenceLine: "black",
	XAxisReferenceBox: "black",
	BaselineFill: "none",
	BaselineColor: "#f37021"
};

var APPID_SCRIPTLOGGER = "{DF71FC53-3A57-4081-B17B-0FADC9321884}";
var GRP_SCRIPT = 1024;

var L_ERROR = 10;
var L_LOG = 15;
var L_WARNING = 15;
var L_RANGE = 20;
var L_PARAM = 30;
var L_INFO = 35;
var L_PROFILE = 35;
var L_RETURN = 40;
var L_PRINT = 100;

var L_CALLUP_HI = 20;

var E_FAIL = 0x80004005;

var g_oScriptLogger = null;

// Use our own logging routines rather than those provided by Loggig.js.
// In a future release, we should consider using require.js and CoffeeScript to get access to such functionality.

// --------------------------------------------------------------------------------------
// InitializeLogger
// --------------------------------------------------------------------------------------
// Calling this function makes certain that the script logger has been initialized,
// returning 'true' if initialization has been successful, or false otherwise.
// --------------------------------------------------------------------------------------
function InitializeLogger() {
	DEFAULT_LOGGING_APP_GUID = '{70ea8c7e-f659-11d4-A90B-00C04F60246B}';

	if (g_oScriptLogger == null) {
		try {
			g_oScriptLogger = window.external.SilverlightServices.getLogger(DEFAULT_LOGGING_APP_GUID);
		}
		catch (err) {
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
function LogMessageEx(grp, par, src, msg) {
	if (!InitializeLogger())
		return;

	try {
		if (g_oScriptLogger.MessagePassesFilter(grp, par)) {
			g_oScriptLogger.LogMessageEx(grp, par, src, msg);
		}
	}
	catch (err) {
		// ignore
	}
}

// --------------------------------------------------------------------------------------
// LogMessage
// --------------------------------------------------------------------------------------
// Logs messages using the default paranoid group
// --------------------------------------------------------------------------------------
function LogMessage(par, src, msg) {
	LogMessageEx(GRP_SCRIPT, par, src, msg);
}

// --------------------------------------------------------------------------------------
// LogErrorMessage
// --------------------------------------------------------------------------------------
// Logs messages using the default paranoid group and L_ERROR
// --------------------------------------------------------------------------------------
function LogErrorMessage(src, msg) {
	LogMessageEx(GRP_SCRIPT, L_ERROR, src, msg);
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
