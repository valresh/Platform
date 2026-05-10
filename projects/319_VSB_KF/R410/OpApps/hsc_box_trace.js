// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_box_trace.js

function boxTrace() {
	var xValue = function (d) { return d[0]; },
		yValue = function (d) { return d[1]; },
		y0Value = function (d) { return d[1] - 5; },
		y1Value = function (d) { return d[1] + 5; },
		colorNumber = function () { return 0; },
		jValue = 0,		// An additional id to put on box elements and return in events
		xScale = d3.time.scale(),
		yScale = d3.scale.linear(),
		baseClassName = "OpAppsBoxColor",
		minBoxPixelHeight = 4,
		width = function () { return 10; }
		dispatch = d3.dispatch('showTooltip', 'hideTooltip', 'click')

	function trace(selection) {
		selection.each(function(data) {

			// Convert data to standard representation greedily;
			// this is needed for nondeterministic accessors.
			var data2 = data.map(function(d, i) {
				return [xValue.call(data, d, i), yValue.call(data, d, i), y0Value.call(data, d, i), y1Value.call(data, d, i), width.call(data, d, i), colorNumber.call(data, d, i)];
			});

			data = data2;
			
			var boxes = d3.select(this).selectAll(".box")
				.data(data);
			var boxCount = 0;

			boxes
				.enter()
				.append("rect")
				.attr("class", function (d) { return "box " + baseClassName + CN(d); })
				.attr("x", function (d) { return X(d) - W(d) / 2; })
				.attr("y", function (d) {
					var actualHeight = Y1(d) - Y0(d);
					if (actualHeight <= minBoxPixelHeight)
						return Y0(d) - (minBoxPixelHeight - actualHeight) / 2;
					else
						return Y0(d);

				})
				.attr("height", function (d) {
					var actualHeight = Y1(d) - Y0(d);
					if (actualHeight <= minBoxPixelHeight)
						return minBoxPixelHeight;
					else
						return actualHeight;
				})
				.attr("i", function (d) { return boxCount++ })
				.attr("j", jValue);

			boxes
				.attr("class", function (d) { return "box " + baseClassName + CN(d); })
				.attr("x", function (d) { return X(d) - W(d) / 2; })
				.attr("y", function (d) {
					var actualHeight = Y1(d) - Y0(d);
					if (actualHeight <= minBoxPixelHeight)
						return Y0(d) - (minBoxPixelHeight - actualHeight) / 2;
					else
						return Y0(d);

				})
				.attr("width", W)
				.attr("height", function (d) {
					var actualHeight = Y1(d) - Y0(d);
					if (actualHeight <= minBoxPixelHeight)
						return minBoxPixelHeight;
					else
						return actualHeight;
				})

			boxes
				.exit()
				.remove();
			
			boxes
				.on("mouseover", function (d) {
				})
				.on("mouseout", function (d) {
				})
				.on("mouseover.tooptip", function (d) {
					dispatch.showTooltip({
						className: this.parentNode.className.baseVal,
						i: d3.select(this).attr("i"),
						j: d3.select(this).attr("j"),
						x: d3.mouse(this)[0],
						y: d3.mouse(this)[1]
					});
				})
				.on("mouseout.tooltip", function (d) {
					dispatch.hideTooltip({});
				})
				.on("click", function (d) {
					dispatch.click({
						className: this.parentNode.className.baseVal,
						i: d3.select(this).attr("i"),
						j: d3.select(this).attr("j")
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

	function Y0(d) {
		return yScale(d[2]);
	}

	function Y1(d) {
		return yScale(d[3]);
	}

	function W(d) {
		return +d[4];
	}

	function CN(d) {
		return +d[5];
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

	trace.j = function (_) {
		if (!arguments.length) return jValue;
		jValue = _;
		return trace;
	};

	trace.width = function (_) {
		if (!arguments.length) return width;
		width = _;
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

	trace.colorNumber = function (_) {
		if (!arguments.length) return colorNumber;
		colorNumber = _;
		return trace;
	};

	trace.baseClassName = function (_) {
		if (!arguments.length) return baseClassName;
		baseClassName = _;
		return trace;
	};

	return trace;
}

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/