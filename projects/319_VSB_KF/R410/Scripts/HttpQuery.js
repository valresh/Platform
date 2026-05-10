//***************************************************************************
// Copyright (C) 2006 Honeywell Limited
// $Archive: /System Displays/Web Application Displays/Scripts/HttpQuery.js $								
// $Workfile: HttpQuery.js $												
// $Modtime: 11/28/07 5:31p $											
// $Revision: 14 $
// $Author: Stickell $														
//***************************************************************************
// This file contains functions which allow callers to perform synchronous or
// asynchronous (AJAX)queries against an HTTP data source. 
//***************************************************************************
// The functions in this file depend on:
//  ErrorHandling.js (ExceptionString, ScriptError)
//	JSExtensions.js (TwoDArray, JsType*)
//	Utilities.js (ParameterXml)
//***************************************************************************

/// --------------------------------------------------------------------------
/// Possible IXMLHTTPRequest readyState values
/// ms-help://MS.MSDNQTR.2006JAN.1033/xmlsdk/html/0e6a34e4-f90c-489d-acff-cb44242fafc6.htm
/// --------------------------------------------------------------------------
var RequestStateUninitialized = 0;  // The object has been created, but not initialized (the open method has not been called).
var RequestStateLoading = 1;        // The object has been created, but the send method has not been called.
var RequestStateLoaded = 2;         // The send method has been called, but the status and headers are not yet available.
var RequestStateInteractive = 3;    // Some data has been received. 
                                    // Calling the responseBody and responseText properties at this state to obtain 
                                    // partial results will return an error, because status and response headers are not fully available.
var RequestStateComplete = 4;       // All the data has been received, and the complete data is available in the responseBody and responseText properties.

/// --------------------------------------------------------------------------
/// Some HTTP Status codes (not the full list)
/// ms-help://MS.MSDNQTR.2006JAN.1033/xmlsdk/html/f6de15fc-72e9-418e-b275-d94b0b2045de.htm
/// --------------------------------------------------------------------------
var HttpStatusOK = 200;                 // The request completed and generated a complete response.
var HttpStatusNoContent = 204;          // The request completed and no response was required.
var HttpStatusPartialContent = 206;     // The request completed and only part of the available content has been sent back in the response.
var HttpStatusMovedPermanently = 301;   // The requested resource has moved permanently.
var HttpStatusFound = 302;              // The original resource is asking you to look elsewhere (used by Response.Redirect in ASP.NET).
var HttpStatusSeeOther = 303;           // The original resource is asking you to look elsewhere (used by CGI scripts).
var HttpStatusTemporaryRedirect = 307;  // The request is being redirected temporarily.

///------------------------------------------------------------------------
/// GetXmlHttpObject
///------------------------------------------------------------------------
/// <summary>
/// This function returns the XmlHttp object for the current browser type,
/// and works in both IE and FireFox.
/// </summary>
///------------------------------------------------------------------------
function GetXmlHttpObject()
{
    var xmlHttp = null; 
    if (window.XMLHttpRequest)
    {
        xmlHttp = new XMLHttpRequest();
    }
    else if (window.ActiveXObject)
    {
        try
        {
            xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
        }
        catch(e)
        {
            ScriptError("HttpQuery::GetXmlHttpObject - attempting Microsoft.XMLHTTP", e);
            try
            {
                xmlHttp = new ActiveXObject("MSXML2.XMLHTTP.6.0");
            }
            catch(e2)
            {
                ScriptError("HttpQuery::GetXmlHttpObject - attempting MSXML2.XMLHTTP.6.0", e2);        
            }
        }
    }
    return xmlHttp;
}

/// --------------------------------------------------------------------------
/// XmlHttpRequestInProgress
/// --------------------------------------------------------------------------
/// <summary>
/// This function tests whether the provided XmlHttp request object
/// currently has a request pending.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object you want to check the status of.
/// </param>
/// --------------------------------------------------------------------------
function XmlHttpRequestInProgress(xmlHttp)
{
    if (typeof(xmlHttp) == JsTypeUndefined || xmlHttp == null)
    {
        //alert("xmlHttp object is not initialized");
        return false;
    }
    else if (xmlHttp.readyState == RequestStateComplete ||
             xmlHttp.readyState == RequestStateUninitialized)
    {
        //alert("xmlHttp object's request has completed");
        return false;
    }
    return true;
} 

/// --------------------------------------------------------------------------
/// HandleXmlHttpResponse
/// -------------------------------------------------------------------------
/// <summary>
/// This function is called to parse and act on a successful XmlHttp response
/// that has been generated by the Honwyell.Web.CurrentContextHelper.SendXmlResponse
/// function. It is suitable for use as the 'success' handler to the
/// HandleXmlHttpStateChange function below.
///
/// This function understands the following response elements:
///  block:  replaces an element in the current document with the block-element's contents.
///  script: attempts to execute the script-elements contents as javascript.
///
/// Elements will be handled in the order in which they appear in the response.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object which contains the response text and body for the 
/// successful request.
/// </param>
/// --------------------------------------------------------------------------
function HandleXmlHttpResponse(xmlHttp)
{
    try
    {
        // Check that we actually _have_ elements in the response XML.
        if (xmlHttp.responseXML.documentElement == null)
        {
            var parseError = xmlHttp.responseXML.parseError;
            if (parseError.errorCode != 0)
		    {
			    throw "Error " + parseError.errorCode + " parsing XML response '" +  parseError.url + "': " + parseError.reason;
		    }
		    else
		    {
                throw "No XML document element available in response. No parse error information is available.";
            }
        }
        
        // Handle each response element in turn.
		var nodes = xmlHttp.responseXML.documentElement.selectNodes("/*/*");
		for (i=0; i < nodes.length; i++)
		{
		    var node = nodes[i];
		    var nodename = node.nodeName;
		    switch(nodename)
		    {
		        case "block":
		        {
		            // This node contains replacement content for one of
		            // the container elements on the page. Substitute that content in.
		            var nodeId = node.getAttribute("name").toLowerCase();
                    var targetElem = HtmlDomObject(nodeId);
                    if (targetElem == null)
                    {
                        LogErrorMessage("HttpQuery::HandleXmlHttpResponse", "Block content received for unexpected element '" + nodeId + "'.");
                    }
                    else
                    {
                        targetElem.outerHTML = node.text;
                    }
                    break;
		        }
		        case "script":
		        {
                    // We have been sent some JavaScript to execute.
                    (new Function(node.text))();
                    break;
		        }
		        case "prompt":
		        {
		            // We have been asked to prompt for an operator response.
		            // Extract the indormation we will need to reply...
		            var promptType = node.getAttribute("promptType");
		            var responseUrl = node.getAttribute("responseUrl");
		            var responseKey = node.getAttribute("responseKey");
		            var promptMessage = node.selectSingleNode("promptText").text;
		            var responseBody = node.selectSingleNode("responseBody").text;
		            var fnCallback = function(responseText)
		            {
		                // Do another ajax post for the prompt handler, reusing whichever
		                // handler function was used in the request which we are currently 
		                // handling the response for.
	                    var arrHeaders = TwoDArray(1, 2, "");
	                    arrHeaders[0][0] = "Content-Type";
	                    arrHeaders[0][1] = "application/x-www-form-urlencoded";
		                
		                DoXmlHttpQuery( xmlHttp, 
		                                false, 
		                                AppendUrlParameter(responseUrl, responseKey, responseText),
		                                xmlHttp.onreadystatechange,
		                                arrHeaders,
		                                responseBody); 
		            };
		            OperatorPromptAsyncEx(promptMessage, promptType, fnCallback);
		            break;
		        }
		        default:
		        {
                    LogErrorMessage("HttpQuery::HandleXmlHttpResponse", "Don't know how to handle response element of type '" + nodename + "'.");
		        }
		    }
		} // End content replacement.		
    }
    catch(e)
    {
        ScriptError("HttpQuery::HandleXmlHttpResponse", e);
    }
} 

/// --------------------------------------------------------------------------
/// HandleXmlHttpStateChange
/// -------------------------------------------------------------------------
/// <summary>
/// This function is called to check the status of an XMLHttpResponse and
/// call an appropriate handler function if it has succeeded or failed.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object which was used to perform the request.
/// </param>
/// <param name="fnHandleSuccess">
/// A function to call if the request has succeeded. This function will be
/// passed the xmlHttp object as its sole parameter. The HandleXmlHttpResponse
/// function above is suitable to use here, or you can pass a custom function
/// or null if nothing should happen.
/// </param>
/// </param name="fnHandleFailure">
/// A function to call if the request has failed. This function will be
/// passed the HTTP status description as its only parameter. The 
/// Utilities::OperatorMessage function is suitable to use here, or you can
/// pass a custom function or null if this case should not be handled.
/// </param>
/// --------------------------------------------------------------------------
function HandleXmlHttpStateChange(xmlHttp, fnHandleSuccess, fnHandleFailure)
{
    if (XmlHttpRequestInProgress(xmlHttp))
    {
        return;
    }
    
    var status = "";
    var statusText = "";
    try
    {
        status = xmlHttp.status;
        statusText = xmlHttp.statusText
	    if (status == HttpStatusOK || 
	        status == HttpStatusNoContent ||
	        status == HttpStatusPartialContent)
	    {
	        // The AJAX response has succeeded. Pass any content to the success handler.
	        LogMessage(L_INFO, "HttpQuery::HandleXmlHttpStateChange", "XmlHttpRequest succeeded with HTTP status code '" + status + "' and description '" + statusText + "'");
	        if (typeof(fnHandleSuccess) == JsTypeFunction)
	        {
	            fnHandleSuccess(xmlHttp);
            }
	    }
	    else if (status == HttpStatusMovedPermanently ||
                status == HttpStatusFound ||
                status == HttpStatusSeeOther ||
                status == HttpStatusTemporaryRedirect)
        { 
            // The AJAX request has issued a redirect response. The HTTP standard requires that
            // the URL of the redirect target is given in the Location Header of the response.
            // http://en.wikipedia.org/wiki/URL_redirection#HTTP_status_codes_3xx
            // We will navigate the current window to the redirection target.
 	        var redirectURL = xmlHttp.getResponseHeader("Location");
 	        LogMessage(L_INFO, "HttpQuery::HandleXmlHttpStateChange", "XmlHttpRequest redirected with HTTP status code '" + status + "', description '" + statusText + "' and location '" + redirectURL + "'");
            window.navigate(redirectURL);
        }       
	    else
	    {
	        // The AJAX request has failed. Pass the description to the failure handler.
	        // Note that we don't log an _error_ message, because this may not be an error case.
	        LogMessage(L_INFO, "HttpQuery::HandleXmlHttpStateChange", "XmlHttpRequest failed with HTTP status code '" + status + "' and description '" + statusText + "'");
	        if (typeof(fnHandleFailure) == JsTypeFunction)
	        {
	            fnHandleFailure(statusText);
	        }
	    }
    }
    catch(e)
    {
        ScriptError("HttpQuery::HandleXmlHttpStateChange", e);
    } 
}

///------------------------------------------------------------------------
/// DoXmlHttpQuery
///------------------------------------------------------------------------
/// <summary>
/// This function uses the XMLHTTP object to post a request to the 
/// specified URL and return the response as a string. This function will
/// sent a custom "IsAjax" request header which can be used by the 
/// server-side code to determine whether a request should have a rendered
/// or partial response.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object to use to perform this query.
/// </param>
/// <param name="getOK">
/// true if the request can use the GET protocol, false if it should use POST.
/// </param>
/// <param name="strURL">
/// The URL for the server-side page that will act on the query.
/// </param>
/// <param name="fnAsyncHandler">
/// The function that will be called when the readyState of the XML HTTP
/// object changes. If this is null, the query will be performed synchronously.
/// The HandleXmlHttpStateChange function is suitable to use for this purpose,
/// but a custom function may also be provided.
/// </param>
/// <param name="arrHeaders">
/// An array of 2-element arrays, with each one containing the name and
/// value of a request header you wish to send to the server.
/// </param>
/// <param name="strBody">
/// The text of the request body you wish to send to the server.
/// </param>
///------------------------------------------------------------------------
function DoXmlHttpQuery(xmlHttp, getOK, strURL, fnAsyncHandler, arrHeaders, strBody)
{
	//alert("DoXmlHttpQuery([" + typeof(xmlHttp) + "], '" + getOK + "','" + strURL + "', [" + typeof(fnAsyncHandler) + "], [" + typeof(arrHeaders) + "], '" + strBody + "')");		
	try
	{
		//..........................................................
	    // Make sure the xmlHttp object exists and is not 
	    // currently busy.
	    //..........................................................
	    if (XmlHttpRequestInProgress(xmlHttp))
	    {
	        // Cancel the pending request.
	        //alert("aborting current XMLHTTP request");
	        xmlHttp.abort();
	    }

	    //..........................................................
	    // Create the server request with the appropriate requesttype.
	    //..........................................................
	    var isAsync = (fnAsyncHandler != null && typeof(fnAsyncHandler) == JsTypeFunction);
        var requestMethod = getOK ? "GET" : "POST";
	    xmlHttp.open(requestMethod, strURL, isAsync);
        
        //..........................................................
	    // Initialize the request properties.
	    //..........................................................
        if (isAsync)
        {
            xmlHttp.onreadystatechange = fnAsyncHandler;
        }
        xmlHttp.setRequestHeader("Accept", "text/xml"); // Set the default header: may be overridden by a later one.
	    xmlHttp.setRequestHeader("IsAjax", true);       // Custom header for use by server-side response generators.
	    if (arrHeaders != null)
	    {
	        for (var i = 0; i < arrHeaders.length; i++)
	        {
	            if (arrHeaders[i].length >= 2)
	            {
				    xmlHttp.setRequestHeader(arrHeaders[i][0], arrHeaders[i][1]);
				}
			}
		}
    		
	    //..........................................................
	    // Send the request to the server.
	    //..........................................................
	    xmlHttp.send(strBody);
	    if (!isAsync)
        {
	        //alert("Synchronous HTTP Query response:\n" + xmlHttp.responseText);
	    }
	}
	catch(e)
	{
	    throw "Error performing XML HTTP Query: " + ExceptionString(e);
    }
}

///------------------------------------------------------------------------
/// DoPostHttpQuery
///------------------------------------------------------------------------
/// <summary>
/// This function posts an HTTP request to the specified URL, passing any
/// arguments to the server in the Forms collection.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object to use to perform this query.
/// </param>
/// <param name="strURL">
/// The URL for the server-side page that will act on the query.
/// </param>
/// <param name="fnAsyncHandler">
/// The function that will be called when the readyState of the XML HTTP
/// object changes. If this is null, the query will be performed synchronously.
/// </param>
/// <param name="arrArgs">
/// An array of 2-element arrays, with each one containing the name and
/// value of a form variable you wish to post to the server.
/// </param>
/// <remarks>
/// Unlike GET requests, POST requests are never cached by the browser,
/// so if you make the same request multiple times, it will hit the
/// server multiple times rather than being served up from the cache.
/// </remarks>
///------------------------------------------------------------------------
function DoPostHttpQuery(xmlHttp, strURL, fnAsyncHandler, arrArgs)
{
	//alert("DoPostHttpQuery([" + typeof(xmlHttp) + "], '" + strURL + "', [" + typeof(fnAsyncHandler) + "], [" + typeof(arrArgs) + "])");		
	
	//..........................................................
	// Create an array of headers appropriate to this query type.
	//..........................................................
	var arrHeaders = TwoDArray(1, 2, "");
	arrHeaders[0][0] = "Content-Type";
	arrHeaders[0][1] = "application/x-www-form-urlencoded";

	//..........................................................
	// Create the body for the request from the parameter array.
	//..........................................................
	var strBody = "";
	if (typeof(arrArgs) == JsTypeString)
	{
	    strBody = arrArgs;
	}
	else if (arrArgs != null)
	{
		for (var i = 0; i < arrArgs.length; i++)
		{
			if (arrArgs[i].length >= 2)
			{
				if (strBody != "")
				{
					strBody = strBody + "&";
				}
				strBody = strBody + arrArgs[i][0] + "=" + escape(arrArgs[i][1]);
			}
		}
	}

	//..........................................................
	// Perform the request.
	//..........................................................
	DoXmlHttpQuery(xmlHttp, false, strURL, fnAsyncHandler, arrHeaders, strBody)	
}

///------------------------------------------------------------------------
/// DoGetHttpQuery
///------------------------------------------------------------------------
/// <summary>
/// This function sents an HTTP GET request to the specified URL, passing 
/// any arguments to the server in the query string.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object to use to perform this query.
/// </param>
/// <param name="strURL">
/// The URL for the server-side script that will act on the query.
/// </param>
/// <param name="fnAsyncHandler">
/// The function that will be called when the readyState of the XML HTTP
/// object changes. If this is null, the query will be performed synchronously.
/// </param>
/// <remarks>
/// The browser can choose to cache and reuse the results of GET requests,
/// so these should only be used for queries which will always have the
/// same result if repeated later! This is standard behavior for GETs. 
/// </remarks>
///------------------------------------------------------------------------
function DoGetHttpQuery(xmlHttp, strURL, fnAsyncHandler)
{
	//alert("DoGetHttpQuery([" + typeof(xmlHttp) + "], '" + strURL + "', [" + typeof(fnAsyncHandler) + "])");		
	DoXmlHttpQuery(xmlHttp, true, strURL, fnAsyncHandler, null, null)	
}

	
///------------------------------------------------------------------------
/// DoSqlHttpQuery
///------------------------------------------------------------------------
/// <summary>
/// This function make a synchronous query against the HTTP data source 
/// that is exposed at the given URL. You can make your SQL Server accessible 
/// in this manner by using the wizard at 
/// Start->Program Files->SQL Server->Configure SQL XML Support in IIS.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object to use to perform this query.
/// </param>
/// <param name="strBaseURL">
/// The URL for the SQLXML web service that will act on the query.
/// </param>
/// <param name="strSQL">
/// The T-SQL you wish to execute.
/// </param>
/// <param name="fnAsyncHandler">
/// The function that will be called when the readyState of the XML HTTP
/// object changes. If this is null, the query will be performed synchronously.
/// </param>
///------------------------------------------------------------------------
function DoSQLHTTPQuery(xmlHttp, strBaseURL, strSQL, fnAsyncHandler)
{
	//alert("DoSqlHttp([" + typeof(xmlHttp) + "], '" + strBaseURL + "', '" + strSQL + "', [" + typeof(fnAsyncHandler) + "])");		
	DoXmlHttpQuery(xmlHttp, true, strBaseURL & "?sql=" & escape(strSQL), fnAsyncHandler, null, null)		
}

///------------------------------------------------------------------------
/// DoSqlHttpQueryAsXml
///------------------------------------------------------------------------
/// <summary>
/// This function does a synchronous SQLHTTP Query and returns the result 
/// as an XML Document.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object to use to perform this query.
/// </param>
/// <param name="strBaseURL">
/// The URL for the SQLXML web service that will act on the query.
/// </param>
/// <param name="strSQL">
/// The T-SQL you wish to execute.
/// </param>
/// <returns>
/// The response to your query in an XML document.
/// </returns>
///------------------------------------------------------------------------
function DoSqlHttpQueryAsXml(xmlHttp, strBaseURL, strSQL)
{
	//alert("DoSqlHttpQueryAsXml([" + typeof(xmlHttp) + "], '" + strBaseURL + "', '" + strSQL + "')");		
			
	//..........................................................
	// Make sure the query returns XML
	//..........................................................
	var strSQLXML = strSQL
	if (strSQL.toUpperCase().indexOf("FOR XML") < 0)
	{
		strSQLXML = strSQL + " FOR XML AUTO&root=ROOT";
	}
	
	//..........................................................
	// Perform the query, returning the result in an XML parser.
	//..........................................................
	DoSqlHttpQuery(xmlHttp, strBaseURL, strSQLXML, null);
	return xmlHttp.responseXML;
}

///------------------------------------------------------------------------
/// DoWebServiceQuery
///------------------------------------------------------------------------
/// <summary>
/// This function makes a synchronous call on the web service at the given 
/// URL, using the SOAP protocol and returns the result as a string.
/// </summary>
/// <param name="xmlHttp">
/// The XMLHTTP object to use to perform this query. If this parameter is 
/// null, then a new object will be created just for this query.
/// </param>
/// <param name="strURL">
/// The URL for the web service that will act on the query.
/// </param>
/// <param name="strFunctionNamespace">
/// The namespace that the webservice is using.
/// </param>
/// <param name="strFunctionName">
/// The name of the function you wish to call.
/// </param>
/// <param name="arrFunctionParameters">
/// An array of 2-element arrays, with each one containing the name and value
/// of a parameter you wish to pass to the function call. The values may be
/// arrays of primitive types. May be null if no parameters are needed.
/// </param>
/// <param name="strAuthNamespace">
/// The namespace to use for the authorization headers in the request.
/// May be an empty string.
/// </param>
/// <param name="arrAuthParameters">
/// An array of 2-element arrays that contain the names and values of
/// the authorization headers you wish to send. May be null.
/// </param>
/// <returns>
/// The XML Node containing the WebService's response to the call.
/// </returns>
///------------------------------------------------------------------------
function DoWebServiceQuery(xmlHttp, strURL, strFunctionNamespace, strFunctionName, arrFunctionParameters, strAuthNamespace, arrAuthParameters)
{	
	//alert("DoWebServiceQuery([" + typeof(xmlHttp) + "], '" + strURL + "', '" + 
    //    strFunctionNamespace + "', '" + strFunctionName + "', [" + typeof(arrFunctionParameters) + "], '" +
	//    strAuthNamespace + "', [" + typeof(arrAuthParameters) + "])");
	try
	{
	    //..........................................................
	    // Construct the XML for the method and authentication parameters.
	    //..........................................................
	    var soapParameters = ParameterXml(arrFunctionParameters);
	    var authParameters = ParameterXml(arrAuthParameters);
    	
	    //..........................................................
	    // Construct the SOAP Envelope
	    //..........................................................
	    var soapEnvelope = "<?xml version='1.0' encoding='utf-8'?>" + 
	    "<soap:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap='http://schemas.xmlsoap.org/soap/envelope/'>" + 
	    "<soap:Header><AuthHeader xmlns='" + strAuthNamespace + "'>" + authParameters + "</AuthHeader></soap:Header>" +
	    "<soap:Body><" + strFunctionName + " xmlns='" + strFunctionNamespace + "'>" + soapParameters + "</" + strFunctionName + "></soap:Body>" +
	    "</soap:Envelope>";
    			
	    //alert("SOAPEnvelope:\n" + soapEnvelope);
    	
	    //..........................................................
	    // Specify the request headers
	    //..........................................................
	    var arrHeaders = TwoDArray(2, 2, "");
	    arrHeaders[0][0] = "Content-Type";
	    arrHeaders[0][1] = "text/xml";
	    arrHeaders[1][0] = "SOAPAction";
	    arrHeaders[1][1] = CombinePath(strFunctionNamespace, strFunctionName);
    	
	    //..........................................................
	    // Perform the synchronous query.
	    //..........................................................	
	    var queryXmlHttp = (xmlHttp == null ? GetXmlHttpObject() : xmlHttp);
	    DoXmlHttpQuery(queryXmlHttp , false, strURL, null, arrHeaders, soapEnvelope);
	
	    //..........................................................
	    // Extract the soap Response.
	    //..........................................................
	    var resultNode = queryXmlHttp.responseXML.selectSingleNode("//" + strFunctionName + "Result");
	    if (resultNode == null)
	    {
		    var xmlSoapError = queryXmlHttp.responseXML.selectSingleNode("//soap:Fault/faultstring");
		    if (xmlSoapError == null)
		    {
			    throw "SOAP result node not found, and no faultstrng data is available.";
		    }
		    else
		    {
			     throw "SOAP Error in Web Service Query: " + xmlSoapError.text;
		    }
	    }
	    else
	    {
	        //alert("SOAP Response: " + resultNode.xml);
	    }
	    queryXmlHttp = null;
	    soapParameters = null;
	    authParameters = null;
	    return resultNode;
	}
	catch(e)
	{
	    ScriptError("HttpQuery::DoWebServiceQuery", e);
	    throw e;
	}
}