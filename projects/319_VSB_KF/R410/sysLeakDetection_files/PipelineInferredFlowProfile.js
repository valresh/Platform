// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// PipelineInferredFlowProfile.js

hscOpAppsPipelineInferredFlowProfile = function () {
	var src = "PipelineInferredFlowProfile.js";

	var RequestType = _DISPLAYTYPE_PipelineInferredFlowProfile;

	var Chart;
	var ChartDivId;
	var WidthFn, HeightFn;
	var TimeControl;

	var responseData;
	var arrayData;
	var breaks;
	var tickLabels;

	var _firstSampleRequest = true;

	function OnReady(chartDiv, widthFn, heightFn, timeControl, navigationControl) {

		try {
			if ($(chartDiv).length === 0)
				throw "Unable to find element with id of " + chartDiv + " on the page.";
			if (!(typeof widthFn === 'function'))
				throw "widthFn passed in is not a function.";
			if (!(typeof heightFn === 'function'))
				throw "heightFn passed in is not a function.";
			if (!(typeof timeControl === 'object'))
				throw "timeControl passed in is not an object.";
			if (!(typeof navigationControl === 'object'))
				throw "navigationControl passed in is not an object.";
		}
		catch (err) {
			var errorString = "Unable to make " + RequestType + " chart type ready.<BR>" + err;
			LogErrorMessage(src, errorString.replace("<BR>"," "));
			throw errorString;
		}

		ChartDivId = chartDiv;
		WidthFn = widthFn;
		HeightFn = heightFn;
		TimeControl = timeControl;
		NavigationControl = navigationControl;

		$(window).resize(function () {
			if (Chart == null)
				return;

			var margin = Chart.margin();

			Chart
				.width(WidthFn(margin))
				.height(HeightFn(margin));

			d3.select(ChartDivId + ' svg')
				.attr('width', WidthFn(margin))
				.attr('height', HeightFn(margin))
				.call(Chart);
		});
	}

	function FormRequest() {
		var requestData = {
			"PipelinePoint": hscGlobalPipelinePoint,
			"Time": hscOpAppsISO(TimeControl.endTime())
		}

		var requestType = RequestType;
		if (hscGlobalPipelinePoint === hscDummyPointName) {
			requestType = "Sample" + requestType;
			if (_firstSampleRequest) {
				LogErrorMessage(src, "WARNING - " + requestType + " RequestType in use");
				_firstSampleRequest = false;
			}
		}

		var request = {
			"RequestType": requestType,
			"RequestData": JSON.stringify(requestData)
		}

		return JSON.stringify(request);
	}

	function ParseResponse(response) {
		arrayData = null;
		tickLabels = [];
		breaks = [];

		try {
			// This code assumes response contains ResponseType and valid JSON ResponseData
			// Check that the ResponseType is the correct type
			responseData = JSON.parse(response.ResponseData);

			// Check that the ResponseData has the required fields
			var goodResponseData = {
				"RouteInferredFlowProfile": {},
				"FlowUnits": "empty",
				"PositionUnits": "empty",
				"Time": "iso"
			}
			try {
				hscCheckRequiredFields(goodResponseData, responseData);
			}
			catch (err) {
				throw err + " in ResponseData element";
			}

			responseData.Time = hscOpAppsISO.parse(responseData.Time);

			if (responseData.RouteInferredFlowProfile.length <= 0)
				throw "Response RouteInferredFlowProfile node is empty";

			var datatemp = [];
			var data = [];
			var totalPreviousRoutePositions = 0;
			var totalPreviousRouteNodes = 0;

			var goodInferredFlow = {
				"Position": 0,
				"MinMeas": 0,
				"MaxMeas": 0,
				"MinCalc": 0,
				"MaxCalc": 0,
				"SegmentShortName": "",
				"SegmentName": ""
			}

			for (var routeNum = 0; routeNum < responseData.RouteInferredFlowProfile.length; routeNum++) {
				var maxRoutePosition = 0;

				var route = responseData.RouteInferredFlowProfile[routeNum];

				if (route.InferredFlow.length <= 0)
					throw "Response InferredFlow node is empty";

				if (routeNum > 0) {
					// Accumulate total number of nodes in previous routes
					var previousRouteNumberNodes = responseData.RouteInferredFlowProfile[routeNum - 1].InferredFlow.length;
					totalPreviousRouteNodes += previousRouteNumberNodes;

					// Accumulate total distance of previous routes, assuming nodes are in order
					totalPreviousRoutePositions += responseData.RouteInferredFlowProfile[routeNum - 1].InferredFlow[previousRouteNumberNodes - 1].Position;

					// Make a break in the charts
					breaks.push(totalPreviousRouteNodes);
				}

				// For each route element, check it has the required fields and unpack the data into arrayData for the chart
				LogMessage(L_PARAM, src, "Route " + routeNum + " InferredFlow array has " + route.InferredFlow.length + " elements.");
				for (var nodeNum = 0; nodeNum < route.InferredFlow.length; nodeNum++) {
					var node = route.InferredFlow[nodeNum];
					try {
						hscCheckRequiredFields(goodInferredFlow, node);
					}
					catch (err) {
						throw err + " in Response InferredFlow element " + nodeNum;
					}

					var temp = {};

					temp["Position"] = node.Position;
					temp["PlotPosition"] = node.Position + totalPreviousRoutePositions;
					temp["MinMeas"] = node.MinMeas;
					temp["MaxMeas"] = node.MaxMeas;
					temp["MeasOk"] = isFinite(node.MinMeas) && isFinite(node.MaxMeas);
					node["MeasOk"] = temp["MeasOk"];

					temp["MinCalc"] = node.MinCalc;
					temp["MaxCalc"] = node.MaxCalc;
					temp["CalcOk"] = isFinite(node.MinCalc) && isFinite(node.MaxCalc);
					node["CalcOk"] = temp["CalcOk"];

					temp["SegmentShortName"] = node.SegmentShortName;
					tickLabels.push(node.SegmentShortName);
					temp["SegmentName"] = node.SegmentName;

					if ((node.MaxCalc < node.MinMeas) || (node.MinCalc > node.MaxMeas))
						temp["ClassNum"] = 3;	// A leak;
					else if ((node.MaxCalc >= node.MinMeas) && (node.MaxCalc <= node.MaxMeas) && (node.MinCalc <= node.MaxMeas) && (node.MinCalc >= node.MinMeas))
						temp["ClassNum"] = 1;	// Definitely not a leak;
					else
						temp["ClassNum"] = 2;	// Maybe a leak;
					node["ClassNum"] = temp["ClassNum"];

					if (node.Position >= maxRoutePosition)
						maxRoutePosition = node.Position;
					else
						LogErrorMessage(src, "Detected nodes out of position order, routeNum = " + routeNum + ", nodeNum = " + nodeNum + ", node.Position = " + node.Position + ", maxRoutePosition = " + maxRoutePosition);

					data.push(temp);
				}
			}

			datatemp.push(data);
			arrayData = datatemp;

			return true;
		}
		catch (err) {
			LogErrorMessage(src, "Error interpreting response - " + err);
			responseData = null;
			return false;
		}
	}

	function DrawChart () {

		if (responseData == null) {
			LogErrorMessage(src, "No response data available for DrawChart");
			return false;
		}
		if (arrayData == null) {
			LogErrorMessage(src, "No array data available for DrawChart");
			return false;
		}
		var derivedLabel = "Derived";
		var measuredLabel = "Measured";
		var margin = { top: 0, right: Math.max(measuredLabel.visualWidth(), derivedLabel.visualWidth()) + 10, bottom: 100, left: 80 }

		var flowString = "Flow";
		var flowTitle = (responseData.FlowUnits == "") ? flowString : flowString + " (" + responseData.FlowUnits + ")";

		var newchart = multiDualBoxChart()
			.x(function (d) { return d.PlotPosition; })
			.y(function (d) { return d.MinMeas; })		// These two...
			.y0(function (d) { return d.MaxMeas; })		// ... are on the TOP.
			.y1(function (d) { return d.MinCalc; })		// These two...
			.y2(function (d) { return d.MaxCalc; })		// ... are on the BOTTOM.
			.y3(function (d) { return d.ClassNum; })	// This changes the color of the TOP boxes between baseClass1, baseClass2 and baseClass3.
														// The bottom boxes are at baseClass0.
			.width(WidthFn(margin))
			.height(HeightFn(margin))
			.xAxisTitle("Segment")
			.yAxisTitle(flowTitle)
			.xAxisTickLabels(tickLabels)
			.baseClass("PipelineInferredFlowProfileBox")
			.topBoxLabel(measuredLabel)
			.botBoxLabel(derivedLabel)
			.baseLabelClass("PipelineInferredFlowProfileValue")
			.cursor("pointer")
			.margin(margin)
			.breaks(breaks)

		d3.select(ChartDivId)
			.datum(arrayData)
			.call(newchart);

		Chart = newchart;

		newchart
			.dispatch.on('showTooltip', function (e) {
				try{
					if ((responseData != null) && (arrayData != null)) {
						var formatter = d3.format(".01f");

						// Range check the route and node returned from the chart before using
						if (e.j < 0 || e.j >= responseData.RouteInferredFlowProfile.length)
							throw "Route " + e.j + " out of valid range";
						if (e.i < 0 || e.i >= responseData.RouteInferredFlowProfile[e.j].InferredFlow.length)
							throw "Node " + e.i + " out of valid range for Route " + e.j;

						var segmentData = responseData.RouteInferredFlowProfile[e.j].InferredFlow[Math.floor(e.i)];

						var SegmentString = "Segment";
						var FlowsatString = "Flows at";
						var MeasuredString = "Measured";
						var CalculatedString = "Derived";
						var ErrorString = "Error";

						var content =
							"<table align='center'>" +
							"<tr><th align=center>" + SegmentString + " " + segmentData.SegmentShortName + " " + FlowsatString + " " + hscOpAppsNice(responseData.Time) + "</th></tr>";

						if (segmentData.MeasOk)
							content +=
								"<tr><td align=center>" + MeasuredString + ": <B><span class=PipelineInferredFlowProfileValue" + segmentData.ClassNum + ">" + formatter(segmentData.MinMeas) + " - " + formatter(segmentData.MaxMeas) + "</span></B> " + responseData.FlowUnits + "</td></tr>";
						else
							content +=
								"<tr><td align=center>" + MeasuredString + ": <B>" + ErrorString + "</B></td></tr>";

						if (segmentData.CalcOk)
							content +=
									"<tr><td align=center>" + CalculatedString + ": <B><span class=PipelineInferredFlowProfileValue0>" + formatter(segmentData.MinCalc) + " - " + formatter(segmentData.MaxCalc) + "</span></B> " + responseData.FlowUnits + "</td></tr>";
						else
							content +=
								"<tr><td align=center>" + CalculatedString + ": <B>" + ErrorString + "</B></td></tr>";

						content += "</table>";

						hscOpAppsTooltip.show([parseInt(e.x) + $(ChartDivId).offset().left, parseInt(e.y) + +$(ChartDivId).offset().top], content);
					}
				}
				catch (err) {
					LogErrorMessage(src, "showTooltip error " + err);
					return false;
				}
			});

		newchart
			.dispatch.on('hideTooltip', function (e) {
				d3.select("#texttooltip")
					.transition()
					.style("opacity", 0)
					.attr("transform", "translate(5, -5)")
					.remove();
				hscOpAppsTooltip.cleanup();
			});

		newchart
			.dispatch.on('click', function (e) {
				try{
					if ((responseData != null) && (arrayData != null)) {
						// Range check the route and node returned from the chart before using
						if (e.j < 0 || e.j >= responseData.RouteInferredFlowProfile.length)
							throw "Route " + e.j + " out of valid range";
						if (e.i < 0 || e.i >= responseData.RouteInferredFlowProfile[e.j].InferredFlow.length)
							throw "Node " + e.i + " out of valid range for Route " + e.j;

						var segmentData = responseData.RouteInferredFlowProfile[e.j].InferredFlow[Math.floor(e.i)];
						var URL = "sysEquipment?hscpoint=" + (hscGlobalPipelinePoint === hscDummyPointName ? hscDummyPointName : segmentData.SegmentName) + "&view=" + _VIEWNAME_SegmentInferredFlow;

						LogMessage(L_CALLUP_HI, src, "Changing window.external.Application.CurrentPage to " + URL);
						window.external.Application.CurrentPage(URL);
					}
				}
				catch (err) {
					LogErrorMessage(src, "click error " + err);
					return false;
				}
			});

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
	}

	return {
		// Public methods
		OnReady: function (chartDiv, widthFn, heightFn, timeControl, navigationControl) {
			return OnReady(chartDiv, widthFn, heightFn, timeControl, navigationControl);
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
			return "Pipeline Flow Snapshot - Measured vs. Pressure-derived Flow";
		},
		Type: function () {
			return RequestType;
		}
	}
}();

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/