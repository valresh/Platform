// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// PipelinePressureProfile.js


hscOpAppsPipelinePressureProfile = function () {

	var src = "PipelinePressureProfile.js";		//For logging
	var RequestType = _DISPLAYTYPE_PipelinePressureProfile;		//What we are requesting
	
	var responseData;
	var arrayDataTotal = [];
	var breaks;
	var nodesInRoutes = [];
	var defaultTemperatureValue = 0;
	var _firstSampleRequest = true;
	var tickLabels = [];
	var keysOfNonEmptyPrimaryData = [];
	var dataToggle = [];
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
				.width(WidthFn(margin))
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
			"PipelinePoint": hscGlobalPipelinePoint,
			"Time": hscOpAppsISO(TimeControl.endTime()),
			"BaselineTime": hscOpAppsISO(TimeControl.baselineTime()),
			"IsLive": TimeControl.playing()
		}
		
		var requestType = RequestType;
		if (hscGlobalPipelinePoint === hscDummyPointName) 
		{
			//Sample data used if equality holds true
			requestType = "Sample" + requestType;
			if (_firstSampleRequest) 
			{
				LogErrorMessage(src, "WARNING - " + requestType + " RequestType in use");
				_firstSampleRequest = false;
			}
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
			// Check that the ResponseData has the required fields
			var goodResponseData =
			{
				"RoutePressureProfile": {},
				"RouteTemperatureProfile": {},
				"PositionUnits" : "empty",
				"PressureUnits" : "empty",
				"TemperatureUnits" : "empty"
			}
			try 
			{
				hscCheckRequiredFields(goodResponseData, responseData);
			}
			catch (err)
			{
				throw err + " in ResponseData element";
			}
			
			LogMessage(L_PARAM, src, "Passed the check for required fields");
			if (responseData.RouteTemperatureProfile.length <=0 || responseData.RoutePressureProfile.length <= 0)
				throw "Response RouteProfile node is empty";
			
			var arrayDataInitial  = [];
			var totalPreviousRoutePositions = 0;

			var goodNodeTemperature = {
			"Position": 0,
			"StationShortName": "",
			"InletTemperature": 0,
			"OutletTemperature": 0,
			"StationName": "",
			"Time": "iso",
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
				"BaselineTime": "iso"
			}

			//Assume we have equal same number of pressure and temperature routes
			
			if(responseData.RouteTemperatureProfile.length != responseData.RoutePressureProfile.length)
			{
				LogMessage(L_PARAM, src, "Unequal number of routes for pressure and temperature data");
			}
			var pressureHasTypesOfData = {
				"MinMaxData": true,
				"BaselineData": true,
				"TargetData": true,
				"PushErrorAndPrevious": true
			};
			var temperatureHasTypesOfData = {
				"MinMaxData": false,
				"BaselineData": false,
				"TargetData": false,
				"PushErrorAndPrevious": false
			};
			
			
			var maxOperatingRange = {
				"Pressure": -Infinity,
				"Temperature": -Infinity
			};	// Determine the global maximum operating range
			var nodesAndBreaks= {};
			var dataPressureNode = [];
			var dataPressureSegment = [];
			var validKeys = [];
			LogMessage(L_PARAM, src, "Extracting Pressure Data from Server");
			var type = "Pressure";
			var nodesAndBreaksPressure = ExtractServerData(goodNodePressure, dataPressureNode, dataPressureSegment, responseData.RoutePressureProfile, pressureHasTypesOfData, maxOperatingRange, type, dataErrors);
			LogMessage(L_PARAM, src, "Finished Extracting Pressure Data from Server");
			if(nodesAndBreaksPressure!= null)
			{
				var strPressure = "Pressure";
				nodesAndBreaks[strPressure] = nodesAndBreaksPressure;
				validKeys.push(strPressure);
			}

			
			var dataTemperatureNode = [];
			var dataTemperatureSegment = [];
			LogMessage(L_PARAM, src, "Extracting Temperature Data from Server");
			var type = "Temperature";
			var nodesAndBreaksTemperature =ExtractServerData(goodNodeTemperature, dataTemperatureNode, dataTemperatureSegment, responseData.RouteTemperatureProfile, temperatureHasTypesOfData, maxOperatingRange,type, dataErrors);
			LogMessage(L_PARAM, src, "Finished Extracting Temperature Data from Server");
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
			
			LogMessage(L_PARAM, src, "Pushing all data values to arrays");
			var dataPresNode = [];
			var dataPresSeg = [];
			var dataTempNode = [];
			var dataTempSeg = [];
			dataPresNode.push(dataPressureNode);
			dataPresSeg.push(dataPressureSegment);
			dataTempNode.push(dataTemperatureNode);
			dataTempSeg.push(dataTemperatureSegment);

			arrayDataInitial.push(dataPresNode);
			arrayDataInitial.push(dataTempNode);		//Node data
			arrayDataInitial.push(dataPresSeg);		//Segment data
			arrayDataInitial.push(dataTempSeg);
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

		var margin = { top: 0, right: 80, bottom: 50, left: 80 }
		var pressureString = "Pressure";
		var pressureTitle = (responseData.PressureUnits == "") ? pressureString : pressureString + " (" + responseData.PressureUnits + ")";
		var temperatureString = "Temperature";
		var temperatureTitle = (responseData.TemperatureUnits == "") ? temperatureString : temperatureString + " (" + responseData.TemperatureUnits + ")";
		var yAxisTitlesArray = {
			"Pressure": pressureTitle,
			"Temperature": temperatureTitle
		};
		var _yValuesPerXPressure = 4;
		var _yValuesPerXTemperature = 1;

		var newchart = multiBandChartMultipleLines(dataToggle)
			.xPlotPosition(function (d) { return d.PlotPosition; })
			.xPosition(function (d) { return d.Position; })
			.yPressure(function (d) { return d.Pressure; })
			.yMinOpPressure(function (d) { return +d.MinOpPressure; })
			.yMaxOpPressure(function (d) { return +d.MaxOpPressure; })
			.yBaselinePressure(function (d) { return +d.BaselinePressure; })
			.ySegmentsInNode(function(d) {return +d.SegmentCounts;})
			.yTemperature(function(d) {return +d.Temperature;})
			.width(WidthFn(margin))
			.height(HeightFn(margin))
			.xAxisTitle("Node")
			.yAxisTitles(yAxisTitlesArray)
			.xAxisTickLabels(tickLabels)
			.yValuesPerXTemperature(_yValuesPerXTemperature)
			.yValuesPerXPressure(_yValuesPerXPressure)
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
			return "Pipeline Profile";
		},
		Type: function () {
			return RequestType;
		}
	}
}();

function SetLimitsOfOperatingRange(arrayDataTotal, plotData, dataErrors, maxOperatingRange)
{

	//This assumes that we initially have pressure and then have temperature data which is the case for arrayDataTotal
	var strFn = "SetLimitsOfOperatingRange";
	LogMessage(L_PARAM, src, "Entered function: "+strFn);
	var keys = plotData.Keys;			
	for(var i=0 ; i< keys.length ;i++)
	{
		var key = keys[i];
		if(plotData[key].isPrimaryPlot)
		{
			if(dataErrors[key].OperatingRangeError && isFinite(maxOperatingRange[key]))
			{
				strMax = "MaxOp"+key;
				strMin = "MinOp"+key;
				//Replace any operating range NaNs with the maximum operating range. THis is done s othat
				// NaNs do not interfere with the maximum operating range
				for(var j=0; j < arrayDataTotal[i][0].length; j++)
				{
					if(!isFinite(arrayDataTotal[i][0][j][strMin]))
					{
						//If min is bad then max will be as well
						arrayDataTotal[i][0][j][strMin] = arrayDataTotal[i][0][j][strMax] = maxOperatingRange[key];
					}
				}
			}
		}
	}
	LogMessage(L_PARAM, src, "Exiting function: "+strFn);
	return arrayDataTotal;
}
function isNumber(testNum)
{
	return !isNaN(parseFloat(testNum));
}

function ToggleEmptynessProperty(plotData, keysNonEmpty)
{
	var strFn = "ToggleEmptynessProperty";
	LogMessage(L_PARAM, src, "Entered function: "+strFn);
	//Keys will basically just be temperature or pressure!
	for(var i=0; i< keysNonEmpty.length; i++)
	{
		plotData[keysNonEmpty[i]].isEmpty = false;
		//Now set all of the keys associated with that!
		var strCommonProperty = "is"+keysNonEmpty[i]+"Plot";
		for(var j=0; j< plotData.Keys.length; j++)
		{
			var key = plotData.Keys[j];
			if(plotData[key][strCommonProperty])		//if "isPressurePlot"
			{
				plotData[key].isEmpty = false;
			}
		}
	}
	LogMessage(L_PARAM, src, "Exiting function: "+strFn);
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

function ExtractServerData(goodNodeData, theData, dataSegment, RouteProfile, hasTypesOfData, maxOperatingRange, type, dataErrors)
{
	var strFn = "ExtractServerData";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var totalPreviousRoutePositions = 0;
	var totalPreviousRouteNodes = 0;
	var nodesInRoutes = [];
	var breaks = [];
	var tickLabels = [];
	var nodeString = "Node"+type;
	var segmentString = "Segment"+type;
	for (var routeNum = 0; routeNum < RouteProfile.length; routeNum++) 
	{
		LogMessage(L_PARAM, src, "Currently in route " + routeNum);
		var maxRoutePosition = 0;

		var route = RouteProfile[routeNum];
		nodesInRoutes.push(route[nodeString].length);		//nodesInRoutes[i], is number of nodes in route i
		if (route[nodeString].length<=0)
			LogMessage(L_PARAM, src, "Route: "+ routeNum+" has all node data points empty");

		if (routeNum > 0) 
		{
			// Accumulate total number of nodes in previous routes
			var previousRouteNumberNodes = RouteProfile[routeNum - 1][nodeString].length;
			totalPreviousRouteNodes += previousRouteNumberNodes;

			// Accumulate total distance of previous routes, assuming nodes are in order
			totalPreviousRoutePositions += RouteProfile[routeNum - 1][nodeString][previousRouteNumberNodes - 1].Position;

			// Make a break in the charts to separate out the different route data
			breaks.push(totalPreviousRouteNodes);
		}

		// For each route element, check it has the required fields and unpack the data into arrayData for the chart
		LogMessage(L_PARAM, src, "Route " + routeNum + " Node array has " + route[nodeString].length + " elements.");
		/* The operating range min and max is shown by the underlying viz as a continous band chart.  If there is any
		 bad operating range data it is to be shown as a gap in the band chart for the node.  This is accomplished by 
		 setting the min and max for the node to the same value so that nothing is shown.  If there are nodes before or after
		 the node with the error then dummy nodes with identical min and max values are also used.*/

		var prevNode = null;
		var prevOutlet = null;

		//We don't want the first iteration for segment data as we need to interpolate the position between nodes
		//Each iteration goes through a node 
		LogMessage(L_PARAM, src, "Cycling through node data for Route: "+routeNum);
		var prevNode = null;
		var prevOutlet = null;
		var validData = ExtractNodeAndSegmentData(goodNodeData, theData, dataSegment,totalPreviousRoutePositions, maxRoutePosition, routeNum, route[nodeString], route[segmentString], type, hasTypesOfData,
		maxOperatingRange, tickLabels, prevNode, prevOutlet, dataErrors);
		
		if(validData ==null)
		{
			return null;	//Error in one part of the node/segment data. Don't plot any of that type/
		}
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"nodesInRoutes": nodesInRoutes,
		"breaks": breaks,
		"tickLabels": tickLabels
	};
}

function ExtractNodeAndSegmentData(goodNodeData, theData, dataSegment, totalPreviousRoutePositions, maxRoutePosition, routeNum, Nodes, Segments, type, hasTypesOfData,maxOperatingRange, tickLabels, prevNode, prevOutlet, dataErrors)
{
	var strFn = "ExtractNodeAndSegmentData";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var isNotFirstNode = false;
	var prevPlotPosition = {};
	var prevPosition = {};
	var currentPlotPosition = {};
	var currentPosition = {};
	try
	{
		for (var nodeNum = 0; nodeNum < Nodes.length; nodeNum++) 
		{
			var node = Nodes[nodeNum];
			try 
			{
				hscCheckRequiredFields(goodNodeData, node);
			}
			catch (err) 
			{
				throw err + " in Response node element " + nodeNum;
			}
			
			var inletData = {};
			var outletData = {};
			inletData["Position"] = outletData["Position"] = node.Position;
			inletData["PlotPosition"] = outletData["PlotPosition"] = node.Position + totalPreviousRoutePositions;

			inletData[type] = node["Inlet"+type];
			outletData[type] = node["Outlet"+type];

			if (!isFinite(inletData[type]) || !isFinite(outletData[type]))
			{
				dataErrors[type]["CurrentError"] = true;
				LogMessage("Node "+type+" for route: " + routeNum + " Node: " + nodeNum + " is invalid");
			}
			
			currentPlotPosition = inletData["PlotPosition"];
			currentPosition = inletData["Position"];
			LogMessage(L_PARAM, src, "Just before isNotFirstNode segment");
			if(isNotFirstNode)
			{
				LogMessage(L_PARAM, src, "Extracing segment data");
				var segment = Segments[nodeNum-1][type+"Values"];
				var segPositionWidth = (currentPlotPosition-prevPlotPosition)/(segment.length-1);
				LogMessage(L_PARAM, src, "Node: " +nodeNum+" has " +segment.length + " Segment points");
				for(var k=0; k<segment.length; k++)
				{
					var segmentDataToPush = {};
					segmentDataToPush["PlotPosition"] = prevPlotPosition + k*segPositionWidth;
					segmentDataToPush["Position"] = prevPosition + k*segPositionWidth;
					segmentDataToPush["SegmentCounts"] = segment.length;
					if(isNumber(segment[k]))
					{
						segmentDataToPush[type] = segment[k];	
					}
					else
					{
						throw "node: " + nodeNum + ", segment: " + k +" is invalid"; 
					}
					dataSegment.push(segmentDataToPush);
				}
				LogMessage(L_PARAM, src, "Segments Length: " +segment.length);
			}
			else
			{
				isNotFirstNode = true;		//This gets executed on the first hit
				LogMessage(L_PARAM, src, "Extracing segment data in next iteration");
			}

			prevPlotPosition = inletData["PlotPosition"];
			prevPosition = inletData["Position"];
			LogMessage(L_PARAM, src, "HastypesOfData "+ hasTypesOfData.BaselineData + " "+ hasTypesOfData.MinMaxData+" " 
			+ " " + hasTypesOfData.TargetData);
			if(hasTypesOfData.BaselineData)
			{
				var baselineString = "Baseline"+type;
				var baselineInletString = "BaselineInlet"+type;
				var baselineOutletString = "BaselineOutlet"+type;
				inletData[baselineString] = node[baselineInletString];
				outletData[baselineString] = node[baselineOutletString]; 
				if (!isFinite(inletData[baselineString]) || !isFinite(inletData[baselineString]))
					dataErrors[type]["BaselineError"] = true;
				
				node.BaselineTime = inletData["BaselineTime"] = outletData["BaselineTime"] = hscOpAppsISO.parse(node.BaselineTime);
			}
			if(hasTypesOfData.MinMaxData)
			{
				var minOpString = "MinOp"+ type;
				var maxOpString = "MaxOp" + type;
				if (isFinite(node[minOpString]) && isFinite(node[maxOpString])) 
				{
					inletData[minOpString] = outletData[minOpString] = node[minOpString];
					inletData[maxOpString] = outletData[maxOpString] = node[maxOpString];
					maxOperatingRange[type] = Math.max(maxOperatingRange[type], node[minOpString], node[maxOpString]);
				}
				else 
				{
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
				if ((prevNode != null) && (!isFinite(prevNode[minOpString]) || !isFinite(prevNode[maxOpString])))
				{	// The previous node had an operating range error
					errorInlet[minOpString] = errorInlet[maxOpString] = NaN;
				}
			}
			inletData["StationShortName"] = outletData["StationShortName"] = node.StationShortName;
			
			tickLabels.push(node.StationShortName);

			inletData["StationName"] = outletData["StationName"] = node.StationName;
			if(hasTypesOfData.TargetData)
			{
				var targetString = "Target"+type;
				inletData[targetString] = outletData[targetString] = isFinite(node[targetString]) ? node[targetString] : NaN;
			}
			node.Time = inletData["Time"] = outletData["Time"] = hscOpAppsISO.parse(node.Time);
			
			if (node.Position >= maxRoutePosition)
			{
				maxRoutePosition = node.Position;
			}
			else
			{
				LogErrorMessage(src, "Detected node out of position order, routeNum:"+routeNum);
				throw "Detected node out of position"
			}

			LogMessage(L_PARAM, src, "Pushing Pressure and Temperature data to relevant data structures");
			if(hasTypesOfData.PushErrorAndPrevious)
			{
				if(prevOutlet != null)
				{
					theData.push(prevOutlet)
				}
				theData.push(errorInlet);
			}
			theData.push(inletData);
			theData.push(outletData);
			
			prevNode = node;
			prevOutlet = outletData;
			LogMessage(L_PARAM, src, "Node Length: " +Nodes.length);
		}
		if (prevOutlet!= null && hasTypesOfData.PushErrorAndPrevious)
		{
			// Add the final one to complete the set of 4 data points per node
			theData.push(prevOutlet);
		}
	}
	catch(err)
	{
		LogErrorMessage(src, "Error in extracting node/segment "+type+" data. Error message: "+ err);
		return null;
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
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
	    "legendClass": "PipelineProfileBaselineProfile",
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
	    "legendClass": "PipelineProfileOperatingRange",
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
	
	var keys = ["Pressure", "BaselinePressure", "OperatingRange", "Temperature" ];
	
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	return {
		"Pressure": pressure,
		"Temperature": temperature,
		"BaselinePressure": baselinePressure,
		"OperatingRange": operatingRange,
		"Keys": keys
	};
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/