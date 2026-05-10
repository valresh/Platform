// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// hsc_dualbox_chart.js


function multiDualLineChart() {
    var tickSize = 6,
        maxXAxisTickLabelSize = 135,
        margin = { top: 20, right: 20, bottom: 50, left: 50 },
        colors = ["#F00", "#0F0", "#00F", "#800", "#080", "#008"],
        width = 400,
        height = 300,       
        xValue = function (d) { return d[0]; },
        yValue = function (d) { return d[1]; },
        y0Value = function (d) { return d[2]; },        
        xScale = d3.time.scale(),
        xScalesVisible = [],
        yScale = d3.scale.linear(),
        xAxis = d3.svg.axis().scale(xScale).orient("bottom").tickSize(tickSize, 0),
        xAxesVisible = [],
        dataVisible = [],
        xAxisTitle = "",
        xAxisTickLabels = [],
        yAxis = d3.svg.axis().scale(yScale).orient("left").tickSize(tickSize, 0),
        yAxisTitle = "",
        dispatch = d3.dispatch('showTooltip', 'hideTooltip', 'click'),
        xAxisGap = 10,
        yAxisGap = 10,
        cursor = "inherit",
        breaks = [],
        graphName = "plot",
        hideLegend = false,
        hideTitle = false;
        
    // Global objects
    var displayLegend = "totalLegend",
        legendEntryText = "Run",
        plotData = "",
        referenceKeys = [];

    // Global display settings
    var displayRectPadding = 2,
        stackpadding = 5,
        tolerance = 3;
        
    // Global variable names defaults
    // svg selection will use these identies through text.RefCircle, circle.RefCircle, rect.RefCircle
    var yCircleClass = "RefCircle", 
        yTextClass = "RefText",
        yRectClass = "RefRect",
        xRefClass = "RefTime";
    
        

    function chart(selection) {

        LogMessage(L_PARAM, src, "In multi dual line charts function");
        selection.each(function (data) {        
            
            if(data.length > 0 )
                {
                // Our Reference Line and Legends are not able to handle array or arrays.
                // Create a single array of object instance
                plotData = JSON.parse(JSON.stringify(data[0]));
                
                // Setup global object that managed a set of generic element names for each plot key.
                // These names will be used to identify elements throughout this script
                referenceKeys = [];         
                for (var i = 0; i < plotRef.Keys.length; i++)
                {
                    // Read as: Each Pressure Plot Chart renders two Traces
                    referenceKeys.push("Trace"+i);
                }

                // Convert data to standard representation greedily;
                // this is needed for nondeterministic accessors.
                // also note, that D3 accepts only data in a certain fashion
                var data2 = data.map(function (d, i) {
                    return d.map(function (sd, j) {
                        return [xValue.call(data[i], sd, j), yValue.call(data[i], sd, j), y0Value.call(data[i], sd, j)];
                    });
                });
                
                data = data2;
                xAxesVisible = [];          
                xScalesVisible = [];        
                dataVisible = [];           

                /*
                 * Validate Data
                 */
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


                var minYs = [], maxYs = [];                 // Track the min and max of each y dataset
                var minFiniteYs = [], maxFiniteYs = [];     // Contains only the finite mins and maxes of the y datasets
                var lastYs = [];                            // Contains the last finite value of each y dataset
                for (var iter = 1; iter <= 4; iter++) {
                    var iterLastY = Infinity;
                    var iterMinY = d3.min(data, function (d) {
                        return d3.min(d, function (sd) {
                            if (isFinite(sd[iter])) {
                                iterLastY = sd[iter];       // Update last good Y value for the dataset
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

                var xAxisPadding = 0;
                if (xAxisTickLabels.length != 0) {
                    // Use custom x-axis tick values
                    xScale = d3.scale.linear();
                    xAxis = d3.svg.axis().scale(xScale).orient("bottom").tickSize(tickSize, 0);
                }
                else {
                    xAxisGap = (width - margin.left - margin.right) / data[0].length / 2;
                    xAxisGap = 0;
                    xAxisPadding = (data[0][1][0] - data[0][0][0]) / 2;
                    minX = new Date(minX.getTime() - xAxisPadding);
                    maxX = new Date(maxX.getTime() + xAxisPadding);
                }
                
                // Update the x-scale.
                xScale
                    .domain([minX, maxX])
                    .range([xAxisGap, width - margin.left - margin.right]);

                var svg = $(".svg");
                
                // Otherwise, create the skeletal chart.
                
                svg = d3.select(this)
                    .append("svg")
                    .attr("width", width)
                    .attr("height", height);
            

                // Update the x-values so they aren't too close together when drawn
                var labelsToShow = 1;   // By default, show all labels

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
                        maxXIndex = data[0].length - 1;
                    else
                        // This is not the last axis, so it ends before the break point specified
                        maxXIndex = breaks[axisNum] - 1;

                    var minXCurrent = data[0][minXIndex][0];
                    var maxXCurrent = data[0][maxXIndex][0];

                    var end = new Date(maxXCurrent);
                    var start = new Date(minXCurrent);
                    
                    var interval = (end.getTime() - start.getTime()) / 60000;
                    var tempScale = d3.scale.linear();
                
                    // Domain denotes the values displayed on the screen ir: 0mins to xmins
                    tempScale.domain([0, interval])
                        .range([xScale(minXCurrent), xScale(maxXCurrent)])
                    xScalesVisible.push(tempScale);
                
                    var tempAxis = d3.svg.axis()
                        .scale(tempScale)
                        .orient("bottom")
                        .tickFormat(d3.format("02d mins"));
        
                    // Use custom x-axis tick values
                    if (xAxisTickLabels.length != 0) {
                        var xAxisTickValues = [];
                        for (var i = minXIndex; i <= maxXIndex; i++) {
                            xAxisTickValues.push(data[0][i][0]);
                        }
                        tempAxis.tickValues(xAxisTickValues);
                    }
                    xAxesVisible.push(tempAxis);
                
                    // Create data subsets // ??
                    var tempDataSubset = [];
                    tempDataSubset.push(data[0].slice(minXIndex, (maxXIndex + 1)));
                    dataVisible.push(tempDataSubset);
                } while (axisNum++ < breaks.length)

                // Update the y-scale.
                yScale
                    .domain([minY, maxY])
                    .range([height - margin.top - margin.bottom/2 - yAxisGap, 0]);

                // Remove all of the traces before re-plotting them
                svg.selectAll("g.series").remove();

                for (axisNum = 0; axisNum < xScalesVisible.length; axisNum++) {
                    
                    var series = svg.selectAll("g.series.g" + axisNum)
                        .data(dataVisible[axisNum]);

                    var enterSeries = series.enter()
                        .append("g")
                        .attr("class", function (d, axisNum) { return "series trace" + axisNum; });

                    // Plot 1 
                    if (plotRef.RunOne.enable) {
                        // Plot 1 LineTrace
                        traceid = "linetrace1_" + axisNum;
                        var linetrace1 = lineTrace()
                            .y(function (d) { return d[1]; })
                            .xScale(xScale)
                            .yScale(yScale);

                        enterSeries
                            .append("g")
                            .attr("id", traceid)
                            .attr("style", "cursor: " + cursor)
                            .attr("class", "DifferentialPressuePlotOne");

                        series.select("#" + traceid)
                            .datum(function (d) { return d; })
                            .call(linetrace1);

                        series
                            .exit()
                            .remove();
                    }

                    if (plotRef.RunTwo.enable) {
                        // Plot 2 LineTrace
                        traceid = "linetrace2_" + axisNum;
                        var linetrace2 = lineTrace()
                            .y(function (d) { return d[2]; })
                            .xScale(xScale)
                            .yScale(yScale);

                        enterSeries
                            .append("g")
                            .attr("id", traceid)
                            .attr("style", "cursor: " + cursor)
                            .attr("class", "DifferentialPressuePlotTwo");

                        series.select("#" + traceid)
                            .datum(function (d) { return d; })
                            .call(linetrace2);

                        series
                            .exit()
                            .remove();
                    }
                }

                // Remove and recreate x-axes each time so that we can change tick text properly
                svg.select("g.x.axisset").remove();
                svg.select("g.y.axisset").remove();

                // Append a set of x axes to the chart 
                var xAxisSet = svg.append("g")
                    .attr("transform", "translate(" + margin.left + "," + (margin.top + yAxisGap) + ")")
                        .attr("class", "x axisset");

           
                    var thisAxis = xAxisSet.append("g")
                        .attr("class", "x axis g")
                        .attr("transform", "translate(0," + yScale.range()[0] + ")")
                        .attr("y", (margin.bottom / 1))
                        .call(xAxesVisible[0]);

                    var title = thisAxis.append("text")                     
                        .attr("id", "xAxisTitle")
                        .attr("x", 0);

                    if(hideTitle != true)
                        title
                            .text(xAxisTitle);
              

                // Append a single y axis for the chart
                svg.append("g")
                    .attr("transform", "translate(" + margin.left + "," + margin.top + ")")
                        .attr("class", "y axisset")
                    .append("g")
                        .attr("class", "y axis")
                    .append("text")     // Add axis title
                        .text(yAxisTitle)
                        .attr("id", "yAxisTitle")
                        .attr('text-anchor', 'middle')
                        .attr("transform", "rotate (-90, -" + (margin.left - 7) + ", -5) translate(-" + ((height / 2)) + ")");

                // Update the inner dimensions.
                svg.selectAll("g.series")
                    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

                // Update the y-axis.
                svg.select("g.y.axis")
                    .call(yAxis);
                
                var yAxisTitlePosition = (d3.select("#yAxisTitle").node().getBBox().width)/2;
                
                // Position y-axis title
                svg.select("#yAxisTitle")
                    .attr("transform", "rotate (-90, -" + (margin.left - 7) + ", -5) translate(-" + ((height / 2) + yAxisTitlePosition) + ")");

                var xAxisTitleY = margin.bottom -40 - (toString(xAxisTitle)).visualHeight();

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
                        .attr('dy', '0.35em')       // vertical-align: middle
                        .attr('x', -tickSize - 1);  // Use tickSize "constant"
                }

                if ((minY < 0) && (maxY > 0)) {
                    var ticks = $('line.tick');
                    for (var i = 0; i < ticks.length ; i++) {
                        if (ticks[i].nextSibling.textContent === "0") {
                            $(ticks[i]).attr('x2', width - margin.left - margin.right - xAxisGap);
                        }
                    }
                }

            
                if(hideLegend != true)
                    appendLegend(svg, legendCoord);

                appendReferenceLine(svg, plotData);
            }
        });

    }

    chart.dispatch = dispatch;

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

    chart.y3 = function (_) {
        if (!arguments.length) return y3Value;
        y3Value = _;
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
    
    chart.cursor = function (_) {
        if (!arguments.length) return cursor;
        cursor = _;
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
    
    chart.graphName = function (_) {
        if (!arguments.length) return graphName;
        graphName = _;
        return chart;
    };
    
    chart.hideLegend = function (_) {
        if (!arguments.length) return hideLegend;
        hideLegend = _;
        return chart;
    };
    chart.legendCoord = function (_) {
        if (!arguments.length) return legendCoord;
        legendCoord = _;
        return chart;
    }
    chart.hideTitle = function (_) {
        if (!arguments.length) return hideTitle;
        hideTitle = _;
        return chart;
    };

    chart.plotRef = function (_) {
        if (!arguments.length) return plotRef;
        plotRef = _;
        return chart;
    };

    appendLegend = function(svg, legendCoord){
        LogMessage(L_LOG, src, "Entering Append Legend");

        // Add Legend - there is only two items
        var bboxes = [];

        var legendRadius = 5;
        var legendFont = 18;
        var separationDistanceAdditional = 3;
        var borderPadding = 10;

        var legendBorderClass = "legendBorder";
        var legendInnerClass = "legendInner";
        var dragClass = "dragClass";

        var legend = {
            "x": 300,
            "y": 200,
            "xPrevious": 0,
            "yPrevious": 0
        };

        // Set up Drag Behaviour
        var drag = d3.behavior.drag()
            .origin(function () {
                var t = d3.transform(d3.select(this).attr("transform"));
                return { x: t.translate[0], y: t.translate[1] };
            })
            .on("drag", function (d, i) {
                legendCoord.x = legendCoord.xPrevious + d3.event.x;
                legendCoord.y = legendCoord.yPrevious + d3.event.y;
                d3.selectAll(".dragClass").attr("transform", function (d, i) {
                    return "translate(" + [d3.event.x, d3.event.y] + ")"
                })
            })
            .on('dragstart', function () {
                legendCoord.xPrevious = legendCoord.x;
                legendCoord.yPrevious = legendCoord.y;
            });

        var legendTotal = svg.append("g")
            .attr("class", displayLegend)           
            .attr("transform", "translate(" + legendCoord.x + "," + legendCoord.y + ")");

        for (var k = 0; k < plotRef.Keys.length; k++) {

            var key = plotRef.Keys[k];

            var legendEntry = legendTotal.append("g")
            .attr("class", "legend" + plotRef[key].ref + " dragClass");
            
            var ybuffer = 0;
            if (k > 0) {
                ybuffer = k * (bboxes[0].height + separationDistanceAdditional);
            }
            
            // Add Legend Text
            legendEntry.append("text")
                .attr("x", 0)
                .attr("y", ybuffer)
                .attr("class", "legend" + plotRef[key].ref + "Text")
                .attr("font-size", legendFont)
                .text(plotRef[key].name);

            var offsetsText = d3.select(".legend" + plotRef[key].ref + "Text")[0][0].getBBox();

            // Add Legend Circle            
            var legendCircle = legendEntry.append("circle")
                .attr("r", legendRadius)
                .attr("stroke-dasharray", null);
            
            if (k == 0) {
                legendCircle
                    .attr("class", plotRef[key].ref + "CircleLegend " + plotRef[key].style)
            
            }
            else {
                legendCircle
                    .attr("class", plotRef[key].ref + "CircleLegend " + plotRef[key].style)
            }

            var offsetsText = d3.select(".legend" + plotRef[key].ref + "Text")[0][0].getBBox();
            
            var legendCircleBBox = legendCircle[0][0].getBBox();
            var xCoord = offsetsText.x - legendCircleBBox.width;
            var yCoord = offsetsText.y + (offsetsText.height / 2);

            d3.select("." + plotRef[key].ref + "CircleLegend")
                .attr("cx", xCoord)
                .attr("cy", yCoord);

            bboxes.push(legendEntry[0][0].getBBox());
        }


        var legendBorder = legendTotal.append("rect")
            .attr("class", legendBorderClass + " " + dragClass);

        // get largest of legend text
        var maxWidth = 0;
        for (var i = 0; i < bboxes.length; i++) {
            maxWidth = Math.max(bboxes[i].width, maxWidth);
        }

        var rectWidth = maxWidth + borderPadding;
        var rectHeight = bboxes[(bboxes.length - 1)].y + bboxes[(bboxes.length - 1)].height - bboxes[0].y + borderPadding / (1.5);
        var borderXCoord = bboxes[0].x - (borderPadding / 2);
        var borderYCoord = bboxes[0].y - (borderPadding / 3);

        legendBorder.attr("stroke", "#f0f0f0")
            .attr("fill", "none")
            .attr("stroke-width", 1)
            .attr("x", borderXCoord)
            .attr("y", borderYCoord)
            .attr("width", rectWidth)
            .attr("height", rectHeight);

        // This will only work when refresh is not updated
        var bBox = legendBorder[0][0].getBBox();

        d3.select("." + displayLegend).insert("rect", ".legendRunOne")
            .attr("x", bBox.x)
            .attr("y", bBox.y)
            .attr("width", bBox.width)
            .attr("height", bBox.height)
            .attr("fill", "#f0f0f0")
            .attr("class", legendInnerClass + " " + dragClass)
            .attr("cursor", "move")
            .call(drag);
        
    }



    function appendReferenceLine(svg, plotdata) {

        LogMessage(L_LOG, src, "Entering Append ReferenceLine");

        var xReferenceRectanglePixelSpacing = 18;
        var referenceLineClass = "PipelineProfileReferenceLine";

        // Set up bisect class traversses a [{},{}] array of values
        // NOTE: xAxis only accepts a certain date time format
        var xBisectDate = d3.bisector(function (d) {
            return d.TimeOne;
        }).left;

        var widthPlotArea = width - margin.left - margin.right;
        var heightPlotArea = height - margin.top - (margin.bottom/2);

        // Add Reference Line GROUP
        var focus = svg.append("g")
            .attr("class", "displayRefLineClass")
            .style("display", "none")
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")");


        // Append Reference Line                
        focus.append("line")
            .attr("class", "y" + " ReferenceLineY" + " " + referenceLineClass)
            .attr("y1", 0)
            .attr("y2", heightPlotArea);


        // Append Time Reference Tooltip
        // identified as .RefTime and text.RefTime
        var yCoordForTimeReferenceBox = heightPlotArea + xReferenceRectanglePixelSpacing;
        var yCoordForTimeReferenceBox2 = heightPlotArea + (xReferenceRectanglePixelSpacing * 2);
        
        appendTimeReferenceBox(focus, yCoordForTimeReferenceBox, yCoordForTimeReferenceBox2);
        appendReferenceBoxes(focus);

        //Area for events for reference line should be underneath the legend
        svg.insert("rect", "." + displayLegend)
            .attr("class", graphName + "refLineEventClass")
            .attr("width", widthPlotArea)
            .attr("height", heightPlotArea)
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")")
            .style("fill", "none")
            .style("pointer-events", "all")
            .on("mouseover", function () { focus.style("display", null); })
            .on("mouseout", function () { focus.style("display", "none"); })
            .on("mousemove", MouseMoveRefLine)

        function MouseMoveRefLine() {
            var xMouseCoord = xScale.invert(d3.mouse(this)[0]);

            var dt1 = new Date(xMouseCoord).toISOString();
            var index = xBisectDate(plotData, dt1, 1);

            // Move the Reference Line
            focus.select(".y")
                .attr("transform", "translate(" + xScale(xMouseCoord) + "," + 0 + ")")
                .attr("y2", heightPlotArea);

            //Get Pressure Readings by using the index. It should contain a Time (use as X), Pressure 1 and Pressure 2
            if (index < plotData.length) {
                var pressureReadings = plotData[index];

                // Move the Time, Pressure 1 and Pressure 2 tooltips            
                moveTimeReferenceBox(focus, pressureReadings, xMouseCoord);
                moveReferenceBoxes(focus, pressureReadings, xMouseCoord);
            }

        };

    };
        
    function appendTimeReferenceBox(focus, yCoord, yCoord2) {
        // The Time Reference Box and Text will be identified as
        // "RefTime" and "Text.RefTime

        var idvTextClass1 = xRefClass + "1";
        var idvTextClass2 = xRefClass + "2";

        focus.append("rect")
            .attr("stroke", "black")
            .attr("stroke-width", 1)
            .attr("fill", "white")
            .attr("stroke-dasharray", null)
            .attr("y", yCoord)
            .attr("class", xRefClass);

        var textElement1 = focus.append("text")
            .attr("class", idvTextClass1)
            .attr("y", yCoord);

        var text1bbox = textElement1[0][0].getBBox();
        focus.append("text")
            .attr("class", idvTextClass2)
            .attr("y", yCoord2);
    };

    function appendReferenceBoxes(focus, yCoord) {

        // For each rendered trace create a circle, text and rectangle. 
        // Plot 1
        var idvCircleClass1 = yCircleClass + "1";
        var idvTextClass1 = yTextClass + "1";
        var idvRectClass1 = yRectClass + "1";

        //Plot 2
        var idvCircleClass2 = yCircleClass + "2";
        var idvTextClass2 = yTextClass + "2";
        var idvRectClass2 = yRectClass + "2";

        // Individual elements are being added temporarily, using a for loop issues a javascript error.
        // append groups of circles, text and rectangles
        if (plotRef.RunOne.enable) {

            focus.append("circle")
                .attr("class", idvCircleClass1)
                .attr("fill", "black")
                .attr("stroke", "black")
                .attr("r", 2);

            focus.append("rect")
                .attr("stroke", "rgba(46, 204, 113, 1)")
                .attr("stroke-width", 1)
                .attr("fill", "white")
                .attr("fill-opacity", "0.8")
                .attr("class", idvRectClass1);

            focus.append("text")
                .attr("class", idvTextClass1)
                .attr("dx", 8)
        }
        
        if (plotRef.RunTwo.enable) {
            focus.append("circle")
                .attr("class", idvCircleClass2)
                .attr("fill", "black")
                .attr("stroke", "black")
                .attr("r", 2);

            focus.append("rect")
                .attr("stroke", "rgba(52, 152, 219,1)")
                .attr("stroke-width", 1)
                .attr("fill", "white")
                .attr("fill-opacity", "0.8")
                .attr("class", idvRectClass2);

            focus.append("text")
              .attr("class", idvTextClass2)
              .attr("dx", 8)
        }

    };

    function moveReferenceBoxes(focus, pressureReadings, xMouseCoord) {
        
        var readingbbox = [];
        var enabledplots = 0;

        // for each PressureReading
        var yCoord = pressureReadings.PressureOne;
        for (var i = 0; i < referenceKeys.length; i++) {

            var key = plotRef.Keys[i];

            if (plotRef[key].enable) {
                
                // Get Reading
                if (i == 1) {
                    yCoord = pressureReadings.PressureTwo;
                }

                if (yCoord != "NaN") {

                    // Get element name
                    var elementid = i + 1;

                    // move circle
                    focus.select("circle." + yCircleClass + elementid)
                        .attr("display", "show")
                        .attr("transform",
                            "translate(" + xScale(xMouseCoord) + "," + yScale(yCoord) + ")");

                    var textElement = focus.select("text." + yTextClass + elementid)
                        .text(yCoord)
                        .attr("display", "show")
                        .attr("transform",
                            "translate(" + xScale(xMouseCoord) + "," + yScale(yCoord) + ")");

                    var textBBox = textElement.node().getBBox();

                    focus.select("rect." + yRectClass + elementid)
                        .attr("display","show")
                        .attr("width", textBBox.width + 2 * displayRectPadding)
                        .attr("height", textBBox.height + 1.5 * displayRectPadding)
                        .attr("x", textBBox.x - displayRectPadding)
                        .attr("y", textBBox.y - displayRectPadding)
                        .attr("transform",
                                  "translate(" + xScale(xMouseCoord) + "," +
                                                 yScale(yCoord) + ")");

                    readingbbox.push(textBBox);
                    enabledplots++;
                }
                else {
                    var elementid = i + 1;
                    
                    focus.select("rect." + yRectClass + elementid)
                        .attr("display", "none")
                    focus.select("text." + yTextClass + elementid)
                        .attr("display", "none")
                    focus.select("circle." + yCircleClass + elementid)
                        .attr("display", "none")
                }

            }
        }

        if (enabledplots >1) {
            detectCollision(pressureReadings, readingbbox, focus, xMouseCoord);
        }

    };

    function detectCollision(pressureReading, bboxes, focus, xMouseCoord) {
        
        var pressureOneReading = pressureReading.PressureOne;
        var pressureTwoReading = pressureReading.PressureTwo;
        
        var pressureOneLocation = yScale(pressureOneReading) + bboxes[0].y;
        var pressureTwoLocation = yScale(pressureTwoReading) + bboxes[1].y;

        // get the smaller of the two
        if (pressureOneReading >= pressureTwoReading) {
            
            if (yScale(pressureOneReading) + bboxes[0].y + bboxes[0].height > yScale(pressureTwoReading) + bboxes[0].y + tolerance) {

                // Transform the Analysis1 origin - the plot will always be on top if there the values are equal
                var t = pressureTwoLocation - bboxes[1].height + stackpadding;
                
                // Adjust pressure two
                focus.selectAll("rect." + yRectClass + "1")
                    .attr("transform",
                        "translate(" + xScale(xMouseCoord) + "," + t + ")");

                focus.selectAll("text." + yTextClass + "1")
                    .attr("transform",
                        "translate(" + xScale(xMouseCoord) + "," + t + ")");

            }
        }
        else {
            if (yScale(pressureTwoReading) + bboxes[1].y + bboxes[1].height + tolerance > yScale(pressureOneReading) + bboxes[1].y + tolerance) {
                
                // Transform the Analysis2. origin by setting it to the origin of analysis 1 + bbox.height + stackpadding
                var t = pressureOneLocation - bboxes[0].height + stackpadding;

                // Adjust pressure two
                focus.selectAll("rect." + yRectClass + "2")
                    .attr("transform",
                        "translate(" + xScale(xMouseCoord) + "," + t + ")");

                focus.selectAll("text." + yTextClass + "2")
                    .attr("transform",
                        "translate(" + xScale(xMouseCoord) + "," + t + ")");

            }
        }

    };

    function moveTimeReferenceBox(focus, pressureReading, xMouseCoord) {

        var localDate1 = new Date(pressureReading.TimeOne);
        var localDate2 = new Date(pressureReading.TimeTwo);

        var localDateString1 = localDate1.toLocaleDateString() + " " + localDate1.toLocaleTimeString();
        var localDateString2 = localDate2.toLocaleDateString() + " " + localDate2.toLocaleTimeString();

        if (plotRef.RunOne.enable == false) {
            localDateString1 = "No Readings Recorded";
        }

        if (plotRef.RunTwo.enable == false || pressureReading.PressureTwo == "NaN") {
            localDateString2 = "No Readings Recorded";
        }

        var timeElement1 = focus.select("text." + xRefClass + "1")
            .text(plotRef.RunOne.name + ": " + localDateString1);
        var timeElement2 = focus.select("text." + xRefClass + "2")
            .text(plotRef.RunTwo.name + ": " + localDateString2);


        var timeBBox = timeElement1[0][0].getBBox();
        var timeBBoxCenter = timeBBox.x - timeBBox.width / 2 + xScale(xMouseCoord);

        // move time box
        focus.select("rect.RefTime")
            .attr("width", timeBBox.width + 2 * displayRectPadding)
            .attr("height", (timeBBox.height * 2) + 3 * displayRectPadding) // from the TimeElement1 origin
            .attr("x", timeBBox.x - displayRectPadding)
            .attr("y", timeBBox.y - displayRectPadding)
            .attr("transform", "translate(" + timeBBoxCenter + "," + 0 + ")");

        timeElement1.attr("transform", "translate(" + timeBBoxCenter + "," + 0 + ")");
        timeElement2.attr("transform", "translate(" + timeBBoxCenter + "," + 0 + ")");
    };

    return chart;
}



// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/