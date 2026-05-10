// ## Leak Detection Calculation Control Binding

define(
[
    'knockout', 'interop/StationAutomation'
],
function(ko, StationAutomation) {

    var LeakDetectionCalcControlBinding = {
        shapesPath: null
    }

    LeakDetectionCalcControlBinding.init = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
        var bindingConfig, eventValueAccessor;
        bindingConfig = valueAccessor();
        eventValueAccessor = function() {
            return {
                    click: function(viewModel, event){
                        return LeakDetectionCalcControlBinding.onclick(viewModel, event, bindingConfig);
                    },
                    OnCalloutResponse: function(viewmodel, event){
                            return LeakDetectionCalcControlBinding.onCalloutResponse(viewModel, event, bindingConfig);
                        }
                };
        };
        return ko.bindingHandlers.event.init(element, eventValueAccessor, allBindingsAccessor, bindingConfig, bindingContext);
    };

        LeakDetectionCalcControlBinding.onclick = function(viewModel, event, bindingConfig) {

            var elementId, element, message, type, timeout, tooltip;
            element = event.originalEvent.srcElement;

            tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));

            if(tooltip.length != 0) {
                return false;
            }

            type = 1;
            timeout = 0;
            elementId = event.originalEvent.srcElement.id;
            element.id = elementId;

            if (elementId.indexOf('startSLD') >= 0) {
                message = "I18N('Are you sure you want to start Static Leak Detection\?')";
            }
            else if (elementId.indexOf('abortSLD') >= 0) {
                message = "I18N('Are you sure you want to abort Static Leak Detection\?')";
            }

            StationAutomation.showCallout(element, message, type, timeout);
            return false;
        };

        LeakDetectionCalcControlBinding.onCalloutResponse = function(viewModel, event, bindingConfig){

            var tooltip, param1, lrn, args, routePointName;

            tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));
            if (tooltip.length != 0) {
                return false;
            }

            var response = StationAutomation.calloutResponse();
            var elementId = event.originalEvent.srcElement.id;
            lrn = 194;

            if (response == 'N') return false;  // no confirmation, do nothing
            if (elementId.indexOf('startSLD') >= 0) {
                param1 = 14;
            }
            else if (elementId.indexOf('abortSLD') >= 0) {
                param1 = 15;
            }
            else{
                return false;
            }

            if(ko.utils.unwrapObservable(bindingConfig.pointName.source.value) !=null) {
                routePointName = ko.utils.unwrapObservable(bindingConfig.pointName.source.value);
                args = "{\"pointName\":\"" + routePointName + "\"}";

                StationAutomation.requestTask(lrn, param1, 0, 0, 0, args);
            }
        };


    LeakDetectionCalcControlBinding.update = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

      var bindingConfig = valueAccessor();
      var status = ko.utils.unwrapObservable(bindingConfig.status.source.value);
      var tooltip = new String(ko.utils.unwrapObservable(bindingConfig.tooltipDescription.source.value));

      var elementId = element.id;
      element.style.display = 'none';

      var commandaffix = ".png";
      var commentaffix = element.getAttribute("alt");
      element.title = commentaffix;

      if (tooltip.length != 0) {
          commandaffix = "_disabled.png";
          commentaffix = tooltip;
      }

      var shapesrc = LeakDetectionCalcControlBinding.shapesPath + '/leakdetectioncalccontrol';

      // Set up images as enabled or disabled
      if (elementId.indexOf('startSLD') >= 0) {
          element.src = shapesrc + "/start" + commandaffix;
          element.title = commentaffix;
      }
      else if (elementId.indexOf('abortSLD') >= 0) {
          element.src = shapesrc + "/abort" + commandaffix;
          element.title = commentaffix;
      }
      else {
          element.src = "";
      }


      // Set up image visibiliy
        switch (status) {

            // Display Abort
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 9:
                if ((elementId.indexOf('abortSLD') >= 0)) {
                    element.style.display = '';
                }
                break;

            case 0:
            case 8:
            case 10:
            case 11:
                if ((elementId.indexOf('startSLD') >= 0)) {
                    element.style.display = '';
                }
                break;

            default:
                break;
        }

    };
    return LeakDetectionCalcControlBinding;
});
