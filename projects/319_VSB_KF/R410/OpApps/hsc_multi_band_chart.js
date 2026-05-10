// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_band_chart.js

function multiBandChart() {
	var src = "hsc_multi_band_chart.js",
		tickSize = 6,
		maxXAxisTickLabelSize = 35,
		margin = { top: 20, right: 50, bottom: 50, left: 50 },
		width = 400,
		height = 300,
		xValue = function (d) { return d[0]; },
		yValue = function (d) { return d[1]; },
		y0Value = function (d) { return d[2]; },
		y1Value = function (d) { return d[3]; },
		y2Value = function (d) { return d[4]; },
		xScale = d3.scale.linear(),
		xScalesVisible = [],
		yScale = d3.scale.linear(),
		xAxis = d3.svg.axis().scale(xScale).orient("bottom").tickSize(tickSize, 0),
		xAxesVisible = [],
		dataVisible = [],
		xAxisTitle = "",
		yValuesPerX = 1,
		xAxisTickLabels = [],
		yAxis = d3.svg.axis().scale(yScale).orient("left").tickSize(tickSize, 0),
		yAxisTitle = "",
		dispatch = d3.dispatch('showScatterTooltip', 'hideScatterTooltip', 'onScatterClick'),
		topLineClass = "topLineClass",
		topScatterClass = "topScatterClass",
		botLineClass = "botLineClass",
		botScatterClass = "botScatterClass",
		bandClass = "bandClass",
		topLineLabel = "",
		botLineLabel = "",
		areaLabel = "",
		breaks = []		// Each element of breaks specifies that the axes and traces restart start at that index

	function chart(selection) {
		selection.each(function (data) {

			// Convert data to standard representation greedily;
			// this is needed for nondeterministic accessors.
			var data2 = data.map(function (d, i) {
				return d.map(function (sd, j) {
					return [xValue.call(data[i], sd, j), yValue.call(data[i], sd, j), y0Value.call(data[i], sd, j), y1Value.call(data[i], sd, j), y2Value.call(data[i], sd, j)];
				});
			});

			data = data2;
			xAxesVisible = [];
			xScalesVisible = [];
			dataVisible = [];

			// Check breaks are legal
			for (var i = 0; i < breaks.length; i++)
				if ((breaks[i] < 1) ||
					(breaks[i] > (data[0].length / yValuesPerX - 1)))
					throw "Bad break of " + breaks[i] + " specified.";

			var maxX = d3.max(data, function (d) {
				return d3.max(d, function (sd) {
					// Check that all x-axis data is legal
					if (!isFinite(sd[0]))
						throw "Invalid x-axis data found.";
					return sd[0];
				});
			});

			var minX = d3.min(data, function (d) {
				return d3.min(d, function (sd) {
					return sd[0];
				});
			});

			var minYs = [], maxYs = [];					// Track the min and max of each y dataset
			var minFiniteYs = [], maxFiniteYs = [];		// Contains only the finite mins and maxes of the y datasets
			var lastYs = [];							// Contains the last finite value of each y dataset
			for (var iter = 1; iter <= 4; iter++) {
				var iterLastY = Infinity;
				var iterMinY = d3.min(data, function (d) {
					return d3.min(d, function (sd) {
						if (isFinite(sd[iter])) {
							iterLastY = sd[iter];		// Update last good Y value for the dataset
							return sd[iter];
						}
						else
							return Infinity;
					});
				});

				var iterMaxY = d3.max(data, function (d) {
					return d3.max(d, function (sd) {
						if (isFinite(sd[iter]))
							return sd[iter];
						else
							return -Infinity;
					});
				});

				minYs.push(iterMinY);
				maxYs.push(iterMaxY);
				lastYs.push(iterLastY);

				if (isFinite(iterMinY)) {
					// If there is a finite Min there should also be a finite max
					minFiniteYs.push(iterMinY);
					maxFiniteYs.push(iterMaxY);
				}
			}

			if (minFiniteYs.length == 0)
				throw "No valid y-axis data.";

			var minY = Math.min.apply(Math, minFiniteYs);
			var maxY = Math.max.apply(Math, maxFiniteYs);

			// Update the x-scale.
			xScale
				.domain([minX, maxX])
				.range([0, width - margin.left - margin.right]);

			// Select the svg element, if it exists.
			var svg = d3.selectAll("svg");

			// Otherwise, create the skeletal chart.
			if (svg.empty()) {
				var bandClassLabel = bandClass + "Label";
				svg = d3.select(this)
					.append("svg")
					.attr("width", width)
					.attr("height", height);

				if (isFinite(minYs[0]))
					svg.append("g")
						.attr("id", "linetracelabel0")
						.append("text")
							.attr('text-anchor', 'start')
							.text(topLineLabel.trimToPx(margin.right - 7))
							.attr("class", topScatterClass)

				if (isFinite(minYs[1]) && isFinite(minYs[2]))
					svg.append("g")
						.attr("id", "areatracelabel0")
						.append("text")
							.attr('text-anchor', 'start')
							.text(areaLabel.trimToPx(margin.right - 7))
							.attr("class", bandClassLabel)

				if (isFinite(minYs[3]))
					svg.append("g")
						.attr("id", "linetracelabel1")
						.append("text")
							.attr('text-anchor', 'start')
							.text(botLineLabel.trimToPx(margin.right - 7))
							.attr("class", botScatterClass)
			}

			// Update the x-values so they aren't too close together when drawn
			var labelsToShow = 1;	// By default, show all labels

			// Pack the data for SpaceValuesOut() call
			var theData = [];
			for (var i = 0; i < data[0].length; i += yValuesPerX)
				theData.push(data[0][i][0]);

			// Determine the minimum pixel width required for a vertically-oriented label
			var minTickLabelSize;
			// Attempt to use y-axis labels
			var labelNode = d3.select("#linetracelabel0").node();
			if (!labelNode)
				labelNode = d3.select("#areatracelabel0").node();
			if (!labelNode)
				labelNode = d3.select("#linetracelabel1").node();
			if (!labelNode)
				// This isn't as accurate
				minTickLabelSize = toString(xAxisTitle).visualHeight();
			else
				// Got one so use it
				minTickLabelSize = Math.floor(labelNode.getBBox().height);

			// Space the x-values out and get the minimum gap back
			var minGap = SpaceValuesOut(theData, xScale, minTickLabelSize);

			// Calculate how many labels to show
			labelsToShow = isFinite(minGap) ? Math.ceil(minTickLabelSize / minGap) : 1;

			// Unpack the data post SpaceValuesOut() call
			for (var i = 0; i < theData.length; i++)
				for (j = 0; j < yValuesPerX; j++)
					data[0][i * yValuesPerX + j][0] = theData[i];

			// Create visible x-scales
			var axisNum = 0;
			do {
				var minXIndex, maxXIndex;
				if (axisNum == 0)
					// This is the first axis, so it starts at the beginning
					minXIndex = 0;
				else
					// This is not the first axis, so it starts at the break point specified
					minXIndex = breaks[axisNum - 1];

				if (axisNum == breaks.length)
					// This is the last axis, so it ends at the last data point
					maxXIndex = data[0].length / yValuesPerX - 1;
				else
					// This is not the last axis, so it ends before the break point specified
					maxXIndex = breaks[axisNum] - 1;

				var minXCurrent = data[0][minXIndex * yValuesPerX][0];
				var maxXCurrent = data[0][maxXIndex * yValuesPerX][0];

				var tempScale = d3.scale.linear()
					.domain([minXCurrent, maxXCurrent])
					.range([xScale(minXCurrent), xScale(maxXCurrent)]);

				xScalesVisible.push(tempScale);

				// Now create the matching Axis
				var tempAxis = d3.svg.axis().scale(xScalesVisible[axisNum]).orient("bottom").tickSize(tickSize, 0);

				// Use custom x-axis tick values
				if (xAxisTickLabels.length != 0) {
					var xAxisTickValues = [];
					for (var i = minXIndex; i <= maxXIndex; i++) {
						xAxisTickValues.push(data[0][i * yValuesPerX][0]);
					}
					tempAxis.tickValues(xAxisTickValues);
				}
				xAxesVisible.push(tempAxis);

				// Create data subsets
				var tempDataSubset = [];
				tempDataSubset.push(data[0].slice(minXIndex * yValuesPerX, (maxXIndex + 1) * yValuesPerX));
				dataVisible.push(tempDataSubset);
			} while (axisNum++ < breaks.length)

			// Update the y-scale.
			yScale
				.domain([minY, maxY])
				.range([height - margin.top - margin.bottom, 0]);

			// Remove all of the traces before re-plotting them
			svg.selectAll("g.series").remove();

			for (axisNum = 0; axisNum < xScalesVisible.length; axisNum++) {
				var series = svg.selectAll("g.series.g" + axisNum)
					.data(dataVisible[axisNum]);

				var enterSeries = series.enter()
					.append("g")
					.attr("class", function (d, axisNum) { return "series trace" + axisNum; });

				// areatrace0
				var traceid = "areatrace0_" + axisNum;
				var areatrace0 = areaTrace()
					.y0(function (d) { return d[2]; })
					.y1(function (d) { return d[3]; })
					.xScale(xScale)
					.yScale(yScale);

				enterSeries.append("g")
					.attr("class", bandClass)
					.attr("id", traceid);

				series.select("#" + traceid)
					.datum(function (d) { return d; })
					.call(areatrace0);

				// linetrace1
				traceid = "linetrace1_" + axisNum;
				var linetrace1 = lineTrace()
					.y(function (d) { return d[4]; })
					.xScale(xScale)
					.yScale(yScale);

				enterSeries.append("g")
					.attr("class", botLineClass)
					.attr("id", traceid);

				series.select("#" + traceid)
					.datum(function (d) { return d; })
					.call(linetrace1);

				// linetrace0
				traceid = "linetrace0_" + axisNum;
				var linetrace0 = lineTrace()
					.y(function (d) { return d[1]; })
					.xScale(xScale)
					.yScale(yScale);

				enterSeries.append("g")
					.attr("class", topLineClass)
					.attr("id", traceid);

				series.select("#" + traceid)
					.datum(function (d) { return d; })
					.call(linetrace0);

				// scattertrace1
				traceid = "scattertrace1_" + axisNum;
				var scattertrace1 = scatterTrace()
					.y(function (d) { return d[4]; })
					.j(axisNum)
					.symbol("circle")
					.xScale(xScale)
					.yScale(yScale);

				enterSeries.append("g")
					.attr("class", botScatterClass)
					.attr("id", traceid);

				series.select("#" + traceid)
					.datum(function (d) { return d; })
					.call(scattertrace1);

				// scattertrace0
				traceid = "scattertrace0_" + axisNum;
				var scattertrace0 = scatterTrace()
					.y(function (d) { return d[1]; })
					.j(axisNum)
					.symbol("circle")
					.xScale(xScale)
					.yScale(yScale);

				enterSeries.append("g")
					.attr("class", topScatterClass)
					.attr("id", traceid);

				series.select("#" + traceid)
					.datum(function (d) { return d; })
					.call(scattertrace0);

				series
					.exit()
					.remove();

				// Hook up to dispatch events
				scattertrace0
					.dispatch.on('showTooltip', function (e) {
						dispatch.showScatterTooltip({
							className: e.className,
							i: e.i,
							j: e.j,
							x: e.x + margin.left,
							y: e.y + margin.top
						});
					});

				scattertrace0
					.dispatch.on('hideTooltip', function (e) {
						dispatch.hideScatterTooltip({});
					});

				scattertrace0
					.dispatch.on('click', function (e) {
						dispatch.onScatterClick({
							className: e.className,
							i: e.i,
							j: e.j,
							x: e.x + margin.left,
							y: e.y + margin.top
						});
					});

				scattertrace1
					.dispatch.on('showTooltip', function (e) {
						dispatch.showScatterTooltip({
							className: e.className,
							i: e.i,
							j: e.j,
							x: e.x + margin.left,
							y: e.y + margin.top
						});
					});

				scattertrace1
					.dispatch.on('hideTooltip', function (e) {
						dispatch.hideScatterTooltip({});
					});

				scattertrace1
					.dispatch.on('click', function (e) {
						dispatch.onScatterClick({
							className: e.className,
							i: e.i,
							j: e.j,
							x: e.x + margin.left,
							y: e.y + margin.top
						});
					});
			}

			// Update the inner dimensions
			svg.selectAll("g.series")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

			// Remove and recreate x-axes each time so that we can change tick text properly
			svg.select("g.x.axisset").remove();
			svg.select("g.y.axisset").remove();

			// Append a set of x axes to the chart
			var xAxisSet = svg.append("g")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")")
					.attr("class", "x axisset");

			for (axisNum = 0; axisNum < xScalesVisible.length; axisNum++) {
				var thisAxis = xAxisSet.append("g")
					.attr("class", "x axis g" + axisNum)
					.attr("transform", "translate(0," + yScale.range()[0] + ")")
					.attr("y", (margin.bottom / 1.5))
					.call(xAxesVisible[axisNum]);

				if (axisNum == 0)
					thisAxis.append("text")
						.text(xAxisTitle)
						.attr("id", "xAxisTitle")
			}

			// Append a single y axis for the chart
			svg.append("g")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")")
					.attr("class", "y axisset")
				.append("g")
					.attr("class", "y axis")
				.append("text")		// Add axis title
					.text(yAxisTitle)
					.attr("id", "yAxisTitle")
					.attr('text-anchor', 'middle')
					.attr("transform", "rotate (-90, -" + (margin.left - 7) + ", -5) translate(-" + ((height / 2)) + ")");

			// Update the y-axis
			svg.select("g.y.axis")
				.call(yAxis);

			// Position y-axis title
			d3.select("#yAxisTitle")
				.attr("transform", "rotate (-90, -" + (margin.left - 7) + ", -5) translate(-" + ((height / 2)) + ")");

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

			// Position trace labels
			var minSpacing = 13;
			var areay = margin.top + yScale(lastYs[2]);
			var trace1y = margin.top + yScale(lastYs[0]);
			var trace2y = margin.top + yScale(lastYs[3]);
			var gap = Math.abs(trace1y - trace2y);

			if (gap < minSpacing) {
				// Labels are too close together
				if (trace1y > trace2y) {
					trace1y += (minSpacing - gap) / 2;
					trace2y -= (minSpacing - gap) / 2;
				}
				else {
					trace1y -= (minSpacing - gap) / 2;
					trace2y += (minSpacing - gap) / 2;
				}
			}

			d3.select("#linetracelabel0")
				.attr("transform", "translate(" + (width - margin.right + 7) + "," + trace1y + ")")

			d3.select("#linetracelabel1")
				.attr("transform", "translate(" + (width - margin.right + 7) + "," + trace2y + ")")

			d3.select("#areatracelabel0")
				.attr("transform", "translate(" + (width - margin.right + 7) + "," + areay + ")")
		});
	}

	chart.dispatch = dispatch;

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

	chart.x = function (_) {
		if (!arguments.length) return xValue;
		xValue = _;
		return chart;
	};
	chart.y = function (_) {
		if (!arguments.length) return yValue;
		yValue = _;
		return chart;
	};
	chart.y0 = function (_) {
		if (!arguments.length) return y0Value;
		y0Value = _;
		return chart;
	};
	chart.y1 = function (_) {
		if (!arguments.length) return y1Value;
		y1Value = _;
		return chart;
	};
	chart.y2 = function (_) {
		if (!arguments.length) return y2Value;
		y2Value = _;
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
	chart.yAxisTitle = function (_) {
		if (!arguments.length) return yAxisTitle;
		yAxisTitle = _;
		return chart;
	};
	chart.xAxisTickLabels = function (_) {
		if (!arguments.length) return xAxisTickLabels;
		xAxisTickLabels = _;
		return chart;
	};
	chart.yValuesPerX = function (_) {
		if (!arguments.length) return yValuesPerX;
		yValuesPerX = _;
		return chart;
	};

	chart.topLineClass = function (_) {
		if (!arguments.length) return topLineClass;
		topLineClass = _;
		return chart;
	};
	chart.topScatterClass = function (_) {
		if (!arguments.length) return topScatterClass;
		topScatterClass = _;
		return chart;
	};
	chart.botLineClass = function (_) {
		if (!arguments.length) return botLineClass;
		botLineClass = _;
		return chart;
	};
	chart.botScatterClass = function (_) {
		if (!arguments.length) return botScatterClass;
		botScatterClass = _;
		return chart;
	};
	chart.bandClass = function (_) {
		if (!arguments.length) return bandClass;
		bandClass = _;
		return chart;
	};
	chart.topLineLabel = function (_) {
		if (!arguments.length) return topLineLabel;
		topLineLabel = _;
		return chart;
	};
	chart.botLineLabel = function (_) {
		if (!arguments.length) return botLineLabel;
		botLineLabel = _;
		return chart;
	};
	chart.areaLabel = function (_) {
		if (!arguments.length) return areaLabel;
		areaLabel = _;
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

	return chart;
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/