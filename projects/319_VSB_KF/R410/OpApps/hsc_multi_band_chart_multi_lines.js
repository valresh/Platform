// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_multi_band_chart_multiple_lines.js
var src = "hsc_multi_band_chart_multiple_lines.js";
function multiBandChartMultipleLines(dataToggle) 
{
		var tickSize = 6;
		var maxXAxisTickLabelSize = 35,
			xScale = d3.scale.linear(),
			xAxis = d3.svg.axis().scale(xScale).orient("bottom").tickSize(tickSize, 0),
			xAxesVisible = [];

		var yScalePressure = d3.scale.linear(),
			yScaleTemperature = d3.scale.linear();

		var yAxisPressure = d3.svg.axis().scale(yScalePressure).orient("left").tickSize(tickSize, 0),
			yAxisTemperature = d3.svg.axis().scale(yScaleTemperature).orient("right").tickSize(tickSize, 0);
		
		var yScales = {
			"Pressure": yScalePressure,
			"Temperature": yScaleTemperature
		};
		var yAxesTotal = {
			"Pressure": yAxisPressure,
			"Temperature": yAxisTemperature
		};

	function chart(selection) 
	{
		LogMessage(L_PARAM, src, "In multi band charts multiple lines function");
		selection.each(function (data) 
		{

			var pressureData = data[0],
				temperatureData = data[1],
				pressureSegmentData = data[2],
				temperatureSegmentData = data[3];
			/*here the d is first element of the array, d = array[BIGNUMBER]
			xValue, yValue etc are functions which return Pressures, Temperatures etc: function(d) {return d.PlotPosition}
			Converts each array entry from object to array. Each element of the below arrays are are arrays of elements (PlotPosition, Press)*/
			pressureData = pressureData.map(function (d, i) 
			{
				return d.map(function (sd, j)	
				{
					return [
						xPlotPosition.call(pressureData[i], sd, j), 
						yPressure.call(pressureData[i], sd, j), 
						yMinOpPressure.call(pressureData[i], sd, j), 
						yMaxOpPressure.call(pressureData[i], sd, j), 
						yBaselinePressure.call(pressureData[i], sd, j)];
				});
			});
			temperatureData = temperatureData.map(function (d, i)
			{
				return d.map(function (sd, j)	
				{
					return [
						xPlotPosition.call(temperatureData[i], sd, j), 
						yTemperature.call(temperatureData[i], sd, j)];
				});
			});
			temperatureSegmentData = temperatureSegmentData.map(function (d, i)
			{
				return d.map(function (sd, j)	
				{
					return [
						xPlotPosition.call(temperatureSegmentData[i], sd, j), 
						xPosition.call(temperatureData[i], sd, j), 
						yTemperature.call(temperatureSegmentData[i], sd, j), 
						ySegmentsInNode.call(temperatureSegmentData[i], sd,j)];
				});
			});
			pressureSegmentData = pressureSegmentData.map(function (d, i) 
			{
				return d.map(function (sd, j)	
				{
					return [
						xPlotPosition.call(pressureSegmentData[i], sd, j),
						xPosition.call(temperatureData[i], sd, j), 
						yPressure.call(pressureSegmentData[i], sd, j),
						ySegmentsInNode.call(temperatureSegmentData[i], sd,j)];
				});
			});
			LogMessage(L_PARAM, src, "Replaced data types from objects to arrays");
			var pressureNodeObject = {
				"yValuesPerX": yValuesPerXPressure,
				"yValuesPerXForBreaks": yValuesPerXPressure,
				"data": pressureData
			};
			var temperatureNodeObject = {
				"yValuesPerX": yValuesPerXTemperature,
				"yValuesPerXForBreaks": yValuesPerXTemperature+1,
				"data": temperatureData
			};
			
			var nodeCombinedData = {
				"Pressure": pressureNodeObject,
				"Temperature": temperatureNodeObject
			};
			
			var segmentCombinedData = {
				"Pressure": pressureSegmentData,
				"Temperature": temperatureSegmentData
			};

			var	dataVisiblePressure = [],
				dataVisibleTemperature = [],
				dataVisibleSegmentPressure = [],
				dataVisibleSegmentTemperature = [];
			
			LogMessage(L_PARAM, src, "Checking if breaks in the plot are valid");
/*			if (AreValidBreaks(breaks, validPrimaryKeys, nodeCombinedData) == null)
			{
				throw "Bad break specified";
			}
*/			
			//Put node data in a object data structure
			var nodeData = {};
			for(var i=0; i<validPrimaryKeys.length; i++)
			{
				var key = validPrimaryKeys[i];
				nodeData[key] = nodeCombinedData[key];
			}

			maxMinXValues = ExtractMaxMinXValues(nodeData, validPrimaryKeys);
			minX = maxMinXValues.minimum;
			maxX = maxMinXValues.maximum;
			xScale.domain([minX, maxX]).range([0, width - margin.left - margin.right]);
			var extremityYData = ExtractMaxMinYValues(validPrimaryKeys, nodeData, segmentCombinedData);
			SetYScales(validPrimaryKeys, yScales, extremityYData, height, margin);

			// Select the svg element, if it exists.
			var svg = d3.selectAll("svg");
			// Otherwise, create the skeletal chart.
			if (svg.empty()) 
			{
				svg = d3.select(this)
					.append("svg")
					.attr("width", width)
					.attr("height", height);
			}
			
			// making a deep copy of the nodes
			var rawNodeData = JSON.parse(JSON.stringify(nodeData));
			
			LogMessage(L_PARAM, src, "Extracting every yValuesPerX(th) point to smother values");
			for(var i=0; i< validPrimaryKeys.length; i++)
			{
				var labelsToShow = SmotherValuesOut(nodeData, validPrimaryKeys[i], xAxisTitle, xScale);
				SmotherProfileValuesOut(segmentCombinedData, validPrimaryKeys[i], rawNodeData, nodeData, xAxisTitle, xScale, breaks);
				
			}

			// Create visible x-scales
			var xAxesVisible = [],
				xScalesVisible = [];
			var xAxisVisibleLen,
				xScalesVisLen;
				
			var key = "Pressure";
			if(plotData[key].isEmpty == false)
			{
				LogMessage(L_PARAM, src, "Extracting " + key + " node data");
				var pressureNodeData = ExtractNodeDataToShow(breaks, pressureData, yValuesPerXPressure, xScale, tickSize, xAxisTickLabels);
				xScalesVisible = pressureNodeData.xScalesVisible;
				xAxesVisible = pressureNodeData.xAxesVisible;
				dataVisiblePressure = pressureNodeData.dataVisible;
				
				var dataVisibleSegmentPressure = ExtractSegmentDataToShow(nodesData, pressureSegmentData).dataVisibleSegment;
			}
			
			var key = "Temperature";
			if(plotData[key].isEmpty == false)
			{
				LogMessage(L_PARAM, src, "Extracting " + key + " node data");
				var temperatureNodeData = ExtractNodeDataToShow(breaks, temperatureData, yValuesPerXTemperature+1, xScale, tickSize, xAxisTickLabels);
				dataVisibleTemperature = temperatureNodeData.dataVisible;
				var dataVisibleSegmentTemperature = ExtractSegmentDataToShow(nodesData, temperatureSegmentData).dataVisibleSegment;
				xScalesVisible = temperatureNodeData.xScalesVisible;
				xAxesVisible = temperatureNodeData.xAxesVisible;
			}

			// Remove all of the traces before re-plotting them
			svg.selectAll("g.series").remove();
			PlotTheData(svg, xScalesVisible, plotData, dataVisiblePressure, dataVisibleSegmentPressure,
					dataVisibleTemperature, dataVisibleSegmentTemperature, xScale, yScales);

			// This transforms the data points such that they are correctly positioned between x and y axis.
			TransformDataToAxes(svg, plotData, validPrimaryKeys);	
		
			// Remove and recreate x-axes and y axes each time so that we can change tick text properly
			svg.select("g.x.axisset").remove();
			for(var i=0; i<validPrimaryKeys.length; i++)
			{
				var key = validPrimaryKeys[i];
				svg.select("g.y.axisset"+plotData[key].name).remove();
			}
			AppendXAxes(svg, margin, validPrimaryKeys, yScales, xScalesVisible, xAxesVisible, xAxisTitle);
			var yAxisPlacements= ObtainYAxesPlacements(width, margin, plotData);
			LogMessage(L_PARAM, src, "Appending Y axes to the charts");
			for(var i=0; i< validPrimaryKeys.length; i++)
			{
				var key = validPrimaryKeys[i];
				AppendYAxis(svg, margin, key, yAxisTitles[key], yAxisPlacements[key].distanceAxis, yAxisPlacements[key].namePlacement);
				svg.select("g.y.axis"+key)
					 .call(yAxesTotal[key]);
			}
			
			AppendXAxisTitleAndTickLabels(margin, width, xAxisTitle,xAxisTickLabels, tickSize, labelsToShow);
			var legendTotalClass = "legendTotal";
			SetupReferenceLine(svg, legendTotalClass, nodeCombinedData, segmentCombinedData, margin, width, height, xScale, yScales, plotData);
			AppendLegend(svg, legendTotalClass, legendCoord, nodesData, plotData, dataToggle);
			LogMessage(L_PARAM, src, "End of multi band multi line function");
		});
	}

	function X(d) {
		return xScale(d[0]);
	}

	function Y(d) {
		return yScale(d[1]);
	}

	function Y0(d) {
		return yScale(d[2]);
	}

	function Y1(d) {
		return yScale(d[3]);
	}

	function Y2(d) {
		return yScale(d[4]);
	}

	chart.margin = function (_) {
		if (!arguments.length) return margin;
		margin = _;
		return chart;
	};
	chart.width = function (_) {
		if (!arguments.length) return width;
		width = _;
		return chart;
	};
	chart.height = function (_) {
		if (!arguments.length) return height;
		height = _;
		return chart;
	};
	chart.xPlotPosition = function (_) {
		if (!arguments.length) return xPlotPosition;
		xPlotPosition = _;
		return chart;
	};
	chart.xPosition = function (_) {
		if (!arguments.length) return xPosition;
		xPosition = _;
		return chart;
	};
	chart.yPressure = function (_) {
		if (!arguments.length) return yPressure;
		yPressure = _;
		return chart;
	};
	chart.yMinOpPressure = function (_) {
		if (!arguments.length) return yMinOpPressure;
		yMinOpPressure = _;
		return chart;
	};
	chart.yMaxOpPressure = function (_) {
		if (!arguments.length) return yMaxOpPressure;
		yMaxOpPressure = _;
		return chart;
	};
	chart.yBaselinePressure = function (_) {
		if (!arguments.length) return yBaselinePressure;
		yBaselinePressure = _;
		return chart;
	};

	chart.yScale = function (_) {
		if (!arguments.length) return yScale;
		yScale = _;
		return chart;
	};
	chart.xAxisTitle = function (_) {
		if (!arguments.length) return xAxisTitle;
		xAxisTitle = _;
		return chart;
	};
	chart.yAxisTitles = function (_) {
		if (!arguments.length) return yAxisTitles;
		yAxisTitles = _;
		return chart;
	};
	chart.xAxisTickLabels = function (_) {
		if (!arguments.length) return xAxisTickLabels;
		xAxisTickLabels = _;
		return chart;
	};
	chart.yValuesPerXTemperature = function (_) {
		if (!arguments.length) return yValuesPerXTemperature;
		yValuesPerXTemperature = _;
		return chart;
	};
	chart.yValuesPerXPressure = function (_) {
		if (!arguments.length) return yValuesPerXPressure;
		yValuesPerXPressure = _;
		return chart;
	};
	chart.ClearChart = function () {
		var svg = d3.select("svg").remove();
		return;
	};
	chart.breaks = function (_) {
		if (!arguments.length) return breaks;
		breaks = _;
		return chart;
	};
	chart.nodesData = function(_){
		if(!arguments.length) return nodesData;
		nodesData = _;
		return chart;
	}
	chart.yTemperature = function(_)
	{
		if(!arguments.length) return yTemperature;
		yTemperature = _;
		return chart;
	}
	chart.ySegmentsInNode = function(_)
	{
		if(!arguments.length) return ySegmentsInNode;
		ySegmentsInNode = _;
		return chart;
	}
	chart.legendCoord = function(_)
	{
		if(!arguments.length) return legendCoord;
		legendCoord = _;
		return chart;
	}
	chart.plotData = function(_)
	{
		if(!arguments.length) return plotData;
		plotData = _;
		return chart;
	}
	chart.validPrimaryKeys = function(_)
	{
		if(!arguments.length) return validPrimaryKeys;
		validPrimaryKeys = _;
		return chart;		
	}
	return chart;
}

function PlotTheData(svg, xScalesVisible, plotData, dataVisiblePressure, dataVisibleSegmentPressure,
					dataVisibleTemperature, dataVisibleSegmentTemperature, xScale, yScales)
{
	/*dataVisibleElemement for 4 routes is: [ Array[1], Array[1], Array[1], Array[1]]
	 When we associate dataVisibleTemperature[0] with a DOM element, we have a 1-1 correspondence
	 with g elements (it is an array of 1 element, that 1 element is associated with the g element), 
	 i.e.  only one <g> </g> where its __data__ property is dataVisibleTemperature[0]*/
	var strFn = "PlotTheData";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);

	var operatingRangeClass = "PipelineProfileOperatingRange";		
	var baselineSeriesClass = "PipelineProfileBaselineProfile";
	var pressureSeriesClass = "PipelineProfilePressureSeries"
	var pressureProfileClass = "PipelineProfilePressureProfile";
	var temperatureSeriesClass = "PipelineProfileTemperatureSeries";
	var temperatureProfileClass = "PipelineProfileTemperatureProfile";

	for (var axisNum = 0; axisNum < xScalesVisible.length; axisNum++) 
	{
		var key = "Pressure";
		if(plotData[key].isEmpty ==false)
		{
			var seriesPressure = svg.selectAll("g.series.g" + axisNum)
				.data(dataVisiblePressure[axisNum]);
			var seriesSegmentPressure = svg.selectAll("g.series.g" + axisNum)
				.data(dataVisibleSegmentPressure[axisNum]);

			var enterSeriesPressure = seriesPressure.enter()
				.append("g")
				.attr("class", "series"+plotData["Pressure"].name+ " trace" + axisNum );
			var enterSeriesSegmentPressure = seriesSegmentPressure.enter()
				.append("g")
				.attr("class", "seriesSegment"+plotData["Pressure"].name+ " trace" + axisNum );
				
			//areatrace
			var traceid = "areatrace0_" + axisNum;
			var areatrace0 = areaTrace()
				.y0(function (d) { return d[2]; })
				.y1(function (d) { return d[3]; })
				.xScale(xScale)
				.yScale(yScales[key]);

			enterSeriesPressure.append("g")			//appends a single g  to the existing g element there
				.attr("class", "group"+plotData["OperatingRange"].name+axisNum + " " + operatingRangeClass)
				.attr("id", traceid);


			seriesPressure.select("#" + traceid)		//we select the element that we appended just above
				.datum(function (d) { return d; })
				.call(areatrace0);

			// linetrace1
			traceid = "linetrace1_" + axisNum;
			var linetrace1 = lineTrace()
				.y(function (d) { return d[4]; })
				.xScale(xScale)
				.yScale(yScales[key]);
			

			enterSeriesPressure.append("g")
				.attr("class", " group"+plotData["BaselinePressure"].name+axisNum + " " + baselineSeriesClass)
				.attr("id", traceid);
				

			seriesPressure.select("#" + traceid)
				.datum(function (d) { return d; })
				.call(linetrace1);
				
			//linetrace0SegmentPressure, linetrace0Pressure is removed as we use segment data!
			traceid = "linetrace0Segment"+plotData["Pressure"].name+"_" + axisNum;
			var linetrace0SegmentPressure = lineTrace()
				.y(function (d) { return d[2]; })
				.xScale(xScale)
				.yScale(yScales[key])

			enterSeriesSegmentPressure.append("g")
				.attr("class", " group"+plotData["Pressure"].name+axisNum + " " + pressureSeriesClass)
				.attr("id", traceid);

			seriesSegmentPressure.select("#" + traceid)
				.datum(function (d) { return d; })
				.call(linetrace0SegmentPressure);
						
			// scattertrace1 - Baselines
			traceid = "scattertrace1_" + axisNum;
			var scattertrace1 = scatterTrace()
				.y(function (d) { return d[4]; })
				.j(axisNum)
				.symbol("circle")
				.xScale(xScale)
				.yScale(yScales[key]);


			enterSeriesPressure.append("g")
				.attr("class", "group"+plotData["BaselinePressure"].name+axisNum + " " + baselineSeriesClass)
				.attr("id", traceid);


			seriesPressure.select("#" + traceid)
				.datum(function (d) { return d; })
				.call(scattertrace1);

			// scattertrace0Pressure	- node Pressure
			traceid = "scattertrace0"+plotData["Pressure"].name+"_" + axisNum;
			var scattertrace0Pressure = scatterTrace()
				.y(function (d) { return d[1]; })
				.j(axisNum)
				.symbol("circle")
				.xScale(xScale)
				.yScale(yScales[key]);

				enterSeriesPressure.append("g")
				.attr("class","group"+plotData["Pressure"].name+axisNum + " " + pressureSeriesClass )	
				.attr("id", traceid)

			seriesPressure.select("#" + traceid)
				.datum(function (d) { return d; })
				.call(scattertrace0Pressure)

			seriesPressure
				.exit()
				.remove();
			
			seriesSegmentPressure
				.exit()
				.remove();
		}
		key = "Temperature";
		if(plotData[key].isEmpty ==false)
		{
			var seriesTemperature = svg.selectAll("g.series.g" + axisNum)
				.data(dataVisibleTemperature[axisNum]);
			var seriesSegmentTemperature = svg.selectAll("g.series.g" + axisNum)
				.data(dataVisibleSegmentTemperature[axisNum]);
			
			var enterSeriesTemperature = seriesTemperature.enter()
				.append("g")
				.attr("class", "series"+plotData["Temperature"].name+ " trace" + axisNum);
				
			var enterSeriesSegmentTemperature = seriesSegmentTemperature.enter()
				.append("g")
				.attr("class", "seriesSegment"+plotData["Temperature"].name+ " trace" + axisNum);


		//linetrace0SegmentTemperature, linetrace0Temperature	is removed as we use segment data!
			traceid = "linetrace0Segment"+plotData["Temperature"].name+"_" + axisNum;
			var linetrace0SegmentTemperature = lineTrace()
				.y(function (d) { return d[2]; })
				.xScale(xScale)
				.yScale(yScales[key]);


			enterSeriesSegmentTemperature.append("g")
				.attr("class", " group"+plotData["Temperature"].name+axisNum + " " + temperatureProfileClass)
				.attr("id", traceid)
				
			seriesSegmentTemperature.select("#" + traceid)
				.datum(function (d) { return d; })
				.call(linetrace0SegmentTemperature);

				// scattertrace0Temperature - node Temperature
			traceid = "scattertrace0"+plotData["Temperature"].name+"_" + axisNum;
			var scattertrace0Temperature = scatterTrace()
				.y(function (d) { return d[1]; })
				.j(axisNum)
				.symbol("circle")
				.xScale(xScale)
				.yScale(yScales[key]);

			
			enterSeriesTemperature.append("g")
				.attr("class", "group"+plotData["Temperature"].name+axisNum + " " + temperatureSeriesClass)		//Class for the scatter tool tip
				.attr("id", traceid)
			
			seriesTemperature.select("#" + traceid)
				.datum(function (d) { return d; })
				.call(scattertrace0Temperature);
		
			seriesTemperature
				.exit()
				.remove();
			seriesSegmentTemperature
				.exit()
				.remove();					
		}
	}
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
}
function AppendXAxisTitleAndTickLabels(margin, width, xAxisTitle,xAxisTickLabels, tickSize, labelsToShow)
{
	var strFn = "AppendXAxisTitleAndTickLabels";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var xAxisTitleY = margin.bottom - toString(xAxisTitle).visualHeight();
	// Position x-axis title
	d3.select("#xAxisTitle")
		.attr("x", (width / 2) - margin.left)
		.attr("y", xAxisTitleY)
		.attr('dy', '0.7em');
	
	for (var i = 0; i < xAxisTickLabels.length; i++) {
		$('line.tick')[i].nextSibling.textContent = ((i % labelsToShow) == 0) ? xAxisTickLabels[i].trimToPx(xAxisTitleY - tickSize - 2) : "";
		$($('line.tick')[i].nextSibling)
			.attr("transform", "rotate(-90)")
			.attr('text-anchor', 'end')
			.attr('y', '0')
			.attr('dy', '0.35em')
			.attr('x', -tickSize - 1);
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function ObtainYAxesPlacements(width, margin, plotData)
{
	var strFn = "ObtainYAxesPlacements";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	distanceYAxisToRightSide = width - margin.right;
	distanceYAxisToLeftSide = margin.left;
	yAxisNamePlacementLeft = 0;
	yAxisNamePlacementRight = margin.left+(margin.right)/2;
	
	var yAxisDistancesPressure = {
			"distanceAxis": 	distanceYAxisToLeftSide,
			"namePlacement": yAxisNamePlacementLeft
	};
	var yAxisDistancesTemperature = {
			"distanceAxis": 	distanceYAxisToRightSide,
			"namePlacement": yAxisNamePlacementRight
	};
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
			"Pressure": yAxisDistancesPressure,
			"Temperature": yAxisDistancesTemperature
	};
}
function AppendXAxes(svg, margin, validPrimaryKeys, yScales, xScalesVisible, xAxesVisible, xAxisTitle)
{
	var strFn = "AppendXAxes";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var xAxisSet = svg.append("g")
		.attr("transform", "translate(" + margin.left + "," + margin.top + ")")
			.attr("class", "x axisset");
	//Need a y axis scale so that we can position it accordingly. 
	var yScaleToUse = yScales[validPrimaryKeys[0]];
	for (axisNum = 0; axisNum < xScalesVisible.length; axisNum++) 
	{
		var thisAxis = xAxisSet.append("g")
			.attr("class", "x axis g" + axisNum)
			.attr("transform", "translate(0," + yScaleToUse.range()[0] + ")")
			.attr("y", (margin.bottom / 1.5))
			.call(xAxesVisible[axisNum]);

		if (axisNum == 0)
			thisAxis.append("text")
				.text(xAxisTitle)
				.attr("id", "xAxisTitle")
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}
function SetYScales(validPrimaryKeys, yScales, extremityYData, height, margin)
{
	var strFn = "SetYScales";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	for(var i=0; i< validPrimaryKeys.length; i++)
	{
		var key = validPrimaryKeys[i];
		var extremity = extremityYData[key];
		
		yScales[key].domain([extremity.minY, extremity.maxY])
			.range([height - margin.top - margin.bottom, 0])

	}	
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function SmotherValuesOut(nodeData, key, xAxisTitle, xScale)
{
	var strFn = "SmotherValuesOut";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var theData = [];
	for (var i = 0; i < nodeData[key].data[0].length; i += nodeData[key].yValuesPerXForBreaks)
		theData.push(nodeData[key].data[0][i][0]);
	
	// Attempt to use y-axis labels. .node() extracts the actual ELEMENT, returns null if it doesnt exist
	var minTickLabelSize = toString(xAxisTitle).visualHeight();

	// Space the x-values out and get the minimum gap back
	var minGap = SpaceValuesOut(theData, xScale, minTickLabelSize);
	// Calculate how many labels to show
	var labelsToShow = isFinite(minGap) ? Math.ceil(minTickLabelSize / minGap) : 1;
	
	/*
	This spreads the same X value across multiple data points (every 4 Y Points).
	EACH Y is essentially y,y0,y1,y2 (so Pressure, MinPressure etc). We've used data.push(inlet), data.push(outlet) etc.
	we are spreading this X value across those 4 Y points (inlet, outlet etc)
	*/
	for (var i = 0; i < theData.length; i++)
	{
		for (j = 0; j < nodeData[key].yValuesPerXForBreaks; j++)
		{
			nodeData[key].data[0][i * nodeData[key].yValuesPerXForBreaks + j][0] = theData[i];
		}
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return labelsToShow;
}
function SmotherProfileValuesOut(combinedData, key, rawNodeData, adjustedNodeData, xAxisTitle, xScale, breaks)
{	
	var strFn = "SmotherProfileValuesOut";
	
	if( key == "Pressure" || key == "Temperature")
	{
	
		// Combined data will appears 
		// {"Pressure": [[
		//    [xPlotPosition, xPosition, yPressure, segmentCount],
		//    [xPlotPosition, xPosition, yPressure, segmentCount]]],
		//  "Temperature": [[
		//    [xPlotPosition, xPosition, yPressure, segmentCount],
		//    [xPlotPosition, xPosition, yPressure, segmentCount]]]
		// }
		
		//Get profile size
		var profilesize = combinedData[key][0][0][3];
		
		// Get all profile positions
		var profileElemPositions = [];
		for( var i=0; i<combinedData[key][0].length; i++)
			profileElemPositions.push(combinedData[key][0][i][0]);
			
		//Figure out which node values have been changed.
		var theRawNodePositions = [];
		for (var i = 0; i < rawNodeData[key].data[0].length; i += rawNodeData[key].yValuesPerXForBreaks)
			theRawNodePositions.push(rawNodeData[key].data[0][i][0]);
			
		var theAdjustedNodePositions = [];
		for (var i = 0; i < adjustedNodeData[key].data[0].length; i += adjustedNodeData[key].yValuesPerXForBreaks)
			theAdjustedNodePositions.push(adjustedNodeData[key].data[0][i][0]);
		
		for(var nodePos=1; nodePos < theRawNodePositions.length; nodePos++)
		{
			// changes between the raw and adjust node positions indicate that nodes have been spaced
			var adjustmentDelta = theAdjustedNodePositions[nodePos] - theRawNodePositions[nodePos];		
			if(adjustmentDelta !=0 )
			{
				var breakadjustment = false;
				
				// check if gap is associated to a break
				for(var breakelem=0; breakelem < breaks.length; breakelem++) {
					if (breaks[breakelem] == nodePos)
					{
						breakadjustment = true;
					}	
				}
				
				
				// ProfileData takes in a subset of segment plotpositions
				// 		  |------------------------------------|
				// ProfileElemStart						ProfileElemEnd
				//   Node-1 (value)    [profileLength]     Node (value)

				var profileElemEnd = profilesize* nodePos;
				var profileElemStart = profileElemEnd-profilesize;
				var profileData = [];
				for(var i=profileElemStart; i<profileElemEnd; i++)
				{
					profileData.push(profileElemPositions[i]);
				}
				
				var firstElement = theAdjustedNodePositions[nodePos-1];
				var adjustment = theAdjustedNodePositions[nodePos] - theAdjustedNodePositions[nodePos-1];
				
				if(breakadjustment ==true)
				{
					firstElement = theAdjustedNodePositions[nodePos];
					adjustment = theAdjustedNodePositions[nodePos+1] - theAdjustedNodePositions[nodePos];
				}

				AdjustProfileValues(profileData, adjustment, firstElement);	

				// Reintroduct recalculated positions to the CombinedData array
				for(var i=0; i< profileData.length; i++)
				{
					combinedData[key][0][profileElemStart+i][0] = profileData[i];
				}			
				
				if(breakadjustment ==false)
				{
					// the combinedData element at nodePos does not get updated. Update only on nodes within routes.
					combinedData[key][0][profileElemStart + profilesize][0] = theAdjustedNodePositions[nodePos];
				}
			}
			
		}
		
		
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}
function ExtractMaxMinYValues(validPrimaryKeys, nodeData, segmentData)
{
	var strFn = "ExtractMaxMinYValues";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var extremityYData = {};
	for(var i=0; i< validPrimaryKeys.length; i++)
	{
		var key = validPrimaryKeys[i];
		var minMaxYData = ExtractMaxMinFiniteYs(nodeData[key], segmentData[key]);
		var minY = Math.min.apply(Math, minMaxYData.minYs[0]);
		var maxY = Math.max.apply(Math, minMaxYData.maxYs[0]);
		extremityYData[key] = {
			"minY": minY,
			"maxY": maxY
		};
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return extremityYData;
}

function AreValidBreaks(breaks, validPrimaryKeys, nodeCombinedData)
{
	//breaks gives total number of nodes up till that point (breaks[1]=total nodes in route0 and 1).
	var strFn = "AreValidBreaks";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	for(var i=0; i<validPrimaryKeys.length; i++)
	{
		var keys = validPrimaryKeys[i];
		for (var i = 0; i < breaks.length; i++)
		{
			if ((breaks[i] < 1) ||
				(breaks[i] > (nodeCombinedData[keys].data[0].length / (nodeCombinedData[keys].yValuesPerXForBreaks))))
			{
				return null;	
			}
		}
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return true; 
}
function SetupReferenceLine(svg, legendTotalClass, nodeCombinedData, segmentCombinedData, margin, width, height, xScale, yScales, plotData)
{
	var strFn = "SetupReferenceLine";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var plotDataClone = JSON.parse(JSON.stringify(plotData));	//Creates a copy of plotData
	AddEntryToClonedPlotData("OperatingRangeMax", "OperatingRange", plotDataClone);

	var xReferenceRectanglePixelSpacing = 18;
	var referenceLineClass = "PipelineProfileReferenceLine";

	var focus = svg.append("g")
		.style("display", "none")
		.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

	var pressureSegmentData = segmentCombinedData["Pressure"][0];
	var temperatureSegmentData = segmentCombinedData["Temperature"][0];
	var pressureNodeData = nodeCombinedData["Pressure"].data[0];
	
	var xBisector = d3.bisector(function (d) { 
		return d[0];}).left;		//returns xValues
	var widthPlotArea = width-margin.left - margin.right;
	var heightPlotArea = height - margin.top - margin.bottom; 

	//Append a y line
	focus.append("line")
		.attr("class", "y"+ " ReferenceLineY" + " " + referenceLineClass )
		.attr("y1", 0)
		.attr("y2", heightPlotArea);

	var classXRefBox = "rectXReferencePosition";
	var yCoordForXBoxPositioning = heightPlotArea+xReferenceRectanglePixelSpacing;
	AppendXReferenceBox(focus, colorScheme.XAxisReferenceBox,yCoordForXBoxPositioning, classXRefBox);
	
	//Should have circles for baseline and operating range as well
	var segmentRef = "SegmentRef";
	var nodeRef = "NodeRef";
	for(var i=0; i< plotDataClone.Keys.length; i++)
	{
		var key = plotDataClone.Keys[i];
		var plotNameForRefBox = plotDataClone[key].nameForReferenceBox;
		var plotNameForToggle = plotDataClone[key].name;
		
		if(plotDataClone[key].isEmpty)		//If it's empty then no need to plot it.
		{
			continue;
		}
		var type = plotData[plotNameForToggle].isNodeData ? nodeRef : segmentRef;
		AppendYReferenceBox(focus, plotData, plotNameForToggle, plotNameForRefBox, type);
	}
		
	//Area for events for reference line should be underneath the legend
	svg.insert("rect", "."+legendTotalClass)
        .attr("width", widthPlotArea)
        .attr("height", heightPlotArea)
		.attr("transform", "translate(" + margin.left + "," + margin.top + ")")
        .style("fill", "none")
        .style("pointer-events", "all")
        .on("mouseover", function() { focus.style("display", null); })
        .on("mouseout", function() { focus.style("display", "none"); })
        .on("mousemove", MouseMoveRefLine)
		
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	function MouseMoveRefLine() 
	{
		var strFn = "MouseMoveRefLine";
		LogMessage(L_PARAM, src, "Entering function: "+ strFn);
		var keysToMoveRefBox = [];
		var xMouseCoord = xScale.invert(d3.mouse(this)[0]);
		var tolerance = 3;
		var hasCloseNodeData;
		var xValue = [], xPosition=[];
		var yValues ={};
		
		var key = "Pressure";
		if(plotData[key].isEmpty == false)
		{
			var pressureSegData = FindClosestSegmentDataPoint(xBisector, pressureSegmentData, xMouseCoord, tolerance);
			xValue.push(pressureSegData.xValue);
			xPosition.push(pressureSegData.xPosition);
			yValues[key] = pressureSegData.value;
			keysToMoveRefBox.push(key);
		}
		
		var key = "Temperature";
		if(plotData[key].isEmpty == false)
		{
			var temperatureSegData = FindClosestSegmentDataPoint(xBisector, temperatureSegmentData, xMouseCoord, tolerance);
			xValue.push(temperatureSegData.xValue);
			xPosition.push(temperatureSegData.xPosition);
			yValues[key] = temperatureSegData.value;
			keysToMoveRefBox.push(key);
		}
		
		if(xValue.length>1)
		{
			//Should all be equal
			var isXValueOneElement = IsArrayJustOneElement(xValue);
			var isXPositionOneElement= IsArrayJustOneElement(xPosition);
			if(isXValueOneElement == false || isXPositionOneElement == false)
			{
				LogErrorMessage(src, "Mismatch of closest x position in segment data");
				return;
			}
		}
		
		xValue = xValue[0];		//Get the first entry
		xPosition = xPosition[0];
		var closestNodeData = FindClosestNodeDataPoint(xBisector, pressureNodeData, xMouseCoord, tolerance);
		if(closestNodeData == null)
		{
			hasCloseNodeData = false;
			focus.selectAll("."+nodeRef).attr("display", "none");	//No data to show to don't show anything		
		}
		else
		{
			hasCloseNodeData = true;
			focus.selectAll("."+nodeRef).attr("display", null);		//Data to show so set display to null	
			var operRangeStr = "OperatingRange";
			var operRangeMaxStr = "OperatingRangeMax";
			var baselinePressureStr = "BaselinePressure";
			yValues[operRangeStr] = closestNodeData.Min;
			yValues[operRangeMaxStr] = closestNodeData.Max;
			yValues[baselinePressureStr] = closestNodeData.Baseline;
			keysToMoveRefBox.push(operRangeStr,operRangeMaxStr, baselinePressureStr);
		}
		
		//Rounds to 1 decimal place
		for(var i=0; i< keysToMoveRefBox.length; i++)
		{
			var key = keysToMoveRefBox[i];
			yValues[key] = Math.round(yValues[key]*10)/10;
		}
		xPosition = Math.round(xPosition*10)/10;
		
		//Need Y value corresponding to this!
		focus.select(".y")
			.attr("transform",
				  "translate(" + xScale(xValue) + "," + 0 + ")")
			.attr("y2", heightPlotArea);		
		var rectanglePadding = 2;
		var yScaleToUse;
		var yCoordsForOverlapDetermination = {};
		var heightOfBox;
		var prevHeightofBox =0;
		//First two keys are for the segment data and then the others are for node
		for(var i=0; i< keysToMoveRefBox.length; i++)
		{
			var key = keysToMoveRefBox[i];
			if(plotDataClone[key].isNodeData == true && hasCloseNodeData == false)
			{
				break;
			}
			
			if(plotDataClone[key].isEmpty)		//If it's empty then no need to plot it.
			{
				continue;
			}
			
			if(plotDataClone[key].isPressurePlot)
			{
				var keyForScaling = "Pressure";
				yScaleToUse = yScales[keyForScaling];
			}
			else if(plotDataClone[key].isTemperaturePlot)
			{
				var keyForScaling = "Temperature";
				yScaleToUse = yScales[keyForScaling];				
			}
			else
			{
				LogErrorMessage(src, "Error: is neither Pressure or Temperature plot, it's: " +type);
				return;
			}

			var bBox = MoveYReferenceBoxes(focus, plotDataClone, key, xScale, xValue, yScaleToUse, yValues,
				rectanglePadding, yCoordsForOverlapDetermination);
			heightOfBox = Math.max(bBox.height, prevHeightofBox);
			prevHeightofBox = heightOfBox;
		}
		if(heightOfBox != null || heightOfBox !=0)
		{
			var collisionParameters = DetectOverlaps(yCoordsForOverlapDetermination, heightOfBox, keysToMoveRefBox, plotDataClone);

			if(collisionParameters.hasOverlaps)
			{
				var updatedYCoords = collisionParameters.yCoordMap;
				for(var i=0; i<keysToMoveRefBox.length; i++)
				{
					key = keysToMoveRefBox[i];
					focus.selectAll("rect.Ref"+plotDataClone[key].nameForReferenceBox)
						.attr("transform",
							  "translate(" + xScale(xValue) + "," +
											 updatedYCoords[key] + ")");
					focus.selectAll("text.Ref"+plotDataClone[key].nameForReferenceBox)
						.attr("transform",
							  "translate(" + xScale(xValue) + "," +
											 updatedYCoords[key] + ")");
				}
			}
		}
		
		//Sets the x axis reference box and moves it to the bottom !
		var xBoxElement = focus.selectAll("text.text"+classXRefBox)
						.text(xPosition);
		var bBoxXElement = xBoxElement[0][0].getBBox();
		var centeredXOfRefLine = bBoxXElement.x - (bBoxXElement.width/2)+ xScale(xValue);
		focus.select("rect."+classXRefBox)
			.attr("width", bBoxXElement.width+ 2*rectanglePadding)
				.attr("height", bBoxXElement.height+ 1.5*rectanglePadding)
				.attr("x", bBoxXElement.x- rectanglePadding)
				.attr("y", bBoxXElement.y- rectanglePadding)
				.attr("transform",  "translate(" +centeredXOfRefLine +"," +0 + ")")
		xBoxElement.attr("transform", "translate(" + centeredXOfRefLine + "," +0+ ")");
		LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	}

}

function MoveYReferenceBoxes(focus, plotDataClone, key, xScale, xValue, yScaleToUse, yValues,
				rectanglePadding, yCoordsForOverlapDetermination)
{
	focus.select("circle.Ref"+plotDataClone[key].nameForReferenceBox)
		.attr("transform",
			  "translate(" + xScale(xValue) + "," +
							 yScaleToUse(yValues[key]) + ")");	
	var element = focus.selectAll("text.Ref"+plotDataClone[key].nameForReferenceBox)
				.attr("transform",
					  "translate(" + xScale(xValue) + "," +
									 yScaleToUse(yValues[key]) + ")")
				.text(yValues[key]);	
	var bBox = element[0][0].getBBox();
	focus.select("rect.Ref"+plotDataClone[key].nameForReferenceBox)
		.attr("width", bBox.width+ 2*rectanglePadding)
		.attr("height", bBox.height+ 1.5*rectanglePadding)
		.attr("x", bBox.x- rectanglePadding)
		.attr("y", bBox.y- rectanglePadding)
		.attr("transform",
				  "translate(" + xScale(xValue) + "," +
								 yScaleToUse(yValues[key]) + ")");
	yCoordsForOverlapDetermination[key] = yScaleToUse(yValues[key]);
	return bBox;
}

function AppendXReferenceBox(focus, color,yCoord, xRefClass)
{
	var strFn = "AppendXReferenceBox";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	focus.append("rect")
		.attr("stroke", color)
		.attr("stroke-width", 1)
		.attr("fill", "none")
		.attr("stroke-dasharray", null)
		.attr("y", yCoord)
		.attr("class", xRefClass)
		
	focus.append("text")
		.attr("class", "text"+xRefClass)
		.style("stroke", "white")
		.style("stroke-width", "3.5px")
		.style("opacity", 0.8)
		.attr("y", yCoord)
	focus.append("text")
		.attr("class", "text"+xRefClass)
		.attr("y", yCoord);
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function AppendYReferenceBox(focus, plotData, nameForToggle, nameForRef, type)
{
	focus.append("circle")
		.attr("class", "Ref"+nameForRef+ " RefForToggle"+nameForToggle+" "+ type)
		.style("fill", colorScheme.ReferenceLine)
		.style("stroke", colorScheme.ReferenceLine)
		.attr("r", 2);
	
	focus.append("text")
		.attr("class", "Ref"+nameForRef+" RefForToggle"+nameForToggle+ " "+type)
		.style("stroke", "white")
		.style("stroke-width", "3.5px")
		.style("opacity", 0.8)
		.attr("dx", 8);
	focus.append("text")
		.attr("class", "Ref"+nameForRef+" RefForToggle"+nameForToggle+" "+type)
		.attr("dx", 8);	

	var color = plotData[nameForToggle].color;
	var strokeDashArray = plotData[nameForToggle].strokeWidthsLegendEntry;
	
	focus.append("rect")
		.attr("stroke", color)
		.attr("stroke-width", 1)
		.attr("fill", "none")
		.attr("stroke-dasharray", strokeDashArray)
		.attr("class", "Ref"+nameForRef+" RefForToggle"+nameForToggle+" "+type);
}
function DetectOverlaps(yCoordsMap, height, keysOfInterest, thePlotData)
{
	var strFn = "DetectOverlaps";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	//Only really concerned with y values
	var hasOverlaps = false;
	var stackPadding = 5;
	var maxCount = 20;		//Number of iterations to try and space out the boxes
	var count = 0;
	while(count < maxCount)
	{
		//yCoordsMap keeps getting updated with the relevant details. do this maxCount times
		var indices = ExtractCollisionIndices(yCoordsMap, height, keysOfInterest);
		
		if(indices.length > 0)
		{
			//Check duplicates
			var nonDuplicatedIndices = RemoveDuplicateEntries(indices);
			LogMessage(L_PARAM, src, "Non duplicated overlapping indices are "+nonDuplicatedIndices);
			for(var i=0; i<nonDuplicatedIndices.length; i++)
			{
				var data =  nonDuplicatedIndices[0];
				var ReferenceEntry = data[1];
				var EntryToChange = data[3];
				var upOrDown = data[4];
				if(upOrDown == 1)
				{
					yCoordsMap[EntryToChange] = yCoordsMap[ReferenceEntry] - height -stackPadding;
					hasOverlaps = true;
				}
				else if(upOrDown ==0)
				{
					yCoordsMap[EntryToChange] = yCoordsMap[ReferenceEntry] + height +stackPadding;
					hasOverlaps = true;
				}
			}
		}
		else
		{
			break;
		}
		count = count+1;
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"yCoordMap": yCoordsMap,
		"hasOverlaps": hasOverlaps
	};
}

function ExtractCollisionIndices(yCoordsMap, height, keysOfInterest)
{
	var strFn = "ExtractCollisionIndices";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var tolerance = 3;
	//0 is down, 1 is up
	CollidingIndices = [];
	for(var i=0; i< keysOfInterest.length; i++)
	{
		var key = keysOfInterest[i];
		var YCoord = yCoordsMap[key]; 
		var Total = YCoord+ height;
		for(var j=0; j< keysOfInterest.length; j++)
		{
			var keyInner = keysOfInterest[j];
			var arrayToPush = [];
			if(j!=i)
			{
				var compareY = yCoordsMap[keyInner];
				var compareTotal = height + compareY;
				if( (Total - compareY)>= tolerance && (Total - compareTotal <= tolerance))
				{
					arrayToPush.push(i, key,j, keyInner, 0);	//Last 0 represents collision down
					CollidingIndices.push(arrayToPush);
				}
				else if( (YCoord - compareTotal) <=tolerance && (YCoord - compareY >=tolerance))
				{
					arrayToPush.push(i, key,j, keyInner, 1);	//Last 1 represents collision up
					CollidingIndices.push(arrayToPush);
				}
			}
		}
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return CollidingIndices;
}

function RemoveDuplicateEntries(indices)
{
	var strFn = "RemoveDuplicateEntries";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var indicesToRemove = [];
	while(true)
	{
		var indexToRemove = ExtractDuplicate(indices);
		if(indexToRemove!= null)
		{
			indices = indices.splice(indexToRemove,1);
			continue;
		}
		break;
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return indices;
}

function ExtractDuplicate(indices)
{
	var strFn = "ExtractDuplicate";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	for(var i=0; i<indices.length; i++)
	{
		var theTwoIndices = {
			"First": indices[i][0],
			"Second": indices[i][2]
		};
		for(var j=0; j<indices.length; j++)
		{
			if(i!=j)
			{
				var FirstIndex = indices[j][0];
				var SecondIndex = indices[j][2];
				if(theTwoIndices.First == SecondIndex &&
					theTwoIndices.Second == FirstIndex)
				{
					LogMessage(L_PARAM, src, "Exiting function successfully: "+ strFn);
					return j;
				}
			}
		}
	}
	LogMessage(L_PARAM, src, "Exiting function with problems: "+ strFn);
	return null;
}

function AddEntryToClonedPlotData(plotNameAdded, EntryToCopyOver, plotDataClone)
{
	//EntryToCopyOver is the other class member we are copying and putting as the new element
	var strFn = "AddEntryToClonedPlotData";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	plotDataClone[plotNameAdded] = JSON.parse(JSON.stringify(plotDataClone[EntryToCopyOver]));		//name is still operating range!
	plotDataClone[plotNameAdded].nameForReferenceBox = plotNameAdded;
	plotDataClone.Keys.push(plotNameAdded);
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function IsArrayJustOneElement(arr)
{
	var strFn = "IsArrayJustOneElement";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var prevValue;
	var curValue;
	for(var i=0; i<arr.length; i++)
	{
		curValue = arr[i];
		if(i!=0)
		{
			if(prevValue != curValue)
			{
				LogMessage(L_PARAM, src, "Exiting function with problems: "+ strFn);
				return false;
			}
		}
		prevValue = curValue;
	}
	LogMessage(L_PARAM, src, "Exiting function Successfully: "+ strFn);
	return true;
}
function FindClosestSegmentDataPoint(xBisector, segmentData, xMouseCoord, tolerance)
{
	var strFn = "FindClosestSegmentDataPoint";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var closestSegment = snapToSegment(xBisector, segmentData, xMouseCoord, tolerance);
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"xValue": closestSegment[0],
		"xPosition": closestSegment[1],
		"value": closestSegment[2]
	};
}

function FindClosestNodeDataPoint(xBisector, nodeData, xMouseCoord, tolerance)
{
	var strFn = "FindClosestNodeDataPoint";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var closestNode = snapToNode(xBisector, nodeData, xMouseCoord, tolerance);
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	if(closestNode !=null)
	{
		return {
			"Min": closestNode[2],
			"Max": closestNode[3],
			"Baseline": closestNode[4]
		};
			
	}
	else
	{
		return null;		//If it is null then don't show the elements
	}	
}

function AppendYAxis(svg, margin, nameString, axisTitle, xDistanceOfYAxis, xDistanceGraphLabel)	//0 as no need to move yAxisNamePlacement to the right
{
	var strFn = "AppendYAxis";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	svg.append("g")
		.attr("transform", "translate(" + xDistanceOfYAxis + "," + margin.top + ")")
		.attr("class", "y axisset"+nameString)
		.append("g")
			.attr("class", "y axis"+nameString)
			.style("stroke", "dimgray")			
		.append("text")	
			.text(axisTitle)
			.attr("id", "yAxisTitle"+nameString)
			.attr('text-anchor', 'middle')
			.attr("transform", "rotate (-90, -" + (margin.left - 7) + ", -5) translate(-" + ((height / 2)) + "," + xDistanceGraphLabel+ ")");
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function ExtractNodeDataToShow(breaks, data, yValuesPerX, xScale, tickSize, xAxisTickLabels)
{
	var strFn = "ExtractNodeDataToShow";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var axisNum = 0;
	var xAxesVis = [];
	var xScalesVis = [];
	var dataVis = [];
	do 
	{
		var minXIndex, maxXIndex;
		if (axisNum == 0)
		{
			minXIndex = 0;		//This is the first axis, so it starts at the beginning
		}
		else
		{
			minXIndex = breaks[axisNum - 1];	// This is not the first axis, so it starts at the break point specified
		}

		if (axisNum == breaks.length)
		{
			// This is the last axis, so it ends at the last data point
			maxXIndex = data[0].length / yValuesPerX - 1;
		}
		else
		{
			// This is not the last axis, so it ends before the break point specified
			maxXIndex = breaks[axisNum] - 1;		//We want index so this is breaks[axisNum] -1  
		}
		var minXCurrent = data[0][minXIndex * yValuesPerX][0];		//Minimum X value
		var maxXCurrent = data[0][maxXIndex * yValuesPerX][0];		//Maximum X value
		//Gets the minimum and maximum X coords for each route!

		var tempScale = d3.scale.linear()
			.domain([minXCurrent, maxXCurrent])			//Gets a scale where the X axis is mapped accordingly. 
			.range([xScale(minXCurrent), xScale(maxXCurrent)]);

		xScalesVis.push(tempScale);

		// Now create the matching Axis
		var tempAxis = d3.svg.axis().scale(xScalesVis[axisNum]).orient("bottom").tickSize(tickSize, 0);
		// Use custom x-axis tick values
		if (xAxisTickLabels.length != 0) {
			var xAxisTickValues = [];
			for (var i = minXIndex; i <= maxXIndex; i++) {
				xAxisTickValues.push(data[0][i * yValuesPerX][0]);
			}
			tempAxis.tickValues(xAxisTickValues);
		}
		xAxesVis.push(tempAxis);
		// Create data subsets
		var tempDataSubset= [];
		tempDataSubset.push(data[0].slice(minXIndex * yValuesPerX, (maxXIndex + 1) * yValuesPerX));
		dataVis.push(tempDataSubset);
	} while (axisNum++ < breaks.length)
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);

	return {
		"xScalesVisible": xScalesVis,
		"xAxesVisible": xAxesVis,
		"dataVisible": dataVis
	};
}

function ExtractSegmentDataToShow(nodesData, segmentData)
{
	var strFn = "ExtractSegmentDataToShow";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var totalCount = 0;
	var dataVisSegment = [];
	var nodesInRoute;
	for(var i=0; i<nodesData.length; i++)
	{
		nodesInRoute = nodesData[i];
		tempSegmentData = [];
		tempStoreData = [];
		
		//nodesInRoute - 1 as this is the amount of segments in the route
		for(var j=0; j<(nodesInRoute-1); j++)
		{
			if(totalCount>= segmentData[0].length)
			{
					LogMessage(L_PARAM, src, "Count exceeds array bounds, NodesInRoute: " +nodesInRoute);
					return {
						"dataVisibleSegment": dataVisSegment
					};
			}
			var segmentDataPoints = segmentData[0][totalCount][3];
			//10 data points per node!
			for(var k=0; k< segmentDataPoints; k++)
			{
			tempSegmentData.push(segmentData[0][totalCount]);
			totalCount++;
			}
		}
		tempStoreData.push(tempSegmentData);
		dataVisSegment.push(tempStoreData);
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"dataVisibleSegment": dataVisSegment
	};
}

function ExtractMaxMinFiniteYs(nodeData, segmentData)
{
	var strFn = "ExtractMaxMinFiniteYs";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	
	// clean up data of position values	 
	var cleanSegmentData = sliceDataArray(segmentData, 2, 3);
	var cleanNodeData = sliceDataArray(nodeData.data, 1);

	var minYs = [], maxYs = [];		
	var yValuesPerX = nodeData.yValuesPerX;
	var nodeDatum = cleanNodeData;
	
	var minYPlaceholder=[],  maxYPlaceholder= [];
	var minFiniteYs = [], maxFiniteYs = [];		// Contains only the finite mins and maxes of the y datasets

	for (var iter = 0; iter <= yValuesPerX; iter++) {
		var iterLastY = Infinity;
		var iterMinY = d3.min(nodeDatum, function (d) {
			return d3.min(d, function (sd) {
				if (isFinite(sd[iter])) {
					iterLastY = d3.min(sd); // Update last good Y value for the dataset
					return iterLastY;
				}
				else
					return Infinity;
			});
		});

		var iterMaxY = d3.max(nodeDatum, function (d) {
			return d3.max(d, function (sd) {
				if (isFinite(sd[iter]))
					return d3.max(sd); 
				else
					return -Infinity;
			});
		});

		minYPlaceholder.push(iterMinY);
		maxYPlaceholder.push(iterMaxY);
		
		if (isFinite(iterMinY)) {	// If there is a finite Min there should also be a finite max
			minFiniteYs.push(iterMinY);
			maxFiniteYs.push(iterMaxY);
		}
	}

	var maxSegment = d3.max(cleanSegmentData, function(d)
	{
		return d3.max(d, function(sd)
		{
			return d3.max(sd); 
		});
	});
	maxYPlaceholder.push(maxSegment);
	
	var minSegment = d3.min(cleanSegmentData, function(d)
	{
		return d3.min(d, function(sd)
		{
			return d3.min(sd);
		});
	});
	minYPlaceholder.push(minSegment);
	
	minYs.push(minYPlaceholder);
	maxYs.push(maxYPlaceholder);
	if (minFiniteYs.length == 0)
		throw "No valid y-axis data.";
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"minYs": minYs,
		"maxYs": maxYs
	};
}

function ExtractMaxMinXValues(nodeData, validKeys)
{
	var strFn = "ExtractMaxMinXValues";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var isFirst = true;
	var prevMax,
		prevMin;
	var max,
		min;
		
	for(var i=0; i<validKeys.length; i++)
	{
		max = ExtractMaxXValue(nodeData[validKeys[i]].data);
		min = ExtractMinXValue(nodeData[validKeys[i]].data);
		if(isFirst)
		{
			prevMax = max;
			prevMin = min;
			isFirst= false;
		}
		if( (min != prevMin) || (max != prevMax))
		{
			LogMessage(L_PARAM, src, "Mismatch of Max/Min values");
		}
		max = Math.max(max, prevMax);
		min = Math.min(min, prevMin);
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"minimum": min,
		"maximum": max
	};
}

function ExtractMaxXValue(data)
{
	return d3.max(data, function (d) {
			return d3.max(d, function (sd) {
				// Check that all x-axis data is legal
				if (!isFinite(sd[0]))
					throw "Invalid x-axis data found.";
				return sd[0];
			});
		});
}

function ExtractMinXValue(data)
{
	return d3.min(data, function (d) {
			return d3.min(d, function (sd) {
				// Check that all x-axis data is legal
				if (!isFinite(sd[0]))
					throw "Invalid x-axis data found.";
				return sd[0];
			});
		});
}

function TogglePlotsBasedOnPastBehavior(nodesData, plotData, dataToggle)
{
	var strFn = "TogglePlotsBasedOnPastBehavior";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	
	var keys = plotData.Keys;
	for(var i=0; i< keys.length; i++)
	{
		if(plotData[keys[i]].dataToggle == true)
		{
			plotData[keys[i]].dataToggle = false;
			ClickLegend(plotData[keys[i]].name, nodesData, plotData, dataToggle);
		}
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return plotData;
}

function HoverOverLegendEntry(type, isMouseOver)
{
	var colorHighlight = isMouseOver? colorScheme.HoverOver: colorScheme.LegendBackground;
	d3.select(".highlighted" + type).attr("fill", colorHighlight);
}

function ClickLegend(type, nodesData, plotData, dataToggle)
{
	var strFn = "ClickLegend";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var	colorText;
	var displayReferenceBox;
	
	var keys = plotData.Keys;
	
	if(keys.indexOf(type)<0)
	{
		LogErrorMessage(src, "Plot clicked in legend is of invalid type: "+type);
	}
	
	var typeValues = plotData[type];
	var active = typeValues.dataToggle ?false:true;
	var colorCircle = typeValues.color;
	var isInitiallyFilled = typeValues.isInitiallyFilled;
	var hasOnePlotForAxis = typeValues.hasOnePlotForAxis;

	//Updating values on whether its active or inactive
	if(active ==true)
	{
		newOpacity = 0;
		colorText = colorScheme.Selection;		
		colorCircle = "none";			//There should be no fill if we are switched off
		displayReferenceBox = "none";
	}
	else		
	{
		newOpacity = 1;
		colorText = colorScheme.RegularText;
		displayReferenceBox = null;
	}
	
	//For the circle legend entries which are initially filled
	if(isInitiallyFilled)
	{
		d3.select("."+type+"CircleLegend").attr("fill", colorCircle);
	}
	
	d3.select(".legend"+type+"Text").attr("fill", colorText);

	var routesToSketch = nodesData.length;
	for(var k = 0; k<routesToSketch; k++)
	{
		d3.selectAll("g.group"+type+k).style("opacity", newOpacity);	//selectAll as multiple groups
	}
	d3.selectAll(".RefForToggle"+type).style("display",displayReferenceBox);
	
	// Update whether the elements are active
	plotData[type].dataToggle = active;
	if(hasOnePlotForAxis)
	{
		d3.select("g.y.axisset"+ type).style("opacity", newOpacity);
	}
	else
	{
		//In here if we are not the temperature curve
		var propertyToCheck = "isPressurePlot";
		YAxisDisplayDetermination(plotData, newOpacity, propertyToCheck);
	}
	//This is so that refreshing of the plot toggling works when auto time-updates are on
	SetDataToggleValues(plotData, dataToggle);	
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function SetDataToggleValues(plotData, dataToggle)
{
	var strFn = "SetDataToggleValues";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var i=0;
	for (var key in plotData) 
	{
		if (plotData.hasOwnProperty(key))
		{
			var valueCorrespondingToKey = plotData[key];
			dataToggle[i] = valueCorrespondingToKey.dataToggle;
			i++;
		}		
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function YAxisDisplayDetermination(plotData, newOpacity, propertyToCheck)
{
	var strFn = "YAxisDisplayDetermination";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var dataToggleValuesPressurePlot= [];
	var countOfPressurePlots = 0;
	var keys = plotData.Keys;
	for(var i=0; i<keys.length; i++)
	{
		var valueCorrespondingToKey = plotData[keys[i]];
		if(valueCorrespondingToKey[propertyToCheck] == true)
		{
			countOfPressurePlots++;
			dataToggleValuesPressurePlot.push(valueCorrespondingToKey.dataToggle);			
		}
	}
	
	//See if all pressure plots are toggled off
	var pressurePlotsTurnedOffCount = 0;
	for(var i=0; i< dataToggleValuesPressurePlot.length; i++)
	{
		if(dataToggleValuesPressurePlot[i] == true)
		{
			pressurePlotsTurnedOffCount++;
		}
	}
	//Remove axis if they are all turned off!
	if(pressurePlotsTurnedOffCount == countOfPressurePlots)
	{
		d3.select("g.y.axisset"+ plotData["Pressure"].name).style("opacity", 0);
	}

	//Pressure scale should come back on if either pressure plot is on
	if( (dataToggleValuesPressurePlot.indexOf(false) > -1) && (newOpacity ==1))
	{
		d3.select("g.y.axisset"+ plotData["Pressure"].name).style("opacity", newOpacity);
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function InitialiseLegendVariables()
{
	var strFn = "InitialiseLegendVariables";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var dictionaryLegendEntries ={
		"Pressure": "Pressure",
		"Temperature": "Temperature",
		"BaselinePressure": "Baseline Pressure",
		"OperatingRange": "Operating Range Pressure"
	};
	
	var legendRadius = 5; 
	var legendFont = 18;
	var separationDistanceAdditional =3;
	var borderPadding = 10;
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
	return {
		"dictionary": dictionaryLegendEntries,
		"radius": legendRadius,
		"font": legendFont,
		"distanceAdditional": separationDistanceAdditional,
		"padding": borderPadding
	};
}
	
function AppendLegend(svg, legendTotalClass, legendCoord, nodesData, plotData, dataToggle)
{
	var strFn = "AppendLegend";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	var bBoxes = [];
	var init = InitialiseLegendVariables();
	var dictionaryLegendEntries = init.dictionary;
	var legendRadius = init.radius;
	var legendFont = init.font;
	var separationDistanceAdditional =init.distanceAdditional;
	var borderPadding = init.padding;
	
	var legendBorderClass = "legendBorder"
	var legendInnerClass = "legendInner";
	var legendOuterClass = "legendOuter";
	var legendDividerClass = "legendDivider";
	var dragClass = "dragClass";

	var drag = d3.behavior.drag()
				.origin(function() {
				var t = d3.transform(d3.select(this).attr("transform"));
				return {x: t.translate[0], y: t.translate[1]};
				})
				.on("drag", function(d,i) 
				{
					legendCoord.x = legendCoord.xPrevious + d3.event.x;
					legendCoord.y = legendCoord.yPrevious + d3.event.y;
					d3.selectAll("."+dragClass).attr("transform", function(d,i)
					{
					return "translate(" + [ d3.event.x,d3.event.y ] + ")"
					})
				})
				.on('dragstart', function () {
					legendCoord.xPrevious = legendCoord.x;
					legendCoord.yPrevious = legendCoord.y;
				})
	var legendTotal = svg.append("g")
							.attr("class", legendTotalClass)
							.style("cursor", "pointer")
							.attr("transform", "translate(" + legendCoord.x + "," + legendCoord.y + ")");
	
    // legend Divider values
	var legendDivX, legendDivY, legendDivWidth = 0;

	for(var k=0; k<plotData.Keys.length; k++)
	{
	    var key = plotData.Keys[k];

		var legendEntry = legendTotal.append("g")
									.attr("class", "legend"+plotData[key].name+" "+dragClass);
		if(k==0)		//Only for first iteration as we need bBoxes otherwise
		{
			AppendCircleAndTextForLegendEntry(legendEntry,plotData[key].name, 0, 0, dictionaryLegendEntries, nodesData, legendFont, legendRadius, plotData, dataToggle);
			bBoxes.push(legendEntry[0][0].getBBox());
		}
		else
		{
		    // add line ahead of temperature
		    if (plotData[key].name == "Temperature")
		    {
		        AppendCircleAndTextForLegendEntry(legendEntry, plotData[key].name, 0, 0 + k * (bBoxes[0].height + (separationDistanceAdditional*1.5)), dictionaryLegendEntries, nodesData, legendFont, legendRadius, plotData, dataToggle);
		        bBoxes.push(legendEntry[0][0].getBBox());

		        // line attributes
		        legendDivX = legendEntry[0][0].getBBox().x;
		        legendDivY = legendEntry[0][0].getBBox().y - (separationDistanceAdditional);

		    }
		    else
		    {
		        AppendCircleAndTextForLegendEntry(legendEntry, plotData[key].name, 0, 0 + k * (bBoxes[0].height + separationDistanceAdditional), dictionaryLegendEntries, nodesData, legendFont, legendRadius, plotData, dataToggle);
		        bBoxes.push(legendEntry[0][0].getBBox());
		    }

		}
	}
	HightlightLegendEntries(bBoxes, nodesData, plotData, dataToggle);		//For the mouseover hover stuff
	
	var legendBorder = legendTotal.append("rect")
								  .attr("class", legendBorderClass+" "+dragClass);
	SetLegendBorder(legendBorder, bBoxes, borderPadding);
	
	
	legendTotal.append("line")
        .attr("class", legendDividerClass + " " + dragClass)
        .attr("x1", legendDivX)
        .attr("y1", legendDivY)
        .attr("x2", (legendBorder[0][0].getBBox().width + legendDivX - borderPadding))
        .attr("y2", legendDivY)
        .on("mouseover", partial(HoverOverLegendEntry, plotData[key].name, true))
        .on("mouseout", partial(HoverOverLegendEntry, plotData[key].name, false))

	for(var i=0; i< plotData.Keys.length; i++)
	{
		var key = plotData.Keys[i];
		if(plotData[key].isFirstEntryForLegend)
		{
			SetInnerLegendRect(legendTotalClass, plotData[key].name, legendInnerClass, legendBorder, drag, dragClass);
			break;
		}
	}
	TogglePlotsBasedOnPastBehavior(nodesData, plotData, dataToggle);
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);	
}

function HightlightLegendEntries(bBoxes, nodesData, plotData, dataToggle) {
    var strFn = "HightlightLegendEntries";
    LogMessage(L_PARAM, src, "Entering function: " + strFn);
    var highlightedRect = {};
    var pixelOffsetToSize = 1;
    var maxWidth = 0;
    for (var i = 0; i < bBoxes.length; i++) {
        var maxWidth = Math.max(maxWidth, bBoxes[i].width);
    }
    highlightedRect.width = (maxWidth + pixelOffsetToSize);
    highlightedRect.x = bBoxes[0].x - pixelOffsetToSize;	//Should all start at the same x
    highlightedRect.height = bBoxes[0].height;		//Text height should be the same

    for (var i = 0; i < plotData.Keys.length; i++) {
        var key = plotData.Keys[i];
        if (plotData[key].name != "LeakAdjusted") {
            highlightedRect.y = bBoxes[i].y;
            d3.select(".legend" + plotData[key].name).insert("rect", ".legend" + plotData[key].name + "Text")
                                                .attr("x", highlightedRect.x)
                                                .attr("y", highlightedRect.y)
                                                .attr("width", highlightedRect.width)
                                                .attr("height", highlightedRect.height)
                                                .attr("class", "highlighted" + plotData[key].name)
                                                .attr("fill", colorScheme.LegendBackground)
                                                .on("click", partial(ClickLegend, plotData[key].name, nodesData, plotData, dataToggle))
                                                .on("mouseover", partial(HoverOverLegendEntry, plotData[key].name, true))
                                                .on("mouseout", partial(HoverOverLegendEntry, plotData[key].name, false))
        }
    }
    LogMessage(L_PARAM, src, "Exiting function: " + strFn);
}

function SetInnerLegendRect(legendTotalClass, plotName, legendInnerClass, legendBorder, drag, dragClass) {
    var strFn = "SetInnerLegendRect";
    LogMessage(L_PARAM, src, "Entering function: " + strFn);
    var bBox = legendBorder[0][0].getBBox();
    d3.select("." + legendTotalClass).insert("rect", ".legend" + plotName)
									.attr("x", bBox.x)
									.attr("y", bBox.y)
									.attr("width", bBox.width)
									.attr("height", bBox.height)
									.attr("fill", colorScheme.LegendBackground)
									.attr("class", legendInnerClass + " " + dragClass)
									.attr("cursor", "move")
									.call(drag);
    LogMessage(L_PARAM, src, "Exiting function: " + strFn);
}

function SetLegendBorder(legendBorder, bBoxes, borderPadding) {
    var strFn = "SetLegendBorder";
    LogMessage(L_PARAM, src, "Entering function: " + strFn);
    var maxWidth = 0;
    for (var i = 0; i < bBoxes.length; i++) {
        maxWidth = Math.max(bBoxes[i].width, maxWidth);
    }
   
    var rectWidth = maxWidth + borderPadding;
    var rectHeight = bBoxes[(bBoxes.length - 1)].y + bBoxes[(bBoxes.length - 1)].height - bBoxes[0].y + borderPadding / (1.5);
    legendDivWidth = rectWidth;

    //Borderpadding fractions above and below are determined visually to ensure legend entries fit properly. 
    var borderXCoord = bBoxes[0].x - (borderPadding / 2);
    var borderYCoord = bBoxes[0].y - (borderPadding / 3);

    legendBorder
        .attr("class", "LegendBorder")
		.attr("x", borderXCoord)
		.attr("y", borderYCoord)
		.attr("width", rectWidth)
		.attr("height", rectHeight);
    LogMessage(L_PARAM, src, "Exiting function: " + strFn);
}

function AppendCircleAndTextForLegendEntry(GElement, type, xToPlace, yToPlace, dictionaryLegendEntries, nodesData, legendFont, legendRadius, plotData, dataToggle) {
    var strFn = "AppendCircleAndTextForLegendEntry";
    LogMessage(L_PARAM, src, "Entering function: " + strFn + " " + type);
    var legendAttributes = plotData[type];
    var legendTitle = legendAttributes.legendTitle;
    var legendClass = legendAttributes.legendClass;
    var color = legendAttributes.color;
    var isFilled = legendAttributes.isInitiallyFilled;
    var dashedValue = legendAttributes.strokeWidthsLegendEntry;

    LogMessage(L_PARAM, src, "Appending legend entry for: " + type);
    
    //xToPlace and yToPlace are zero as we ultimately use a transform to move the legend to the appropriate place
    //var fill= isFilled ? color: "none";
    GElement.append("text")
		.attr("x", xToPlace)
		.attr("y", yToPlace)
		.attr("class", "legend" + type + "Text")
		.attr("font-size", legendFont)
		.on("click", partial(ClickLegend, type, nodesData, plotData, dataToggle))
		.on("mouseover", partial(HoverOverLegendEntry, type, true))
		.on("mouseout", partial(HoverOverLegendEntry, type, false))
		.text(legendTitle);

    var offsetsText = d3.select(".legend" + type + "Text")[0][0].getBBox();

    var legendCircle = GElement.append("circle")
    .attr("r", legendRadius)
    .attr("class", type + "CircleLegend " + legendClass)
    .on("click", partial(ClickLegend, type, nodesData, plotData))
    .on("mouseover", partial(HoverOverLegendEntry, type, true))
    .on("mouseout", partial(HoverOverLegendEntry, type, false))

    var legendCircleBBox = legendCircle[0][0].getBBox();
    var xCoord = offsetsText.x - legendCircleBBox.width;
    var yCoord = offsetsText.y + (offsetsText.height / 2);

    d3.select("." + type + "CircleLegend")
        .attr("cx", xCoord)
        .attr("cy", yCoord);
    LogMessage(L_PARAM, src, "Exiting function: " + strFn);
}

function AppendLeakLegendEntry(GElement, type, xToPlace, yToPlace, legendFont, legendRadius, plotData) {
    var strFn = "AppendLeakLegendEntry";
    LogMessage(L_PARAM, src, "Entering function: " + strFn + " " + type);

    var legendAttributes = plotData[type];
    var legendTitle = legendAttributes.legendTitle;
    var legendClass = legendAttributes.legendClass;
    var inactiveClass = "";

    GElement.append("text")
    .attr("x", xToPlace)
    .attr("y", yToPlace)
    .attr("class", "legend" + type + "Text")
    .attr("font-size", legendFont)
    .text(legendTitle);



    var offsetsText = d3.select(".legend" + type + "Text")[0][0].getBBox();

    var legendCircle = GElement.append("circle")
    .attr("r", legendRadius)
    .attr("class", type + "CircleLegend " + legendClass);

    var legendCircleBBox = legendCircle[0][0].getBBox();
    var xCoord = offsetsText.x - legendCircleBBox.width;
    var yCoord = offsetsText.y + (offsetsText.height / 2);

    d3.select("." + type + "CircleLegend")
        .attr("cx", xCoord)
        .attr("cy", yCoord);

    LogMessage(L_PARAM, src, "Exiting function: " + strFn);
}
function TransformDataToAxes(svg, plotData, validPrimaryKeys)
{
	var strFn = "TransformDataToAxes";
	LogMessage(L_PARAM, src, "Entering function: "+ strFn);
	//TransformDataToAxes(svg, plotData, validPrimaryKeys);
	LogMessage(L_PARAM, src, "Entered TransformDataToAxes function");
	for(var i=0; i<validPrimaryKeys.length; i++)
	{
		var key = validPrimaryKeys[i];
		svg.selectAll("g.series"+plotData[key].name)		
			.attr("transform", "translate(" + margin.left + "," + margin.top + ")");
		svg.selectAll("g.seriesSegment"+plotData[key].name)		
			.attr("transform", "translate(" + margin.left + "," + margin.top + ")");
	}
	LogMessage(L_PARAM, src, "Exiting function: "+ strFn);
}

function partial(func /*, 0..n args */) 
{
  var args = Array.prototype.slice.call(arguments, 1);
  return function() {
	var allArguments = args.concat(Array.prototype.slice.call(arguments));
	return func.apply(this, allArguments);
	};
}


// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/