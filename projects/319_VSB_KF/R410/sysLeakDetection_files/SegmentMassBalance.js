// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// SegmentMassBalance.js

hscOpAppsSegmentMassBalance = function () {
	var src = "SegmentMassBalance.js";

	var RequestType = _DISPLAYTYPE_SegmentMassBalance;

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
				"MassBalance": {},
				"FlowUnits": "empty"
			}
			try {
				hscCheckRequiredFields(goodResponseData, responseData);
			}
			catch (err) {
				throw err + " in ResponseData element";
			}

			if (responseData.MassBalance.length <= 0)
				throw "Response MassBalance node is empty";

			var datatemp = [];
			var data = [];

			var goodMassBalance = {
				"Time": "",
				"FlowIn": 0,
				"FlowOut": 0,
				"FlowStored": 0,
				"ErrorNeg": 0,
				"ErrorPos": 0,
				"Leak": 0
			}

			// For each ResponseData element, check it has the required fields and 
			// unpack the data into arrayData for the chart
			LogMessage(L_PARAM, src, "MassBalance array has " + responseData.MassBalance.length + " elements.");
			for (var i = 0; i < responseData.MassBalance.length; i++) {
				var node = responseData.MassBalance[i];
				try {
					hscCheckRequiredFields(goodMassBalance, node);
				}
				catch (err) {
					throw err + " in Response MassBalance element " + i;
				}

				var temp = {};

				temp["Flow"] = node.Leak;
				temp["FlowOk"] = isFinite(temp["Flow"]);

				temp["Leak"] = node.Leak;
				temp["LeakOk"] = isFinite(temp["Leak"]);
                
				temp["ErrorNeg"] = node.ErrorNeg;
				temp["ErrorPos"] = node.ErrorPos;
				temp["ErrorOk"] = isFinite(node.ErrorNeg) && isFinite(node.ErrorPos);
                
				responseData.MassBalance[i].Time = temp["Time"] = hscOpAppsISO.parse(node.Time);

				if ( ((temp["Flow"] > 0) && (temp["Flow"] > node.ErrorPos)) || 
					 ((temp["Flow"] < 0) && (temp["Flow"] < node.ErrorNeg)) )
					temp["ClassNum"] = 3;
				else
					temp["ClassNum"] = 1;

				data.push(temp);
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

		var topBoxLabel = "Leak";
		var botBoxLabel = "Uncertainty";
		var margin = { top: 0, right: Math.max(topBoxLabel.visualWidth(), botBoxLabel.visualWidth()) + 10, bottom: 50, left: 80 }

		var flowString = "Flow";
		var flowTitle = (responseData.FlowUnits == "") ? flowString : flowString + " (" + responseData.FlowUnits + ")";

		var newchart = multiDualBoxChart()
			.x(function (d) { return d.Time; })
			.y(function (d) { return d.Flow; })
			.y0(function (d) { return +d.Flow; })
			.y1(function (d) { return +d.ErrorNeg; })
			.y2(function (d) { return +d.ErrorPos; })
			.y3(function (d) { return d.ClassNum; })
			.width(WidthFn(margin))
			.height(HeightFn(margin))
			.xAxisTitle("Time") 
			.yAxisTitle(flowTitle)
			.baseClass("SegmentMassBalanceBox")
			.topBoxLabel(topBoxLabel)
			.botBoxLabel(botBoxLabel)
			.baseLabelClass("SegmentMassBalanceValue")
			.margin(margin)
			.botYAxisNegFudgeFactor(1.5)
			.botYAxisPosFudgeFactor(5.0)

		d3.select(ChartDivId)
			.datum(arrayData)
			.call(newchart);

		Chart = newchart;

		newchart
			.dispatch.on('showTooltip', function (e) {
				try{
					if ((responseData != null) && (arrayData != null)) {
						var formatter = d3.format(".01f");

						var timeData = responseData.MassBalance[e.i];
						var arrayDataItem = arrayData[0][e.i];
						var content;

						var LeakDataAtString = "Leak Data at";
						var CalculatedLeakString = "Calculated leak";
						var UncertaintyRangeString = "Uncertainty range";
						var ErrorString = "Error";

						var content =
							"<table align='center'>" +
							"<tr><th align=center>" + LeakDataAtString + " " + hscOpAppsNice(timeData.Time) + "</th></tr>";

						if (arrayDataItem.FlowOk)
							content +=
								"<tr><td align=center>" + CalculatedLeakString + ": <B><span class=SegmentMassBalanceValue" + arrayDataItem.ClassNum + ">" + formatter(arrayDataItem.Flow) + "</span></B> " + responseData.FlowUnits + "</td></tr>";
						else
							content +=
								"<tr><td align=center>" + CalculatedLeakString + ": <B>" + ErrorString + "</B></td></tr>";

						if (!arrayDataItem.ErrorOk)
							content += "<tr><td align=center>" + UncertaintyRangeString + ": <B>" + ErrorString + "</B></td></tr>";
						else if (Math.abs(arrayDataItem.ErrorNeg) == Math.abs(arrayDataItem.ErrorPos))
							content +=
								"<tr><td align=center>" + UncertaintyRangeString + ": +/- <B><span class=SegmentMassBalanceValue0>" + formatter(Math.abs(arrayDataItem.ErrorPos)) + "</span></B> " + responseData.FlowUnits + "</td></tr>";
						else
							content +=
								"<tr><td align=center>" + UncertaintyRangeString + ": <B><span class=SegmentMassBalanceValue0>" + formatter(arrayDataItem.ErrorNeg) + " - " + formatter(arrayDataItem.ErrorPos) + "</span></B> " + responseData.FlowUnits + "</td></tr>";

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
			return "Corrected Volume Balance";
		},
		Type: function () {
			return RequestType;
		}
	}
}();

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/