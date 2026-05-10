// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// opapps_chart.js

var hscOpAppsISO = d3.time.format.utc("%Y-%m-%dT%H:%M:%SZ");
var hscOpAppsNice = d3.time.format("%d-%b-%y %H:%M:%S");

hscOpAppsChart = function () {
	var src = "opapps_chart.js";

	var _titleId = "#title";
	var _title;
	var _opAppsChart;
	var _silverlightId;
	var _requesting = false;
	var _initialRequestNeeded = true;
	var _timeControl;
	var _inError = false;
	var _requestTime;				// The time at which the current outstanding request was made
	var _initialRequestTime;		// The time at which the current outstanding "initial" request was made
	var _silverlightLoadedTime;		// The time at which the silverlight control finished loading
	var _firstStandardRequest;		// True if the current outstanding request is the first one after an "initial" request
	var _firstCallup = true;		// True if this is the first callup of the display
	var _readyTime;					// The time at which the hosting display was made ready

	function callupLX() {
		return "Call Up Diagnostics OpApps: L1: ";
	}

	function OnReady(timeControl, readyTime) {
		_firstCallup = true;
		try {
			if (!(typeof timeControl === 'object'))
				throw "timeControl passed in is not an object.";
		}
		catch (err) {
			var errorString = "Unable to make generic chart ready.<BR>" + err;
			LogErrorMessage(src, errorString.replace("<BR>", " "));
			throw errorString;
		}

		_timeControl = timeControl;
		_readyTime = readyTime;
	}

	function ChangeChart(theOpAppsChart) {
		try {
			if (theOpAppsChart == null)
				throw "theOpAppsChart is null";

			if ((_title = $(_titleId)).length == 0)
				throw "Unable to find element with id of " + _titleId + " on the page.";
		}
		catch (err) {
			LogErrorMessage(src, "Error associated with theOpAppsChart - " + err);
			return false;
		}

		_requesting = false;
		_opAppsChart = theOpAppsChart;

		// If the chart has an FormInitialRequest() then made an initial request first
		_initialRequestNeeded = (typeof _opAppsChart.FormInitialRequest === 'function');

		_opAppsChart.AdjustTimeControl();
		_title.text(_opAppsChart.Title());

		return true;
	}

	function RequestDataFromSilverlight(request) {	// Private function
		if (_silverlightId == null)
			return false;

		try {
			LogMessage(L_PARAM, src, "Making request of silverlight object '" + _silverlightId + "'");
			var sl = document.getElementById(_silverlightId);
			sl.Content.DataPage.GetOpAppsData(request);
		} catch (err) {
			alert("Error calling silverlight object: " + err.message);
			return false;
		}

		return true;
	}

	function OnSLPluginLoaded(objectId, sender, args) {
		_silverlightId = objectId;
		_silverlightLoadedTime = new Date();
		LogMessage(L_CALLUP_HI, src, callupLX() + _silverlightId + " load time: " + (_silverlightLoadedTime.getTime() - _readyTime.getTime()) + "ms after doc ready");

		var sl = document.getElementById(_silverlightId);
		sl.Content.DataPage.OpAppsDataEvent = OnResponse;

		if (!_opAppsChart) {
			LogErrorMessage(src, "No _opAppsChart present in OnSLPluginLoaded().");
			return false;
		}

		RequestData();
	}

	function RequestData() {
		if (!_opAppsChart) {
			// This isn't such a big deal because the time control may start early.
			LogMessage(L_PRINT, src, "No _opAppsChart present in RequestData().");
			return false;
		}

		if (_requesting) {
			LogMessage(L_PARAM, src, "Not requesting again as a response is already pending");
			return false;
		}

		var request;

		if (_initialRequestNeeded)
			request = _opAppsChart.FormInitialRequest();
		else
			request = _opAppsChart.FormRequest();

		LogMessage(L_PARAM, src, "Requesting data with '" + request + "'");

		_requestTime = new Date();
		if (_initialRequestNeeded)
			_initialRequestTime = new Date();

		if (RequestDataFromSilverlight(request)) {
			_requesting = true;
			$("#loading").show();
			if (_inError) {
				// Don't leave previous error string on screen when attempting another refresh
				$("#graphdivfull").html("");
			}
		}
	}

	function OnResponse(sender, result) {
		if (!_opAppsChart) {
			LogErrorMessage(src, "No _opAppsChart present in OnResponse().");
			_firstCallup = false;
			return false;
		}

		try {
			var endRequest = new Date();
			LogMessage(L_CALLUP_HI, src, callupLX() + (_initialRequestNeeded ? "Initial response time: " : "Response time: ") + (endRequest.getTime() - _requestTime.getTime()) + "ms");

			_requesting = false;

			$("#loading").hide();

			var parseStartTime = new Date();
			try {
				var response = JSON.parse(result.response);
			}
			catch (err) {
				var responseString = result.response.toString();
				LogErrorMessage(src, "Problem with " + responseString.length + " character Response string:");

				var i = 0;
				var perLine = 1000;
				for (i = 0; i * perLine < responseString.length; i++)
					LogErrorMessage(src, responseString.substr(i * perLine, Math.min(perLine, responseString.length - i * perLine)));
				throw "JSON.parse threw '" + err + "' parsing Response string.";
			}

			if (response == null)
				throw "Unable to parse JSON response";

			var goodResponse = {
				"ResponseType": "",
				"ResponseData": "empty",
				"Message": "empty"
			}

			try {
				hscCheckRequiredFields(goodResponse, response);
			}
			catch (err) {
				LogErrorMessage(src, "response = " + JSON.stringify(response));
				throw err + " in Response element";
			}

			if (result.hresult != 0) {
				if (response.Message != "")
					throw "Server returned an error response: " + response.Message;
				else
					throw "Error requesting data from the server. Check client log for details..";
			}
			else if (response.ResponseData == "") 
				throw "Server returned successfully but with no response data.";

			try {
				var responseDataString = response.ResponseData.toString();
				var initialString = response.ResponseData.toString();
				var initialResponseDataLength = responseDataString.length;

				while (responseDataString.search(":INF") != -1)
					responseDataString = responseDataString.replace(":INF", ":\"INF\"");

				while (responseDataString.search(":-INF") != -1)
					responseDataString = responseDataString.replace(":-INF", ":\"-INF\"");

				while (responseDataString.search(":NaN") != -1)
					responseDataString = responseDataString.replace(":NaN", ":\"NaN\"");

				if (initialResponseDataLength != responseDataString.length) {
					LogErrorMessage(src, "ResponseData string contained invalid INF,-INF or NaN strings. They have been replaced with valid INF, -INF or NaN strings.");
				}

				JSON.parse(responseDataString);

				response.ResponseData = responseDataString;	// Stick it back in now that it has had it's INF's removed
			}
			catch (err) {
				var responseDataString = response.ResponseData.toString();
				LogErrorMessage(src, "Problem with " + responseDataString.length + " character ResponseData string:");

				var i = 0;
				var perLine = 1000;
				for (i = 0; i * perLine < responseDataString.length; i++)
					LogErrorMessage(src, responseDataString.substr(i * perLine, Math.min(perLine, responseDataString.length - i * perLine)));
				throw "JSON.parse threw '" + err + "' parsing ResponseData string.";
			}

			var parseEndTime = new Date();
			LogMessage(L_CALLUP_HI, src, callupLX() + (_initialRequestNeeded ? "Initial response parse time: " : "Response parse time: ") + (parseEndTime.getTime() - parseStartTime.getTime()) + "ms");

			if (response.ResponseType.indexOf(_opAppsChart.Type()) != 0) {
				// Don't make a big fuss about this as it is probably going to occur to do internal navigation
				// Only match on the start of the Chart's type.  e.g. this allows CompressorMapHistory and CompressorMapDefinition responses to both go to CompressorMap.
				_firstCallup = false;
				return false;
			}

			if (!_opAppsChart.ParseResponse(response))
				throw "Error parsing response string.";
			   

			// Update the title again in case it relies on data in the response
			_title.text(_opAppsChart.Title());

			_inError = false;
		}
		catch (err) {
			_inError = true;

			_initialRequestNeeded = (typeof _opAppsChart.FormInitialRequest === 'function');	// Re-do initial request if one is required

			LogErrorMessage(src, "OnResponse parsing: " + err);
			_opAppsChart.ClearChart();
			var errorString = "<BR>Check client and server logs for details.";

			if (_timeControl.playingContinously() == false) {
			    try {
			        if (_timeControl && _timeControl.playing) {
			            _timeControl.Pause();
			            errorString += "<BR>The time control has been paused.";
			        }
			    }
			    catch (ignoredErr) {
			        // Ignore any errors that occur when trying to stop a playing time control
			    }
			}
						
			$("#graphdivfull").html(err + errorString);
			_firstCallup = false;
			return false;
		}

		if (_initialRequestNeeded) {
			// Got this far with the initial request, now make the next request
			_initialRequestNeeded = false;
			_firstStandardRequest = true;
			RequestData();
		}
		else {
			$("#graphdivfull").html("");

			var startDrawTime = new Date();
			LogMessage(L_PARAM, src, "Drawing chart");
			$("#loading").show();
			try {
				_opAppsChart.DrawChart();
				var endDrawTime = new Date();
				LogMessage(L_CALLUP_HI, src, callupLX() + "Chart drawn time: " + (endDrawTime.getTime() - startDrawTime.getTime()) + "ms");
			}
			catch (err) {
				LogErrorMessage(src, "OnResponse drawing: " + err);
				$("#graphdivfull").html(err + "<BR>Check client log for details.");
			}

			var endEverythingTime = new Date();
			if (_firstCallup) {
				LogMessage(L_CALLUP_HI, src, callupLX() + "Total callup time after SL loaded: " + (endEverythingTime.getTime() - _silverlightLoadedTime.getTime()) + "ms after silverlight loaded");
				LogMessage(L_CALLUP_HI, src, callupLX() + "Total callup time                : " + (endEverythingTime.getTime() - _readyTime.getTime()) + "ms after doc ready");
				_firstStandardRequest = false;
			}
			else {
				if (_firstStandardRequest) {
					LogMessage(L_CALLUP_HI, src, callupLX() + "Total refresh time: " + (endEverythingTime.getTime() - _initialRequestTime.getTime()) + "ms");
					_firstStandardRequest = false;
				}
				else
					LogMessage(L_CALLUP_HI, src, callupLX() + "Total refresh time: " + (endEverythingTime.getTime() - _requestTime.getTime()) + "ms");
			}

			$("#loading").hide();
			_firstCallup = false;
		}
	}

	return {
		// Public methods
		OnReady: function (timeControl, readyTime) {
			return OnReady(timeControl, readyTime);
		},
		ChangeChart: function (theOpAppsChart) {
			return ChangeChart(theOpAppsChart);
		},
		OnSilverlightPluginLoaded: function (sender, args) {
			OnSLPluginLoaded(sender, args)
		},
		RequestData: function () {
			return RequestData();
		}
	}
}();

function hscCheckRequiredFields(goodObj, testObj) {
	for (var a in goodObj) {
		if (!testObj.hasOwnProperty(a))
			throw "Property '" + a + "' is missing";
		if (!(typeof goodObj[a] === 'number' && typeof testObj[a] === 'string' &&
			(testObj[a] === "INF" || testObj[a] === "-INF" || testObj[a] === "NaN")))	// Allow for INF, -INF and NaN instead of numbers
			if (typeof goodObj[a] != typeof testObj[a])
				throw "Property '" + a + "' is of type '" + typeof testObj[a] + "' and not '" + typeof goodObj[a] + "' as expected";
		if (typeof goodObj[a] === 'string' && testObj[a] == '' && goodObj[a] != 'empty')	// Allow empty string only if good string is 'empty'
			throw "Property '" + a + "' is an empty string";
		if (typeof goodObj[a] === 'object' && testObj[a] == null)
			throw "Property '" + a + "' is null";
		if (typeof goodObj[a] === 'string' && goodObj[a] === 'iso' && (hscOpAppsISO.parse(testObj[a]) == null))
			throw "Property '" + a + "' value of '" + testObj[a] + "' is not a valid UTC time";
	}
}


// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/