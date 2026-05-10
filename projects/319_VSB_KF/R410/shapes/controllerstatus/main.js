define(
[
    './ControllerStatusBinding',
    'text!./ControllerStatusTemplate.htm'
],
function(ControllerStatusBinding, ControllerStatusTemplate) {

    var ShapeModule = {
        _config: {}
    };

    ShapeModule.init = function(config) {
        ShapeModule._config = config;
        return true;
    }

    ShapeModule.getViewTemplates = function() {
        return {
            extControllerStatusTemplate: ControllerStatusTemplate
        };
    }

    ShapeModule.getBindings = function() {
        ControllerStatusBinding.shapesPath = ShapeModule._config.shapesPath;
        return {
            extControllerStatus: ControllerStatusBinding
        }
    }

    ShapeModule.getStyleSheets = function() {
        return [
            ShapeModule._config.shapesPath + '/controllerstatus/ControllerStatusStyle.css'
        ];
    }

    ShapeModule.createViewModel = function() {
        return 'PointParamViewModel';
    }

    return ShapeModule;
});
