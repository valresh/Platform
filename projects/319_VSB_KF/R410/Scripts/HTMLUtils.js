//***************************************************************************
// Copyright (C) 2006 Honeywell Limited
// $Archive: /System Displays/_Master/Common/Scripts/HTMLUtils.js $								
// $Workfile: HTMLUtils.js $												
// $Modtime: 24/12/08 12:20p $											
// $Revision: 39 $
// $Author: Scordon $														
//***************************************************************************
// The following group of functions depend on the Microsoft IE DHTML DOM,
// and may not be compatible with other browsers.
//***************************************************************************
// If you reference this script file then you also need to reference:
// ErrorHandling.js (ScriptError)
// JSExtensions.js (JsType*)
//***************************************************************************

// --------------------------------------------------------------------------------------
// Possible values for the readyState property of an HTML element.
// --------------------------------------------------------------------------------------
var HtmlStateUninitialized = "uninitialized";   // Object is not initialized with data.
var HtmlStateLoading = "loading";               // Object is loading its data. 
var HtmlStateLoaded  = "loaded";                // Object has finished loading its data. 
var HtmlStateInteractive = "interactive";       // User can interact with the object even though it is not fully loaded. 
var HtmlStateComplete = "complete";             // Object is completely initialized. 

// --------------------------------------------------------------------------------------
// ParseZoom
// --------------------------------------------------------------------------------------
//	Converts a zoom value returned by style.zoom into a float.
// --------------------------------------------------------------------------------------
function ParseZoom(z)
{
	var dZoom = 1.0;

	if( typeof(z) == JsTypeString )
	{
		if( z != "normal" )
		{
			if( z.length > 0 )
			{
				if( z.charAt( z.length - 1 ) == '%' )
				{	
					dZoom = parseFloat(z) / 100.0;
				}
				else
				{
					dZoom = parseFloat(z);
				}
			}
		}
	}
	else if( typeof(z) == JsTypeNumber )
	{
		dZoom = z;
	}

	if( dZoom <= 0 ) dZoom = 1.0;

	return dZoom;
}

// ----------------------------------------------------------------------------------
// Pad
// ----------------------------------------------------------------------------------
//	Pads the string representation of the specified value with the specified character
//	if its length is less than that desired.
// ----------------------------------------------------------------------------------
function Pad(value, numChars, character, bPadLeft)
{
	var s = "" + value;
	
	while( s.length < numChars )
	{
		if( bPadLeft )
			s = character + s;
		else
			s = s + character;
	}
	
	return s;
}

//------------------------------------------------------------------------
// StripPath
//------------------------------------------------------------------------
// This function strips the path information from a file-path or URL,
// returning the file with no attached path information.
//------------------------------------------------------------------------
function StripPath(fileAndPath)
{
	var file = new String(fileAndPath);
	var cutoffLeft = file.lastIndexOf("/");
	if (cutoffLeft > 0 && cutoffLeft == file.length - 1)
	{
	    // The '/' character is a terminating one: the href property does this for some reason, so remove it and continue.
	    file = file.substring(0, cutoffLeft);
	    cutoffLeft = file.lastIndexOf("/");
	}
	if (cutoffLeft > 0)
	{
		file = file.substr(cutoffLeft + 1);
	}

	cutoffLeft = file.lastIndexOf("\\");			
	if (cutoffLeft > 0)
	{
		file = file.substr(cutoffLeft + 1);
	}
	cutoffLeft = null;
	return file;
}

//------------------------------------------------------------------------
// GetPath
//------------------------------------------------------------------------
// This function returns the path information from a file-path or URL.
//------------------------------------------------------------------------
function GetPath(fileAndPath)
{
	var path = new String(fileAndPath);
	var cutoffLeft = path.lastIndexOf("/");
	if (cutoffLeft < 0)
	{
		cutoffLeft = path.lastIndexOf("\\");
	}
	if (cutoffLeft > 0)
	{
		path = path.substring(0, cutoffLeft + 1);
	}
	cutoffLeft = null;
	return path;
}

//------------------------------------------------------------------------
// CombinePath
//------------------------------------------------------------------------
// This function combines a base and relative path, ensuring that 
// appropriate and consistent sepearators are in use in the result.
//------------------------------------------------------------------------
function CombinePath(path, file)
{
    var correctSeparator = "/";
    var wrongSeparator = new RegExp("\\\\", "g");
    if (typeof(path) == JsTypeString && path.indexOf(correctSeparator) < 0 && path.indexOf("\\") >= 0)
    {
        // Reverse the correct/wrong status of the separators :)
        correctSeparator = "\\";
        wrongSeparator = new RegExp("/", "g");
    }
    
    var correctedPath = new String(path.replace(wrongSeparator, correctSeparator));
    if (correctedPath.length > 0 && correctedPath.lastIndexOf(correctSeparator) != correctedPath.length - 1)
    {
        // Ensure the path has a terminating separator.
        correctedPath = correctedPath + correctSeparator;
    }

    var correctedFile = new String(file.replace(wrongSeparator, correctSeparator));
    if (correctedFile.length > 0 && correctedFile.indexOf(correctSeparator) == 0)
    {
        // Ensure the file does _not_ have a starting separator.
        correctedFile = correctedFile.substr(1);
    }
    correctSeparator = null;
	wrongSeparator = null;
    return correctedPath + correctedFile;
}

//------------------------------------------------------------------------
// StripQuery
//------------------------------------------------------------------------
// This function strips the query information from a file-path or URL,
// returning the file with no attached query information.
//------------------------------------------------------------------------
function StripQuery(fileAndQuery)
{
	var file = new String(fileAndQuery);
	var cutoffRight = file.indexOf("?");
	if (cutoffRight < 0)
	{
		cutoffRight = file.indexOf("#");
	}
	if (cutoffRight > 0)
	{
		file = file.substring(0, cutoffRight);
	}
	cutoffRight = null;
	return file;
}

//------------------------------------------------------------------------
// GetBaseURL
//------------------------------------------------------------------------
// Function grabs the base URL of the server: http://servername/cms/
//------------------------------------------------------------------------
function GetBaseURL()
{
    return window.location.protocol + "//" + window.location.hostname + "/cms/";
}

//------------------------------------------------------------------------
// AppendUrlParameter
//------------------------------------------------------------------------
// This function appends a new key-value pair parameter to the end of an
// existing URL string.
//------------------------------------------------------------------------
function AppendUrlParameter(baseUrl, key, value)
{
    return baseUrl + (baseUrl.indexOf("?") >= 0 ? "&" : "?") + key + "=" + value;
}

//------------------------------------------------------------------------
// CurrentFileName
//------------------------------------------------------------------------
// This function returns the unadorned name of the currently loaded
// file (stripped of path and querystring information).
//------------------------------------------------------------------------
function CurrentFileName()
{
	return StripQuery(StripPath(window.location.href));
}

// --------------------------------------------------------------------------
// EndEventHandling
// --------------------------------------------------------------------------
// This function is called to terminate handling of the current DHTML event.
// Calling this will prevent the current event handler from triggering
// submission of the QuickSearch form.
// --------------------------------------------------------------------------
function EndEventHandling()
{
    if (window.event != null)
    {
        window.event.cancelBubble = true;
        window.event.returnValue = false;
    }	          
}

// --------------------------------------------------------------------------
// InsertScriptBlock
// --------------------------------------------------------------------------
// This function inserts a client script block in the specified language
// into the current display.
// --------------------------------------------------------------------------
function InsertScriptBlock(language, script)
{
	// Create a script block for the required function in the header.
	var scriptBlock = window.document.createElement("script");
	scriptBlock.language = language;
	scriptBlock.type = "text/" + language;
	scriptBlock.text = script;
	window.document.body.appendChild(scriptBlock);
	scriptBlock = null;
}

//------------------------------------------------------------------------
// HtmlDomObject
//------------------------------------------------------------------------
// This function returns the DOM object with the argument for an ID if
// the argument is a string, or just the argument if it is already a 
// DOM object.
//------------------------------------------------------------------------
function HtmlDomObject(target)
{
	return (typeof(target) == JsTypeString ? document.all(target) : target);
}

//------------------------------------------------------------------------
// ZoomFactor
//------------------------------------------------------------------------
// This function returns the numeric zoom factor that has been 
// applied to the given element.
//------------------------------------------------------------------------
function ZoomFactor(elem)
{
	var objElem = HtmlDomObject(elem);
	if (objElem == null || 
		typeof(objElem) != JsTypeObject)
	{
		return 1;
	}
	else
	{
		var zoom = objElem.currentStyle.zoom;
		var nZoom = 1;
	    if (typeof(zoom) == JsTypeString)
	    {
	        if (zoom.toLowerCase() == "normal")
	        {
	            nZoom = 1;
	        }
	        else if (zoom.indexOf("%") >= 0)
	        {
	            nZoom = parseFloat(zoom)/100;
	        }
	        else
	        {
   	            nZoom = parseFloat(zoom);
	        }
	    }
	    else if (typeof(zoom) == JsTypeNumber)
	    {
	        nZoom = zoom;
	    }
	   
        // Accumulate any zoom factor specified on the parent element.
        if (objElem != objElem.ownerDocument.documentElement)
        {
            // Recurse up the tree of the current document.
            return nZoom * ZoomFactor(objElem.parentElement);
        }
        else
        {
            // Recurse up the tree of document windows and frames.
            return nZoom * ZoomFactor(objElem.ownerDocument.parentWindow.frameElement);
        }
	}
}

//------------------------------------------------------------------------
// ActualLeft
//------------------------------------------------------------------------
// This function returns the offset (in pixels) between the left edge of
// the element and the body of the page in which it is hosted.
//------------------------------------------------------------------------
function ActualLeft(elem)
{
	var objElem = HtmlDomObject(elem);
	if (objElem == null || 
		typeof(objElem) != JsTypeObject)
	{
		return 0;
	}
	else
	{    
	    var canvasElem = objElem.ownerDocument.documentElement;
	    if (objElem == canvasElem)
	    {
	        return 0; 
	    } 
	    var canvasBounds = canvasElem.getBoundingClientRect();
	    var canvasOffset = canvasBounds.left - canvasElem.scrollLeft;

 	    var bodyElem = objElem.ownerDocument.body;
	    var bodyBounds = bodyElem.getBoundingClientRect();
	    var bodyScale = (bodyBounds.right - bodyBounds.left)/bodyElem.offsetWidth;
	    var bodyOffset = bodyElem.clientLeft - bodyElem.scrollLeft; 
	        
	    if (objElem == bodyElem)
	    {
	        return (bodyBounds.left - canvasOffset)/bodyScale - bodyOffset
	    }
	    else
	    {
	        var elemBounds = objElem.getBoundingClientRect();
	        var elemOffset = (canvasOffset > 0 ? objElem.clientLeft : 0);   // We only need to offset this in strict mode, => canvasOffset > 0.
	        return (elemBounds.left + elemOffset - canvasOffset)/bodyScale - bodyOffset;
	    }
	}
}

//------------------------------------------------------------------------
// ActualTop
//------------------------------------------------------------------------
// This function returns the offset (in pixels) between the top edge of
// the element and the body of the page in which it is hosted.
//------------------------------------------------------------------------
function ActualTop(elem)
{
	var objElem = HtmlDomObject(elem);
	if (objElem == null || 
		typeof(objElem) != JsTypeObject)
	{
		return 0;
	}
	else
	{
	    var canvasElem = objElem.ownerDocument.documentElement;
	    if (objElem == canvasElem)
	    {
	        return 0; 
	    } 
	    var canvasBounds = canvasElem.getBoundingClientRect();
	    var canvasOffset = canvasBounds.top - canvasElem.scrollTop;

 	    var bodyElem = objElem.ownerDocument.body;
	    var bodyBounds = bodyElem.getBoundingClientRect();
	    var bodyScale = (bodyBounds.bottom - bodyBounds.top)/bodyElem.offsetHeight;
	    var bodyOffset = bodyElem.clientTop - bodyElem.scrollTop; 
	        
	    if (objElem == bodyElem)
	    {
	        return (bodyBounds.top - canvasOffset)/bodyScale - bodyOffset
	    }
	    else
	    {
	        var elemBounds = objElem.getBoundingClientRect();
	        var elemOffset = (canvasOffset > 0 ? objElem.clientTop : 0);   // We only need to offset this in strict mode, => canvasOffset > 0.
	        return (elemBounds.top + elemOffset - canvasOffset)/bodyScale - bodyOffset;
	    }	
	}
}

//------------------------------------------------------------------------
// FitToContainer
//------------------------------------------------------------------------
// Apply this function to an html element to make it position itself
// to take up as much of its container as possible.
// This should be assigned as the 'onload' event handler of the image
// element, and also as the onresize handler of the container.
//------------------------------------------------------------------------
function FitToContainer(element, fitInside, center, forPrint)
{
	try
	{
		
		if (element.complete != true && element.readyState != 'complete') 
		{             
		   // We need to wait until the element has been drawn to screen to fit it to its container. 
		   // Unfortunately, the readyState doesn't always change from loading to complete for some reason, 
		   // so we can just check that it's not in the uninitialized state and has size. 
		   element.attachEvent('onload', function() { FitToContainer(element, fitInside, center, forPrint); }); 
		   return; 
		} 

	    // Ensure that the variables to be the types that we need (be forgiving about input types).
	    element = HtmlDomObject(element);
	    fitInside = parseBoolean(fitInside, false);
	    center = parseBoolean(center, true);
	    forPrint = parseBoolean(forPrint, false);
	    
	    // Make sure the user won't see the image being adjusted.
	    if (!forPrint)
	    {
	        element.style.visibility = "hidden";
	    }
        
        // Ensure that the container won't change its size as a result of the
        // contents size changing: that would end up with a stack overflow if
        // this function is bound to the onresize handler, which is how it is
        // normally used.
	    var container = element.parentElement;
	    if (container.currentStyle.overflowX == "visible" ||
	        container.currentStyle.overflowY == "visible")
	    {
	        throw "Unable to fit element '" + element.id + "' to its container because the container's size depends on the size of its content.";
	    }
	    
        //LogErrorMessage("HTMLUtils::FitImageToContainer", "for element '" + element.id + "', fitInside = '" + fitInside + "', center = '" + center + "', forPrint = '" + forPrint + "'");     
	    	    
	    // Work out the relative aspect ratios of the container and content.
	    var containerWidth = parseFloat(container.clientWidth);
        var containerHeight = parseFloat(container.clientHeight);
        
        var contentWidth = parseFloat(element.offsetWidth);
        var contentHeight = parseFloat(element.offsetHeight);

        var xScale = contentWidth/containerWidth;
        var yScale = contentHeight/containerHeight;
        var aspectRatio = contentWidth / contentHeight;

        // Calculate the correct CSS properties for the element.
        var elemWidth = "auto";
        var elemHeight = "auto";
        var marginX = 0;
        var marginY = 0;
        var scrollRequired = false;
        
        if (((xScale >= yScale) && fitInside) || 
            ((yScale > xScale) && !fitInside))
        {
            // We need to match the width of the content to the width of the container.
            elemWidth = containerWidth;
            if (center)
            {
                // Calculate the margin that will vertically-center the content in the container.
                marginY = (containerHeight - (containerWidth / aspectRatio)) / 2;
                if (marginY < 0 && container.currentStyle.overflowY != "hidden")
                {
                    if (forPrint)
                    {
                        container.style.overflowY = "hidden";   // Do not support scrolled images when printing.
                    }
                    else
                    {
                        scrollRequired = true;
                        marginY = 0;
                    }
                }
            }
        }
        else
        {
            // Match the height of the element to the height of its container.
            elemHeight = containerHeight;
            if (center)
            {
                // Calculate the margin that will horizontally-center the content in the container.
                marginX = (containerWidth - (containerHeight * aspectRatio)) / 2;
                if (marginX < 0 && container.currentStyle.overflowX.toLowerCase() != "hidden")
                {
                    if (forPrint)
                    {
                        container.style.overflowX = "hidden";   // Do not support scrolled images when printing.
                    }
                    else
                    {
                        scrollRequired = true;
                        marginX = 0;
                    }
                }
            }
        }
        
        // Apply the CSS styles to the element within the container.
        // Note that we need to set the margins on opposite sides symmetrically,
        // because otherwise the image will be off-center if the container has
        // its alignment already set to 'center'.
        if (forPrint && element.tagName.toLowerCase() == "img")
	    {
	        // We need to use dynamic styles in print mode, or the HMI-Web 
	        // card templates don't pick up the changes when printing.
            var selector = "#" + element.id; 
            if (element.id == "")
            {
                selector = "#" + element.parentElement.id + " img";
            }
                
            // Create a screen style sheet if one doesn't already exist.
            var screenStyleSheet = GetStyleSheetByTitle("screenStyles");
            if (screenStyleSheet == null)
            {
                // Create the style sheet.
                screenStyleSheet = CreateStyleSheet("screenStyles", "screen");
            }
            // Add the rules for this element to the screen stylesheet
            ChangeRule(screenStyleSheet, selector, "width", elemWidth);
            ChangeRule(screenStyleSheet, selector, "height", elemHeight);
            ChangeRule(screenStyleSheet, selector, "margin-left", marginX);
            ChangeRule(screenStyleSheet, selector, "margin-right", marginX);
            ChangeRule(screenStyleSheet, selector, "margin-top", marginY);
            ChangeRule(screenStyleSheet, selector, "margin-bottom", marginY);
    
            // Create a print style sheet if one doesn't already exist.
            var printStyleSheet = GetStyleSheetByTitle("printStyles");
            if (printStyleSheet == null)
            {
                // Create the style sheet.
                printStyleSheet = CreateStyleSheet("printStyles", "print");
            }
            
            // Add the rules for this element to the print stylesheet.
            ChangeRule(printStyleSheet, selector, "width", (elemWidth == "auto" ? elemWidth : elemWidth));
            ChangeRule(printStyleSheet, selector, "height", (elemHeight == "auto" ? elemHeight : elemHeight));
            ChangeRule(printStyleSheet, selector, "margin-left", (marginX));
            ChangeRule(printStyleSheet, selector, "margin-right", (marginX));
            ChangeRule(printStyleSheet, selector, "margin-top", (marginY));
            ChangeRule(printStyleSheet, selector, "margin-bottom", (marginY));
	    }
	    else
	    {
	        // We can just set the new CSS styles directly on the element.
	        element.style.width = elemWidth;
	        element.style.height = elemHeight;
	        element.style.marginLeft = marginX;
	        element.style.marginRight = marginX;
            element.style.marginTop = marginY;
            element.style.marginBottom = marginY;
        }    
        
        if (scrollRequired)
        {
            ScrollToCenter(container);
        }
	}
	catch(e)
	{
		ScriptError("HTMLUtils.js: FitToContainer", e);
	}
	
    try
	{
	    // Make the element visible again.
	    // Do this after the catch block so that if anything goes wrong in the script at least the image is still visible.
	    if (!forPrint)
	    {
	        element.style.visibility = "visible";
	    }
	}
    catch(e)
    {
        ScriptError("HTMLUtils.js: FitToContainer", e);
    }
}

//------------------------------------------------------------------------
// ScrollToCenter
//------------------------------------------------------------------------
// If the parameter element has contents bigger than its size, then
// it will be scrolled to show the center of its contents.
//------------------------------------------------------------------------
function ScrollToCenter(elem)
{
	try
	{
		var overhangX = elem.scrollWidth - elem.clientWidth;
		if (overhangX > 0)
		{
			elem.scrollLeft = overhangX / 2;
		}
		
		var overhangY = elem.scrollHeight - elem.clientHeight;
		if (overhangY > 0)
		{
			elem.scrollTop = overhangY / 2;
		}
	}
	catch(e)
	{
		ScriptError("HTMLUtils.js: ScrollToCenter", e);
	}
}

//------------------------------------------------------------------------
// ChangeRule
//------------------------------------------------------------------------
// Changes or adds the given rule with the given selector from the given style sheet.
//------------------------------------------------------------------------
function ChangeRule(styleSheet, selectorText, rule, value)
{
    for (var i=0; i<styleSheet.rules.length; i++)
    {
        if ((styleSheet.rules[i].selectorText == selectorText) && (styleSheet.rules[i].style.cssText.indexOf(rule.toUpperCase()) != -1))
        {
            // Remove the rule.
            styleSheet.removeRule(i)
            // Re-add the rule.
            styleSheet.addRule(selectorText, (rule + ":" + value));
            return;
        }
    }
    
    // If we haven't found a rule to exist yet, we'll add it instead.
    styleSheet.addRule(selectorText, (rule + ":" + value));
}

//------------------------------------------------------------------------
// ClearStyleSheet
//------------------------------------------------------------------------
// Clears all the rules from the given style sheet.
//------------------------------------------------------------------------
function ClearStyleSheet(styleSheet)
{
    while (styleSheet.rules.length > 0)
    {
        styleSheet.removeRule();
    }
}

//------------------------------------------------------------------------
// CreateStyleSheet
//------------------------------------------------------------------------
// Creates a style sheet with the given title for the given media.
//------------------------------------------------------------------------
function CreateStyleSheet(styleSheetTitle, media)
{
    // Create the style sheet.
    styleSheet = document.createStyleSheet();
    styleSheet.title = styleSheetTitle;
    styleSheet.media = media;
    styleSheetObject = styleSheet.owningElement || styleSheet.ownerNode;
    styleSheetObject.setAttribute("type", "text/css");
    
    // Return the newly created stylesheet.
    return styleSheet;
}

//------------------------------------------------------------------------
// GetStyleSheetByTitle
//------------------------------------------------------------------------
// Returns the first style sheet in this documents style sheet collection
// with the given title.  If no style sheet with the given title is found
// it returns null.
//------------------------------------------------------------------------
function GetStyleSheetByTitle(styleSheetTitle)
{
    // Get the document style sheets collection.
    var styleSheetsCollection = document.styleSheets;
    
    var styleSheet = null;
    
    // Navigate through the style sheets collection to find the first style sheet with the given title.
    for (var i=0; i<styleSheetsCollection.length; i++)
    {
        if (styleSheetsCollection[i].title == styleSheetTitle)
        {
            styleSheet = styleSheetsCollection[i];
            break;
        }
    }

    return styleSheet;
}

//------------------------------------------------------------------------
// ClassIsOnElement
//------------------------------------------------------------------------
// This function determines whether a particular class is currently 
// applied to the given element.
//------------------------------------------------------------------------
function ClassIsOnElement(elem, className)
{
    var element = HtmlDomObject(elem);
    if (element != null)
    {
        return (element.className.toLowerCase().indexOf(className.toLowerCase()) >= 0);
    }
    return false;
}

//------------------------------------------------------------------------
// IncludeClassOnElement
//------------------------------------------------------------------------
// This function makes sure the given class is included in the list of
// css classes applied to this element.
//------------------------------------------------------------------------
function IncludeClassOnElement(elem, className)
{
    
    var element = HtmlDomObject(elem);
    if (element != null && !ClassIsOnElement(element, className))
    {
        element.className = element.className + " " + className;
    }
}

//------------------------------------------------------------------------
// RemoveClassFromElement
//------------------------------------------------------------------------
// This function makes sure the given class is not included in the list of
// css classes applied to this element.
//------------------------------------------------------------------------
function RemoveClassFromElement(elem, className)
{
    var element = HtmlDomObject(elem);
    if (element != null && ClassIsOnElement(element, className))
    {
        element.className = element.className.toLowerCase().replace(className.toLowerCase(), "").replace(/(\s+$)|(^\s+)/g, "");
    }  
}

//------------------------------------------------------------------------
// GetURLParameters
//------------------------------------------------------------------------
// This function returns an associative array of key value pairs which
// can be accessed by: var value = myArray['key'];
//------------------------------------------------------------------------
function GetURLParameters(urlString, makeKeysLowerCase)
{
    var doLowerCase = parseBoolean(makeKeysLowerCase, false);

    // Create an array which we will extend to hold key value pairs as well.
    var urlKeyValuePairs = new Object;
    var parameterCount = 0;
    try
    {
        // Make sure we only have the query string of a URL - things after the last "?".
        var urlParametersString = urlString;
        if (urlString.indexOf('?') >= 0)
        {
            // Use the first index of '?' because a valid URL string can surprisingly contain more than one '?'
            // The first one is the start of the query string marker, but consecutive '?'s are actually treated as
            // normal '?' characters.  We find this occuring when we use the redirect URL and want to pick up its
            // URL parameters as well.
            urlParametersString = urlString.substr(urlString.indexOf('?') + 1);
        }
        
        // Extract the key-value pairs from the string. 
        // Note that the RegExp puts the first parsed-out value at 
        // index 1 of its results array, not at index 0.
        var parameterExpression = new RegExp("(?:^|&)[ \t]*([A-Za-z0-9_]+)[ \t]*=[ \t]*([A-Za-z0-9_/\.]*)[ \t]*", "ig");
        var parameterParts;
        var parameterKey;
        while ((parameterParts = parameterExpression.exec(urlParametersString)) != null)
        {
            //LogErrorMessage("GetURLParameters", "Parsing parameter expression '" + parameterParts[0] + "'");
            parameterKey = parameterParts[1];
            if (doLowerCase)
            {
                parameterKey = parameterKey.toLowerCase();
            }
            urlKeyValuePairs[parameterKey] = parameterParts[2];
            parameterCount++;
        }
    }
    catch(e)
    {
        ScriptError("GetURLParameters", e);
    }
	parameterKey = null;
	parameterParts = null;
	parameterExpression = null;
	urlParametersString = null;
    if (parameterCount > 0)
    {
        // Put the length in the associative array and return it.
        urlKeyValuePairs.length = parameterCount;
        return urlKeyValuePairs;
    }
    else
    {
        // No query parameters were found, so just return null instead.
        return null;
    }
}

//------------------------------------------------------------------------
// GetCookieParameters
//------------------------------------------------------------------------
// This function parses the cookie-string for the current document, and
// returns an associative array of key value pairs which can be accessed 
// by: var value = myArray['key'];
//------------------------------------------------------------------------
function GetCookieParameters()
{
    // Load the cookie information.
    var cookieArray = document.cookie.split(";");
    var readCookie = new Object;
    
    for (var i=0; i < cookieArray.length; i++)
    {
        var currentKeyVal = cookieArray[i].split("="); 
        if (currentKeyVal[1] != undefined)
        {
            readCookie[currentKeyVal[0].trim()] = unescape(currentKeyVal[1].trim());
        }
        else
        {
            readCookie[currentKeyVal[0].trim()] = "";            
        }
    }

    // Return an associative array containing the key value pairs.
	cookieArray= null;
    return readCookie;
}

//------------------------------------------------------------------------
// GetCookieValue
//------------------------------------------------------------------------
// This function returns the value of the specified cookie if found,
// otherwise null.
//------------------------------------------------------------------------
function GetCookieValue(cookieName)
{
	var cookieValue = null;
	var cookies = GetCookieParameters();
	
	if(cookies != null && typeof(cookies[cookieName]) != JsTypeUndefined)
	{
		cookieValue = cookies[cookieName];
	}
	cookies = null;
	return cookieValue;
}

/// --------------------------------------------------------------------------------------
/// AddOption
/// --------------------------------------------------------------------------------------
/// This funtion is provided to overcome the IE BUG documented by Q276228.
/// This supposedly only effected IE up to ver 5.....
/// It encapsulates the creation of a new option for SELECT object as writing directly
/// to the innerHTML does not always work. 
/// It does appear to work for clearing the current options.
/// EG: oSelect.innerHTML = ""; 
/// --------------------------------------------------------------------------------------
function AddOption(selectElement, optionValue, optionText)
{
	var oSelect = HtmlDomObject(selectElement);
	var oOption = document.createElement("OPTION");
	oOption.text = optionText;
	oOption.value = optionValue;
	oSelect.add(oOption);
	oSelect= null;
	oOption = null;
}

//------------------------------------------------------------------------
// GetTotalDivWidth
//------------------------------------------------------------------------
// This function determines the width of a specified div including paddings, margins and borders
//------------------------------------------------------------------------
function GetTotalDivWidth(elem)
{
	totalWidth = elem.clientWidth;

	if(paddingLeft = parseInt(elem.style.paddingLeft)) {
		totalWidth += paddingLeft
	}
	if(paddingRight = parseInt(elem.style.paddingRight)) {
		totalWidth += paddingRight
	}
	if(marginLeft = parseInt(elem.style.marginLeft)) {
		totalWidth += marginLeft
	}
	if(marginRight = parseInt(elem.style.marginRight)) {
		totalWidth += marginRight
	}
	if(borderLeft  = parseInt(elem.style["border-left-width"])) {
		totalWidth += borderLeft
	}
	if(borderRight = parseInt(elem.style["border-right-width"])) {
		totalWidth += borderRight
	}
	return totalWidth;
}

//------------------------------------------------------------------------
// GetRemainingWidthForLastChild
//------------------------------------------------------------------------
// This function determines the remaining width in a div for its last child
//------------------------------------------------------------------------
function GetRemainingWidthForLastChild(divID)
{
	parentDiv = document.getElementById(divID);
	children = parentDiv.childNodes;
	childrenWidth = 0;
	
	//The last child has been excluded
	for(i=0; i < (children.length - 1); i++) {
		childrenWidth += GetTotalDivWidth(children[i]);
	}
	
	return (parentDiv.clientWidth - childrenWidth);
}
