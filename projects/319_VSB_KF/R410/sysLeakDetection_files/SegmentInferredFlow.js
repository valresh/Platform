// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// SegmentInferredFlow.js

hscOpAppsSegmentInferredFlow = function () {
	var src = "SegmentInferredFlow.js";

	var RequestType = _DISPLAYTYPE_SegmentInferredFlow;

	var Chart;
	var ChartDivId;
	var WidthFn, HeightFn;
	var TimeControl;

	var responseData;
	var arrayData;
	var tickLabels;

	var _firstSampleRequest = true;

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
		catch (err) {
			var errorString = "Unable to make " + RequestType + " chart type ready.<BR>" + err;
			LogErrorMessage(src, errorString.replace("<BR>", " "));
			throw errorString;
		}

		ChartDivId = chartDiv;
		WidthFn = widthFn;
		HeightFn = heightFn;
		TimeControl = timeControl;

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
			"SegmentPoint": hscGlobalSegmentPoint,
			"StartTime": hscOpAppsISO(TimeControl.startTime()),
			"EndTime": hscOpAppsISO(TimeControl.endTime()),
			"Interval": TimeControl.interval()
		}

		var requestType = RequestType;
		if (hscGlobalSegmentPoint === hscDummyPointName) {
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

		try {
			// This code assumes response contains ResponseType and valid JSON ResponseData
			responseData = JSON.parse(response.ResponseData);

			// Check that the ResponseData has the required fields
			var goodResponseData = {
				"InferredFlow": {},
				"FlowUnits": "empty"
			}
			try {
				hscCheckRequiredFields(goodResponseData, responseData);
			}
			catch (err) {
				throw err + " in ResponseData element";
			}

			if (responseData.InferredFlow.length <= 0)
				throw "Response InferredFlow node is empty";

			var datatemp = [];
			var data = [];

			var goodInferredFlow = {
				"Time": "iso",
				"MinMeas": 0,
				"MaxMeas": 0,
				"MinCalc": 0,
				"MaxCalc": 0
			}

			// For each ResponseData element, check it has the required fields and 
			// unpack the data into arrayData for the chart
			LogMessage(L_PARAM, src, "InferredFlow array has " + responseData.InferredFlow.length + " elements.");
			for (var i = 0; i < responseData.InferredFlow.length; i++) {
				var node = responseData.InferredFlow[i];
				try {
					hscCheckRequiredFields(goodInferredFlow, node);
				}
				catch (err) {
					throw err + " in Response InferredFlow element " + i;
				}

				var temp = {};

				temp["MinMeas"] = node.MinMeas;
				temp["MaxMeas"] = node.MaxMeas;
				temp["MeasOk"] = isFinite(node.MinMeas) && isFinite(node.MaxMeas);

				temp["MinCalc"] = node.MinCalc;
				temp["MaxCalc"] = node.MaxCalc;
				temp["CalcOk"] = isFinite(node.MinCalc) && isFinite(node.MaxCalc);

				responseData.InferredFlow[i].Time = temp["Time"] = hscOpAppsISO.parse(node.Time);

				if ((node.MaxCalc < node.MinMeas) || (node.MinCalc > node.MaxMeas))
					temp["ClassNum"] = 3;	// A leak;
				else if ((node.MaxCalc >= node.MinMeas) && (node.MaxCalc <= node.MaxMeas) && (node.MinCalc <= node.MaxMeas) && (node.MinCalc >= node.MinMeas))
					temp["ClassNum"] = 1;	// Definitely not a leak;
				else
					temp["ClassNum"] = 2;	// Maybe a leak;

				data.push(temp);
			}
			datatemp.push(data);

			arrayData = datatemp;

			return true;
		}
		catch (err) {
			LogErrorMessage(src, "Error interpreting response - '" + err + "'.");

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
		var margin = { top: 0, right: Math.max(measuredLabel.visualWidth(), derivedLabel.visualWidth()) + 10, bottom: 50, left: 80 }

		var flowString = "Flow";
		var flowTitle = (responseData.FlowUnits == "") ? flowString : flowString + " (" + responseData.FlowUnits + ")";

		var newchart = multiDualBoxChart()
			.x(function (d) { return d.Time; })
			.y(function (d) { return d.MinMeas; })		// These two...
			.y0(function (d) { return d.MaxMeas; })		// ... are on the TOP.
			.y1(function (d) { return d.MinCalc; })		// These two...
			.y2(function (d) { return d.MaxCalc; })		// ... are on the BOTTOM.
			.y3(function (d) { return d.ClassNum; })	// This changes the color of the TOP boxes between baseClass1, baseClass2 and baseClass3.  
														// The bottom boxes are at baseClass0.
			.width(WidthFn(margin))
			.height(HeightFn(margin))
			.xAxisTitle("Time")
			.yAxisTitle(flowTitle)
			.xAxisTickLabels(tickLabels)
			.baseClass("SegmentInferredFlowBox")
			.topBoxLabel(measuredLabel)
			.botBoxLabel(derivedLabel)
			.baseLabelClass("SegmentInferredFlowValue")
			.margin(margin)

		d3.select(ChartDivId)
			.datum(arrayData)
			.call(newchart);

		Chart = newchart;

		newchart
			.dispatch.on('showTooltip', function (e) {
				try {
					if ((responseData != null) && (arrayData != null)) {
						var formatter = d3.format(".01f");

						var timeData = responseData.InferredFlow[e.i];
						var arrayDataItem = arrayData[0][e.i];
						var content;

						var FlowsatString = "Flows at";
						var MeasuredString = "Measured";
						var CalculatedString = "Derived";
						var ErrorString = "Error";

						var content =
							"<table align='center'>" +
							"<tr><th align=center>" + FlowsatString + " " + hscOpAppsNice(timeData.Time) + "</th></tr>";

						if (arrayDataItem.MeasOk)
							content +=
								"<tr><td align=center>" + MeasuredString + ": <B><span class=SegmentInferredFlowValue" + arrayDataItem.ClassNum + ">" + formatter(timeData.MinMeas) + " - " + formatter(timeData.MaxMeas) + "</span></B> " + responseData.FlowUnits + "</td></tr>";
						else
							content +=
								"<tr><td align=center>" + MeasuredString + ": <B>" + ErrorString + "</B></td></tr>";

						if (arrayDataItem.CalcOk)
							content +=
								"<tr><td align=center>" + CalculatedString + ": <B><span class=PipelineInferredFlowProfileValue0>" + formatter(timeData.MinCalc) + " - " + formatter(timeData.MaxCalc) + "</span></B> " + responseData.FlowUnits + "</td></tr>";
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
		TimeControl.ShowPeriodInterval();
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
			return "Segment Flow Trend - Measured vs. Pressure-derived Flow";
		},
		Type: function () {
			return RequestType;
		}
	}
}();

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/