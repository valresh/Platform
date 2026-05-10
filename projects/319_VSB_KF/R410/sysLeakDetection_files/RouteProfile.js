// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// RouteProfile.js


hscOpAppsRouteProfile = function () {


	var src = "RouteProfile.js";		//For logging
	var RequestType = _DISPLAYTYPE_RouteProfile;		//What we are requesting
	
	var responseData;
	var arrayDataTotal = [];
	var breaks;
	var nodesInRoutes = [];
	var leakLocations = [];
	var defaultTemperatureValue = 0;
	var _firstSampleRequest = true;
	var tickLabels = [];
	var keysOfNonEmptyPrimaryData = [];
	var dataToggle = [];
	var pressureProfile = [];
	dataToggle.push(false, false, false, false)		//dataToggle determines if we toggle data on or off 

	var plotData = InitialisePlotData(dataToggle);
	var legend = {
		"x": 300,
		"y": 200,
		"xPrevious": 0,
		"yPrevious": 0
	};
	var errPressure = {
		"CurrentError": false,
		"BaselineError": false,
		"OpeatingRangeError": false
	};
	var errTemperature = JSON.parse(JSON.stringify(errPressure));

	var dataErrors = {
		"Pressure": errPressure,
		"Temperature": errTemperature
	};
	
	var chartDivId;
	
	//Function references
	var Chart;
	var WidthFn, HeightFn;
	var TimeControl;

	function OnReady(chartDiv, widthFn, heightFn, timeControl) {
		try {
			if ($(chartDiv).length === 0)
				throw "Unable to find element with id of " + chartDiv + " on the page.";
			if (!(typeof widthFn === 'function'))
				throw "widthFn passed in is not a function.";
			if (!(typeof heightFn === 'function'))
				throw "heightFn passed in is not a function.";
			if (!(typeof timeControl === 'object'))
				throw "timeControl passed in is not an object.";
		}
		catch (err) 
		{
			var errorString = "Unable to make " + RequestType + " chart type ready.<BR>" + err;
			LogErrorMessage(src, errorString.replace("<BR>", " "));
			throw errorString;
		}

		chartDivId = chartDiv;
		WidthFn = widthFn;
		HeightFn = heightFn;
		TimeControl = timeControl;

		$(window).resize(function () {
			$("svg").empty();		//Clears the svg to prevent dragging
			if (Chart == null)
				return;

			var margin = Chart.margin();

			Chart
				.width(WidthFn(margin) -100)
				.height(HeightFn(margin));

			d3.select(chartDivId + ' svg')
				.attr('width', WidthFn(margin))
				.attr('height', HeightFn(margin))
				.call(Chart);
		});
	}

	function FormRequest() 
	{
		var requestData = 
		{
			"RoutePoint": hscGlobalRoutePoint,
			"Time": hscOpAppsISO(TimeControl.endTime()),
			"BaselineTime": hscOpAppsISO(TimeControl.baselineTime()),
			"IsLive": TimeControl.playing()
		}
		
		var requestType = RequestType;
		if (hscGlobalRoutePoint === hscDummyPointName) 
		{
			LogMessage(L_PARAM, src, "Sample Route Point not supported");
		}

		var request = {
			"RequestType": requestType,
			"RequestData": JSON.stringify(requestData)
		};

		return JSON.stringify(request);
	}

	function ParseResponse(response) 
	{
		LogMessage(L_PARAM, src, "In parse response function");

		try {
			responseData = JSON.parse(response.ResponseData);
			LogMessage(L_PARAM, src, "Parse Response: " + JSON.stringify(responseData));
			
			// Check that the ResponseData has the required fields
			var goodResponseData =
			{
				"NodePressureProfile": {},
				"NodeTemperatureProfile": {},
				"SegmentPressureProfile": {},
				"SegmentTemperatureProfile": {},
				"LeakAdjustedPressure":{},
				"LeakLocations":{},
				"LeakRate":0,
				"PositionUnits" : "empty",
				"PressureUnits" : "empty",
				"TemperatureUnits" : "empty",
				"LeakRateUnits" : "empty"
			}
			try 
			{
				hscCheckRequiredFields(goodResponseData, responseData);
			}
			catch (err)
			{
				throw err + " in ResponseData element";
			}
			
			// Given the pressure readings at the nodes, interpolate the profile readings
			LogMessage(L_PARAM, src, "Passed the check for required fields"); 
			
			// Check size and quality of response data values - NOT EMPTY
			if( responseData.SegmentTemperatureProfile.length <=0 || responseData.SegmentPressureProfile.length <= 0 ||
					responseData.NodeTemperatureProfile.length <=0 || responseData.NodePressureProfile.length <=0 )
				throw "Response contains empty segment and node profile values";
				
			// Check size and quality of response data values - EQUAL SEGMENT PROFILE LENGTHS
			if( responseData.SegmentTemperatureProfile.length != responseData.SegmentPressureProfile.length )
				throw "Response contains unequal segment profile values";
			
			// Check size and quality of response data values - EQUAL NODE PROFILE LENGTHS
			if( responseData.NodeTemperatureProfile.length != responseData.NodePressureProfile.length )
			    throw "Response contains unequal node profile values";

			// Set up data models
			var arrayDataInitial  = [];
			var totalPreviousRoutePositions = 0;

			var goodNodeTemperature = {
				"Position": 0,
				"StationShortName": "",
				"InletTemperature": 0,
				"OutletTemperature": 0,
				"StationName": "",
				"Time": "iso"
			}
			
			var goodNodePressure = {
				"Position": 0,
				"InletPressure": 0,
				"OutletPressure": 0,
				"MinOpPressure": 0,
				"MaxOpPressure": 0,
				"BaselineInletPressure": 0,
				"BaselineOutletPressure": 0,
				"StationShortName": "",
				"StationName": "",
				"TargetPressure": 0,
				"Time": "iso",
				"BaselineTime": "iso",
				"CorrectedInletPressure":0,
				"CorrectedOutletPressure":0
			}

			var goodLeakPressure = {
			    "Position": 0,
                "Value": 0
			}
			
			// Inhertied from PipelineProfile
			// used to determine which additional JSON elements to read
			var pressureHasTypesOfData = {
				"MinMaxData": true,					// MinOpPressure, MaxOpPressure
				"BaselineData": true,				// BaselineInletPressure, BaselineOutletPressure, BaselineTime
				"TargetData": true,					// TargetPressure
				"PushErrorAndPrevious": true,		// ?
				"ElevationCorrectedData": true		// CorrectedInletPressure, CorrectedOutletPressure
			};
			var temperatureHasTypesOfData = {
				"MinMaxData": false,                
				"BaselineData": false,
				"TargetData": false,
				"PushErrorAndPrevious": false,
				"ElevationCorrectedData": false
			};
            
			var maxOperatingRange = {
				"Pressure": -Infinity,
				"Temperature": -Infinity
			};	// Determine the global maximum operating range
						
			var nodesAndBreaks= {};
			var dataPressureNode = [];
			var dataPressureSegment = [];
			var dataPressureLeaks = [];
			var validKeys = [];
				
		    var nodesAndBreaksPressure = ExtractPressureData(goodNodePressure, goodLeakPressure, dataPressureNode, dataPressureSegment, dataPressureLeaks, 
                responseData, pressureHasTypesOfData, maxOperatingRange, dataErrors);

		    LogMessage(L_PARAM, src, "Route Profile leak positions: " + JSON.stringify(dataPressureLeaks));
					
			if(nodesAndBreaksPressure!= null)
			{
				var strPressure = "Pressure";
				nodesAndBreaks[strPressure] = nodesAndBreaksPressure;
				validKeys.push(strPressure);
			}
			
			var dataTemperatureNode = [];
			var dataTemperatureSegment = [];
			LogMessage(L_PARAM, src, "Extracting Temperature Data from Server");
			var nodesAndBreaksTemperature = ExtractTemperatureData(goodNodeTemperature, dataTemperatureNode, dataTemperatureSegment,
                responseData, temperatureHasTypesOfData, maxOperatingRange, dataErrors);

			
			/*nodesAndBreaks has elements added to it if data is extracted just fine.
			  If there are issues extracting data (null values and errors due to not having length property) then we do not
			  have this element added to nodesAndBreaks*/
			if(nodesAndBreaksTemperature!= null)
			{
				var strTemperature = "Temperature";
				nodesAndBreaks[strTemperature] = nodesAndBreaksTemperature;
				validKeys.push(strTemperature);
			}
            
			/* CompareReturnedServerData is invoked when data is obtained successfully and now we wish to compare the data
				for discrepancies such as one of them being empty or mismatch of lengths of data etc*/
			var nodePlotBreakData = CompareReturnedServerData(nodesAndBreaks, validKeys, plotData);
			
			if(nodePlotBreakData !=null)
			{
				breaks = nodePlotBreakData.breaks;
				nodesInRoutes = nodePlotBreakData.nodesInRoute;
				tickLabels = nodePlotBreakData.tickLabels;
				keysOfNonEmptyPrimaryData =nodePlotBreakData.keysNonEmpty;
			}
			else
			{
				throw "Problem with CompareReturnedServerData function"
			}
			
			//Add whether the plot is empty to plotData
			ToggleEmptynessProperty(plotData, keysOfNonEmptyPrimaryData);
			HideLeakElements(plotData, responseData.LeakLocations);
			
			LogMessage(L_PARAM, src, "Pushing all data values to arrays");
			var dataPresNode = [];
			var dataPresSeg = [];
			var dataTempNode = [];
			var dataTempSeg = [];
			var dataPresLeak = [];
            
			dataPresNode.push(dataPressureNode);
			dataPresSeg.push(dataPressureSegment);
			dataTempNode.push(dataTemperatureNode);
			dataTempSeg.push(dataTemperatureSegment);
			dataPresLeak.push(dataPressureLeaks);

			
			arrayDataInitial.push(dataPresNode);        
			arrayDataInitial.push(dataTempNode);
			arrayDataInitial.push(dataPresSeg);	
			arrayDataInitial.push(dataTempSeg);
			arrayDataInitial.push(dataPresLeak);
			
			arrayDataTotal = SetLimitsOfOperatingRange(arrayDataInitial, plotData, dataErrors, maxOperatingRange);
			
			return true;
		}
		catch (err) {
			LogErrorMessage(src, "Error interpreting response - " + err);
			responseData = null;
			return false;
		}
	}
	
	function DrawChart() 
	{
		LogMessage(L_PARAM, src, "Entering DrawChart function");
		if (responseData == null) 
		{
			LogErrorMessage(src, "No response data available for DrawChart");
			return false;
		}

		var yAxisUnits = {
			"Pressure" : responseData.PressureUnits,
			"Temperature" : responseData.TemperatureUnits,
			"Position" : responseData.PositionUnits,
			"LeakRate": responseData.LeakRateUnits
		};
		var margin = { top:45, right: 100, bottom: 50, left: 80 }
		var pressureString = "Pressure";
		var pressureTitle = (responseData.PressureUnits == "") ? pressureString : pressureString + " (" + responseData.PressureUnits + ")";
		var temperatureString = "Temperature";
		var temperatureTitle = (responseData.TemperatureUnits == "") ? temperatureString : temperatureString + " (" + responseData.TemperatureUnits + ")";
		var positionString = "Node";
		var positionTitle = (responseData.PositionUnits == "") ? positionString : positionString + " (" + responseData.PositionUnits + ")";
		var yAxisTitlesArray = {
			"Pressure": pressureTitle,
			"Temperature": temperatureTitle
		};
		var _yValuesPerXPressure = 4;
		var _yValuesPerXTemperature = 1;
		var leakRate = responseData.LeakRate;

		var newchart = multiLineChartLeaks(dataToggle)
			.xPlotPosition(function (d) { return d.PlotPosition; })
			.xPosition(function (d) { return d.Position; })
			.yPressure(function (d) { return d.Pressure; })
			.yMinOpPressure(function (d) { return +d.MinOpPressure; })
			.yMaxOpPressure(function (d) { return +d.MaxOpPressure; })
			.yBaselinePressure(function (d) { return +d.BaselinePressure; })
			.ySegmentsInNode(function(d) {return +d.SegmentCounts;})
			.yTemperature(function (d) { return +d.Temperature; })
			.yLeakAdjustedPressure(function (d) { return +d.LeakAdjusted; })
			.yLeakPressure(function (d) { return +d.Leak; })
			.yCorrectedPressure(function (d) { return +d.CorrectedPressure; })
			.width(WidthFn(margin))
			.height(HeightFn(margin))
			.xAxisTitle(positionTitle)
			.yAxisTitles(yAxisTitlesArray)
			.xAxisTickLabels(tickLabels)
			.yValuesPerXTemperature(_yValuesPerXTemperature)
			.yValuesPerXPressure(_yValuesPerXPressure)
			.yAxisUnits(yAxisUnits)
			.leakRate(leakRate)			
			.margin(margin)
			.breaks(breaks)
			.nodesData(nodesInRoutes)
			.plotData(plotData)
			.legendCoord(legend)
			.validPrimaryKeys(keysOfNonEmptyPrimaryData);

		d3.select(chartDivId)
			.datum(arrayDataTotal)
			.call(newchart);

		Chart = newchart;
		
		return true;
	}

	function ClearChart() {
		if (Chart != null) {
			Chart.ClearChart();
			Chart = null;
		}
		hscOpAppsTooltip.cleanup();
	}

	function AdjustTimeControl() {
		TimeControl.ShowBaseline();
		TimeControl.HidePeriodInterval();
	}

	return {
		// Public methods
		OnReady: function (chartDiv, widthFn, heightFn, timeControl) {
			return OnReady(chartDiv, widthFn, heightFn, timeControl);
		},
		FormRequest: function () {
			return FormRequest();
		},
		ParseResponse: function (responseData) {
			return ParseResponse(responseData);
		},
		DrawChart: function () {
			return DrawChart();
		},
		ClearChart: function () {
			return ClearChart();
		},
		AdjustTimeControl: function () {
			return AdjustTimeControl();
		},
		Title: function () {
			return "Route Profile";
		},
		Type: function () {
			return RequestType;
		}
	}
}();


function HideLeakElements(plotData, leakLocations)
{
    if (leakLocations.length < 1)
    {
        // If there is no leaks found, do not render leak notification, leak adjusted pressure or the legend entry
        var element = plotData.Keys.indexOf("LeakAdjusted");
        if(element != -1)
            plotData.Keys.splice(element, 1);
    }
}

function CompareReturnedServerData(nodesAndBreaks, validKeys, plotData)
{
	var strFn = "CompareReturnedServerData";
	LogMessage(L_PARAM, src, "Entered function: "+ strFn);
	var hasError = false;
	var keys = plotData.Keys;
	var key;
	var errorInfo = [];
	//Extracts whether the data is empty or not
	for(var i=0; i< keys.length; i++)
	{
		key = keys[i];
		if(plotData[key].isPrimaryPlot)
		{
			//Check against validKeys!
			if(validKeys.indexOf(key)>= 0)
			{
				var nodesAndBreaksValue = nodesAndBreaks[key];
				var isEmpty;
				if(nodesAndBreaksValue.nodesInRoutes.length == null || nodesAndBreaksValue.breaks.length == null 
				|| nodesAndBreaksValue.tickLabels.length == null)
				{
					isEmpty = true;
					LogMessage(L_PARAM, src, key +" data is empty. Function: "+ functionName);
				}
				else
				{
					isEmpty = false;
				}
			}
			else
			{
				//Not a valid key
				isEmpty = true;
			}
			
			errorInfo.push(
				{
					"Key": key,
					"isEmpty": isEmpty
				}
			);
		}
	}

	var keysOfNonEmptyData = [];
	for(var i=0; i< errorInfo.length; i++)
	{
		if(errorInfo[i].isEmpty == false)		//So it's NOT empty
		{
			keysOfNonEmptyData.push(errorInfo[i].Key);
		}
	}
	
	if(keysOfNonEmptyData.length<=0)
	{
		LogErrorMessage(src, "No valid data to display");
		return null;
	}
	
	//Check if there is a mismatch of data!
	var isBreaksEqual;
	var isNodesInRouteEqual;
	var isTickLabelsEqual;
	var prevBreaks, currentBreaks;
	var prevNodesInRoutes, currentNodesInRoutes;
	var prevTickLabels, currentTickLabels;
	for(var i=0; i< keysOfNonEmptyData.length; i++)
	{
		key = keysOfNonEmptyData[i];
		//First iteration get the values
		currentBreaks = nodesAndBreaks[key].breaks;
		currentNodesInRoutes = nodesAndBreaks[key].nodesInRoutes;
		currentTickLabels = nodesAndBreaks[key].tickLabels;
		if(i!=0)	
		{
			//Not the first iterate
			if( (currentBreaks.length != prevBreaks.length) ||
				(currentNodesInRoutes.length != prevNodesInRoutes.length) ||
				(currentTickLabels.length != prevTickLabels.length))
			{
				LogErrorMessage(src, "Mismatch of node/break lengths");
				return null;
			}
			
			//if lengths are equal
			for(var i=0; i<currentBreaks.length; i++)
			{
				if(currentBreaks[i] != prevBreaks[i])
				{
					LogErrorMessage(src, "Mismatch of breaks data");
					return null;
				}
			}
			for(var i=0; i<currentNodesInRoutes.length; i++)
			{
				if(currentNodesInRoutes[i] != prevNodesInRoutes[i])
				{
					LogErrorMessage(src, "Mismatch of nodesInRoutes data");
					return null;
				}
			}
			for(var i=0; i<currentTickLabels.length; i++)
			{
				if(currentTickLabels[i] != prevTickLabels[i])
				{
					LogErrorMessage(src, "Mismatch of tickLabels data");
					return null;
				}
			}
		}
		prevBreaks = currentBreaks
		prevNodesInRoutes = currentNodesInRoutes;
		prevTickLabels = currentTickLabels;
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"breaks": currentBreaks,
		"nodesInRoute": currentNodesInRoutes,
		"tickLabels": currentTickLabels,
		"keysNonEmpty": keysOfNonEmptyData
	};
}


function ExtractPressureData(nodePressureModel, leakPressureModel, nodePressureValues, segmentPressureValues, leakPressureValues, responseData,
    typesIncluded, maxOperatingRange, dataErrors)
{
    var strFn = "ExtractingPressureData";
    LogMessage(L_PARAM, src, "Entering function: " + strFn);

    // required by multi_band_multi_charts.js
    var totalPreviousRoutePositions = 0;
    var totalPreviousRouteNodes = 0;
    var routenum = 0;
    var nodesInRoutes = [];
    var breaks = []; 
    var tickLabels = [];

    // There is only one route
    var maxRoutePosition = 0;
    nodesInRoutes.push(responseData.NodePressureProfile.length);    // how many nodes in the route?

    var prevNode = null;
    var prevOutlet = null;

    var validData = ExtractNodeAndSegmentData(nodePressureModel, nodePressureValues, segmentPressureValues,
        totalPreviousRoutePositions, maxRoutePosition, routenum, responseData.NodePressureProfile, responseData.SegmentPressureProfile,
        "Pressure", typesIncluded, maxOperatingRange, tickLabels, prevNode, prevOutlet, dataErrors);

    if (validData == null)
        return null;



    // Leak Adjusted pressures, will only be rendered if there are leaks.
    if (responseData.LeakLocations.length > 0) {
 
        // how many segment readings do we need?
        var segmentMax = segmentPressureValues.length;
        var currentReading = 0;
        var defaultValue = 0;
        var value = {};
        
        // interpolate leak-adjusted pressure readings
        if (responseData.LeakAdjustedPressure.length > 1) {

            for (var pressureKink = 0; pressureKink < responseData.LeakAdjustedPressure.length; pressureKink++) {
                var plotdata1 = responseData.LeakAdjustedPressure[pressureKink];

                // check the model of all leak-adjusted plots
                try {
                    hscCheckRequiredFields(leakPressureModel, plotdata1);
                }
                catch (err) {
                    throw err + " in Response Leak-Adjusted element " + pressureKink;
                }

                // The leak-adjusted pressure kinks(?) do not necessarily match up against a node. 
                // Each leak-adjusted pressure values must be calculated and added manually to the Segment and Node Profiles
                if (pressureKink > 0) {

                    var plotdata0 = responseData.LeakAdjustedPressure[pressureKink - 1];                  
                    // Add Leak Adjusted pressure to Segment Profile                   
                    while (currentReading < segmentPressureValues.length) {
                        var segmentProfile = segmentPressureValues[currentReading];
                        if (plotdata1.Position >= segmentProfile.Position) {
                            var leakValue = interpolateSingleReading(plotdata0.Position, plotdata1.Position, plotdata0.Value, plotdata1.Value, segmentProfile.Position);

                            segmentPressureValues[currentReading]["LeakAdjusted"] = leakValue;
                        }
                        else {
                            break;
                        }
                        currentReading++;
                    }

                    // Get reading at 
                    for (var j = 0; j < responseData.LeakLocations.length; j++) {                        
                        if (responseData.LeakLocations[j] >= plotdata0.Position && responseData.LeakLocations[j] <= plotdata1.Position) {
                            var leak = interpolateSingleReading(plotdata0.Position, plotdata1.Position, plotdata0.Value, plotdata1.Value, responseData.LeakLocations[j]);
                            value["Position"] = responseData.LeakLocations[j];
                            value["Leak"] = +leak.toFixed(2);
                            leakPressureValues.push(value);
                        }
                    }
                }

            }
        }

        // where the leakAdjusted Pressure is malformed, we still want to indicate the leak
        else {
            LogMessage(L_PARAM, src, "Invalid Leak Adjusted Pressure encountered. Pressure reset to 0");
            for (var i = 0; i < responseData.LeakLocations.length; i++) {
                value["Position"] = responseData.LeakLocations[i];
                value["Leak"] = 0;
                leakPressureValues.push(value);
            }
        }
    }
            
    LogMessage(L_PARAM, src, "Exiting function: " + strFn);

    return {
        "nodesInRoutes": nodesInRoutes,
        "breaks": breaks,
        "tickLabels": tickLabels
    };
}


function ExtractTemperatureData(nodeTemperatureModel, nodeTemperatureValues, segmentTemperatureValues, responseData,
    typesIncluded, maxOperatingRange, dataErrors)
{
    var strFn = "ExtractingTemperatureData";
    LogMessage(L_PARAM, src, "Entering function: " + strFn);

    // required by multi_band_multi_charts.js
    var totalPreviousRoutePositions = 0;
    var totalPreviousRouteNodes = 0;
    var routenum = 0;
    var nodesInRoutes = [];
    var breaks = [];
    var tickLabels = [];

    // There is only one route
    var maxRoutePosition = 0;
    nodesInRoutes.push(responseData.NodeTemperatureProfile.length);    // how many nodes in the route?

    var prevNode = null;
    var prevOutlet = null;

    var validData = ExtractNodeAndSegmentData(nodeTemperatureModel, nodeTemperatureValues, segmentTemperatureValues,
        totalPreviousRoutePositions, maxRoutePosition, routenum, responseData.NodeTemperatureProfile, responseData.SegmentTemperatureProfile,
        "Temperature", typesIncluded, maxOperatingRange, tickLabels, prevNode, prevOutlet, dataErrors);

    if (validData == null)
        return null;

    LogMessage(L_PARAM, src, "Exiting function: " + strFn);
    return {
        "nodesInRoutes": nodesInRoutes,
        "breaks": breaks,
        "tickLabels": tickLabels
    };
}


function ExtractNodeAndSegmentData(goodNodeData, theData, dataSegment, totalPreviousRoutePositions, maxRoutePosition, routeNum, Nodes, Segments, type, hasTypesOfData, maxOperatingRange, tickLabels, prevNode, prevOutlet, dataErrors) {
    var strFn = "ExtractNodeAndSegmentData";
    LogMessage(L_PARAM, src, "Entering function: " + strFn);
    var isNotFirstNode = false;
    var prevPlotPosition = {};
    var prevPosition = {};
    var currentPlotPosition = {};
    var currentPosition = {};
    try {
        for (var nodeNum = 0; nodeNum < Nodes.length; nodeNum++) {
            var node = Nodes[nodeNum];
            try {
                hscCheckRequiredFields(goodNodeData, node);
            }
            catch (err) {
                throw err + " in Response node element " + nodeNum;
            }

            var inletData = {};
            var outletData = {};

            // Collect the Node RoutePosition(Position) and ChartPostion(PlotPosition)
            inletData["Position"] = outletData["Position"] = node.Position;
            inletData["PlotPosition"] = outletData["PlotPosition"] = node.Position + totalPreviousRoutePositions;

            // Get Raw value
            inletData[type] = node["Inlet" + type];
            outletData[type] = node["Outlet" + type];

            if (!isFinite(inletData[type]) || !isFinite(outletData[type])) {
                dataErrors[type]["CurrentError"] = true;
                LogMessage("Node " + type + " for route: " + routeNum + " Node: " + nodeNum + " is invalid");
            }

            // Decipher the length of segment, and RoutePosition(Position) and ChartPosition(PlotPosition) of each profile reading
            currentPlotPosition = inletData["PlotPosition"];
            currentPosition = inletData["Position"];

            LogMessage(L_PARAM, src, "Just before isNotFirstNode segment");
            if (isNotFirstNode) {
                LogMessage(L_PARAM, src, "Extracing segment data");
                var segment = Segments[nodeNum - 1][type + "Values"];
                var segPositionWidth = (currentPlotPosition - prevPlotPosition) / (segment.length - 1);

                LogMessage(L_PARAM, src, "Node: " + nodeNum + " has " + segment.length + " Segment points");
                var node0 = Nodes[nodeNum - 1];
                var calcCorrectedPressureProfile = [];

                if (type == "Pressure") {
                    // Use Linear Equation to calculate profile values
                    calcCorrectedPressureProfile = interpolateProfileReadings(segment.length, node0["Position"], node["Position"], node0["CorrectedOutletPressure"], node["CorrectedInletPressure"]);
                }

                for (var k = 0; k < segment.length; k++) {
                    var segmentDataToPush = {};
                    segmentDataToPush["PlotPosition"] = prevPlotPosition + k * segPositionWidth;
                    segmentDataToPush["Position"] = prevPosition + k * segPositionWidth;
                    segmentDataToPush["SegmentCounts"] = segment.length;
                    segmentDataToPush["LeakAdjusted"] = 0;

                    if (type == "Pressure") {
                        
                        segmentDataToPush["CorrectedPressure"] = calcCorrectedPressureProfile[k]["Value"];
                    }

                    if (isNumber(segment[k])) {
                        segmentDataToPush[type] = segment[k];
                    }
                    else {
                        throw "node: " + nodeNum + ", segment: " + k + " is invalid";
                    }
                    dataSegment.push(segmentDataToPush);
                }
                LogMessage(L_PARAM, src, "Segments Length: " + segment.length);

            }
            else {
                isNotFirstNode = true;		//This gets executed on the first hit
                LogMessage(L_PARAM, src, "Extracing segment data in next iteration");
            }

            prevPlotPosition = inletData["PlotPosition"];
            prevPosition = inletData["Position"];

            LogMessage(L_PARAM, src, "HastypesOfData " + hasTypesOfData.BaselineData + " " + hasTypesOfData.MinMaxData + " " + " " + hasTypesOfData.TargetData + " " + hasTypesOfData.ElevationCorrectedData);

            // Collected Baseline Pressures
            if (hasTypesOfData.BaselineData) {
                var baselineString = "Baseline" + type;
                var baselineInletString = "BaselineInlet" + type;
                var baselineOutletString = "BaselineOutlet" + type;
                inletData[baselineString] = node[baselineInletString];
                outletData[baselineString] = node[baselineOutletString];
                if (!isFinite(inletData[baselineString]) || !isFinite(inletData[baselineString]))
                    dataErrors[type]["BaselineError"] = true;

                node.BaselineTime = inletData["BaselineTime"] = outletData["BaselineTime"] = hscOpAppsISO.parse(node.BaselineTime);
            }

            // Collect Operating Range Pressures 
            if (hasTypesOfData.MinMaxData) {
                var minOpString = "MinOp" + type;
                var maxOpString = "MaxOp" + type;
                if (isFinite(node[minOpString]) && isFinite(node[maxOpString])) {
                    inletData[minOpString] = outletData[minOpString] = node[minOpString];
                    inletData[maxOpString] = outletData[maxOpString] = node[maxOpString];
                    maxOperatingRange[type] = Math.max(maxOperatingRange[type], node[minOpString], node[maxOpString]);
                }
                else {
                    // Some operating range error for this node, so mark both min and max bad
                    inletData[minOpString] = outletData[minOpString] = NaN;
                    inletData[maxOpString] = outletData[maxOpString] = NaN;
                    if (prevOutlet != null)
                        // Mark previous outlet node as bad as well
                        prevOutlet[minOpString] = prevOutlet[maxOpString] = NaN;
                    dataErrors[type]["OperatingRangeError"] = true;
                }
                // Create a copy of this node's inlet data to use to reflect the operating range error status of the previous node
                var errorInlet = inletData;
                if ((prevNode != null) && (!isFinite(prevNode[minOpString]) || !isFinite(prevNode[maxOpString]))) {	// The previous node had an operating range error
                    errorInlet[minOpString] = errorInlet[maxOpString] = NaN;
                }
            }

            // Collect the Elevation-Corrected Pressures
            if (hasTypesOfData.ElevationCorrectedData) {
            	var correctedString = "Corrected" + type;
                var correctedInletString = "CorrectedInlet" + type;
                var correctedOutletString = "CorrectedOutlet" + type;

                inletData[correctedString] = node[correctedInletString];
                outletData[correctedString] = node[correctedOutletString];
                
                 if (!isFinite(inletData[correctedString]) || !isFinite(inletData[correctedString]))
                 {
                    dataErrors[type]["CorrectedPressureError"] = true;
                 }
            }


            LogMessage(L_PARAM, src, "Leak Location: Collecting Elevation Corrected Node Values" + JSON.stringify(inletData));
            inletData["StationShortName"] = outletData["StationShortName"] = node.StationShortName;

            tickLabels.push(node.StationShortName);

            inletData["StationName"] = outletData["StationName"] = node.StationName;
            if (hasTypesOfData.TargetData) {
                var targetString = "Target" + type;
                inletData[targetString] = outletData[targetString] = isFinite(node[targetString]) ? node[targetString] : NaN;
            }
            node.Time = inletData["Time"] = outletData["Time"] = hscOpAppsISO.parse(node.Time);

            if (node.Position >= maxRoutePosition) {
                maxRoutePosition = node.Position;
            }
            else {
                LogErrorMessage(src, "Detected node out of position order, routeNum:" + routeNum);
                throw "Detected node out of position"
            }

            LogMessage(L_PARAM, src, "Pushing Pressure and Temperature data to relevant data structures");
            if (hasTypesOfData.PushErrorAndPrevious) {
                if (prevOutlet != null) {
                    theData.push(prevOutlet)
                }
                theData.push(errorInlet);
            }
            theData.push(inletData);
            theData.push(outletData);

            prevNode = node;
            prevOutlet = outletData;
            LogMessage(L_PARAM, src, "Node Length: " + Nodes.length);
            LogMessage(L_PARAM, src, "Leak Location extracted: " + JSON.stringify(theData));


        }
        if (prevOutlet != null && hasTypesOfData.PushErrorAndPrevious) {
            // Add the final one to complete the set of 4 data points per node
            theData.push(prevOutlet);
        }
    }
    catch (err) {
        LogErrorMessage(src, "Error in extracting node/segment " + type + " data. Error message: " + err);
        return null;
    }
    LogMessage(L_PARAM, src, "Exiting function: " + strFn);
    return true;
}

function InitialisePlotData(dataToggle)
{
	var strFn = "InitialisePlotData";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	/*
	1)isPrimaryPlot property is true if it's the first plot of the type (i.e. pressure, temperature etc)
	this is so that we can determine the number of independent Y axes in the plot. This is also used for determining
	the primary plots where data is obtained from for other plots (such as baslines, max/min etc)
	THe returned object should be ordered such that objects where isPrimaryPlot = true should be first!
	2) IsNodeData determines if we only have node data for this and NO segment data!
	*/
	var dottedLines = "2,2";		//For the dotted lines legend entries
	var pressure = {
		"dataToggle": dataToggle[0],
		"legendClass": "PipelineProfilePressureSeries",
		"isInitiallyFilled": true,
		"hasOnePlotForAxis": false,
		"name": "Pressure",
		"nameForReferenceBox": "Pressure",
		"strokeWidthsLegendEntry": null,
		"isPressurePlot": true,
		"isTemperaturePlot": false,
		"isEmpty": true,
		"isPrimaryPlot": true,
		"isNodeData": false,
		"isFirstEntryForLegend": true,
		"legendTitle": "Raw Pressure"
	};
	
	var temperature = {
		"dataToggle": dataToggle[1],
		"legendClass": "PipelineProfileTemperatureSeries",
		"isInitiallyFilled": true,
		"hasOnePlotForAxis": true,
		"name": "Temperature",
		"nameForReferenceBox": "Temperature",
		"strokeWidthsLegendEntry": null,
		"isPressurePlot": false,
		"isTemperaturePlot": true,
		"isEmpty": true,
		"isPrimaryPlot": true,
		"isNodeData": false,
		"isFirstEntryForLegend": false,
		"legendTitle": "Temperature"
	};
	var baselinePressure = {
		"dataToggle": dataToggle[2],
		"legendClass":"PipelineProfileBaselineProfile",
		"isInitiallyFilled": true,
		"hasOnePlotForAxis": false,
		"name": "BaselinePressure",
		"nameForReferenceBox": "BaselinePressure",
		"strokeWidthsLegendEntry": dottedLines,
		"isPressurePlot": true,
		"isTemperaturePlot": false,
		"isEmpty": true,
		"isPrimaryPlot": false,
		"isNodeData": true,
		"isFirstEntryForLegend": false,
		"legendTitle": "Baseline Pressure"
	};
	var operatingRange = {
		"dataToggle": dataToggle[3],
		"legendClass":"PipelineProfileOperatingRange",
		"isInitiallyFilled": true,
		"hasOnePlotForAxis": false,
		"name": "OperatingRange",
		"nameForReferenceBox": "OperatingRange",
		"strokeWidthsLegendEntry": null,
		"isPressurePlot": true,
		"isTemperaturePlot": false,
		"isEmpty": true,
		"isPrimaryPlot": false,
		"isNodeData": true,
		"isFirstEntryForLegend": false,
		"legendTitle": "Operating Range Pressure"
	};
	var correctedPressure = {
	    "dataToggle": dataToggle[4],
	    "legendClass": "PipelineProfileCorrectedPressureSeries",
	    "isInitiallyFilled": true,
	    "hasOnePlotForAxis": true,
	    "name": "CorrectedPressure",
	    "nameForReferenceBox": "CorrectedPressure",
	    "strokeWidthsLegendEntry": null,
	    "isPressurePlot": true,
	    "isTemperaturePlot": false,
	    "isEmpty": true,
	    "isPrimaryPlot": false,
	    "isNodeData": false,
	    "isFirstEntryForLegend": false,
	    "legendTitle": "Elevation-Corrected Pressure"
	};
	var leakAdjusted = {
	    "dataToggle": dataToggle[5],
	    "legendClass": "PipelineProfileLeakPressureSeries",
	    "isInitiallyFilled": true,
	    "hasOnePlotForAxis": true,
	    "name": "LeakAdjusted",
	    "nameForReferenceBox": "LeakAdjusted",
	    "strokeWidthsLegendEntry": null,
	    "isPressurePlot": true,
	    "isTemperaturePlot": false,
	    "isEmpty": true,
	    "isPrimaryPlot": false,
	    "isNodeData": false,
	    "isFirstEntryForLegend": false,
	    "legendTitle": "Leak-Adjusted Pressure"
	};
	
	// todo: add new pressure traces
	var keys = ["Pressure", "CorrectedPressure", "BaselinePressure", "OperatingRange", "LeakAdjusted", "Temperature"];
	
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	return {
		"Pressure": pressure,
		"CorrectedPressure": correctedPressure,
		"LeakAdjusted": leakAdjusted,
		"Temperature": temperature,
		"BaselinePressure": baselinePressure,
		"OperatingRange": operatingRange,
		"Keys": keys
	};
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/