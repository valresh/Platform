// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_scatter_trace.js

function scatterTrace() {
	var xValue = function (d) { return d[0]; },
		yValue = function (d) { return d[1]; },
		jValue = 0,		// An additional id to put on box elements and return in events
		xScale = d3.time.scale(),
		yScale = d3.scale.linear(),
		dispatch = d3.dispatch('showTooltip', 'hideTooltip', 'click'),
		symbol = "circle",
		symbolSize = 50,
		finalSymbolSize = 50,
		mouseOverFactor = 2,
		finalClass = "";

	function trace(selection) {
		selection.each(function (data) {

			// Convert data to standard representation greedily;
			// this is needed for nondeterministic accessors.
			var data2 = data.map(function (d, i) {
				return [xValue.call(data, d, i), yValue.call(data, d, i)];
			});

			data = data2;

			var symbols = d3.select(this).selectAll(".data-point")
				.data(data);

			var dotCount = 0;
			symbols
				.enter().append("path")
				.attr("transform", function (d) { 
					return "translate(" + X(d) + "," + Y(d) + ")"; })
				.attr("i", function (d) { return dotCount++ })
				.attr("j", jValue)
				.attr("class", function () { 
					return ((parseInt(d3.select(this).attr("i")) + 1) == data.length) ? finalClass : "";
				})
				.attr("display", function (d) {
					return (!isFinite(d[0]) || !isFinite(d[1])) ? "none" : "";
				})
				.attr("d", d3.svg.symbol().type(symbol).size(function () {
					return ((parseInt(d3.select(this).attr("i")) + 1) == data.length) ? finalSymbolSize : symbolSize;
				}))


			symbols
				.attr("transform", function (d) { 
				return "translate(" + X(d) + "," + Y(d) + ")"; });
			symbols
				.exit()
				.remove();

			symbols
				.on("mouseover", function (d) {
					d3.select(this)
						.transition()
						.attr("d", d3.svg.symbol().type(symbol).size((((parseInt(d3.select(this).attr("i")) + 1) == data.length) ? finalSymbolSize : symbolSize ) * mouseOverFactor));
				})
				.on("mouseout", function (d) {
					d3.select(this)
						.transition()
						.attr("d", d3.svg.symbol().type(symbol).size(((parseInt(d3.select(this).attr("i")) + 1) == data.length) ? finalSymbolSize : symbolSize));
				})
				.on("mouseover.tooltip", function (d) {
					dispatch.showTooltip({
						className: this.parentNode.className.baseVal,
						i: d3.select(this).attr("i"),
						j: d3.select(this).attr("j"),
						x: d3.mouse(this.parentNode)[0],
						y: d3.mouse(this.parentNode)[1]
					});
				})
				.on("mouseout.tooltip", function (d) {
					dispatch.hideTooltip({});
				})
				.on("click", function (d) {
					dispatch.click({
						className: this.parentNode.className.baseVal,
						i: d3.select(this).attr("i"),
						j: d3.select(this).attr("j"),
						x: d3.mouse(this.parentNode)[0],
						y: d3.mouse(this.parentNode)[1]
					});
				});
		});
	}

	trace.dispatch = dispatch;

	function X(d) {
		return xScale(d[0]);
	}

	function Y(d) {
		return yScale(d[1]);
	}

	trace.x = function (_) {
		if (!arguments.length) return xValue;
		xValue = _;
		return trace;
	};

	trace.y = function (_) {
		if (!arguments.length) return yValue;
		yValue = _;
		return trace;
	};

	trace.j = function (_) {
		if (!arguments.length) return jValue;
		jValue = _;
		return trace;
	};

	trace.xScale = function (_) {
		if (!arguments.length) return xScale;
		xScale = _;
		return trace;
	};

	trace.yScale = function (_) {
		if (!arguments.length) return yScale;
		yScale = _;
		return trace;
	};

	trace.symbol = function (_) {
		if (!arguments.length) return symbol;
		symbol = _;
		return trace;
	};

	trace.symbolSize = function (_) {
		if (!arguments.length) return symbolSize;
		symbolSize = _;
		return trace;
	};

	trace.finalSymbolSize = function (_) {
		if (!arguments.length) return finalSymbolSize;
		finalSymbolSize = _;
		return trace;
	};

	trace.mouseOverFactor = function (_) {
		if (!arguments.length) return mouseOverFactor;
		mouseOverFactor = _;
		return trace;
	};

	trace.finalClass = function (_) {
		if (!arguments.length) return finalClass;
		finalClass = _;
		return trace;
	};

	return trace;
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/