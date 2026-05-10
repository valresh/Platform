// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_line_trace.js

function lineTrace() {
	var xValue = function (d) { return d[0]; },
		yValue = function (d) { return d[1]; },
		xScale = d3.time.scale(),
		yScale = d3.scale.linear(),
		xyLine = d3.svg.line().x(X).y(Y),
		dispatch = d3.dispatch('showTooltip', 'hideTooltip');
 
	function trace(selection) {
		selection.each(function (data) {
			// Convert data to standard representation greedily;
			// this is needed for nondeterministic accessors.
			var NaNPresent = false;
			var data2 = data.map(function (d, i) {
				if (!isFinite(d[0]) || !isFinite(d[1]))
					NaNPresent = true;
				return [xValue.call(data, d, i), yValue.call(data, d, i)];
			});

			if (NaNPresent)
				// Don't draw the line unless all values were finite (and therefore plotable)
				return;

			data = data2;

			var line = d3.select(this).selectAll(".line")
				.data([data]);

			line
				.enter()
				.append("path")
				.attr("class", "line")
				.attr("d", xyLine)

			line
				.attr("d", xyLine)

			line
				.exit()
				.remove();
		});
	}

	trace.dispatch = dispatch;

	function X(d) {
		return xScale(d[0]);
	}

	function Y(d) {
		return yScale(d[1]);
	}

	trace.x = function(_) {
		if (!arguments.length) return xValue;
		xValue = _;
		return trace;
	};
	trace.y = function (_) {
		if (!arguments.length) return yValue;
		yValue = _;
		return trace;
	};
	trace.xScale = function (_) {
	    if (!arguments.length) return xScale;
		xScale = _;
		return trace;
	};
	trace.yScale = function(_) {
		if (!arguments.length) return yScale;
		yScale = _;
		return trace;
	};

	return trace;
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/