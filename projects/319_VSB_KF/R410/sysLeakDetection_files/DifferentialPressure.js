// /************************************************************************/
// /*****	  COPYRIGHT 2013	HONEYWELL INTERNATIONAL SARL			****/
// /************************************************************************/
// DifferentialPressure.js


hscOpAppsDifferentialPressure = function () {

	var src = "DifferentialPressure.js";		//For logging
	var RequestType = _DISPLAYTYPE_DifferentialPressure;		
		
	var Chart;
	var ChartDivId;
	var InletChart;
	var OutletChart;
	
	var WidthFn, HeightFn;
	var TimeControl;
		
	var responseData;
	var inletData; 
	var outletData;
	
	var tickLabels; 
	var _firstSampleRequest = true;

	var legend = {
		"x": 300,
		"y": 200,
		"xPrevious": 0,
		"yPrevious": 0
	};

	var chartmargin = { top: 0, right: 80, bottom: 20, left: 80 }

	var plotRef = initialisatDFPPlotData();

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
						
		ChartDivId = chartDiv;		
		
		WidthFn = widthFn;
		HeightFn = heightFn;
		TimeControl = timeControl;

		$(window).resize(function () {
			$("svg").empty();		//Clears the svg to prevent dragging
			if (Chart == null) 
				return;

			
			var margin = Chart.margin();

			InletChart
				.width(WidthFn(margin))
				.height((HeightFn(margin) / 2) - chartmargin.bottom);

			OutletChart
				.width(WidthFn(margin))
				.height((HeightFn(margin) / 2) - chartmargin.bottom);
			
			var inlet = d3.select(".inletDiv svg")
				.attr('height', "50%")
				.call(InletChart);
			
			var inlet = d3.select(".outletDiv svg")
				.attr('height', "50%")
				.call(OutletChart);

		});
		
	}

	function FormRequest() 
	{

		
		var requestData = 
		{
			"SegmentPoint": hscGlobalSegmentPoint
		}
		
		var requestType = RequestType;
		
		// There is no dummy point
		if (hscGlobalSegmentPoint === hscDummyPointName) {
			requestType = "Sample" + requestType;
			if (_firstSampleRequest) {
				LogErrorMessage(src, "WARNING - " + requestType + "	 DummyPoint is not supported");
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
		LogMessage(L_LOG, src, "In parse response function");
		inletData = null;
		outletData = null;
		
		try {
			responseData = JSON.parse(response.ResponseData);
			LogMessage(L_LOG, src, "Process Differential Pressure Response: " + JSON.stringify(responseData));
			
			// Check that the ResponseData has the required fields
			var goodResponseData = 
			{
				"InletChartData": null,
				"OutletChartData": null,
				"PressureUnits": "empty",
				"DurationUnits": "empty",
				"PlotOneEnabled": true,
				"PlotTwoEnabled": true
			}
			
			var goodChartData = 
			{
				"PlotOne": {},
				"PlotTwo": {}
			}

			var goodPlotData =
			{
				"Time": "iso",
				"Pressure": 0
			}

			// Check Response
			try 
			{
				hscCheckRequiredFields(goodResponseData, responseData);
			}
			catch (err)
			{
				throw err + " in ResponseData element";
			}
			
			if(responseData.PlotOneEnabled)
			{
				if(responseData.InletChartData != null)
				{
					try 
					{
						hscCheckRequiredFields(goodChartData, responseData.InletChartData);
					}
					catch(err)
					{
						throw err + " in ResponseData InletChart element";
					}
				}
				
				if( responseData.OutletChartData != null)
				{
					try 
					{
						hscCheckRequiredFields(goodChartData, responseData.OutletChartData);
					}
					catch(err)
					{
						throw err + " in ResponseData InletChart element";
					}
				}
			}
			
			// Run 1 results for the Inlet and Outlet Charts are required
			if(responseData.InletChartData.PlotOne.length <= 0)				
				throw "InletChartData PlotOne data results returned empty";
			if(responseData.OutletChartData.PlotOne.length <= 0)
				throw "OutletChartData PlotOne data results returned empty";
			
			// Run 2 results for the Inlet and Outlet Charts is optional
			if(responseData.InletChartData.PlotTwo.length <= 0)
				LogMessage(L_LOG, src, "InletChartData Plot Two data results returned empty");
			if(responseData.OutletChartData.PlotTwo.length <= 0)
				LogMessage(L_LOG, src, "OutletChartData Plot Two data results returned empty");
			
			var datatemp = [];
			var data = [];
			var datatemp2 = [];
			var data2 = [];

			if(responseData.InletChartData.PlotOne.length != responseData.OutletChartData.PlotOne.length)
				throw "The number of Plot One data results for the InletChart and OutletCharts are not equal";
			
			if(responseData.InletChartData.PlotTwo.length != responseData.OutletChartData.PlotTwo.length)
				throw "The number of Plot Two data results for the InletChart and OutletCharts are not equal";
			
			LogMessage(L_LOG, src, "Response Inlet and Outlet Chart plots validated");
			
			for (var i = 0; i < responseData.InletChartData.PlotOne.length; i++) {
				
				var inletPlotOne = responseData.InletChartData.PlotOne[i];
				var inletPlotTwo = responseData.InletChartData.PlotTwo[i];
				
				try
				{					
					hscCheckRequiredFields(goodPlotData, inletPlotOne);
				}
				catch(err) {
					throw err + " in Response DifferentialPressure element " + i;
				}
				
				var temp = {};

				temp["PressureOne"] = inletPlotOne.Pressure;
				responseData.InletChartData.PlotOne[i].Time = temp["TimeOne"] = hscOpAppsISO.parse(inletPlotOne.Time);
				
				if(responseData.PlotTwoEnabled)
				{
					hscCheckRequiredFields(goodPlotData, inletPlotTwo);
					temp["PressureTwo"] = inletPlotTwo.Pressure;
					responseData.InletChartData.PlotTwo[i].Time = temp["TimeTwo"] = hscOpAppsISO.parse(inletPlotTwo.Time);
				}				
				
				data.push(temp);
			}
			
			datatemp.push(data);
			inletData = datatemp;
			LogMessage(L_LOG, src, "Response Inlet Chart data populated");
			
			for (var i = 0; i < responseData.OutletChartData.PlotOne.length; i++) {
				
				var outletPlotOne = responseData.OutletChartData.PlotOne[i];
				var outletPlotTwo = responseData.OutletChartData.PlotTwo[i];
				
				try
				{					
					hscCheckRequiredFields(goodPlotData, outletPlotOne);
				}
				catch(err) {
					throw err + " in Response DifferentialPressure element " + i;
				}
				
				var temp = {};

				temp["PressureOne"] = outletPlotOne.Pressure;
				responseData.OutletChartData.PlotOne[i].Time = temp["TimeOne"] = hscOpAppsISO.parse(outletPlotOne.Time);
				
				if(responseData.PlotTwoEnabled)
				{
					hscCheckRequiredFields(goodPlotData, outletPlotTwo);
					temp["PressureTwo"] = outletPlotTwo.Pressure;
					responseData.OutletChartData.PlotTwo[i].Time = temp["TimeTwo"] = hscOpAppsISO.parse(outletPlotTwo.Time);
				}				
				
				data2.push(temp);
			}
	
			datatemp2.push(data2);
			outletData = datatemp2;
			LogMessage(L_LOG, src, "Response Outlet Chart data populated");
			
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
		LogMessage(L_LOG, src, "Entering DFP DrawChart function");
		if (responseData == null) 
		{
			LogErrorMessage(src, "No response data available for DrawChart");
			return false;
		}
		
		if (inletData == null) {
			LogErrorMessage(src, "No array data available for DrawChart");
			return false;
		}
		
		// bottom margin changed from 60 to 80, to handle tabs
		var margin = { top: 0, right: 80, bottom: 100, left: 80 }
		
		var pressureString = "Pressure";
		var pressureTitle = (responseData.PressureUnits == "") ? pressureString : pressureString + " (" + responseData.PressureUnits + ")";

		var durationString = "Time Elapsed" + " " + "(mins)";
		var durationTitle = (responseData.DurationUnits == "") ? durationString : durationString + " (" + responseData.DurationUnits + ")";
		
		plotRef.RunOne.enable = responseData.PlotOneEnabled;
		plotRef.RunTwo.enable = responseData.PlotTwoEnabled;
		
		var childInletChart = multiDualLineChart()
			.x(function (d) { return d.TimeOne; })
			.y(function (d) { return d.PressureOne; })
			.y0(function (d) { return +d.PressureTwo; })
			.width(WidthFn(margin))
			.height((HeightFn(margin) / 2) - chartmargin.bottom)
			.xAxisTitle(durationString)
			.yAxisTitle(pressureTitle)
			.margin(margin)
			.graphName("InletReadings")
			.hideLegend(true)
			.legendCoord(legend)
			.plotRef(plotRef)
			.hideTitle(false);
		
		var childOutletChart = multiDualLineChart()
			.x(function (d) { return d.TimeOne; })
			.y(function (d) { return d.PressureOne; })
			.y0(function (d) { return +d.PressureTwo; })
			.width(WidthFn(margin))
			.height((HeightFn(margin)/2 )-chartmargin.bottom)	
			.xAxisTitle(durationString)
			.yAxisTitle(pressureTitle)
			.margin(margin)
			.graphName("OutletReadings")
			.hideLegend(false)
			.legendCoord(legend)
			.plotRef(plotRef)
			.hideTitle(true);
		
		var parentDiv = d3.select(ChartDivId)
			.append("div")
			.style("width","100%")
			.attr("class", "parentDiv svg");
		
		parentDiv.append("div")			
			.attr("class", "inletDiv svg")
			.attr('height',"50%")
		
		parentDiv.append("div")
			.attr("class", "outletDiv svg")
			.attr('height', "50%")
				
		var inlet = d3.select(".inletDiv");
		inlet.append("p")
			.attr("class", "graphDescription")
			.text("Inlet Pressure");
		inlet
			.datum(inletData)
			.call(childInletChart);
			
		var outlet = d3.select(".outletDiv")
		outlet.append("p")
			.attr("class", "graphDescription")
			.text("Outlet Pressure");
		outlet
			.datum(outletData)			
			.call(childOutletChart);
		
		
		Chart = parentDiv;
		InletChart = inlet;
		OutletChart = outlet;
		
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
		TimeControl.HideBaseline();
		TimeControl.HidePeriodInterval();
		TimeControl.PlayContinously();
		document.getElementById('OpAppsFooter').style.display="none";
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
			return "Static Leak Detection Differential Pressure";
		},
		Type: function () {
			return RequestType;
		}
	}
}();

function initialisatDFPPlotData() {

	var runOne = {
		"name": "Run 1",
		"ref":"RunOne",
		"enable": true,
		"style": "DifferentialPressuePlotOne",
	};

	var runTwo = {
		"name": "Run 2",
		"ref": "RunTwo",
		"enable": true,
		"style": "DifferentialPressuePlotTwo",
	};

	var keys = ["RunOne", "RunTwo"];

	return {
		"RunOne": runOne,
		"RunTwo": runTwo,
		"Keys": keys
	};
}


// /************************************************************************/
// /*****	  COPYRIGHT 2013	HONEYWELL INTERNATIONAL SARL			****/
// /************************************************************************/