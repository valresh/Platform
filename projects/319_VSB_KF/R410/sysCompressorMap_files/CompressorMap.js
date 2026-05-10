// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// CompressorMap.js

hscOpAppsCompressorMap = function () {
	var src = "CompressorMap.js";

	var RequestType = _DISPLAYTYPE_CompressorMap;

	var CompressorLineClass = "compressorLine";
	var CompressorScatterClass = "compressorScatter";
	var FinalCompressorScatterClass = "compressorFinalScatter";

	var Chart;
	var ChartDivId;
	var WidthFn, HeightFn;
	var TimeControl;

	var responseData;
	var arrayData;
	var _dataContainsError = false;

	var _firstSampleRequest = true;
	var _compressorMapConfig = "";
	var _compressorMapX = "";
	var _compressorMapY = "";
	var _mapDefinition = null;

	var _blankSVG = '<svg xmlns="http://www.w3.org/2000/svg" xml:space="preserve" color-interpolation-filters="sRGB"><g><g></g></g></svg>';

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

		_compressorMapConfig = GetURLParam("compressormapconfig");
		if (_compressorMapConfig == "")
			throw "URL parameter 'compressormapconfig' not supplied.";

		_compressorMapX = GetURLParam("compressormapx");
		if (_compressorMapX == "")
			throw "URL parameter 'compressormapx' not supplied.";

		_compressorMapY = GetURLParam("compressormapy");
		if (_compressorMapY == "")
			throw "URL parameter 'compressormapy' not supplied.";

		ChartDivId = chartDiv;
		WidthFn = widthFn;
		HeightFn = heightFn;
		TimeControl = timeControl;

		$(window).resize(function () {
			if (Chart == null)
				return;

			var margin = Chart.margin();

			Chart
				.x(function (d) { return d.X; })
				.y(function (d) { return d.Y; })
				.width(WidthFn(margin))
				.height(HeightFn(margin));

			d3.select(ChartDivId + ' svg')
				.attr('width', WidthFn(margin))
				.attr('height', HeightFn(margin))
				.datum(arrayData)
				.call(Chart);
		});
	}

	function FormInitialRequest() {

		var requestData = {
			"CompressorMapPoint": hscGlobalCompressorPoint,
			"CompressorMapConfig": _compressorMapConfig,
			"CompressorMapX": _compressorMapX,
			"CompressorMapY": _compressorMapY
		}

		var requestType = _RESPONSETYPE_CompressorMapDefinition;

		var request = {
			"RequestType": requestType,
			"RequestData": JSON.stringify(requestData)
		}

		return JSON.stringify(request);
	}

	function FormRequest() {
		var requestData = {
			"CompressorMapPoint": hscGlobalCompressorPoint,
			"CompressorMapConfig": _compressorMapConfig,
			"CompressorMapX": _compressorMapX,
			"CompressorMapY": _compressorMapY,
			"StartTime": hscOpAppsISO(TimeControl.startTime()),
			"EndTime": hscOpAppsISO(TimeControl.endTime()),
			"Interval": TimeControl.interval()
		}

		var requestType = _RESPONSETYPE_CompressorMapHistory;

		var request = {
			"RequestType": requestType,
			"RequestData": JSON.stringify(requestData)
		}

		return JSON.stringify(request);
	}

	function ParseResponse(response) {
		try {
			// This code assumes response contains ResponseType and valid JSON ResponseData
			// Check that the ResponseType is the correct type(s)
			responseData = JSON.parse(response.ResponseData);

			// Check that the ResponseData has the required fields
			var goodResponseData;
			if (response.ResponseType == _RESPONSETYPE_CompressorMapDefinition)
				goodResponseData = {
					"MapDefinition": {}
				}
			else if (response.ResponseType == _RESPONSETYPE_CompressorMapHistory)
				goodResponseData = {
					"OperatingPoint": {}
				}
			else {
				throw "Unknown ResponseType of " + response.ResponseType;
			}

			if ((response.ResponseType == _RESPONSETYPE_CompressorMapDefinition) && (responseData.MapDefinition == null))
				throw "Response MapDefinition node does not exist";
			else if ((response.ResponseType == _RESPONSETYPE_CompressorMapHistory) && (responseData.OperatingPoint.length <= 0))
				throw "Response OperatingPoint node is empty";

			if (response.ResponseType == _RESPONSETYPE_CompressorMapDefinition) {
				var goodMapDefinition = {
					"MapTitle": "",
					"SVG": "empty",
					"XAxisEngUnit": "",
					"XAxisMaxValue": 0,
					"XAxisMinValue": 0,
					"XAxisLabel": "",
					"YAxisEngUnit": "",
					"YAxisMaxValue": 0,
					"YAxisMinValue": 0,
					"YAxisLabel": ""
				}

				// For each ResponseData element, check it has the required fields
				var node = responseData.MapDefinition;
				try {
					hscCheckRequiredFields(goodMapDefinition, node);
				}
				catch (err) {
					throw err + " in Response MapDefinition element";
				}
				_mapDefinition = responseData.MapDefinition;

				if (_mapDefinition.SVG == "") {
					LogErrorMessage(src, "No SVG in MapDefinition - Data will be plotted without an SVG background.");
				}
			}
			else {
				var datatemp = [];
				var data = [];

				var goodOperatingPoint = {
					"X": 0,
					"Y": 0,
					"Time": "iso"
				}

				arrayData = null;
				LogMessage(L_PARAM, src, "OperatingPoint array has " + responseData.OperatingPoint.length + " elements.");

				_dataContainsError = false;
				for (var i = 0; i < responseData.OperatingPoint.length; i++) {
					var node = responseData.OperatingPoint[i];
					try {
						hscCheckRequiredFields(goodOperatingPoint, node);
					}
					catch (err) {
						throw err + " in Response OperatingPoint element " + i;
					}

					if (isFinite(node.X) && isFinite(node.Y)) {
						var temp = {
							"X": node.X,
							"Y": node.Y,
							"Time": hscOpAppsISO.parse(node.Time)
						}
						responseData.OperatingPoint[i].Time = hscOpAppsISO.parse(node.Time);
						data.push(temp);
					}
					else
						_dataContainsError = true;
				}
				datatemp.push(data);
				arrayData = datatemp;
			}
			return true;
		}
		catch (err) {
			LogErrorMessage(src, "Error interpreting response - " + err);
			responseData = null;
			return false;
		}
	}

	function DrawChart() {

		if (responseData == null) {
			LogErrorMessage(src, "No response data available for DrawChart");
			return false;
		}
		if (arrayData == null) {
			LogErrorMessage(src, "No array data available for DrawChart");
			return false;
		}

		if (Chart) {
			hscOpAppsTooltip.cleanup();
		}

		var margin = { top: 0, right: 20, bottom: 30, left: 60 };

		var newchart = svgWithLineChart()
			.x(function (d) { return d.X; })
			.y(function (d) { return d.Y; })
			.width(WidthFn(margin))
			.height(HeightFn(margin))
			.xAxisTitle(_mapDefinition.XAxisLabel + " (" + _mapDefinition.XAxisEngUnit + ")")
			.yAxisTitle(_mapDefinition.YAxisLabel + " (" + _mapDefinition.YAxisEngUnit + ")")
			.lineClass(CompressorLineClass)
			.scatterClass(CompressorScatterClass)
			.finalScatterClass(FinalCompressorScatterClass)
			.margin(margin)
			.svgMinX(_mapDefinition.XAxisMinValue)
			.svgMaxX(_mapDefinition.XAxisMaxValue)
			.svgMinY(_mapDefinition.YAxisMinValue)
			.svgMaxY(_mapDefinition.YAxisMaxValue)
			.svg((_mapDefinition.SVG == "") ? _blankSVG : _mapDefinition.SVG)
			.drawLine(!_dataContainsError)
			.chartDivId(ChartDivId);

		d3.select(ChartDivId)
			.datum(arrayData)
			.call(newchart);

		Chart = newchart;

		newchart
			.dispatch.on('showScatterTooltip', function (e) {
				try {
					if (responseData != null) {
						var formatter = d3.format(".01f");

						var arrayDataItem = arrayData[0][e.i];
						var content;

						var OperatingPointAtString = "Operating Point at";
						var AtString = "at";
						var ErrorString = "Error";

						content =
							"<table align='center'>" +
							"<tr><th colspan=2>" + OperatingPointAtString + " " + hscOpAppsNice(arrayDataItem.Time) + "</th></tr>";
					
						content +=
							"<tr><td align=right><B>" + formatter(arrayDataItem.Y) + "</B> " + _mapDefinition.YAxisEngUnit + "</td><td> " + AtString + " <B>" + formatter(arrayDataItem.X) + "</B> " + _mapDefinition.XAxisEngUnit + "</td></tr>";

						if (_dataContainsError)
							content +="<tr><td colspan=2 align=center><B>Dataset contained invalid data.</B></td></tr>";

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
			.dispatch.on('hideScatterTooltip', function (e) {
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
		FormInitialRequest: function () {
			return FormInitialRequest();
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
			var title = "Compressor Map";
			if (_mapDefinition == null)
				return title;
			else
				return title + " - " + _mapDefinition.MapTitle;
		},
		Type: function () {
			return RequestType;
		}
	}
}();

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/