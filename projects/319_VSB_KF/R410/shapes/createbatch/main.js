// ## Shape Module Main Entry Point
define(
[
    './CreateBatchBinding',
    'text!./CreateBatchTemplate.htm',
	'asari/viewmodels/PointParamViewModel'
],
function(CreateBatchBinding, CreateBatchTemplate, PointParamViewModel) {

    var ShapeModule = {
        _config: {}
    };

    ShapeModule.init = function(config) {
        ShapeModule._config = config;
        return true;
    }

    ShapeModule.getViewTemplates = function() {
        return {
            extCreateBatchTemplate: CreateBatchTemplate,
            MAIN_VIEW_TEMPLATE: 'extCreateBatchTemplate' // This is not strictly required for a single view template.
        };
    }
		
		ShapeModule.getBindings = function() {
        CreateBatchBinding.shapesPath = ShapeModule._config.shapesPath;
        return {
            extCreateBatch: CreateBatchBinding
        }
    }

    ShapeModule.createViewModel = function() {
        return new PointParamViewModel.MultiPointParamViewModel(['pointname','tooltipdescription']);
    }

    return ShapeModule;
});
