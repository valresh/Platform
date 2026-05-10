// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_area_trace.js

function areaTrace() {
	var xValue = function (d) { return d[0]; },
		y0Value = function (d) { return d[1]; },
		y1Value = function (d) { return d[2]; },
		xScale = d3.time.scale(),
		yScale = d3.scale.linear(),
		xyArea = d3.svg.area().x(X).y0(Y0).y1(Y1)
		dispatch = d3.dispatch('showTooltip', 'hideTooltip', 'click')
		;

	function trace(selection) {
		selection.each(function(data) {

			// Convert data to standard representation greedily;
			// this is needed for nondeterministic accessors.
			var data2 = data.map(function (d, i) {
				return [xValue.call(data, d, i), y0Value.call(data, d, i), y1Value.call(data, d, i)];
			});

			data = data2;

			var line = d3.select(this).selectAll(".area")
				.data([data]);

			line
				.enter()
				.append("path").attr("class", "area")
				.attr("d", xyArea)
			
			line
				.attr("d", xyArea)

			line
				.exit()
				.remove();

			line
				.on("mousemove", function (d) {
					dispatch.hideTooltip({});
					xPos = xScale.invert(d3.mouse(this)[0]);
					yPos = yScale.invert(d3.mouse(this)[1]);
					dispatch.showTooltip({
						className: this.parentNode.className.baseVal,
						time: xPos,
						x: d3.mouse(this)[0],
						y: d3.mouse(this)[1]
					});
				})
				.on("mouseover.tooltip", function (d) {
					xPos = xScale.invert(d3.mouse(this)[0]);
					yPos = yScale.invert(d3.mouse(this)[1]);
					dispatch.showTooltip({
						className: this.parentNode.className.baseVal,
						time: xPos,
						x: d3.mouse(this)[0],
						y: d3.mouse(this)[1]
					});
				})
				.on("mouseout.tooltip", function (d) {
					dispatch.hideTooltip({});
				})
				.on("click", function (d) {
					xPos = xScale.invert(d3.mouse(this)[0]);
					yPos = yScale.invert(d3.mouse(this)[1]);
					dispatch.click({
						className: this.parentNode.className.baseVal,
						time: xPos,
						x: d3.mouse(this)[0],
						y: d3.mouse(this)[1]
					});
				});
		});
	}

	trace.dispatch = dispatch;

	function X(d) {
		return xScale(d[0]);
	}

	function Y0(d) {
		return yScale(d[1]);
	}

	function Y1(d) {
		return yScale(d[2]);
	}

	trace.x = function (_) {
		if (!arguments.length) return xValue;
		xValue = _;
		return trace;
	};

	trace.y0 = function (_) {
		if (!arguments.length) return y0Value;
		y0Value = _;
		return trace;
	};

	trace.y1 = function (_) {
		if (!arguments.length) return y1Value;
		y1Value = _;
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