// ## Shape Module Main Entry Point
define(
[
    './PipelineBatchCommandBinding',
    'text!./pipelineBatchCommandTemplate.htm',
	'asari/viewmodels/PointParamViewModel'
],
function(PipelineBatchCommandBinding, pipelineBatchCommandTemplate, PointParamViewModel) {

    var ShapeModule = {
        _config: {}
    };

    ShapeModule.init = function(config) {
        ShapeModule._config = config;
        return true;
    }

    ShapeModule.getViewTemplates = function() {
        return {
            extPipelineBatchCommandTemplate: pipelineBatchCommandTemplate,
            MAIN_VIEW_TEMPLATE: 'extPipelineBatchCommandTemplate' // This is not strictly required for a single view template.
        };
    }

    ShapeModule.getBindings = function() {
        PipelineBatchCommandBinding.shapesPath = ShapeModule._config.shapesPath;
        return {
            extPipelineBatchCommand: PipelineBatchCommandBinding
        }
    }

    ShapeModule.createViewModel = function() {
        return new PointParamViewModel.MultiPointParamViewModel(['status','pointname','batchid','isnextscheduled','islastscheduled','isnextarriving','tooltipdescription','routetype','ismanualbatchtracking']);
    }
	
    return ShapeModule;
});
