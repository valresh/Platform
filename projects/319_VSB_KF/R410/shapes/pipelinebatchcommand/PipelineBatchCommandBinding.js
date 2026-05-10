// ## Pipeline Batch Status Binding

define(
[
    'knockout', 'interop/StationAutomation'
],
function (ko, StationAutomation) {

    var PipelineBatchCommandBinding = {
        shapesPath: null
    }

    PipelineBatchCommandBinding.init = function (element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
        var bindingConfig, eventValueAccessor;
        bindingConfig = valueAccessor();
        eventValueAccessor = function () {
            return {
                click: function(viewmodel, event){
			return PipelineBatchCommandBinding.onClick(viewModel, event, bindingConfig);
		},
                OnCalloutResponse: function(viewmodel, event){
			return PipelineBatchCommandBinding.onCalloutResponse(viewModel, event, bindingConfig);
		}
            };
        };
        return ko.bindingHandlers.event.init(element, eventValueAccessor, allBindingsAccessor, bindingConfig, bindingContext);
    };

    PipelineBatchCommandBinding.onClick = function (viewModel, event, bindingConfig) {
        var elementId, element, message, type, timeout, args, batchId, batchPointName;
        element = event.originalEvent.srcElement;
        batchId = ko.utils.unwrapObservable(bindingConfig.batchId.source.value);

        elementId = event.originalEvent.srcElement.id + ko.utils.unwrapObservable(bindingConfig.pointName.source.value);
        element.id = elementId;

        type = 1; // show yes/no buttons
        timeout = 0; // wait for user input

        if (elementId.indexOf('deleteBatch') >= 0) {
            message = "Are you sure you want to delete batch \'" + batchId + "\'?";
        }
        else if (elementId.indexOf('startBatch') >= 0) {
            message = "Are you sure you want to inject batch \'" + batchId + "\'?";
        }
        else if (elementId.indexOf('abortBatch') >= 0) {
            message = "Are you sure you want to abort batch \'" + batchId + "\'?";
        }
        else if (elementId.indexOf('deliverBatch') >= 0) {
            message = "Are you sure you want to deliver batch \'" + batchId + "\'?";
        }
        else if (elementId.indexOf('completeBatch') >= 0) {
            message = "Are you sure you want to complete batch \'" + batchId + "\'?";
        }
        else if (elementId.indexOf('raiseBatch') >= 0) {
            if (ko.utils.unwrapObservable(bindingConfig.pointName.source.value) != null) {
                // build JSON to pass in batch id and name
                batchPointName = ko.utils.unwrapObservable(bindingConfig.pointName.source.value);
                args = "{\"pointName\":\"" + batchPointName + "\",\"batchId\":\"" + batchId + "\"}";
                StationAutomation.requestTask(194, 7, 0, 0, 0, args);
            }
            return false;
        }
        else if (elementId.indexOf('lowerBatch') >= 0) {
            if (ko.utils.unwrapObservable(bindingConfig.pointName.source.value) != null) {
                // build JSON to pass in batch id and name
                batchPointName = ko.utils.unwrapObservable(bindingConfig.pointName.source.value);
                args = "{\"pointName\":\"" + batchPointName + "\",\"batchId\":\"" + batchId + "\"}";
                StationAutomation.requestTask(194, 8, 0, 0, 0, args);
            }
            return false;
        }
        else {
            return false;  // unknown element
        }

        tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));
        if (tooltip.length != 0) {
            if ((elementId.indexOf('startBatch') >= 0) ||
                (elementId.indexOf('deliverBatch') >= 0)) {
                return false;
            }
        }

        StationAutomation.showCallout(element, message, type, timeout);

        return false;
    };

    PipelineBatchCommandBinding.onCalloutResponse = function (viewModel, event, bindingConfig) {

        tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));

        var response = StationAutomation.calloutResponse();
        if (response == 'N') return false;  // no confirmation, do nothing
        var batchId = ko.utils.unwrapObservable(bindingConfig.batchId.source.value);
        var elementId = event.originalEvent.srcElement.id;
        var lrn, param1, args, batchPointName;
        lrn = 194; // calcmngr task

        if (elementId.indexOf('deleteBatch') >= 0) {
            param1 = 9;
        }
        else if (elementId.indexOf('startBatch') >= 0) {
            param1 = 4;
        }
        else if (elementId.indexOf('abortBatch') >= 0) {
            param1 = 6;
        }
        else if (elementId.indexOf('deliverBatch') >= 0) {
            param1 = 10;
        }
        else if (elementId.indexOf('completeBatch') >= 0) {
            param1 = 5;
        }
        else if (elementId.indexOf('raiseBatch') >= 0) {
            param1 = 7;
        }
        else if (elementId.indexOf('lowerBatch') >= 0) {
            param1 = 8;
        }
        else {
            return false;  // unknown element
        }

        if (tooltip.length != 0) {
            if ((elementId.indexOf('startBatch') >= 0) ||
                (elementId.indexOf('deliverBatch') >= 0)) {
                // if there is an error or batch tracking disabled, only scheduled activities is permitted
                return false;
            }
        }

        if (ko.utils.unwrapObservable(bindingConfig.pointName.source.value) != null) {
            // build JSON to pass in batch id and name
            batchPointName = ko.utils.unwrapObservable(bindingConfig.pointName.source.value);
            args = "{\"pointName\":\"" + batchPointName + "\",\"batchId\":\"" + batchId + "\"}";
            StationAutomation.requestTask(lrn, param1, 0, 0, 0, args);
        }

        return false;
    };

    PipelineBatchCommandBinding.update = function (element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

        var bindingConfig = valueAccessor();
        var status = ko.utils.unwrapObservable(bindingConfig.batchStatus.source.value);
        var isnext = ko.utils.unwrapObservable(bindingConfig.isNextScheduled.source.value);
        var islast = ko.utils.unwrapObservable(bindingConfig.isLastScheduled.source.value);
        var isarriving = ko.utils.unwrapObservable(bindingConfig.isNextArriving.source.value);
        var tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));
        var routetype = ko.utils.unwrapObservable(bindingConfig.routeType.source.value);
        var ismanual = ko.utils.unwrapObservable(bindingConfig.isManualBatchTracking.source.value);

		var ismultiproduct = false;
		if (routetype == "Multi-product")
		{
			ismultiproduct = true;
		}

        var elementId = element.id;
        element.style.display = 'none';

        var commandaffix = ".png";
        var commentaffix = element.getAttribute("alt");
        element.title = commentaffix;

        if (tooltip.length != 0) {
            commandaffix = "_disabled.png";
            commentaffix = tooltip;
        }

        // set up shape source
        var shapesrc = PipelineBatchCommandBinding.shapesPath + "/pipelinebatchcommand";

        if (elementId.indexOf('startBatch') >= 0) {
            element.src = shapesrc + "/start" + commandaffix;
            element.title = commentaffix;
        }
        else if (elementId.indexOf('deliverBatch') >= 0) {
            element.src = shapesrc + "/arrive" + commandaffix;
            element.title = commentaffix;
        }
        else if (elementId.indexOf('raiseBatch') >= 0) {
            element.src = shapesrc + "/up.png";
        }
        else if (elementId.indexOf('lowerBatch') >= 0) {
            element.src = shapesrc + "/down.png";
        }
        else if (elementId.indexOf('deleteBatch') >= 0) {
            element.src = shapesrc + "/delete.png";
        }
        else if (elementId.indexOf('completeBatch') >= 0) {
            element.src = shapesrc + "/complete.png";
        }
        else if (elementId.indexOf('abortBatch') >= 0) {
            element.src = shapesrc + "/abort.png";
        }
        else {
            element.src = "";
        }


        // Map the value to an image to display.
        // Assumes that the data types of the value is an integer.
        var imgFile = null;
        switch (status) {

            // Pipeline Batch State: Scheduled
            // Show up/down arrows, inject, deliver and delete buttons
            case 1:
                if ((elementId.indexOf('startBatch') >= 0) && isnext && ismultiproduct && ismanual) {
                    element.style.display = '';
                }
                if ((elementId.indexOf('deliverBatch') >= 0) && isnext && ismanual) {
                    element.style.display = '';
                }
                if ((elementId.indexOf('raiseBatch') >= 0) && !isnext) {
                    element.style.display = '';
                }
                if (elementId.indexOf('lowerBatch') >= 0 && !islast) {
                    element.style.display = '';
                }
                if (elementId.indexOf('deleteBatch') >= 0) {
                    element.style.display = '';
                }
                break;

            // Pipeline Batch State: Injecting, Inline, Arriving
            case 2:
            case 3:
            case 4:
                if (isarriving == 1) {
                    if ((elementId.indexOf('deliverBatch') >= 0) && ismanual) {
                        element.style.display = '';
                    }
                }
                if (elementId.indexOf('abortBatch') >= 0) {
                    element.style.display = '';
                }

                break;

            // Pipeline Batch State: Delivering
            case 5:
                if ((elementId.indexOf('completeBatch') >= 0) && ismanual) {
                    element.style.display = '';
                }
                if (elementId.indexOf('abortBatch') >= 0) {
                    element.style.display = '';
                }
                break;

            // Pipeline Batch State: Unassigned, Delivered, Aborted
            case 0:
            case 6:
            case 7:
            default:
                break;
        }
    }

    return PipelineBatchCommandBinding;
});
