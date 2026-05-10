// ## Shape Module Main Entry Point

define(
[
    './LeakDetectionCalcControlBinding',
    'text!./leakDetectionCalcControlTemplate.htm',
    'asari/viewmodels/PointParamViewModel'
],

function(LeakDetectionCalcControlBinding, leakDetectionCalcControlTemplate, PointParamViewModel) {

    var ShapeModule = {    
        _config: {}
    };

    ShapeModule.init = function(config) {
        ShapeModule._config = config;
        return true;
    }

    ShapeModule.getViewTemplates = function() {
        return {
            extLeakDetectionCalcControlTemplate: leakDetectionCalcControlTemplate,
            MAIN_VIEW_TEMPLATE: 'extLeakDetectionCalcControlTemplate' // This is not strictly required for a single view template.
        };
    }

    ShapeModule.getBindings = function() {
        LeakDetectionCalcControlBinding.shapesPath = ShapeModule._config.shapesPath;
        return {
            extLeakDetectionCalcControl: LeakDetectionCalcControlBinding
        }
    }

    ShapeModule.createViewModel = function() {
        return new PointParamViewModel.MultiPointParamViewModel(['status','tooltipdescription','pointname']);
    }

    return ShapeModule;
});
