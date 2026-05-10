// ## Create Pipeline Batch Binding

define(
[
    'knockout', 'interop/StationAutomation'
],
function (ko, StationAutomation) {

    var CreateBatchBinding = {
        shapesPath: null
    }

    CreateBatchBinding.init = function (element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
        var bindingConfig, eventValueAccessor;
        bindingConfig = valueAccessor();
        eventValueAccessor = function () {
            return {
                click: function(viewmodel, event) {
                                        return CreateBatchBinding.onClick(viewModel, event, bindingConfig);
                                },
                OnCalloutResponse: function(viewmodel, event){
			return CreateBatchBinding.onCalloutResponse(viewModel, event, bindingConfig);
		}

            };
        };
        return ko.bindingHandlers.event.init(element, eventValueAccessor, allBindingsAccessor, bindingConfig, bindingContext);
    };

    CreateBatchBinding.onClick = function (viewModel, event, bindingConfig) {
        var elementId, element, message, type, timeout, path, productName, tooltip;
        element = event.originalEvent.srcElement;
        productName = ko.utils.unwrapObservable(bindingConfig.pointName.source.value);

        elementId = event.originalEvent.srcElement.id + productName;
        element.id = elementId;

        type = 0; // show callout with input
        timeout = 0; // wait for user input
        message = "Enter ID for new batch of \'" + productName + "\'?";

        tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));
        if (tooltip.length != 0) {
            return false;
        }

        StationAutomation.showCallout(element, message, type, timeout);
        return false;
    };

    CreateBatchBinding.onCalloutResponse = function (viewModel, event, bindingConfig) {
        var batchId = StationAutomation.calloutResponse();

        // validate batchId length
        if (batchId.length > 40) {
            var type = 2;       // error callout
            var timeout = 10;   // number of seconds to show callout
            var element = event.srcElement;
            message = "Batch ID must be between 0 and 40 characters.";
            StationAutomation.showCallout(element, message, type, timeout);
            return false;
        }

        var lrn, param1, args, productPointName;
        lrn = 194; // calcmngr task
        param1 = 3; // create batch request

        if (ko.utils.unwrapObservable(bindingConfig.pointName.source.value) != null) {
            productPointName = ko.utils.unwrapObservable(bindingConfig.pointName.source.value);
            // build JSON to pass in user input and product name
            args = "{\"pointName\":\"" + productPointName + "\",\"batchId\":\"" + batchId + "\"}";
            StationAutomation.requestTask(lrn, param1, 0, 0, 0, args);
        }

        return false;
    };

    CreateBatchBinding.update = function (element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
        var bindingConfig = valueAccessor();
        var tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));

        var commandaffix = ".png";
        var commentaffix = element.getAttribute("alt");
        element.title = commentaffix;

        // set up disabled shape source and tooltip
        if (tooltip.length != 0) {
            commandaffix = "_disabled.png";
            commentaffix = tooltip;
        }

        var shapesrc = CreateBatchBinding.shapesPath + "/createbatch";
        element.src = shapesrc + "/create" + commandaffix;
        element.title = commentaffix;

    };

    return CreateBatchBinding;
});
