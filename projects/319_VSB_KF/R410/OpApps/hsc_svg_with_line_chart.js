// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_svg_with_line_chart.js

function svgWithLineChart() {
	var tickSize = 6,
		maxXAxisTickLabelSize = 35,
		margin = { top: 20, right: 50, bottom: 50, left: 50 },
		width = 400,
		height = 300,
		xValue = function (d) { return d[0]; },
		yValue = function (d) { return d[1]; },
		xScale = d3.scale.linear(),
		yScale = d3.scale.linear(),
		xAxis = d3.svg.axis().scale(xScale).orient("bottom").tickSize(tickSize, 0),
		xAxisTitle = "",
		xAxisTickLabels = [],
		yAxis = d3.svg.axis().scale(yScale).orient("left").tickSize(tickSize, 0),
		yAxisTitle = "",
		dispatch = d3.dispatch('showScatterTooltip', 'hideScatterTooltip', 'onScatterClick'),
		lineClass = "LineClass",
		scatterClass = "ScatterClass",
		finalScatterClass = "FinalScatterClass",
		theSVG = "",
		chartDivId = "",
		svgMinX = 0,
		svgMaxX = 100,
		svgMinY = 0,
		svgMaxY = 100,
		xAxisGap = 10,
		yAxisGap = 10,
		drawLine = true
	;


	function chart(selection) {
		selection.each(function (data) {

			// Convert data to standard representation greedily;
			// this is needed for nondeterministic accessors.
			var data2 = data.map(function (d, i) {
				return d.map(function (sd, j) {
					return [xValue.call(data[i], sd, j), yValue.call(data[i], sd, j)];
				});
			});

			data = data2;
			
			var dataMaxX = d3.max(data, function (d) {
				return d3.max(d, function (sd) {
					return isFinite(sd[0]) ? sd[0] : -Infinity;
				});
			});

			var dataMinX = d3.min(data, function (d) {
				return d3.min(d, function (sd) {
					return isFinite(sd[0]) ? sd[0] : Infinity;
				});
			});
			
			if (!isFinite(dataMaxX) || !isFinite(dataMinX))
				throw "Invalid x-axis data found.";

			var dataMaxY = d3.max(data, function (d) {
				return d3.max(d, function (sd) {
					return isFinite(sd[1]) ? sd[1] : -Infinity;
				});
			});

			var dataMinY = d3.min(data, function (d) {
				return d3.min(d, function (sd) {
					return isFinite(sd[1]) ? sd[1] : Infinity;
				});
			});

			if (!isFinite(dataMaxY) || !isFinite(dataMinY))
				throw "Invalid y-axis data found.";

			var minX = Math.min(dataMinX, svgMinX);
			var maxX = Math.max(dataMaxX, svgMaxX);
			var minY = Math.min(dataMinY, svgMinY);
			var maxY = Math.max(dataMaxY, svgMaxY);

			if ((dataMaxX > 0.0 && dataMinX > 0.0) || (dataMaxX < 0.0 && dataMinX < 0.0))
				xAxisGap = 10;
			else
				xAxisGap = 0;

			if ((dataMaxY > 0.0 && dataMinY > 0.0) || (dataMaxY < 0.0 && dataMinY < 0.0))
				yAxisGap = 10;
			else
				yAxisGap = 0;

			// Update the x-scale.
			xScale
				.domain([minX, maxX])
				.range([xAxisGap, width - margin.right]);

			// Update the y-scale.
			yScale
				.domain([minY, maxY])
				.range([height - margin.bottom - yAxisGap, 0]);

			$(chartDivId).html("");

			// Select the svg element, if it exists.
			var svg = d3.selectAll("svg");
			
			var NewStuff = false;
			// Otherwise, create the skeletal chart.
			if (svg.empty()) {
				NewStuff = true;
				$(chartDivId).html(theSVG);

				svg = d3.select("svg")
					.data(data)
					.attr("width", width)
					.attr("height", height);

				// Give the svg within the group a name?
				svg.select("g")
					.classed("background", true);

				// Append a single x axis for the chart
				svg.append("g")
					.attr("transform", "translate(" + margin.left + "," + margin.top + ")")
						.attr("class", "x axisset")
					.append("g")
						.attr("class", "x axis")
					.append("text")		// Add axis title
						.text(xAxisTitle)
						.attr("id", "xAxisTitle")

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
			}

			// Resize background svg group
			var BBox = d3.select("g.background")[0][0].getBBox();
			var xFactor = (width - xAxisGap - margin.right) / BBox.width * (svgMaxX - svgMinX) / (maxX - minX);
			var yFactor = (height - yAxisGap - margin.bottom) / BBox.height * (svgMaxY - svgMinY) / (maxY - minY);
			d3.select("g.background").attr("transform", "translate(" + (margin.left + xScale(svgMinX)) + "," + (margin.top + yScale(svgMaxY)) + ") scale(" + xFactor + "," + yFactor + ")");

			// Remove and recreate x-axis each time so that we can change tick text properly
			svg.select("g.series trace0").remove();

			var series = svg.selectAll("g.series")
				.data(data);

			var enterSeries = series.enter()
				.append("g")
				.attr("class", function (d, i) { return "series trace" + i; });

			if (drawLine) {
				// linetrace0
				traceid = "linetrace0";
				var linetrace0 = lineTrace()
					.y(function (d) { return d[1]; })
					.xScale(xScale)
					.yScale(yScale);

				enterSeries.append("g")
					.attr("class", lineClass)
					.attr("id", traceid);

				series.select("#" + traceid)
					.datum(function (d) { return d; })
					.call(linetrace0);
			}

			// scattertrace0
			traceid = "scattertrace0";
			var scattertrace0 = scatterTrace()
				.y(function (d) { return d[1]; })
				.symbol("circle")
				.xScale(xScale)
				.yScale(yScale)
				.symbolSize(50)
				.finalSymbolSize(200)
				.finalClass(finalScatterClass);

			enterSeries.append("g")
				.attr("class", scatterClass)
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
						x: e.x + margin.left,
						y: e.y + margin.top
					});
				});

			// Update the inner dimensions.
			svg.selectAll("g.series")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

			// Update the x-axis
			svg.select("g.x.axis")
				.attr("transform", "translate(0," + yScale.range()[0] + ")")
				.attr("x", (width / 2) - margin.left)
				.attr("y", (margin.bottom / 1.5))
				.call(xAxis);

			// Update the y-axis.
			svg.select("g.y.axis")
				.call(yAxis);

			// Position y-axis title
			d3.select("#yAxisTitle")
				.attr("transform", "rotate (-90, -" + (margin.left) + ", -5) translate(-" + ((height / 2)) + ")");

			// Position x-axis title
			d3.select("#xAxisTitle")
				.attr("x", (width / 2) - margin.left)
				.attr("y", margin.bottom)
				.attr('dy', '0.7em');
		});
	}

	chart.dispatch = dispatch;

	function X(d) {
		return xScale(d[0]);
	}

	function Y(d) {
		return yScale(d[1]);
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
	chart.svgMinX = function (_) {
		if (!arguments.length) return svgMinX;
		svgMinX = _;
		return chart;
	};
	chart.svgMaxX = function (_) {
		if (!arguments.length) return svgMaxX;
		svgMaxX = _;
		return chart;
	};
	chart.svgMinY = function (_) {
		if (!arguments.length) return svgMinY;
		svgMinY = _;
		return chart;
	};
	chart.svgMaxY = function (_) {
		if (!arguments.length) return svgMaxY;
		svgMaxY = _;
		return chart;
	};
	chart.y = function (_) {
		if (!arguments.length) return yValue;
		yValue = _;
		return chart;
	};
	chart.svg = function (_) {
		if (!arguments.length) return theSVG;
		theSVG = _;
		return chart;
	};
	chart.chartDivId = function (_) {
		if (!arguments.length) return chartDivId;
		chartDivId = _;
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

	chart.lineClass = function (_) {
		if (!arguments.length) return lineClass;
		lineClass = _;
		return chart;
	};
	chart.scatterClass = function (_) {
		if (!arguments.length) return scatterClass;
		scatterClass = _;
		return chart;
	};
	chart.finalScatterClass = function (_) {
		if (!arguments.length) return finalScatterClass;
		finalScatterClass = _;
		return chart;
	};

	chart.drawLine = function (_) {
		if (!arguments.length) return drawLine;
		drawLine = _;
		return chart;
	};

	chart.ClearChart = function () {
		var svg = d3.select("svg").remove();
		return;
	};

	return chart;
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/